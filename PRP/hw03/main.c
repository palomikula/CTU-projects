#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

long long mx=0,vstup[10000];
long long i,j,k,l,n;
short int pocet_vstupov;
char kontrola_vstupu;
int main(void)
{
	for(i=0;;++i)
	{
        vstup[i]=-1;
        scanf("%lld",&vstup[i]);
        if(vstup[i]>mx)
        {
            mx=vstup[i];
        }
		if(vstup[i]<=0)
		{
            if(vstup[i]<0)
            {
                kontrola_vstupu='X';
            }
			break;
		}
	}


	pocet_vstupov=i;
	//printf("%lld",mx);
	mx = (int)sqrt(mx);
	//printf("%lld",mx);
	int *A;
	A= (int*)malloc((mx+1)*sizeof(int));
	int *prvocisla;
	prvocisla=(int*)malloc((mx+1)*sizeof(int));
    for(i=0;i<=mx;++i)
    {
        A[i]=1;
    }

    A[0]=0;
    A[1]=0;
    for(i=0;i<=mx;++i)
    {
        if(A[i]==1)
        {
            for(j=i*i,k=1;j<=mx;j=i*i+k*i,++k)
            {
                A[j]=0;
            }
        }
    }
    for(i=0,l=0;i<=mx;++i)
    {
        if(A[i]==1)
        {
            prvocisla[l]=i;
           // printf("%lld, ",prvocisla[l]);
            ++l;
        }
    }
    int x =l;


    free(A);

    for(i=0;i!=pocet_vstupov;++i)
    {
		if(vstup[i]==1)
		{
			printf("Prvociselny rozklad cisla 1 je:\n1\n");
		}
		if(vstup[i]==2)
		{
			printf("Prvociselny rozklad cisla 2 je:\n2\n");
		}
		if(vstup[i]==3)
		{
			printf("Prvociselny rozklad cisla 3 je:\n3\n");
		}
		if(vstup[i]!=1 && vstup[i]!=2 && vstup[i]!=3)
		{
		    printf("Prvociselny rozklad cisla %lld je:\n",vstup[i]);
		    for(l=0;vstup[i]>1;++l)
		    {
		        for(j=0;vstup[i]%prvocisla[l]==0;++j)
		        {
		            vstup[i]=vstup[i]/prvocisla[l];

		        }
		        if(x==l+1)
		        {
		            prvocisla[l+1]=vstup[i];
		        }
		        if(j==1)
		        {
		            if(vstup[i]==1)
		            {
		                printf("%d\n",prvocisla[l]);
		            }
		            else
		            {
		                printf("%d x ",prvocisla[l]);
		            }
		        }
		        else if(j!=0)
		        {
		            if(vstup[i]==1)
		            {
		                printf("%d^%lld\n",prvocisla[l],j);
		            }
		            else
		            {
		                printf("%d^%lld x ",prvocisla[l],j);
		            }
		         }
		       }
			}
    }
	free(prvocisla);
    if(kontrola_vstupu=='X')
    {
        fprintf(stderr, "Error: Chybny vstup!\n");
        return 100;
    }
  return 0;
}

