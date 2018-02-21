#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lock.h"
int tries=0;
int end=0;
void move(int idx1, int idx2, char *str)
{
    char tmp;
    int i;
    if(idx2>idx1){
        tmp=str[idx2];
        for(i=idx2;i>idx1;--i){
            str[i]=str[i-1];
        }
        str[idx1]=tmp;
    }else if(idx1>idx2){
        tmp=str[idx2];
        for(i=idx2;i<idx1;++i){
            str[i]=str[i+1];
        }
        str[idx1]=tmp;
    }
}

int print_comb(char *str, int idx, int argc)
{
    int i;
    int n=strlen(str)-1;
    if (idx == n){
        tries++;
        if(argc==1){
            printf("%s\n", str);
        }
        else if(unlock(str)){
            printf("%s - %dx\n",str,tries);
            end=1;
        }
    }
    else
    {
        for (i = idx; i <= n; i++)
        {
            move(idx,i,str);
            print_comb(str, idx+1, argc);
            move(i,idx,str);
        }
    }
    if(argc>1&&end==0){
        return 0;
    }
    else{
        return 1;
    }
}

int main(int argc, char *argv[])
{
    int i,j;
    char *input;
    input=(char*)calloc(9,sizeof(char));
    input[0]='0';
    char tmp[9];
    scanf("%s",input);
    for(i=0;i<strlen(input);++i){
        if(input[i]<'1'||(input[i]>'9'&& input[i]<'A')||(input[i]>'Z' && input[i]<'a')||input[i]>'z'){
            fprintf(stderr, "Error: Chybny vstup!\n");
            free(input);
            return 100;
        }
        tmp[i]=input[i];
    }
    char min;
    int min_idx;
    for(i=0;i<strlen(input);++i){
        min='z'+1;
        for(j=0;j<strlen(input);++j){
            if(min>tmp[j]){
                min=tmp[j];
                min_idx=j;
            }
        }
        tmp[min_idx]=(char)'z'+1;
        input[i]=min;
    }
    if(print_comb(input,0,argc)==0){
        fprintf(stderr,"Error: Heslo nebylo nalezeno!\n");
        free(input);
        return 101;
    }
    free(input);
    return 0;
}

