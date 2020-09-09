/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double ** transpose(double** ori, int r, int c){
	
	double **trans = (double **)malloc(c*sizeof(double*));
        int i,j;
        for(i=0; i<c; i++) {
                trans[i] = (double *)malloc(r*sizeof(double));
        }
        for(i=0; i<c; i++) {
                for(j=0; j<r; j++) {
                        trans[i][j] = ori[j][i];
                }
        }
	return trans;
}

double ** multiply(double** a, double** b, int r1, int c1, int r2, int c2){
	int i, j, k;
	if(c1!=r2){
          return NULL;
        }
	double** res = (double**)malloc(r1*sizeof(double*));
	for(i=0; i<r1; i++) {
                res[i] = (double *)malloc(c2*sizeof(double));
        }
	for( i = 0; i < r1; i++ ) {
    		for( j = 0; j < c2; j++ ) {
			(res)[i][j] = 0;
      			for( k = 0; k < c1; k++ ) {
        			res[i][j] += a[i][k] * b[k][j];
      			}
    		}
  	}
	return res;
}

double** divide(double** ori, int i, int j,int r,int c){
	int o,k;
	double factor;
	if(ori[i][j]==0){
		for(o=1;o<r;o++){
			if(ori[o][j]!=0){
				for(k=0;k<c;k++){
					ori[i][k]=ori[i][k]+ori[o][k];
				}
				return ori;
			}
		}
		return NULL;
	}else{
		factor=ori[i][j];
		for(k=0;k<c;k++){
			ori[i][k]=ori[i][k]/factor;
		}
		return ori;
	}
	return NULL;
}

double ** inverse(double **m, int r, int c){
	int i, j, k, o;
	double factor;
	double **res = (double **)malloc(r*sizeof(double*));
        for(i=0; i<r; i++) {
                res[i] = (double *)malloc((c*2)*sizeof(double));
        }

	for(i=0;i<r;i++){
		for(j=0;j<c;j++){
			res[i][j]=m[i][j];
		}
	}	
	for(i = 0; i <r; i++){
		for(j = c; j <c*2; j++){
			if(j-c == i)
				res[i][j] = 1;
			else
				res[i][j]=0;
		}
	}

	for(i = 0; i < r; i++){
		for(j = 0; j <c; j++){
			if(i==j){
				if(res[i][j] != 1){
					res = divide(res, i, j, r, c*2);
					for(k=i+1; k<r; k++){
						if(res[i][i]!=0){
							factor=res[k][j];
							for(o=0;o<c*2;o++){
								res[k][o]=res[k][o]/factor;
							}	
							for(o=0;o<c*2;o++){
								res[k][o]=res[k][o]-res[j][o];
							}
						}
					}
				}
			}
		}
	}

	for(i = r-1; i >=0 ; i--){
		for(j = c-1; j>=0; j--){
			if(i!=j&&res[i][j]!=0){
				factor=res[i][j];
				for(k=0;k<c*2;k++){
					res[i][k]=res[i][k]-(res[j][k]*factor);
				}
			}
		}
	}

	double **invert = (double **)malloc((r)*sizeof(double*));
	for(i=0; i<r; i++) {
                invert[i] = (double *)malloc(c*sizeof(double));
	}
	for(i = 0; i<r; i++){
		for(j = 0; j < c; j++){
			invert[i][j] = res[i][j+c];
		}
        }
	return invert;
}

void print(double** matrix,int r,int c){
	int i,j;
	for(i=0;i<r;i++){
		for(j=0;j<c;j++){
			printf("%0.0f",matrix[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	char t[6];
	strcpy(t, "train");
	char date[5];
	strcpy(date, "data");


	FILE *fp = fopen(argv[1], "r");
	char title[10];
	fscanf(fp, "%s\n", title);
	if(strncmp(title, t, 5) != 0)
		return EXIT_FAILURE;
	int k =0;
	fscanf(fp, "%d\n", &k);
	int n = 0;
	fscanf(fp, "%d\n", &n);
	
	double **x = (double **)malloc(n*sizeof(double*));
        int i, j;
        for(i=0; i<n; i++) {
                x[i] = (double *)malloc((k+1)*sizeof(double));
        }
	
	double **y = (double **)malloc(n*sizeof(double*));
	for(i=0; i<n; i++) {
                y[i] = (double *)malloc(1*sizeof(double));
        }

        int r,c;
        for(r=0; r<n; r++) {
		x[r][0] = 1;
                for(c=1; c<k+1; c++) {
                        fscanf(fp,"%lf ",&x[r][c]);
                }
		fscanf(fp,"%lf ",&y[r][0]);
		fscanf(fp,"\n");
        }
	fclose(fp);

	double **xtransposed;
	xtransposed = transpose(x, n, k+1);
	double **XtY = multiply(xtransposed, y, k+1, n, n, 1);
	double **XtX = multiply(xtransposed, x, k+1, n, n, k+1);
	double **xinverse = inverse(XtX, k+1, k+1);
	double **w = multiply(xinverse, XtY, k+1, k+1, k+1, 1);

	FILE* data = fopen(argv[2], "r");
	fscanf(data, "%s\n", title);
	if(strncmp(title, date, 4) != 0)
		return EXIT_FAILURE;
	int dataN=0;
	fscanf(data, "%d\n", &dataN);
	int dataP=0;
	fscanf(data, "%d\n", &dataP);
	

	//error case that will be check. K's need to be the same
	if(dataN !=k){
		printf("error\n");
		return 1;
	}

	double ** d = (double**)malloc(dataP*sizeof(double*));
	for(i=0;i<dataP;i++){
              d[i]=(double*)malloc((dataN+1)*sizeof(double));
	}

	for(i=0;i<dataP;i++){
		d[i][0] = 1;
		for(j=1;j<dataN+1;j++){
			fscanf(data,"%lf",&d[i][j]);
		}
		fscanf(data,"\n");
	}
	fclose(data);
	
	int o;
	double** final=(double**)malloc(dataP*sizeof(double*));
		for(o=0;o<dataP;o++){
		final[o]=(double*)malloc(1*sizeof(double));
	}
	final = multiply(d, w, dataP,dataN+1, k+1, 1);
	print(final, dataP, 1);
	
	free(x);
	free(y);
	free(w);
	free(xtransposed);
	free(XtX);
	free(XtY);
	free(xinverse);
	free(final);
	return 0;
}
