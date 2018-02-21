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
    void *ret=0;
    if(break_cur==0){
        break_cur = brk(0);
        break_first = brk(0);
        if(brk((char*)break_cur+size+2*sizeof(int))==0){
            return ret;
        }
        *(unsigned int*)break_cur=(size<<1 | 1);
        ret = (unsigned int*)break_cur+1;
        //*(unsigned int*)((char*)break_cur+size+sizeof(int))=size;
        break_cur=brk(0);
        return ret;
    }

    unsigned int block_size=*(unsigned int*)break_first;
    char *tmp = (char*)break_first+sizeof(int);

    do{
        if((block_size&1) == 0 && (block_size>>1)>=size){
            *((unsigned int*)tmp-1)=(size<<1 | 1);
            return (void*)tmp;
        }
        if(block_size == 0 && break_cur>tmp+size+sizeof(int)){
            *((unsigned int*)tmp-1)=(size<<1 | 1);
            return (void*)tmp;
        }
        tmp+=(block_size>>1)+2*sizeof(int);
        //block_size=*((unsigned int*)tmp-1);
        printf("break_cur %p\n",break_cur);
        printf("block_size %u\n",*((unsigned int*)tmp-1)>>1);
        printf("tmp %p\n",tmp);
    }while(tmp+(block_size<<1)+sizeof(int)<break_cur);
    printf("passed, tmp is %p\n",tmp);
    if(brk((char*)tmp+size+sizeof(int))==0){
        return 0;
    }
    *((unsigned int*)tmp-1)=(size<<1 | 1);
    ret = (unsigned int*)break_cur+1;
    //*(unsigned int*)((char*)break_cur+size+sizeof(int))=(size);
    break_cur=brk(0);
    return tmp;

}

int my_free(void *address){
    char *ptr = (char*)address;
    printf("freeing address %p",ptr);
    if(ptr < break_first || ptr > break_cur){
        return -1;
    }
    if(((*((unsigned int*)ptr-1))&1) == 0){
        return -1;
    }
    *(ptr-sizeof(int)) = (*(ptr-sizeof(int))>>1)<<1;
    return 0;
}
