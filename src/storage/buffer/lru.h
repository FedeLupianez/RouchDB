#pragma once

#include "common/types.h"
#include "storage/file/file.h"
#include "storage/page/page.h"

#define CACHE_SIZE 3

typedef struct {
    Page slots[CACHE_SIZE];
    uint32_t actual_size;
} LruCache;

void moveToTop(LruCache* cache, uint32_t page_idx)
{
    Page tmp_page = cache->slots[page_idx];
    for (int i = page_idx; i > 0; i--)
        cache->slots[i] = cache->slots[i - 1];
    cache->slots[0] = tmp_page;
}

int getIDXFromCache(LruCache* cache, uint32_t page_id)
{
    for (uint32_t i = 0; i < cache->actual_size; i++) {
        if (cache->slots[i].header.page_id == page_id)
            return i;
    }
    return -1;
}

Page* getPageFromCache(LruCache* cache, uint32_t page_id)
{
    for (uint32_t i = 0; i < cache->actual_size; i++) {
        if (cache->slots[i].header.page_id == page_id) {
            moveToTop(cache, i);
            return &cache->slots[0];
        }
    }
    return NULL;
}

Page* putPageInCache(LruCache* cache, RouchFile* file, uint32_t page_id)
{
    int page_idx = getIDXFromCache(cache, page_id);

    if (page_idx != -1) {
        moveToTop(cache, page_idx);
        return &cache->slots[0];
    }

    Page* page = get_page(file, page_id);

    if (page == NULL)
        return NULL;

    int limit = cache->actual_size < CACHE_SIZE
        ? cache->actual_size
        : CACHE_SIZE - 1;

    for (int i = limit; i > 0; i--)
        cache->slots[i] = cache->slots[i - 1];

    cache->slots[0] = *page;
    if (cache->actual_size < CACHE_SIZE)
        cache->actual_size++;
    return &cache->slots[0];
}
