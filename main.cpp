#include "hash_table.h"
#include "Text-parsing-functions/text_parse.h"
#include "ListDump/list_dump.h"

int main(int argc, const char *argv[])
{
    DumpHashTableBegin();

    WordSet word_set = {0};

    ReadWordSetOutOfFile(&word_set, argv[1]);

    HashTable hash_table = {0};

    HashTableInit(&hash_table, 692);

    for (size_t i = 0; i < word_set.word_count; i++)
    {
        HashTableAddData(&hash_table, word_set.word_array[i].str);
    }

    PrintHashTableLoadedData(&hash_table,
                             "hash_table_load.txt");

    HashTableDtor(&hash_table);

    WordSetDtor(&word_set);

    DumpHashTableEnd();

    return 0;
}
