#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<time.h>

int		hmlDm, hmlDn, hmlDh, *hmlVecU, **hmlMatU, *hmlVecA, **hmlMatA, *hmlVecB, **hmlMatB, *hmlVecC, *hmlVecD, *hmlVecI, *hmlVecJ, *hmlVecS, *hmlVecT;
double	**hmlMatX, *hmlVecV, *hmlVecW, *hmlVecX;

void    hmlReadValue1(const char *fn1)
{
	FILE		*fp;
	int		i, j, k;
	double	v; 
	if((fp = fopen(fn1, "r")) == NULL){ fprintf(stderr, "Unknown File = %s\n", fn1); exit(1);}
	fscanf(fp, "%d %d", &hmlDm, &hmlDn); 
	while((j = getc(fp)) != EOF) if(j == '\n') break; 
	hmlVecU = (int *) malloc(sizeof(int)*hmlDm);
	hmlMatU = (int **) malloc(sizeof(int *)*hmlDm);
	hmlMatX = (double **) malloc(sizeof(double *)*hmlDm);
	for(i = 0; i < hmlDm; i++){
		fscanf(fp, "%d", &hmlVecU[i]);
		hmlMatU[i] = (int *) malloc(sizeof(int)*hmlVecU[i]);
		hmlMatX[i] = (double *) malloc(sizeof(double)*hmlVecU[i]);
		for(j = 0; j < hmlVecU[i]; j++){ 
			fscanf(fp, "%d:%lf", &k, &v);
			hmlMatU[i][j] = k-1;
			hmlMatX[i][j] = v;
		}
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
}
void    hmlReadValue2(const char *fn1)
{
	FILE		*fp;
	int		i, j, k, h;
	if((fp = fopen(fn1, "r")) == NULL) { fprintf(stderr, "Unknown File = %s\n", fn1); exit(1);}
	while((j = getc(fp)) != EOF) if(j == '\n') break; 
	hmlVecA = (int *) malloc(sizeof(int)*hmlDm);
	hmlMatA = (int **) malloc(sizeof(int *)*hmlDm);
	for(i = 0; i < hmlDm; i++){
		fscanf(fp, "%d", &hmlVecA[i]); 
		hmlMatA[i] = (int *) malloc(sizeof(int)*hmlVecA[i]);
		for(j = 0; j < hmlVecA[i]; j++){ fscanf(fp, "%d:%d", &k, &h); hmlMatA[i][j] = k-1; }
		while((j = getc(fp)) != EOF) if(j == '\n') break; 
	}
	fclose(fp);
}
void	hmlInitValue()
{
	int		h, i, j, k;
	double	v, w;
	for(i = 0; i < hmlDm; i++){
		for(j = 0, v = 0.0; j < hmlVecU[i]; j++) v += hmlMatX[i][j]*hmlMatX[i][j];
		for(j = 0, v = 1.0/sqrt(v); j < hmlVecU[i]; j++) hmlMatX[i][j] *= v;
	}
	hmlVecX = (double *) malloc(sizeof(double)*hmlDn);
	for(j = 0; j < hmlDn; j++) hmlVecX[j] = 0.0; 
	hmlVecD = (int *) malloc(sizeof(int)*hmlDm);
	hmlVecI = (int *) malloc(sizeof(int)*(hmlDh+hmlDm));
	hmlVecJ = (int *) malloc(sizeof(int)*(hmlDh+hmlDm));
	hmlVecV = (double *) malloc(sizeof(double)*(hmlDh+hmlDm));
	hmlVecW = (double *) malloc(sizeof(double)*(hmlDh+hmlDm));
	hmlVecS = (int *) malloc(sizeof(int)*(hmlDh+hmlDm));
	hmlVecT = (int *) malloc(sizeof(int)*(hmlDh+hmlDm));
}
void	hmlMySort(int *sp, int *tp, double *vp, int n)
{
	int		i, j, k, h;
	if((h = n/2) == 0) return;
	hmlMySort(&sp[0], &tp[0], &vp[0], h);
	hmlMySort(&sp[h], &tp[h], &vp[h], n-h); 
	for(i = j = 0, k = h; i < n; i++)
		if(j < h && (k == n || vp[j] >= vp[k])){ hmlVecI[i] = sp[j]; hmlVecJ[i] = tp[j]; hmlVecW[i] = vp[j++]; } 
		else { hmlVecI[i] = sp[k]; hmlVecJ[i] = tp[k]; hmlVecW[i] = vp[k++]; }
	for(i = 0; i < n; i++){ sp[i] = hmlVecI[i]; tp[i] = hmlVecJ[i]; vp[i] = hmlVecW[i]; }
}
void	hmlCalValue()
{
	int		h, i, j, k, n;
	double	v; 
	for(j = 0; j < hmlDm; j++) hmlVecD[j] = 0; 
	for(i = n = 0; i < hmlDm-1; i++){ 
		for(j = 0; j < hmlVecA[i]; j++) hmlVecD[hmlMatA[i][j]] = 1; 
		for(k = 0; k < hmlVecU[i]; k++) hmlVecX[hmlMatU[i][k]] = hmlMatX[i][k];
		for(j = i+1; j < hmlDm; j++, n++){ 
			if(hmlVecD[j] ==  1) continue; 
			hmlVecS[n] = i; hmlVecT[n] = j; 
			for(k = 0, v = 0.0; k < hmlVecU[j]; k++) v += hmlVecX[hmlMatU[j][k]]*hmlMatX[j][k];
			hmlVecV[n] = v; 
		}
		for(j = 0; j < hmlVecA[i]; j++) hmlVecD[hmlMatA[i][j]] = 0; 
		for(k = 0; k < hmlVecU[i]; k++) hmlVecX[hmlMatU[i][k]] = 0.0;
		hmlMySort(hmlVecS, hmlVecT, hmlVecV, n); if(n > hmlDh) n = hmlDh; 
	}
}
void	hmlPrintValue(const char *fn1)
{
	FILE	*fp;
	int		g, h, i, j, u, v, c;
	double	z; 
	hmlVecB = (int *) malloc(sizeof(int)*hmlDm);
	for(j = 0; j < hmlDm; j++) hmlVecB[j] = 0; 
	for(i = 0; i < hmlDh; i++){ hmlVecB[hmlVecS[i]] += 1; hmlVecB[hmlVecT[i]] += 1; }
	hmlMatB = (int **) malloc(sizeof(int *)*hmlDm);
	for(j = 0; j < hmlDm; j++){ 
		hmlMatB[j] = (int *) malloc(sizeof(int)*hmlVecB[j]);
		hmlVecB[j] = 0; 
	}
	for(h = 0; h < hmlDh; h++){ 
		u = hmlVecS[h]; v = hmlVecT[h]; z = hmlVecV[h]; hmlMatB[u][hmlVecB[u]++] = v; hmlMatB[v][hmlVecB[v]++] = u; 
	}
	fp = fopen(fn1, "w");
	fprintf(fp, "%d %d 1\n", hmlDm, hmlDm); 
	for(i = 0; i < hmlDm; i++){
		fprintf(fp, "%d ", hmlVecA[i]+hmlVecB[i]); 
		for(j = 0; j < hmlVecA[i]; j++, g++){ 
			fprintf(fp, "%d:1 ", hmlMatA[i][j]+1); 
		}
		for(j = 0; j < hmlVecB[i]; j++){ 
			fprintf(fp, "%d:2 ", hmlMatB[i][j]+1); 
		}
		fprintf(fp, "1 1\n"); 
	}
	fclose(fp); 
}
int		hml(const char **argv)
{
	hmlReadValue1(argv[0]);
	hmlReadValue2(argv[1]);
	hmlDh = atoi(argv[2])*(hmlDm-1); 
	hmlInitValue();
	hmlCalValue();
	hmlPrintValue(argv[3]);
	return 0;
}
