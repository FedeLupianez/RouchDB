
#include "common/types.h"
#include "storage/file/file.h"
#include "storage/page/page.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    RouchFile* file = empty_file("temp.dat", 3);
    Page new_page = get_new_page(file, 0, DATA);
    const int size = sizeof(new_page.data);
    char data[size];
    memset(&data, 'a', size);
    IO_RESULT result = write_in_file(file, &new_page, (uint8_t*)data, size);
    printf("result error : %d \n", result.error);
    RouchFileFree(file);
    return EXIT_SUCCESS;
}
