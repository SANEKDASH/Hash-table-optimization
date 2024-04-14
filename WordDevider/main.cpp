#include <string.h>

#include "../FastList/list.h"
#include "../ListDump/list_dump.h"
#include "../Text-parsing-functions/text_parse.h"

int main(int argc, const char *argv[])
{
    if (argc < 1 || argc > 3)
    {
        ColorPrintf(kGreen, ">> WordDevider: you should write single cmd string argument - name of readable file\n");

        return -1;
    }

    Text words = {0};

    WordSet word_set = {0};

    ReadWordsFromFile   (&words, argv[1]);
    ConvertTextToWordSet(&words, &word_set);

    size_t word_len = 0;

    WriteWordSetInFile(&word_set, argv[2]);

    TextDtor(&words);

    return 0;
}
