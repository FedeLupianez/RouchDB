#pragma once

#include "common/types.h"

enum ColConfig {
    NULLABLE = 1,
    INDEX = 1 << 1,
    PK = 1 << 2,
    UNIQUE = 1 << 3,
};

typedef struct {
    char name[STR_LENGHT];
    TypeID type;
    uint8_t configs;
} Column;

typedef struct {
    char name[STR_LENGHT];
    int size;
    int col_count;
    Column* columns;
} Table;
