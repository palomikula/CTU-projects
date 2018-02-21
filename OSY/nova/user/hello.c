#include "stdio.h"

static unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile (
        "   mov %%esp, %%ecx    ;"
        "   mov $1f, %%edx      ;"
        "   sysenter            ;"
        "1:                     ;"
        : "+a" (w0) : "S" (w1) : "ecx", "edx", "memory");
    return w0;
}

unsigned syscall3(unsigned w0, unsigned w1, unsigned w2)
{
    asm volatile (
        "   mov %%esp, %%ecx    ;"
        "   mov $1f, %%edx      ;"
        "   sysenter            ;"
        "1:                     ;"
        : "+a" (w0) : "S" (w1), "D" (w2) : "ecx", "edx", "memory");
    return w0;
}

void write_to_terminal(const void *data, unsigned len)
{
	syscall3(1, (unsigned)data, len);
}

void *brk(void *address)
{
    return (void*)syscall2(3, (unsigned)address);
}

void print(const char *string)
{
    unsigned len = 0;

    while (string[len] != '\0')
        len++;

    write_to_terminal(string, len);
}

void *my_malloc(unsigned int size);
int my_free(void *address);

int main ()
{
    print("Hello worrrrrrld!\n");
    //void *sfdsa=my_malloc(4);
    //sfdsa=my_malloc(8);
    void *b = brk(1000);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x20800);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x21000);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x20800);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x20900);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x20950);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x22000);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);
    b = brk(0x19900);
    printf("current break: %p\n", b);
    b = brk(0);
    printf("new break: %p\n\n", b);

    /*void *aa=my_malloc(0);
    //char *af=(char *)(af);
    //af[11]='b';
    printf("aa %p\n", aa);
    void *ab=my_malloc(12);
    printf("ab %p\n", ab);
    void *ac=my_malloc(12);
    printf("ac %p\n", ac);
    void *ad=my_malloc(12);
    printf("ad %p\n", ad);
    int ac_ret=my_free(ac);
    printf("ac_ret %d\n", ac_ret);
    int aa_ret=my_free(aa);
    printf("aa_ret %d\n", aa_ret);
    int ab_ret=my_free(ab);
    printf("ab_ret %d\n", ab_ret);
    int ad_ret=my_free(ad);
    printf("ad_ret %d\n", ad_ret);
    void *ae=my_malloc(10);
    printf("ae %p\n", ae);*/
    /*int i;
    void* free[500];
    for(i=0;i<10;i++){
        printf("before malloc break: %p\n",brk(0));
        free[i]=my_malloc(1024);
        printf("address is %p\n",free[i]);
        //brk(brk(0)+1032);
        printf("after malloc break: %p\n",brk(0));
    }
    for(i=0;i<10;i++){
        my_free(free[i]);
    }
    void* a;
    a=my_malloc(0x20000);
    printf("big %p\n",a);
    my_free(a);*/

    while(1);
}
