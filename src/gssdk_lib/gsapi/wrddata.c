#include "types.h"

#include <stddef.h>
#include <string.h>

#define WRD_INVALID_ARGUMENT 0x80CC0080
#define WRD_OUT_OF_MEMORY 0x80CC0086

typedef struct WrdWordList {
    u32 count;
    char **words;
} WrdWordList;

typedef struct WrdData {
    u32 reserved;
    u32 count;
    char words[];
} WrdData;

extern void *heap_Calloc(void *heap, u32 count, u32 size);
extern void heap_Free(void *heap, void *ptr);

u32 WrdCreateWordList(void *heap, WrdWordList *wordList, WrdData *data)
{
    u32 i;
    char *word;

    if (wordList == NULL) {
        return WRD_INVALID_ARGUMENT;
    }

    wordList->count = data->count;
    wordList->words = heap_Calloc(heap, wordList->count, sizeof(char *));
    if (wordList->words == NULL) {
        return WRD_OUT_OF_MEMORY;
    }

    word = data->words;
    for (i = 0; i < wordList->count; i++) {
        wordList->words[i] = word;
        word += strlen(word) + 1;
    }
    return 0;
}

u32 WrdDestroyWordList(void *heap, WrdWordList *wordList)
{
    if (wordList == NULL) {
        return 0;
    }
    if (wordList->words != NULL) {
        heap_Free(heap, wordList->words);
    }
    wordList->words = NULL;
    wordList->count = 0;
    return 0;
}
