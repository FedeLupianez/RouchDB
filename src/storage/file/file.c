#include "file.h"
#include "common/errors.h"
#include "common/functions.h"
#include "common/types.h"
#include "storage/page/page.h"
#include <openssl/sha.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

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
        logging("ERROR", "Error mapping file");
        return NULL;
    }
    madvise(new_file->mapped, pages * PAGE_SIZE, MADV_RANDOM);
    return new_file;
}

Page* get_new_page(RouchFile* file, uint32_t page_id, PageType type)
{
    uint32_t new_page_count = file->page_count + 1;
    uint32_t new_size = new_page_count * PAGE_SIZE;
    if (ftruncate(fileno(file->ptr), new_size) != 0) {
        logging("ERROR", "error resizing the file");
        return NULL;
    }
    munmap(file->mapped, file->page_count * PAGE_SIZE);
    file->mapped = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, fileno(file->ptr), 0);

    if (file->mapped == MAP_FAILED) {
        logging("ERROR", "Error mapping file");
        file->mapped = NULL;
        return NULL;
    }
    file->page_count = new_page_count;
    Page* new_page = (Page*)(file->mapped + (file->page_count - 1) * PAGE_SIZE);
    init_page(new_page, page_id, type);
    logging("INFO", "New page created");

    return new_page;
}

Page* get_page(RouchFile* file, uint32_t page_id)
{
    if (!file || !file->mapped) {
        return NULL;
    }
    if (page_id >= file->page_count) {
        return NULL;
    }
    Page* page = (Page*)((uint8_t*)file->mapped + (page_id * PAGE_SIZE));
    logging("INFO", "Page %d got", page_id);
    unsigned char hash[SHA256_DIGEST_LENGTH];
    checksum_page(page, hash);
    printf("Calculated checksum: ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    printf("Checksum of page:    ");
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        printf("%02x", page->header.checksum[i]);
    }
    printf("\n");

    if (memcmp(page->header.checksum, hash, SHA256_DIGEST_LENGTH) != 0) {
        logging("ERROR", "Page corrupted");
        return NULL;
    }
    return page;
}

IO_RESULT write_in_file(RouchFile* file, uint16_t page_id, uint8_t* data, uint32_t data_length)
{
    Page* page = get_page(file, page_id);
    IO_RESULT result = write_data(page, data, data_length);
    printf("page writed\n");
    if (result.error != OK)
        return result;
    // Sync page to file mapped by kernel
    if (msync(page, PAGE_SIZE, MS_SYNC) != 0)
        return (IO_RESULT) { .error = ERR_MEMORY };
    return result;
}

IO_RESULT read_from_file(RouchFile* file, char* data_dest, uint32_t page_id, uint32_t slot_id)
{
    if (!data_dest)
        return (IO_RESULT) {
            .error = ERR_MEMORY
        };
    printf("entered in function\n");
    Page* page = get_page(file, page_id);
    if (!page)
        return (IO_RESULT) {
            .error = ERR_NOT_FOUND
        };
    printf("page got: %u\n", page->header.page_id);
    Slot* slot = get_slot(page, slot_id);
    Record* record = get_record(page, slot->offset);
    if (!record) {
        printf("Error");
        return (IO_RESULT) {
            .error = ERR_NOT_FOUND
        };
    }
    memcpy(data_dest, record->data, slot->length);
    return (IO_RESULT) {
        .error = OK
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
