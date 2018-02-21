#include <stdio.h>
#include <stdlib.h>

char *get_input()
{
    int i;
    int len=10;
    char *in;
    in = (char*) malloc(len);
    for(i=0;(in[i]=getchar())!='\n';++i){
        if(((int)in[i] > 'Z' || (int)in[i] < 'A') && ((int)in[i] > 'z' || (int)in[i] < 'a')){
            free(in);
            return NULL;
        }
        if(i+1==len){
            len*=2;
            in = (char*) realloc(in,len);
        }
    }
    in[i+1]='\0';
    return in;
}
 
int str_len(char *str)
{
    int i;
    for(i=0;str[i]!='\0';++i);
    return i;
}

int dif(char a, char b)
{
    int dif=0;
    //printf("%d",(int)a);
    while((int)a != (int)b)
    {
        if(b =='Z'){
            b ='a';
        }
        else if(b =='z'){
            b ='A';
        }
        else{
            b++;
        }
        dif++;
        //printf("%d=%d\n",b,(int)a);
    }
    //printf("********************************************************************************");
    return dif;
}

char decrypt(char a, char b)
{
    while((int)b!=0)
    {
        if(a == 'a'){
            a = 'Z';
        }
        else if(a == 'A'){
            a = 'z';
        }
        else{
            a--;
        }
        b--;
    }
    int result=(int)a;
    return (char)result;
}
int main()
{
    //printf("%c",decrypt('U',(char)42));
    char *in_c;
    int i,j;
    in_c=get_input();
    if(in_c == 0){
        fprintf(stderr,"Error: Chybny vstup!\n");
        return 100;
    }
    char *in_t;
    in_t=get_input();
    if(str_len(in_c)!=str_len(in_t)){
        fprintf(stderr,"Error: Chybna delka vstupu!\n");
        free(in_c);
        free(in_t);
        return 101;
    }
    //printf("852");
    int *diffs;
    int len=str_len(in_c)-1;
    //printf("%d",len);
    diffs=(int*)malloc(str_len(in_c)*sizeof(int));
    for(i=0;i<len;++i){
        diffs[i]=dif(in_c[i],in_t[i]);
        //printf("%d, ",diffs[i]);
    }
    int *itr;
    itr=(int*)calloc(52,sizeof(int));
    int mx=0;
    int d;
    for(i=0;i<52;++i){
        //printf("%d",len);
        for(j=0;j<len;++j){
            //printf("%d",len);
            if(diffs[j]==i){
                itr[i]++;
                //printf("%d",itr[i]);
            }
        }
        if(itr[i]>mx){
            mx=itr[i];
            d=i;
        }
    }
    //printf("%d",d);
    for(i=0;i<=len-1;++i){
        printf("%c",decrypt(in_c[i],(char)d));
    }
    printf("\n");
    free(in_c);
    free(in_t);
    free(diffs);
    free(itr);
    return 0;
}
