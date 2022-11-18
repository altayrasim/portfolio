#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


double* scale(double** inputLeft, double* inputRight, int row, int col, double* output){
    for (int i=0; i<row; ++i) {
        for (int j=0; j<col; ++j) {
            output[i] += inputLeft[i][j] * inputRight[j];
      }
    }
    return output;
}
double** multiply(double** inputLeft, double** inputRight, int row, int col, int colLeft, double** output){
   for (int i=0; i<row; i++) {
      for (int j=0; j<col; j++) {
         for (int a=0; a<colLeft; a++) {
            output[i][j] += inputLeft[i][a] * inputRight[a][j];
         }
      }
   }
   return output;
}
double** transpose(double** input, int row, int col, double** output){
    for(int i=0;i<row;i++){
        for(int j=0; j<col;j++){
            output[j][i]=input[i][j];
        }
    }
return output;
}
double** inverse(double** M, double** N, int n){
    double f = 0;
    for(int p=0; p<n; p++){
        f=M[p][p];
        for(int k=0; k<n;k++){//divide M[p] and N[p] by f
        M[p][k] = M[p][k] / f;
        N[p][k] = N[p][k] / f;
        }
        for(int i=p+1;i<n;i++){
            f=M[i][p];
            for(int k=0; k<n;k++){//subtract M[p] X f from M[i] and N[p] from N[i];
            M[i][k] = M[i][k] - (M[p][k]*f);
            N[i][k] = N[i][k] - (N[p][k]*f);
            }
        }
    }
    for(int p = n-1;p>=0;p--){
        for(int i=p-1;i>=0;i--){
            f = M[i][p];
            for(int k=0; k<n;k++){//subtract M[p] X f from M[i] and N[p] X f from N[i];
            M[i][k] = M[i][k] - (M[p][k]*f);
            N[i][k] = N[i][k] - (N[p][k]*f);
            }
        }
    }
    return N;
}



int main(int argc, char** argv){
    int wop;
    if (argc==3){
        FILE *fileTrain=fopen(argv[1],"r");
        FILE *fileData=fopen(argv[2],"r");
        int k; //number of attributes
        int n; //number of houses, train
        int m; //number of houses, data
        double** matrixTrain;
        double** matrixData;
        double* matrixY;

        if(fileTrain!=NULL){
            fscanf(fileTrain, "%*s");
            fscanf(fileTrain, "%d",&k);
            fscanf(fileTrain, "%d",&n);
            wop=k;
            matrixTrain = malloc(sizeof(double*)*n);
            for(int i=0;i<n;i++){
                matrixTrain[i] = malloc(sizeof(double)*(k+1));
            }
            matrixY = malloc(sizeof(double*)*n);
            for(int i=0;i<n;i++){
                for(int j=0;j<=k+1;j++){
                    if(j==0){
                        matrixTrain[i][j]=1;
                    }
                    else if(j==k+1){
                    fscanf(fileTrain, "%lf", &matrixY[i]);
                    }
                    else{
                    fscanf(fileTrain, "%lf", &matrixTrain[i][j]);
                    }
                }
            }
        }
        if(fileData!=NULL){
            fscanf(fileData, "%*s");
            fscanf(fileData, "%d",&k);
            fscanf(fileData, "%d",&m);
            if(wop!=k){
                printf("ERROR\n");
                return 1;
            }
            matrixData = malloc(sizeof(double*)*m);
            for(int i=0;i<m;i++){
                matrixData[i] = malloc(sizeof(double)*(k+1));
            }
            for(int i=0;i<m;i++){
                for(int j=0;j<k+1;j++){
                     if(j==0){
                        matrixData[i][j]=1;
                    }
                    else
                    fscanf(fileData, "%lf", &matrixData[i][j]);
                }
            }
        }

            double** transX = malloc(sizeof(double*)*(k+1));
                                    for(int i=0;i<k+1;i++){
                                        transX[i] = malloc(sizeof(double)*(n));
                                        for(int j=0;j<n;j++)
                                        transX[i][j]=0;
                                    }
            transX = transpose(matrixTrain, n, k+1, transX);
            double** multX = malloc(sizeof(double*)*(k+1));
                                    for(int i=0;i<k+1;i++){
                                        multX[i] = malloc(sizeof(double)*(k+1));
                                        for(int j=0;j<k+1;j++)
                                        multX[i][j]=0;
                                    }
            multX = multiply(transX, matrixTrain, k+1, k+1, n, multX);
            double** invX = malloc(sizeof(double*)*(k+1));
                                    for(int i=0;i<k+1;i++){
                                        invX[i] = malloc(sizeof(double)*(k+1));
                                        for(int j=0;j<k+1;j++){
                                            if(i==j)
                                            invX[i][j]=1;
                                            else
                                            invX[i][j]=0;
                                        }
                                    }
            invX = inverse(multX, invX, k+1);
            double** finalX = malloc(sizeof(double*)*(k+1));
                                    for(int i=0;i<k+1;i++){
                                        finalX[i] = malloc(sizeof(double)*n);
                                        for(int j=0;j<n;j++)
                                        finalX[i][j]=0;
                                    }
            finalX = multiply(invX, transX, k+1, n, k+1, finalX);
            double* matrixW = malloc(sizeof(double*)*(k+1));
                                    for(int i=0;i<k+1;i++)
                                        matrixW[i]=0;
            matrixW = scale(finalX, matrixY, k+1, n, matrixW);
            double* prices = malloc(sizeof(double*)*m);
                                    for(int i=0;i<m;i++){
                                        prices[i]=0;
                                    }
            prices = scale(matrixData, matrixW, m, k+1, prices);



            for(int i=0;i<m;i++){
                printf("%.0f\n", prices[i]);
            }



                for(int i=0;i<n;i++){
                free(matrixTrain[i]);
                }
                free(matrixTrain);
                free(matrixY);
                for(int i=0;i<m;i++){
                free(matrixData[i]);
                }
                free(matrixData);
                for(int i=0;i<k+1;i++){
                free(transX[i]);
                }
                free(transX);
                for(int i=0;i<k+1;i++){
                free(multX[i]);
                }
                free(multX);
                for(int i=0;i<k+1;i++){
                free(invX[i]);
                }
                free(invX);
                for(int i=0;i<k+1;i++){
                free(finalX[i]);
                }
                free(finalX);
                free(matrixW);
                free(prices);

    }
    else{
        printf("ERROR\n");
        return 1;
    }
    return 0;
}  