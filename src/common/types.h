#pragma once
#include "./errors.h"
#include <stddef.h>
#include <stdint.h>

#define KB(n) (n << 10)
#define MB(n) (n << 20)
#define GB(n) (n << 30)

typedef struct {
    RouchError error;
} RowResult;

typedef struct {
    RouchError error;
    uint32_t offset;
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
    char column[50];
    char op;
    SecureType value;
} Conditional;

typedef struct {
    char type;
    char columns[10][50];
    char table[50];
    Conditional condition;
} Query;
