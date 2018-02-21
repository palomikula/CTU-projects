void end(int err){
    asm volatile("int $0x80" : : "a"(1), "b"(err));
}

int write(unsigned fd, char *buf, unsigned size){
    unsigned ret;
    asm volatile("int $0x80": "=a"(ret) : "a"(4), "b"(fd), "c"(buf), "d"(size) : "memory");
    return ret;
}

int read(unsigned fd, char *buf){
    unsigned ret;
    asm volatile("int $0x80"
                : "=a" (ret)
                : "0" (3), "b" (0), "c" (buf), "d" (sizeof(buf))
                : "memory", "cc");
    return ret;
}

void print(long long num){
    char backwards[50],buf[50];
    char hex_alphabet[]="0123456789abcdef";
    int i = 0,j = 2;
    buf[0] = '0';
    buf[1] = 'x';
    do{
        backwards[i++] = hex_alphabet[num%16];
        num /= 16;
    }while(num > 0);

    while(--i >= 0)buf[j++] = backwards[i];

    buf[j++] = '\0';
    buf[j++] = '\n';
    write(1, buf, j);
}

int isnum(char ch){
    return ch >= '0' && ch <= '9';
}

int isspc(char ch){
    return ch == ' ' || ch == '\n';
}

int _start()
{
    char buf[20];
    unsigned num = 0;
    int i;
    int num_digits = 0;
    unsigned chars_in_buffer = 0;

    for ( ; ; i++, chars_in_buffer--) {
        if (chars_in_buffer == 0){
            int ret = read(0, buf);
            if (ret < 0)
                end(0);
            i = 0;
            chars_in_buffer = ret;
        }
        if (num_digits > 0 && (chars_in_buffer == 0 || !isnum(buf[i]))) {
            print(num);
            num_digits = 0;
            num = 0;
        }
        if (chars_in_buffer == 0 || (!isspc(buf[i]) && !isnum(buf[i])))
            end(0);

        if (isnum(buf[i])) {
            num = num * 10 + (unsigned long)(buf[i] - '0');
            num_digits++;
            }
        }

    end(0);
}
