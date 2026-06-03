#pragma once

// +------------------------+
// | PageHeader             |
// +------------------------+
// | Records (heap grows ↓) |
// |  R1                    |
// |  R2                    |
// |  R3                    |
// |        free space      |
// +------------------------+
// | Slots (grow ↑)         |
// |  slot[0] -> R2        |
// |  slot[1] -> R1        |
// |  slot[2] -> R3        |
// +------------------------+
// | checksum               |
// +------------------------+

#include "record.h"
#include <common/functions.h>
#include <common/types.h>
#include <stdio.h>
#include <string.h>

#define PAGE_SIZE KB(16)
#define SLOT_INVALID 0xFFFF
typedef enum {
    INDEX = 0,
    DATA,
    WAL
} PageType;

typedef struct {
    uint32_t page_id;
    uint32_t space_id;

    uint32_t next_page;
    uint32_t prev_page;

    uint16_t page_type;

    uint16_t record_count;
    uint16_t slot_count;

    uint16_t free_start;
    uint16_t free_end;
    uint16_t free_space;

    uint16_t infimum;
    uint16_t supremum;

    uint32_t checksum;
} PageHeader;

typedef struct {
    uint16_t offset;
    uint16_t length;
    uint8_t deleted;
} Slot;

typedef struct {
    PageHeader header;
    char data[PAGE_SIZE - sizeof(PageHeader) - sizeof(uint32_t)];
    uint32_t checksum;
} Page;

void init_page(Page* page, uint32_t page_id, PageType type);

IO_RESULT write_data(Page* page, uint8_t* data, uint32_t data_length);

Slot* get_slot(Page* page, uint16_t slot_id);

Record* get_record(Page* page, uint16_t record_id);

uint32_t checksum_page(Page* page);
