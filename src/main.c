
#include "common/types.h"
#include "storage/file/file.h"
#include "storage/page/page.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    RouchFile* file = empty_file("temp.dat", 3);
    Page new_page = get_new_page(file, 0, DATA);
    const int size = 5;
    char data[size];
    memset(&data, 'a', size);
    IO_RESULT result = write_in_file(file, new_page.header.page_id, (uint8_t*)data, size);
    memset(&data, 'b', size);
    result = write_in_file(file, new_page.header.page_id, (uint8_t*)data, size);
    printf("result error : %d \n", result.error);

    char* read_data = malloc(size);
    result = read_from_file(file, read_data, 0, 0);
    if (result.error != OK)
        printf("Error reading\n");
    else
        printf("Readed from file : %s\n", read_data);
    free(read_data);

    RouchFileFree(file);
    return EXIT_SUCCESS;
}
