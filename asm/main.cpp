#include <stdio.h>
#include <string.h>

extern "C" unsigned int asm_CRC32Hash(const char *string);

int main()
{
    const char *string = "Huyatina";

    printf("hash val: %u\n", asm_CRC32Hash(string));

    return 0;
}
