#include "./tablespace.h"
#include "common/types.h"
#include "storage/file/file.h"

void create_space(Table* table)
{
    char path[strlen(table->name) + 9];
    sprintf(path, "database/%s", table->name);

    RouchFile* file = empty_file(path, 2);

    IO_RESULT result = write_in_file(file, 0, (uint8_t*)table, sizeof(Table));

    table->registers_slot = result.slot;

    uint32_t i = 0;
    do {
        const Column* col = (table->columns + i);
        IO_RESULT new_row = write_in_file(file, 0, (uint8_t*)col, sizeof(Column));
    } while (i < table->col_count);
}
