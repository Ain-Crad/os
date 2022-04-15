#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "ename.c.inc"
#include "error_functions.h"

#define BUF_SIZE 500

static void outputError(bool useErr, int err, bool flushStdout, const char* format, va_list ap){
    char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, format, ap);
    if(useErr){
        snprintf(errText, BUF_SIZE, " [%s %s]", 
                (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?", strerror(err));
    }else{
        snprintf(errText, BUF_SIZE, ":");
    }

    snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);
    
    if(flushStdout) fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);

}

void errExit(const char* format, ...){
    va_list argList;
    
    int savedErrno = errno;

    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);
    
    errno = savedErrno;
}
