#include <stdio.h>
#include <stdlib.h>


int a,b,i,j,k,l,m,n,tmp;
int *ptr1[100];
int *ptr2[100];
int *tmp_mat;
int *get_matrix(int *r, int *c)
{
    scanf("%d %d",r,c);

    int *mat;
    mat=(int*)malloc(*r * *c * sizeof(int));
    for(i=0;i<*r;i++){
        for(j=0;j<*c;j++){
            if((scanf("%d",&mat[i * *c+j]))==0)
            {
                free(mat);
                return NULL;
            }
        }
    }
    return mat;
}
int *get_result(int *mat1,char op,int *mat2, int r1, int c1, int r2, int c2)
{
    tmp_mat=(int*)malloc(10000*sizeof(int));
    switch(op){

        case '+':
			for (l=0;l<r2;l++){
				for(m=0;m<c1;m++){
                    //printf("%d + %d = %d\n",mat1[l*c1+m],mat2[l*c1+m],mat1[l*c1+m]+mat2[l*c1+m]);
					mat2[l*c1+m]=mat1[l*c1+m]+mat2[l*c1+m];
					ptr2[i]=tmp_mat;
				}
			}
            return mat2;

        case '-':
			for (l=0;l<r1;l++){
				for(m=0;m<c1;m++){
                    //printf("%d - %d = %d\n",mat1[l*c1+m],mat2[l*c1+m],mat1[l*c1+m]-mat2[l*c1+m]);
					mat2[l*c1+m]=mat1[l*c1+m]-mat2[l*c1+m];
					ptr2[i]=tmp_mat;
				}
			}
            return mat2;

        case '*':
            n=0;
			for (l=0;l<r1;l++){
                if(c1!=r2){break;}
                for(n=0;n<c2;n++){
                    for(tmp=0,m=0;m<c1;m++){
                        //printf("%d * %d = %d\n",mat1[l*c1+m],mat2[m*c2+n],mat1[l*c1+m]*mat2[m*c2+n]);
                        tmp+=mat1[l*c1+m]*mat2[m*c2+n];
                    }

                    tmp_mat[l*c2+n]=tmp;
                }
			}
            mat2=tmp_mat;
            ptr2[i]=tmp_mat;

            return mat2;
    }
    return NULL;
}

void print_matrix(int *mat,int r,int c)
{
    for(a=0;a<r;a++){
        for(b=0;b<c;b++){
            printf("%d",mat[a*c+b]);
            if(b+1<c){
            printf(" ");
            }
        }
        printf("\n");
    }
}

int main()
{
    int r,c;
    int mat_r[100];
    int mat_c[100];
    int *mat_arr[100];
    char op[100];
    for(k=0;(mat_arr[k]=get_matrix(&r,&c))!=NULL;++k)
    {

        mat_r[k]=r;
        mat_c[k]=c;
        ptr1[k]=mat_arr[k];
        if(scanf("%s",&op[k])==EOF)
        {
            break;
        }
    }
    if(mat_arr[k]==NULL)
        {
            fprintf(stderr,"Error: Chybny vstup!\n");
            return 100;
        }
    int *result;
    for(i=0;i<k;++i)
    {
        if(op[i]!='*' && op[i+1]=='*')
        {

            ptr2[i] = mat_arr[i+2];
            mat_arr[i+2]=get_result(mat_arr[i+1],op[i+1],mat_arr[i+2],mat_r[i+1],mat_c[i+1],mat_r[i+2],mat_c[i+2]);
            result=mat_arr[i+2];
            if(mat_c[i+1]!=mat_r[i+2])
            {
                for(i=0;i<k;i++)
                {
                    free(ptr1[i]);
                    free(ptr2[i]);
                }
                fprintf(stderr,"Error: Chybny vstup!\n");
                return 100;
            }
            mat_r[i+2]=mat_r[i+1];
            mat_arr[i+1]=mat_arr[i];
            op[i+1]=op[i];
            mat_r[i+1]=mat_r[i];
            mat_c[i+1]=mat_c[i];


        }
        else
        {
            ptr2[i] = mat_arr[i+1];
            mat_arr[i+1]=get_result(mat_arr[i],op[i],mat_arr[i+1],mat_r[i],mat_c[i],mat_r[i+1],mat_c[i+1]);

            result=mat_arr[i+1];
            if(op[i]=='*')
            {
                if(mat_c[i]!=mat_r[i+1])
                {
                    for(i=0;i<=k;i++)
                    {
                        free(ptr1[i]);
                        free(ptr2[i]);
                    }
                    fprintf(stderr,"Error: Chybny vstup!\n");
                    return 100;
                }
                mat_r[i+1]=mat_r[i];

            }

        }


    }
    printf("%d %d\n",mat_r[i],mat_c[i]);
    print_matrix(result,mat_r[i],mat_c[i]);


    for(i=0;i<=k;i++)
    {
        free(ptr1[i]);
        free(ptr2[i]);
    }

    return 0;
}
