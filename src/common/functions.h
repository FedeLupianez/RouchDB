#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) (n << 30)

static inline void logging(char* level, char* msg, ...)
{
    time_t now = time(NULL);
    fprintf(stderr, "%ld %s:", now, level);
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
}
