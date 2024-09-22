#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int		mstDm, mstDn, *mstVecU, **mstMatU, *mstVecA, **mstMatA, *mstVecB, **mstMatB, *mstVecE, *mstVecF, *mstVecS, *mstVecT;
double	**mstMatX, **mstMatY, *mstVecW, *mstVecD, *mstVecZ;

/* lblk.txt をロードする関数 */
void mstReadValue(const char *fn1)
{
	FILE *fp;
	int	  i, j, k, h;
	
	if((fp = fopen(fn1, "r")) == NULL)
	{
		fprintf(stderr, "Unknown File = %s\n", fn1); 
		exit(1);
	}
	
	fscanf(fp, "%d %d", &mstDm, &mstDn); // mstDm = 100, mstDn = 89993
	
	while((j = getc(fp)) != EOF)
		if(j == '\n') 
			break;
		
	mstVecU = (int *)     malloc(sizeof(int)*mstDm);
	mstMatU = (int **)    malloc(sizeof(int *)*mstDm);
	mstMatX = (double **) malloc(sizeof(double *)*mstDm);
	
//	指定された文書数まで繰り返し
	for(i = 0; i < mstDm; i++)
	{
		fscanf(fp, "%d", &mstVecU[i]);
		mstMatU[i] = (int *)    malloc(sizeof(int)*mstVecU[i]);
		mstMatX[i] = (double *) malloc(sizeof(double)*mstVecU[i]);
		
//		1文書の単語情報を記録する (単語ID:ネイピア数)
		for(j = 0; j < mstVecU[i]; j++)
		{ 
			fscanf(fp, "%d:%lf", &k, &mstMatX[i][j]);
			mstMatU[i][j] = k-1; // 単語ID
		}
		
		while((j = getc(fp)) != EOF)
			if(j == '\n')
				break; 
	}
	
	fclose(fp);
}


void mstInitValue()
{
	int		h, i, j, k;
	double	v, w;
	mstVecB = (int *)     malloc(sizeof(int)*mstDn);
	mstMatB = (int **)    malloc(sizeof(int *)*mstDn);
	mstMatY = (double **) malloc(sizeof(double *)*mstDn);
	
	for(j = 0; j < mstDn; j++) 
		mstVecB[j] = 0; 
	
//	指定された文書数まで繰り返し
	for(i = 0; i < mstDm; i++)
		for(j = 0; j < mstVecU[i]; j++) //	100個の文書中に存在している単語はVecB[単語ID]に出現数を代入
			mstVecB[mstMatU[i][j]]++;
		
	for(j = 0; j < mstDn; j++)
	{
		mstMatB[j] = (int *)    malloc(sizeof(int)*mstVecB[j]); 
		mstMatY[j] = (double *) malloc(sizeof(double)*mstVecB[j]); 
	}
	
	mstVecW = (double *) malloc(sizeof(double)*mstDn);

	for(i = 0; i < mstDm; i++)
	{
//		ノルムを1にするための正規化(過学習を抑制できる)
		for(j = 0, v = 0.0; j < mstVecU[i]; j++) 
			v += mstMatX[i][j] * mstMatX[i][j]; // v = v + MatXi[TF-IDF]^2 (MatXiは文書) ← 文書 i における全単語の TF-IDF を v に加算
		
		v = 1.0 / sqrt(v); // 1.0 / √(∑MatX(i,j)[TF-IDF]^2) (i = 固定値, 0 <= j <= N)
		
//		全単語に対して正規化したTF-IDFベクトルを計算
		for(j = 0; j < mstVecU[i]; j++) 
			mstMatX[i][j] = mstMatX[i][j] * v; // (文書 i における単語 j の TF-IDF) / √(文書 i における全単語の TF-IDF)
	}
	
//	繰り返し(j < 全文書の総単語数)
	for(j = 0; j < mstDn; j++) 
		mstVecB[j] = 0; 
	
//	繰り返し(i < 指定された文書数)
	for(i = 0; i < mstDm; i++)
	{
//		繰り返し(j < 任意の単語の出現数)
		for(j = 0; j < mstVecU[i]; j++)
		{
			k                = mstMatU[i][j]; // 単語ID
			mstMatB[k][mstVecB[k]] = i;          // 任意の単語が存在する文書ID (最初はMatB[単語ID][0]に代入される)
			mstMatY[k][mstVecB[k]] = mstMatX[i][j]; // 正規化したTF-IDFベクトル (最初はMatY[単語ID][0]に代入される)
			mstVecB[k]++;                     // 登録した文書IDが上書きされないように、MatB[単語ID][1]を参照させる
		}
	}
	
	mstVecE = (int *)    malloc(sizeof(int)*mstDm);
	mstVecF = (int *)    malloc(sizeof(int)*mstDm);
	mstVecD = (double *) malloc(sizeof(double)*mstDm);
	mstVecZ = (double *) malloc(sizeof(double)*mstDm);
	mstVecS = (int *)    malloc(sizeof(int)*mstDm);
	mstVecT = (int *)    malloc(sizeof(int)*mstDm);
}

