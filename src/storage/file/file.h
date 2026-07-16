#pragma once

#include "common/errors.h"
#include "common/types.h"
#include "storage/page/page.h"
#include <sys/mman.h>

typedef struct {
    uint32_t page_count;
    FILE* ptr;
    void* mapped;
} RouchFile;

RouchFile* empty_file(char* path, uint32_t pages);
Page get_new_page(RouchFile* file, uint32_t page_id, PageType type);
Page* get_page(RouchFile* file, uint32_t page_id);
IO_RESULT write_in_file(RouchFile* file, uint16_t page_id, uint8_t* data, uint32_t data_length);
IO_RESULT read_from_file(RouchFile* file, char* data_dest, uint32_t page_id, uint32_t slot_id);
void RouchFileFree(RouchFile* file);
