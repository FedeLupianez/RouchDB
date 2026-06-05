#include "page.h"
#include "common/errors.h"
#include "common/types.h"
#include <stdio.h>
#include <stdlib.h>

void init_page(Page* page, uint32_t page_id, PageType type)
{
    memset(page, 0, sizeof(Page));
    page->header.page_id = page_id;
    page->header.page_type = type;

    page->header.record_count = 0;
    page->header.slot_count = 0;

    page->header.free_start = 0;
    page->header.free_end = sizeof(page->data);
    page->header.free_space = sizeof(page->data);
    page->checksum = checksum_page(page);
}

Slot* get_slot(Page* page, uint16_t slot_id)
{
    if (slot_id >= page->header.slot_count)
        return NULL;
    return (Slot*)(page->data + page->header.free_end + slot_id * sizeof(Slot));
}

Slot* get_new_slot(Page* page)
{
    uint16_t slot_id = page->header.slot_count;
    Slot* slot = (Slot*)(page->data + page->header.free_end + slot_id * sizeof(Slot));
    slot->deleted = 0;
    slot->offset = page->header.free_start;
    page->header.slot_count++;
    return slot;
}

Record* get_record(Page* page, uint32_t offset)
{
    Record* record = (Record*)(page->data + offset);
    return record;
}

uint32_t checksum_page(Page* page)
{
    uint8_t* data = (uint8_t*)page->data;
    uint16_t sum = 0;

    for (int i = 0; i < (int)(PAGE_SIZE - sizeof(uint32_t)); i++)
        sum += (sum * 31) ^ data[i];

    return sum;
}

IO_RESULT write_in_page(Page* page, const uint8_t* data, uint32_t data_length)
{
    if (page->header.free_space <= 0) {
        printf("without free space\n");
        return (IO_RESULT) {
            ERR_IO,
        };
    }

    if (page->header.free_space < data_length) {
        printf("data is more then free space : %d | %d\n", page->header.free_space, data_length);
        return (IO_RESULT) {
            ERR_IO,
        };
    }

    if (page->header.free_start > page->header.free_end) {
        printf("Free space error with start and end\n");
        return (IO_RESULT) {
            ERR_IO,
        };
    }

    uint32_t offset = page->header.free_start;
    memcpy(page->data + offset, data, data_length);

    page->header.free_start += data_length;
    return (IO_RESULT) {
        OK,
        offset
    };
}

IO_RESULT read_data(char* data_dest, Page* page, uint32_t slot_id)
{
    Slot* slot = get_slot(page, slot_id);
    if (!slot) {
        printf("Error with slot\n");
        return (IO_RESULT) { ERR_IO };
    }
    if (!data_dest)
        data_dest = malloc(slot->length);
    Record* record = get_record(page, slot_id);
    memcpy(data_dest, record->data, slot->length);
    return (IO_RESULT) {
        OK,
        slot->offset
    };
}

IO_RESULT write_data(Page* page, uint8_t* data, uint32_t data_length)
{
    Slot* new_slot = get_new_slot(page);
    if (!new_slot) {
        printf("Error with slot\n");
        return (IO_RESULT) { ERR_IO };
    }

    IO_RESULT result = write_in_page(page, data, data_length);
    new_slot->length = data_length;

    if (result.error != OK) {
        printf("Error with write_in_page\n");
        return result;
    }

    return (IO_RESULT) {
        .error = OK,
        .offset = result.offset
    };
}