/* 最小全域木を求める */
void mstCalMST()
{
	int		g, h, i, j, k;
	double	v, min;
	
//	繰り返し(j < 指定された文書数)
	for(j = 0; j < mstDm; j++)
	{
		mstVecD[j] = 1.0; 
		mstVecE[j] = 0; 
		mstVecF[j] = 0; 
	}
	
//	繰り返し(j < 任意の単語の出現数)
	for(i = j = 0; j < mstVecU[i]; j++)
	{
		h = mstMatU[i][j];  // 単語ID
		v = mstMatX[i][j];  // 正規化したTF-IDFベクトル (最初はMatX[単語ID][0]に代入される)
		
//		繰り返し(k < 単語 h が出現する文書数)
		for(k = 0; k < mstVecB[h]; k++) 
			mstVecD[mstMatB[h][k]] -= v * mstMatY[h][k]; // mstVecD[DF] = mstVecD[DF] - mstMatX(i,j) * mstMatY(h,k) ← mstMatX, mstMatY は正規化したTF-IDFベクトル
/*
			mstVecB : 単語 h が出現する文書数
			mstVecD : コサイン類似度
			mstMatB : 文書ID
			mstMatY : 正規化したTF-IDFベクトル
			v    : 正規化したTF-IDFベクトル
*/
	}
	
//	よりコサイン類似度が高い文書が見つかった場合
	for(j = 1, min = 2.0; j < mstDm; j++)
	{
		if(mstVecD[j] < min)
		{
			k   = j;       // よりコサイン類似度が高い文書IDに変更
			min = mstVecD[j]; // よりコサイン類似度が高いベクトルに変更
		}
	}
		
	mstVecS[0] = i; 
	mstVecT[0] = k; // 文書ID
	mstVecE[i] = 1; 
	mstVecE[k] = 1;
	
//	指定された文書数まで繰り返し
	for(g = 1; g < mstDm-1; g++)
	{
//		指定された文書数まで繰り返し
		for(j = 0; j < mstDm; j++) 
			mstVecZ[j] = 1.0; 
		
//		繰り返し(j < 任意の単語の出現数)
		for(i = k, j = 0; j < mstVecU[i]; j++)
		{
			h = mstMatU[i][j]; // 単語ID
			v = mstMatX[i][j]; // 正規化したTF-IDFベクトル (最初はMatX[単語ID][0]に代入される)
			
			for(k = 0; k < mstVecB[h]; k++) 
				mstVecZ[mstMatB[h][k]] -= v * mstMatY[h][k]; // mstVecZ[DF] = mstVecZ[DF] - mstMatX(i,j) * mstMatY(h,k) ← mstMatX, mstMatY は正規化したTF-IDFベクトル
		}
		
		for(j = 0, min = 2.0; j < mstDm; j++)
		{ 
			if(mstVecE[j] == 1) 
				continue; 
			
//			mstVecZ < mstVecD → mstVecD に mstVecZ を代入
			if((v = mstVecZ[j]) < mstVecD[j])
			{
				mstVecF[j] = i; // 文書ID
				mstVecD[j] = v; // コサイン類似度
			} 
			
//			mstVecD < mstVecZ → mstVecZ に mstVecD を代入
			else 
				v = mstVecD[j]; // コサイン類似度
			
//			よりコサイン類似度が高い文書が見つかった場合
			if(v < min)
			{
				k   = j; // よりコサイン類似度が高い文書IDに変更
				min = v; // よりコサイン類似度が高いベクトルに変更
			}
		}
		
		mstVecS[g] = mstVecF[k]; // 文書ID
		mstVecT[g] = k;       // 文書ID
		mstVecE[k] = 1; 
	}
}


void mstPrintValue(const char *fn1)
{
	FILE	*fp;
	int		i, j, k, u, v;
	
	mstVecA = (int *) malloc(sizeof(int)*mstDm);
	for(j = 0; j < mstDm; j++) 
		mstVecA[j] = 0; 
	
	for(i = 0; i < mstDm-1; i++)
	{
		mstVecA[mstVecS[i]] += 1; 
		mstVecA[mstVecT[i]] += 1; 
	}
	
	mstMatA = (int **) malloc(sizeof(int *)*mstDm);
	for(j = 0; j < mstDm; j++)
	{ 
		mstMatA[j] = (int *) malloc(sizeof(int)*mstVecA[j]);
		mstVecA[j] = 0; 
	}
	
	for(i = 0; i < mstDm-1; i++)
	{ 
		u                  = mstVecS[i]; 
		v                  = mstVecT[i]; 
		mstMatA[u][mstVecA[u]++] = v; 
		mstMatA[v][mstVecA[v]++] = u; 
	}
	
	fp = fopen(fn1, "w");
	fprintf(fp, "%d %d 1\n", mstDm, mstDm); 
	
	for(i = k = 0; i < mstDm; i++)
	{
		fprintf(fp, "%d ", mstVecA[i]); 
		k += mstVecA[i]; 
		
//		周囲の文書数まで繰り返し
		for(j = 0; j < mstVecA[i]; j++)
			fprintf(fp, "%d:1 ", mstMatA[i][j]+1); // 線で繋げる文書IDを記録する
		
		fprintf(fp, "1 1\n"); 
	}
	
	fclose(fp); 
}


int	mst(const char **argv)
{
	mstReadValue(argv[0]); // lblk.txt
	mstInitValue();
	mstCalMST();
	mstPrintValue(argv[1]);
	return 0;
}
