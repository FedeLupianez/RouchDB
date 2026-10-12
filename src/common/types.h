#pragma once
#include "./errors.h"
#include <stddef.h>
#include <stdint.h>

#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) (n << 30)
#define STR_LENGHT 32

#define MAX_COLUMNS 32

typedef uint8_t bool;

typedef struct {
    RouchError error;
} RowResult;

typedef struct {
    RouchError error;
    uint32_t offset;
    uint32_t slot;
} IO_RESULT;

typedef enum {
    TYPE_INT = 0,
    TYPE_FLOAT,
    TYPE_STR
} TypeID;

union VType {
    int i;
    float f;
    char s[50];
};

typedef struct {
    TypeID active;
    union VType value;
} SecureType;

typedef struct {
    char column[STR_LENGHT];
    char* op;
    SecureType value;
} Conditional;
