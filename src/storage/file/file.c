
#include "file.h"
#include "common/types.h"
#include "storage/page/page.h"
#include <stdio.h>
#include <stdlib.h>

RouchFile* empty_file(char* path, uint32_t pages)
{
    if (!path)
        return NULL;
    RouchFile* new_file = malloc(sizeof(RouchFile));
    if (!new_file)
        return NULL;
    new_file->ptr = fopen(path, "wb");
    if (!new_file->ptr)
        return NULL;
    if (fseek(new_file->ptr, (long)(pages * PAGE_SIZE - 1), SEEK_SET) != 0) {
        fclose(new_file->ptr);
        free(new_file);
        return NULL;
    }
    if (fputc(0, new_file->ptr) == EOF) {
        fclose(new_file->ptr);
        free(new_file);
        return NULL;
    }
    fflush(new_file->ptr);
    return new_file;
}

Page get_new_page(RouchFile* file, uint32_t page_id, PageType type)
{
    Page page;
    init_page(&page, page_id, type);
    fseek(file->ptr, file->page_count * PAGE_SIZE, SEEK_SET);
    fwrite(&page, PAGE_SIZE, 1, file->ptr);
    file->page_count++;
    printf("Page created\n");
    return page;
}

IO_RESULT write_in_file(RouchFile* file, Page* page, uint8_t* data, uint32_t data_length)
{
    IO_RESULT result = write_data(page, data, data_length);
    if (result.error != OK)
        return result;
    printf("page writed\n");

    uint32_t offset = (page->header.page_id * PAGE_SIZE);
    const int move_result = fseek(file->ptr, offset, SEEK_SET);
    if (move_result != 0)
        return (IO_RESULT) {
            ERR_IO
        };
    fwrite(page, PAGE_SIZE, 1, file->ptr);
    fflush(file->ptr);
    return (IO_RESULT) {
        OK,
        offset
    };
}

void RouchFileFree(RouchFile* file)
{
    if (!file)
        return;
    fclose(file->ptr);
    file->ptr = NULL;
    free(file);
    file = NULL;
}
