#pragma once
typedef enum {
    OK = 0,
    ERR_IO,
    ERR_PARSE,
    ERR_NOT_FOUND,
    ERR_MEMORY
} RouchError;
