#pragma once

#include "common/types.h"
#include "storage/file/file.h"
#include <stdint.h>

#define HEADER_PAGE_ID

enum ColConfig {
    NULLABLE = 1,
    IDX = 1 << 1,
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
    uint32_t size;
    uint32_t col_count;
    uint32_t registers_slot;
} TableHeader;

typedef struct {
    TableHeader header;
    Column* columns;
} Table;

typedef struct {
    uint32_t row_id;
    SecureType* values;
} Row;

void create_space(Table* table);
