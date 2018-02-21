#include <stdio.h>
#include <stdlib.h>

int mn;

typedef struct{
    int R;
    int T;
}sum;

sum *sum_mat;

typedef struct {
	int ulx;
	int uly;
	int lrx;
	int lry;
}sq;

sum plus(sum s1, sum s2){

    sum s3;
    s3.R = s1.R + s2.R;
    s3.T = s1.T + s2.T;

    return s3;
}

sum minus(sum s1, sum s2){

    sum s3;
    s3.R = s1.R - s2.R;
    s3.T = s1.T - s2.T;

    return s3;
}

int min(int a, int b)
{
    return (a<b) ? a : b;
}

int check_sq(sq cursq, int M, int N)
{
    sum tmp1,tmp2,tmp3,tmp4,result;
    if(cursq.ulx > 0 && cursq.uly > 0){
        tmp1 = plus(sum_mat[cursq.lrx + N*cursq.lry], sum_mat[cursq.ulx-1+N*(cursq.uly-1)]);
        tmp2 = plus(sum_mat[cursq.lrx + N*(cursq.uly-1)], sum_mat[cursq.ulx-1+N*cursq.lry]);
    }else if(cursq.ulx > 0){
        tmp1 = sum_mat[cursq.lrx + N*cursq.lry];
        tmp2 = sum_mat[cursq.ulx-1 + N*cursq.lry];
    }else if(cursq.uly > 0){
        tmp1 = sum_mat[cursq.lrx + N*cursq.lry];
        tmp2 = sum_mat[cursq.lrx + N*(cursq.uly-1)];
    }else{
        tmp1 = sum_mat[cursq.lrx + N*cursq.lry];
        tmp2.R = 0;
        tmp2.T = 0;
    }
    tmp3 = plus(sum_mat[cursq.lrx-1 + N*(cursq.lry-1)], sum_mat[cursq.ulx+N*cursq.uly]);
    tmp4 = plus(sum_mat[cursq.lrx-1 + N*(cursq.uly)], sum_mat[cursq.ulx+N*(cursq.lry-1)]);

    result = minus(minus(tmp1,tmp2),minus(tmp3,tmp4));

    if(result.R >= 2*result.T){

        return result.R;

    }else{

        return -1;
    }
}

sq next_sq(sq cursq, int M, int N)
{
    if((cursq.lrx+1) < N){

        cursq.ulx++;
        cursq.lrx++;

    }else if ((cursq.lry+1) < M){

        cursq.ulx = 0;
        cursq.lrx = cursq.lry-cursq.uly;
        cursq.uly++;
        cursq.lry++;

    }else{
        cursq.ulx = 0;
        cursq.lrx = cursq.lry-cursq.uly-1;
        cursq.uly = 0;
        cursq.lry = cursq.lrx;
    }

    return cursq;

}

void *get_matrix(int *M, int *N)
{
    if(scanf("%d %d",M,N)==-1) return NULL;
    sum line;
    int *mat;
	int i,j;
    mat=(int*)malloc(*M * *N * sizeof(int));
    sum_mat=(sum*)malloc(*M * *N * sizeof(sum));
    for(i=0;i<*M;i++){
        line.R=0;
        line.T=0;
        for(j=0;j<*N;j++){
            if((scanf("%d",&mat[i * *N+j]))==0)
            {
                free(mat);
                return NULL;
            }
            if(mat[i * *N+j]==1)line.R++;
            if(mat[i * *N+j]==2)line.T++;
            if(i>0){
                sum_mat[i * *N+j] = plus(sum_mat[(i-1) * *N+j], line);
            }
            else{
                sum_mat[i * *N+j].R = line.R;
                sum_mat[i * *N+j].T = line.T;
            }
            //printf("%d ",sum_mat[i * *N+j].R);
        }
        //printf("\n");
    }
}

int main(){

    int M,N,tmp,max;
    sq cursq;
    get_matrix(&M,&N);
    mn = min(M,N);
    cursq.ulx = 0;
	cursq.uly = 0;
	cursq.lrx = mn-1;
	cursq.lry = mn-1;

    while((cursq.lrx - cursq.ulx)*4 > max){
        tmp = check_sq(cursq, M, N);
        if(tmp>max){
            max = tmp;
        }
        /*printf("ulx = %d\n",cursq.ulx);
        printf("uly = %d\n",cursq.uly);
        printf("lrx = %d\n",cursq.lrx);
        printf("lry = %d\n",cursq.lry);
        printf("next square\n");*/
        cursq = next_sq(cursq, M, N);
	}
	printf("%d\n",max);

    return 0;
}
