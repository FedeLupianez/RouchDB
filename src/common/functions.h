#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) (n << 30)

// \e[0;30m 	Black
// \e[0;31m 	Red
// \e[0;32m 	Green
// \e[0;33m 	Yellow
// \e[0;34m 	Blue
// \e[0;35m 	Purple
// \e[0;36m 	Cyan
// \e[0;37m 	White
// \e[0m 	    Reset

static inline void logging(char* level, char* msg, ...)
{
    const char* black = "\e[0;30m";
    const char* red = "\e[0;31m";
    const char* green = "\e[0;32m";
    const char* yellow = "\e[0;33m";
    const char* blue = "\e[0;34m";
    const char* purple = "\e[0;35m";
    const char* cyan = "\e[0;36m";
    const char* white = "\e[0;37m";
    const char* reset = "\e[0m";

    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);

    char* color = malloc(8);
    // view color
    if (strcmp(level, "ERROR") == 0) {
        color = (char*)red;
    } else if (strcmp(level, "INFO") == 0) {
        color = (char*)blue;
    } else if (strcmp(level, "SETUP") == 0) {
        color = (char*)green;
    } else if (strcmp(level, "MEMORY") == 0) {
        color = (char*)purple;
    } else if (strcmp(level, "NETWORK") == 0) {
        color = (char*)cyan;
    }

    fprintf(stderr, "%d:%d:%d %s[ %s ] ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, color, level);
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "%s\n", reset);
}
