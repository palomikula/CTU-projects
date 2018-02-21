#include <stdio.h>
#include <stdlib.h>

/* The main program */
int main(int argc, char *argv[])
{
int c1=0,c2=0,c3=10001,i,j;
scanf("%d %d", &c1, &c2);

if(c1==0 || c2==0)
    {
        fprintf(stderr,"Error: Chybny vstup!\n");
        return 100;
    }

if((c1 < 3 || c2 < 3 || c1 > 69 || c2 > 69) && (c1!=0 && c2!=0))
	{
	    fprintf(stderr,"Error: Vstup mimo interval!\n");
	    return 101;
	}
if(c1%2 == 0)
    {
        fprintf(stderr,"Error: Sirka neni liche cislo!\n");
        return 102;
    }


else 
    {
	if(c1==c2)
	    {
		   scanf("%d",&c3);

		   if(c3==10001)
		   {
		       fprintf(stderr,"Error: Chybny vstup!\n");
		       return 100;
		   }
		   if(c3>=c2 || c3<=0)
		   {
			  fprintf(stderr,"Error: Neplatna velikost plotu!\n");   
			  return 103;
		   }        
	    }
        for(i=1; i<=c1/2; i++)  //roof
        {
            printf(" ");
        }
        printf("X\n");
        for(j=1; j!=c1/2;j++)
        {
            for(i=1; i<=c1/2-j; i++)
            {
                printf(" ");   
            }
            printf("X");
            for(i=1; i<=1+2*(j-1); i++)
            {
                printf(" ");   
            }
            printf("X\n");
        }
        
        for(i=1; i<=c1;i++)
        {
            printf("X");  
        }
        printf("\n");
        
        if(c1!=c2)
        {
            for(j=1; j!=c2-1;j++)   //"box"
            {
                printf("X");
                for(i=1; i<=c1-2; i++)
                {
                    printf(" ");   
                }
                printf("X\n");
            }
            for(i=1; i<=c1;i++)
            {
                printf("X");  
            }
	    printf("\n");
        }
        if(c1==c2)
        {
            for(j=1; j!=c2-1;j++)   //"box on steroids"
            {
                printf("X");
                if(c1==3)
                {
                    printf("o");
                }
                else
                {
                    if(j%2 == 1)
                    {    
                        for(i=1; i<=c1/2-1; i++)
                        {
                            printf("o*");   
                        }
                        printf("o");
                    }
                    else
                    {
                        for(i=1; i<=c1/2-1; i++)
                        {
                            printf("*o");   
                        }
                        printf("*"); 
                    }
                }
                if(c1-j>c3)
                {
                    printf("X\n");
                }
                if(c1-j==c3)
                {
                    printf("X"); 
                    if(c3%2 == 0)
                    {
                        for(i=1; i<=c3/2; i++)
                        {
                            printf("-|");   
                        }                
                        
                    }
                    else
                    {
                        for(i=1; i<=c3/2; i++)
                        {
                            printf("|-");   
                        }  
                        printf("|");
                    }
                    printf("\n"); 
                    
                }
                if(c1-j<c3)
                {
                    printf("X");
                        if(c3%2 == 0)
                        {

                            for(i=1; i<=c3/2; i++)
                            {
                                printf(" |");   
                            }                            
                        }
                        else
                        {
                            for(i=1; i<=c3/2; i++)
                            {
                                printf("| ");   
                            }  
                            printf("|");
                        }
                        printf("\n");
                    
                    
                }
            }
        
            for(i=1; i<=c1;i++)
            {
                printf("X");  
            }
            if(c3%2 == 0)
            {
                for(i=1; i<=c3/2; i++)
                {
                    printf("-|");   
                }                            
            }
            else
            {
                for(i=1; i<=c3/2; i++)
                {
                    printf("|-");   
                }  
                printf("|");
            }
            printf("\n");
        }
    return 0;    
    }

  
}

