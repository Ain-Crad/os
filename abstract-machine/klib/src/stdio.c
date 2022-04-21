#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>
#include <limits.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


static inline void puts(const char *s) {
    for (; *s; s++) putch(*s);
}

static void getNum(int v, char* digits){
    if(v == INT_MIN) {
        strcpy(digits, "-2147483648");
        return;
    }
    
    bool neg = v < 0 ? true : false;
    if(neg) v = -v;

    int p = 0;
    while(v){
        int digit = v % 10;
        v /= 10;
        digits[p++] = digit + '0';
    }
    if(neg) digits[p++] = '-';

    //reverse
    int l = 0, r = p - 1;
    while(l < r){
        char tmp = digits[l];
        digits[l] = digits[r];
        digits[r] = tmp;

        ++l, --r;
    }
}

int printf(const char *fmt, ...) {
    int v;
    const char* str;

    int p = 0;
    char buff[100] = {'\0'};
    char digits[12] = {'\0'};

    va_list args;
    va_start(args, fmt);
    while(*fmt != '\0'){
        if(*fmt != '%'){
            buff[p++] = *fmt;
        }else{
            char ch = *(++fmt);
            switch(ch){
                case 'd':
                    v = va_arg(args, int);
                    getNum(v, digits);
                    for(int i = 0; i < strlen(digits); ++i){
                        buff[p++] = digits[i];
                    }
                    break;
                case 's':
                    str = va_arg(args, const char*);
                    for(; *str; str++) buff[p++] = *str;
                    break;
                default:
                    buff[p++] = '%';
                    buff[p++] = ch;
            }
        }
        
        ++fmt;
    }
    va_end(args);

    puts(buff);

    return p;

}


int vsprintf(char *out, const char *fmt, va_list ap) {
    panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    panic("Not implemented");
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
    panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    panic("Not implemented");
}

#endif
