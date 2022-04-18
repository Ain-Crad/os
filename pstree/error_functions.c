#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "ename.c.inc"
#include "error_functions.h"

#define ERR_BUF_SIZE 500

static void terminate(){
    char* s;
    s = getenv("EF_DUMPCORE");

    if(s != NULL && *s != '\0'){
        abort();
    }else{
        exit(EXIT_FAILURE);
    }
}

static void outputError(bool useErr, int err, bool flushStdout, const char* format, va_list ap){
    char buf[2 * ERR_BUF_SIZE + 10], userMsg[ERR_BUF_SIZE], errText[ERR_BUF_SIZE];

    vsnprintf(userMsg, ERR_BUF_SIZE, format, ap);
    if(useErr){
        snprintf(errText, ERR_BUF_SIZE, " [%s %s]", 
                (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?", strerror(err));
    }else{
        snprintf(errText, ERR_BUF_SIZE, ":");
    }

    snprintf(buf, 2 * ERR_BUF_SIZE + 10, "ERROR%s %s\n", errText, userMsg);
    
    if(flushStdout) fflush(stdout);
    fputs(buf, stderr);
    fflush(stderr);

}

void errExit(const char* format, ...){
    va_list argList;
    
    va_start(argList, format);
    outputError(true, errno, true, format, argList);
    va_end(argList);

    terminate();
}
