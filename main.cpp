#include "hash_table.h"
#include "Text-parsing-functions/text_parse.h"
#include "ListDump/list_dump.h"

int main(int argc, const char *argv[])
{
    WordSet word_set = {0};
    ReadWordSetOutOfFile(&word_set, argv[1]);

    HashTable hash_table = {0};

    HashTableInit       (&hash_table, 797);
    LoadWordsInHashTable(&hash_table, &word_set);

    WordSet seek_word_set = {0};
    ReadWordSetOutOfFile(&seek_word_set, argv[2]);

    TestHashTable(&hash_table, &seek_word_set);

    PrintHashTableLoadedData(&hash_table,
                             "hash_table_load_data.csv");

    HashTableDtor(&hash_table);

    WordSetDtor(&seek_word_set);
    WordSetDtor(&word_set);

    return 0;
}
