#pragma once

#include "common/types.h"
#include "storage/page/page.h"

typedef struct {
    uint32_t page_count;
    FILE* ptr;
} RouchFile;

RouchFile* empty_file(char* path, uint32_t pages);
Page get_new_page(RouchFile* file, uint32_t page_id, PageType type);
IO_RESULT write_in_file(RouchFile* file, Page* page, uint8_t* data, uint32_t data_length);

void RouchFileFree(RouchFile* file);
