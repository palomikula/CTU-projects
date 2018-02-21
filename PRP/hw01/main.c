#include <stdio.h>
#include <stdlib.h>

/* The main program */
int main()
{

	int pole[50];
	float k=0,z=0,s=0,l=0,sum=0;
	int i=0,mx,mn;
	float pk,pz,ps,pl,p;
    	while(scanf("%d ", &pole[i]) == 1)
	{
		i++;
	}
	mn=pole[0];
	mx=pole[0];
	for(int j=0; j!= i;j++)
	{
		if(pole[j]>0) // kladne
		{
			k++;
		}
		if(pole[j]<0) //zaporne
		{
			z++;
		}
		if(pole[j]%2==0) //sude
		{
			s++;
		}
		else  //liche
		{
			l++;
		}
		if(pole[j]<-10000 || pole[j]>10000)
		{
			printf("\nError: Vstup je mimo interval!\n");
			return 100;
		}
		else
		{
			if(j==i-1)
			{
				printf("%d\n", pole[j]);

			}
			else
			{
				if(pole[j+1]<-10000 || pole[j+1]>10000)
				{
					printf("%d", pole[j]);
				}
				else
				{
					printf("%d, ",pole[j]);
				}
			}
		}
		sum += pole[j];
		if(pole[j]>mx)
		{
			mx = pole[j];
		}
		if(pole[j]<mn)
		{
			mn = pole[j];
		}
	}
	pk = k*100/i;
	pz = z*100/i;
	ps = s*100/i;
	pl = l*100/i;
	p = sum/i;
	printf("Pocet cisel: %d\n", i);
	printf("Pocet kladnych: %.0f\n", k);
	printf("Pocet zapornych: %.0f\n", z);
	printf("Procento kladnych: %.2f\n", pk);
	printf("Procento zapornych: %.2f\n", pz);
	printf("Pocet sudych: %.0f\n", s);
	printf("Pocet lichych: %.0f\n", l);
	printf("Procento sudych: %.2f\n", ps);
	printf("Procento lichych: %.2f\n", pl);
	printf("Prumer: %.2f\n", p);
	printf("Maximum: %d\n", mx);
	printf("Minimum: %d\n", mn);
    return 0;

}
