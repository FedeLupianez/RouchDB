#include "./tablespace.h"
#include "common/types.h"
#include "storage/file/file.h"
#include <stdint.h>
#include <stdio.h>

void create_space(Table* table)
{
    if (!table || !table->columns || table->header.col_count == 0)
        return;

    char path[STR_LENGHT + 14];
    snprintf(path, sizeof(path), "database/%s.ibd", table->header.name);

    RouchFile* file = empty_file(path, 2);
    if (!file)
        return;

    IO_RESULT result = write_in_file(file, 0, (uint8_t*)&table->header, sizeof(TableHeader));
    if (result.error != OK) {
        RouchFileFree(file);
        return;
    }

    table->header.registers_slot = result.slot;

    for (uint32_t i = 0; i < table->header.col_count; i++) {
        IO_RESULT col_result = write_in_file(file, 0, (uint8_t*)&table->columns[i], sizeof(Column));
        if (col_result.error != OK)
            break;
    }

    RouchFileFree(file);
}
