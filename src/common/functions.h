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

static const struct {
    const char* BLACK;
    const char* RED;
    const char* GREEN;
    const char* YELLOW;
    const char* BLUE;
    const char* PURPLE;
    const char* CYAN;
    const char* WHITE;
    const char* RESET;
} ANSI = {
    "\e[0;30m",
    "\e[0;31m",
    "\e[0;32m",
    "\e[0;33m",
    "\e[0;34m",
    "\e[0;35m",
    "\e[0;36m",
    "\e[0;37m",
    "\e[0m"

};

static inline void logging(char* level, char* msg, ...)
{

    time_t now = time(NULL);
    struct tm* time_info = localtime(&now);

    char* color = malloc(8);
    // view color
    if (strcmp(level, "ERROR") == 0) {
        color = (char*)ANSI.RED;
    } else if (strcmp(level, "INFO") == 0) {
        color = (char*)ANSI.BLUE;
    } else if (strcmp(level, "SETUP") == 0) {
        color = (char*)ANSI.GREEN;
    } else if (strcmp(level, "MEMORY") == 0) {
        color = (char*)ANSI.PURPLE;
    } else if (strcmp(level, "NETWORK") == 0) {
        color = (char*)ANSI.CYAN;
    }

    fprintf(stderr, "%d:%d:%d %s[ %s ] ", time_info->tm_hour, time_info->tm_min, time_info->tm_sec, color, level);
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "%s\n", ANSI.RESET);
}
