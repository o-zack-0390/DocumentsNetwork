#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>

int		knnDm, knnDn, *knnVecU, **knnMatU, *knnVecV, *knnVecA, **knnMatA, *knnVecB, **knnMatB, *knnVecG, *knnVecC, **knnMatD, *knnVecT;
double	**knnMatX, **knnMatY, *knnVecW, **knnMatZ;

/* lbl3000k100.txt をロード */
void knnReadValue(char *fn1)
{
	/*
		knnDm               : 検索文書数
		knnDn               : 語彙種類数
		k, v             : 単語 ID, 類似貢献度ベクトルの配列(D-次元特徴ベクトル)
		knnMatU[i], knnMatX[i] : 単語 ID, 類似貢献度ベクトルの配列(D-次元特徴ベクトル)
		knnVecU[i]          : 文書 i に出現する単語種数
	*/
	FILE		*fp;
	int		i, j, k, h;
	double	v; 
	if((fp = fopen(fn1, "r")) == NULL){ fprintf(stderr, "Unknown File = %s\n", fn1); exit(1);}
	fscanf(fp, "%d %d", &knnDm, &knnDn); 
	while((j = getc(fp)) != EOF) if(j == '\n') break; 
	knnVecU = (int *) malloc(sizeof(int)*knnDm);
	knnMatU = (int **) malloc(sizeof(int *)*knnDm);
	knnMatX = (double **) malloc(sizeof(double *)*knnDm);
	for(i = 0; i < knnDm; i++){
		fscanf(fp, "%d", &knnVecU[i]);
		knnMatU[i] = (int *) malloc(sizeof(int)*knnVecU[i]);
		knnMatX[i] = (double *) malloc(sizeof(double)*knnVecU[i]);
		for(j = 0; j < knnVecU[i]; j++){ 
			fscanf(fp, "%d:%lf", &k, &v);
			knnMatU[i][j] = k-1;
			knnMatX[i][j] = v;
		}
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
}


void knnInitValue()
{
	/*
		knnDm               : 検索文書数
		knnDn               : 語彙種類数
		knnVecB[j]          : 単語 j が出現する文書数
		knnVecU[i]          : 文書 i に出現する単語種数
		knnMatU[i], knnMatX[i] : 単語 ID, 類似貢献度ベクトルの配列(D-次元特徴ベクトル)
		knnMatB[i], knnMatY[i] : 文書 ID, 類似貢献度ベクトルの配列(D-次元特徴ベクトル)
	*/
	
	int		h, i, j, k;
	double	v, w;
	knnVecB = (int *) malloc(sizeof(int)*knnDn);
	knnMatB = (int **) malloc(sizeof(int *)*knnDn);
	knnMatY = (double **) malloc(sizeof(double *)*knnDn);
	
	for(j = 0; j < knnDn; j++) 
		knnVecB[j] = 0; 
	
	for(i = 0; i < knnDm; i++)
	{
		for(j = 0; j < knnVecU[i]; j++) 
			knnVecB[knnMatU[i][j]]++;
	}
	
	for(j = 0; j < knnDn; j++)
	{ 
		knnMatB[j] = (int *) malloc(sizeof(int)*knnVecB[j]); 
		knnMatY[j] = (double *) malloc(sizeof(double)*knnVecB[j]); 
	}
	
	knnVecW = (double *) malloc(sizeof(double)*knnDn);
	
//	for(j = 0, v = log(1.0*knnDm); j < knnDn; j++) knnVecW[j] = v-log(1.0*knnVecB[j]);

	for(i = 0; i < knnDm; i++)
	{
//		for(j = 0; j < knnVecU[i]; j++) knnMatX[i][j] *= knnVecW[knnMatU[i][j]];

		for(j = 0, v = 0.0; j < knnVecU[i]; j++) 
			v += knnMatX[i][j]*knnMatX[i][j];
		
		for(j = 0, v = 1.0/sqrt(v); j < knnVecU[i]; j++) 
			knnMatX[i][j] *= v;
	}
	
	for(j = 0; j < knnDn; j++) 
		knnVecB[j] = 0; 
	
//	転置ファイルの構成
	for(i = 0; i < knnDm; i++)
	{
		for(j = 0; j < knnVecU[i]; j++)
		{
			k = knnMatU[i][j]; 
			knnMatB[k][knnVecB[k]] = i;
			knnMatY[k][knnVecB[k]++] = knnMatX[i][j];
		}
	}
	
	knnMatZ = (double **) malloc(sizeof(double *)*knnDm);
	for(i = 0; i < knnDm; i++)
	{
		knnMatZ[i] = (double *) malloc(sizeof(double)*knnDm);
		
		for(j = 0; j < knnDm; j++) 
			knnMatZ[i][j] = 1.0; 
		
		for(j = 0; j < knnVecU[i]; j++)
		{
			h = knnMatU[i][j]; 
			v = knnMatX[i][j]; 
			
			for(k = 0; k < knnVecB[h]; k++) 
				knnMatZ[i][knnMatB[h][k]] -= v*knnMatY[h][k]; 
		}
		
//		エッジが存在しないので最小値として -1 を代入
		knnMatZ[i][i] = -1.0; 
	}
	
//	ノード i -> ノード j のエッジが最小全域木に含まれるかを示す配列
	knnMatD = (int **) malloc(sizeof(int *)*knnDm);
	for(i = 0; i < knnDm; i++)
	{
		knnMatD[i] = (int *) malloc(sizeof(int)*knnDm);
		
		for(j = 0; j < knnDm; j++) 
			knnMatD[i][j] = 0;
	}
	
//	ノード次数
	knnVecA = (int *) malloc(sizeof(int)*knnDm);
	for(i = 0; i < knnDm; i++) 
		knnVecA[i] = 0; 
	
//	隣接ノードのID
	knnMatA = (int **) malloc(sizeof(int *)*knnDm);
	for(i = 0; i < knnDm; i++) 
		knnMatA[i] = (int *) malloc(sizeof(int)*knnDm);
	
//	距離行列
	knnVecG = (int *) malloc(sizeof(int)*knnDm);
	
//	計算した中心性を格納
	knnVecC = (int *) malloc(sizeof(int)*knnDm);
	for(i = 0; i < knnDm; i++) 
		knnVecC[i] = 0; 
	
//	距離行列の計算に必要な配列
	knnVecT = (int *) malloc(sizeof(int)*knnDm);
}


/* 距離行列計算 (ist230530 p5) */
int	knnIsConnected()
{
	/*
		knnVecG[s] : knnVecG[0] を始点とし、ノード 0:s 間の距離を算出する1次元配列
		knnVecT[j] : 探索対象のノード
		h1, h2  : 探索ノードの範囲で、 h1 ~ h2 までの範囲を探索
	*/
	
	int	d, j, k, h1, h2, s;
	
	for(j = 0; j < knnDm; j++) 
		knnVecG[j] = -1; 
	
	for(d = 1, knnVecG[0] = 0, knnVecT[0] = 0, h1 = 0, h2 = 1; ; d++)
	{
		for(j = h1, h1 = h2; j < h2; j++)
		{
//			繰り返し(k < knnVecT[j] に隣接するノードの数)
			for(k = 0; k < knnVecA[knnVecT[j]]; k++)
			{
//				knnVecT[j] に隣接しており、探索対象であるノードを s に設定
				s = knnMatA[knnVecT[j]][k];
				
//				隣接ノード s を既に探索している場合は処理をスキップ
				if(knnVecG[s] != -1) 
					continue;
				
//				未探索隣接ノード s に knnVecT[j] との距離を設定
				knnVecG[s] = d;
				
//				knnVecT[j] に隣接ノード s を設定 (j == h1)
				knnVecT[h1++] = s;
			}
		}
		
//		全てのノードを探索したか、新たなノードが発見されなくなった場合は深さ優先探索を終了
		if(h1 == knnDm || h1 == h2) 
			break;
		
		j = h1; 
		h1 = h2; 
		h2 = j; 
	}
	
//	全ノードが同一の連結成分に属している
	if(h1 == knnDm) 
		return(1); 
	
//	連結していないノードが残っている
	else 
		return(0);
}


void knnCalValue()
{
	/*
		h       : ノードの高さ
		knnDm      : 語彙種類数
		knnVecA[i] : ノード次数 (ノード i に接続しているエッジ数)
		knnMatA[i] : ノード i の隣接行列
		knnMatD[i] : ノード i が最小全域木に含まれているか示す
		min     : ノード i と隣接するエッジの中で最小の重み
	*/
	
	int		i, j, k, h, x;
	double	min; 
	
	for(h = 0, x = 0; h < knnDm; h++)
	{
		for(i = 0; i < knnDm; i++)
		{
//			高さが h を超えている場合は類似文書の対象外なので処理をスキップ
			if(knnVecC[i] > h) 
				continue; 
			
//			min に対する処理
			for(j = 0, min = -1.0; j < knnDm; j++)
			{
//				エッジが存在しないか、 knnMatZ[i][j] が最小のエッジとして定義されているので処理をスキップ
				if(knnMatZ[i][j] == -1.0) 
					continue; 
				
//				エッジが最小の重みではないので処理をスキップ
				if(min != -1.0 && knnMatZ[i][j] >= min) 
					continue; 
				
//				ノード i と隣接するエッジの中で最小の重みを更新
				min = knnMatZ[i][j]; 
			}
			
//			knnMatD[i][j], knnMatA[i][j], knnMatZ[i][j] に対する処理
			for(j = 0; j < knnDm; j++)
			{
//				エッジが存在しないか、 knnMatZ[i][j] が最小のエッジとして定義されているので処理をスキップ
				if(knnMatZ[i][j] == -1.0) 
					continue; 
				
//				エッジが最小の重みではないので処理をスキップ
				if(knnMatZ[i][j] > min) 
					continue; 
				
//				min == knnMatZ[i][j] && 最小全域木に含まれるエッジが未設定
				if(knnMatD[i][j] == 0)
				{
//					隣接ノード j を設定し、ノード i のエッジ数に +1
					knnMatA[i][knnVecA[i]++] = j;
					
//					ノード i からノード j へのエッジが最小全域木に含まれていることを設定
					knnMatD[i][j] = 1;
					
//					追加されたエッジ数を更新
					x++; 
				}
				
//				min == knnMatZ[j][i] && 最小全域木に含まれるエッジが未設定
				if(knnMatD[j][i] == 0)
				{
//					隣接ノード i を設定
					knnMatA[j][knnVecA[j]++] = i;
					
//					ノード j からノード i へのエッジが最小全域木に含まれていることを設定
					knnMatD[j][i] = 1;
					
//					追加されたエッジの数を更新
					x++; 
				}
				
//				最小の重みを持つエッジに -1 を代入することで、最小の重みであることを設定
				knnMatZ[i][j] = -1.0; 
				
//				最小全域木のノードとして選択された回数を更新
				knnVecC[i] += 1; 
			}
		}
		
//		全ノードが同一の連結成分に属している場合はグラフ構築が完了しているためループ終了
		if(knnIsConnected() == 1) 
			break;
	}
}


/* knn3000k100.txt に各ノード間の繋がりを示す隣接行列を出力 */
void knnPrintValue(char *fn1)
{
	/*
		h       : ノードの高さ
		knnDm      : 検索文書数(100)
		knnVecA[i] : ノード次数 (ノード i に接続しているエッジ数)
		knnMatA[i] : ノード i の隣接行列
		knnMatD[i] : ノード i が最小全域木に含まれているか示す
		min     : ノード i と隣接するエッジの中で最小の重み
	*/
	
	FILE *fp;
	int	  i, j;
	
	fp = fopen(fn1, "w");
	fprintf(fp, "%d %d 1\n", knnDm, knnDm); 
	
	for(i = 0; i < knnDm; i++)
	{
//		ノード i に接続しているエッジ数を出力
		fprintf(fp, "%d ", knnVecA[i]);
		
//		隣接ノードを隣接行列として出力
		for(j = 0; j < knnVecA[i]; j++) 
			fprintf(fp, "%d:1 ", knnMatA[i][j]+1);
		
		fprintf(fp, "1 1\n");
	}
	
	fclose(fp);
}


int	knn(char **argv)
{
	knnReadValue(argv[0]);
	knnInitValue(); 
	knnCalValue(); 
	knnPrintValue(argv[1]); 
	return 0; 
}
