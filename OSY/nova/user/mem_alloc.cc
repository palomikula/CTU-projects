#include <stdio.h>

extern "C" void *my_malloc(unsigned int);
extern "C" int my_free(void*);

void *brk(void *address){
  unsigned w0 = 3;
  asm volatile (
      "   mov %%esp, %%ecx    ;"
      "   mov $1f, %%edx      ;"
      "   sysenter            ;"
      "1:                     ;"
      : "+a" (w0) : "S" ((unsigned)address) : "ecx", "edx", "memory");
    return (void*)w0;
}

void *break_cur;
void *break_first;

void *my_malloc(unsigned int size){

    if(break_first==0){
        break_cur = brk(0);
        break_first = brk(0);
    }

    char *tmp = (char*)break_first;
    while(tmp<(char*)break_cur){
        unsigned int block_header=*((unsigned int*)tmp);
        if((block_header&1) == 0 && ((block_header>>1)+sizeof(int)>size || block_header==0))
            break;
        tmp+=sizeof(int)+(block_header>>1);
    }

    if(tmp>=(char*)break_cur || (*((unsigned int*)tmp)>>1) < size+sizeof(int)){
        if(brk((char*)break_cur + 2*(size+sizeof(int)))==0)
            return 0;
        else
            break_cur=brk(0);
    }
    if(((*((unsigned int*)tmp))>>1)){
        unsigned int block_header=*((unsigned int*)tmp);
        char *next = tmp + size + sizeof(int);
        *((unsigned int*)next) =((block_header>>1)-size - 2*sizeof(int))<<1;
    }
    *((unsigned int*)tmp) = (size<<1)|1;
    return tmp+sizeof(int);

}

int my_free(void *address){
    char *ptr = (char*)address;
    if(ptr < break_first || ptr > break_cur){
        return -1;
    }
    if(((*((unsigned int*)ptr-1))&1) == 0){
        return -1;
    }
    *(ptr-sizeof(int)) = (*(ptr-sizeof(int))>>1)<<1;
    printf("freeing %p\n",address);
    return 0;
}
