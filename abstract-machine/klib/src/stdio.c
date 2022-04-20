#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)


static inline void puts(const char *s) {
    for (; *s; s++) putch(*s);
}

static void getNum(int v, char* digits){
    bool neg = v < 0 ? false : true;

    int p = 0;
    while(v){
        int digit = v % 10;
        v /= 10;
        digits[p++] = digit + '0';
    }
    if(neg) digits[p++] = '-';    
}

int printf(const char *fmt, ...) { 
    panic("Not implemented");
    //int v;
    //const char* str;

    //int p = 0;
    //char buff[100] = {'\0'};
    //char digits[12] = {'\0'};

    //va_list args;
    //va_start(args, fmt);
    //while(*fmt != '\0'){
    //    if(*fmt != '%'){
    //        buff[p++] = *fmt;
    //    }else{
    //        char ch = *(++fmt);
    //        switch(ch){
    //            case 'd':
    //                v = va_arg(args, int);
    //                getNum(v, digits);
    //                for(int i = strlen(digits) - 1; i >= 0; --i){
    //                    buff[p++] = digits[i];
    //                }
    //                break;
    //            case 's':
    //                str = va_arg(args, const char*);
    //                for(; *str; str++) buff[p++] = *str;
    //                break;
    //            default:
    //                buff[p++] = '%';
    //                buff[p++] = ch;
    //        }
    //    }
    //    
    //    ++fmt;
    //}
    //va_end(args);

    //puts(buff);

    //return p;

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
