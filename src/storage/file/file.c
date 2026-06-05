#include "file.h"
#include "common/types.h"
#include <stdio.h>
#include <stdlib.h>

RouchFile* empty_file(char* path, uint32_t pages)
{
    if (!path)
        return NULL;
    RouchFile* new_file = malloc(sizeof(RouchFile));
    if (!new_file)
        return NULL;
    new_file->ptr = fopen(path, "w+b");
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
    new_file->mapped = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(new_file->ptr), 0);
    if (new_file->mapped == MAP_FAILED) {
        fclose(new_file->ptr);
        free(new_file);
        printf("error mapping file\n");
        return NULL;
    }
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

Page* get_page(RouchFile* file, uint32_t page_id)
{
    if (!file || !file->mapped) {
        return NULL;
    }
    if (page_id >= file->page_count) {
        return NULL;
    }
    return (Page*)(file->mapped + (page_id * PAGE_SIZE));
}

IO_RESULT write_in_file(RouchFile* file, uint16_t page_id, uint8_t* data, uint32_t data_length)
{
    Page* page = get_page(file, page_id);
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

IO_RESULT read_from_file(RouchFile* file, char* data_dest, uint32_t page_id, uint32_t slot_id)
{
    if (!data_dest)
        return (IO_RESULT) {
            ERR_MEMORY
        };
    printf("entered in function\n");
    Page* page = get_page(file, page_id);
    if (!page)
        return (IO_RESULT) {
            ERR_NOT_FOUND
        };
    printf("page got: %u\n", page->header.page_id);
    Slot* slot = get_slot(page, slot_id);
    Record* record = get_record(page, slot->offset);
    if (!record) {
        printf("Error");
        return (IO_RESULT) {
            ERR_NOT_FOUND
        };
    }
    memcpy(data_dest, record->data, slot->length);
    return (IO_RESULT) {
        OK
    };
}

void RouchFileFree(RouchFile* file)
{
    if (!file)
        return;
    munmap(file->mapped, file->page_count * PAGE_SIZE);
    fclose(file->ptr);
    file->ptr = NULL;
    free(file);
    file = NULL;
}
