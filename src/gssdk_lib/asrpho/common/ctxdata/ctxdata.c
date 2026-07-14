#include "types.h"

#include <stddef.h>
#include <string.h>

typedef struct ContextDataV2 {
    u32 dataType;
    u32 size;
    u8 languageVersionInfo[0x80];
    u32 ts;
    u32 accuracy;
    u32 rejection;
    u32 garbage;
    u32 nbrItem;
    u32 nbrWord;
    u32 nbrPron;
    u32 nbrFollower;
    u32 nbrStartWord;
    u32 nbrStopWord;
    u32 garbageWordID;
    u32 contextType;
    u16 lex[1];
} ContextDataV2;

typedef struct ContextData {
    ContextDataV2 *data;
} ContextData;

typedef void (*ContextV2Function)(void);

BOOL _contextCheckDataType(ContextData *context)
{
    return context->data->dataType == 6;
}

u32 _contextGetSize(ContextData *context)
{
    return context->data->size;
}

void *_contextGetLanguageVersionInfo(ContextData *context)
{
    return context->data->languageVersionInfo;
}

u32 _contextGetTS(ContextData *context)
{
    return context->data->ts;
}

u32 _contextGetAccuracy(ContextData *context)
{
    return context->data->accuracy;
}

u32 _contextGetRejection(ContextData *context)
{
    return context->data->rejection;
}

u32 _contextGetGarbage(ContextData *context)
{
    return context->data->garbage;
}

u32 _contextGetNbrItem(ContextData *context)
{
    return context->data->nbrItem;
}

u32 _contextGetNbrWord(ContextData *context)
{
    return context->data->nbrWord;
}

u32 _contextGetNbrPron(ContextData *context)
{
    return context->data->nbrPron;
}

u32 _contextGetNbrFollower(ContextData *context)
{
    return context->data->nbrFollower;
}

u32 _contextGetNbrStartWord(ContextData *context)
{
    return context->data->nbrStartWord;
}

u32 _contextGetNbrStopWord(ContextData *context)
{
    return context->data->nbrStopWord;
}

u32 _contextGetGarbageWordID(ContextData *context)
{
    return context->data->garbageWordID;
}

u8 _contextIsSpellingContext(ContextData *context)
{
    return context->data->contextType == 1;
}

u8 _contextIsIsoWordContext(ContextData *context)
{
    return context->data->contextType == 2;
}

u16 *_contextGetpLex(ContextData *context)
{
    return context->data->lex;
}

u16 *_contextGetpBeginOfWords(ContextData *context)
{
    ContextDataV2 *data = context->data;
    u32 count = data->nbrItem + data->nbrPron;
    u8 *words;

    count += count & 1;
    words = (u8 *)data + count * sizeof(u16);
    return (u16 *)(words + offsetof(ContextDataV2, lex));
}

void *_contextGetpWordProp(ContextData *context)
{
    ContextDataV2 *data = context->data;
    u32 lexCount = data->nbrItem + data->nbrPron;
    u32 wordPropCount;
    u8 *wordProp;

    lexCount += lexCount & 1;
    wordPropCount = data->nbrPron + data->nbrWord;
    wordProp = (u8 *)data + lexCount * sizeof(u16);
    wordProp += offsetof(ContextDataV2, lex);
    return wordProp + (wordPropCount + 2) * sizeof(u32);
}

void *_contextGetpSyntax(ContextData *context)
{
    ContextDataV2 *data = context->data;
    u32 lexCount = data->nbrItem + data->nbrPron;
    u32 wordPropCount;
    u32 syntaxCount;
    u8 *syntax;

    lexCount += lexCount & 1;
    wordPropCount = data->nbrPron + data->nbrWord;
    syntaxCount = data->nbrWord;
    syntaxCount += syntaxCount & 1;
    syntax = (u8 *)data + lexCount * sizeof(u16);
    syntax += offsetof(ContextDataV2, lex);
    syntax += (wordPropCount + 2) * sizeof(u32);
    return syntax + syntaxCount * sizeof(u16);
}

const ContextV2Function _ContextV2VirtualTable[] = {
    NULL,
    (ContextV2Function)_contextCheckDataType,
    (ContextV2Function)_contextGetSize,
    (ContextV2Function)_contextGetLanguageVersionInfo,
    (ContextV2Function)_contextGetTS,
    (ContextV2Function)_contextGetAccuracy,
    (ContextV2Function)_contextGetRejection,
    (ContextV2Function)_contextGetGarbage,
    (ContextV2Function)_contextGetNbrItem,
    (ContextV2Function)_contextGetNbrWord,
    (ContextV2Function)_contextGetNbrPron,
    (ContextV2Function)_contextGetNbrFollower,
    (ContextV2Function)_contextGetNbrStartWord,
    (ContextV2Function)_contextGetNbrStopWord,
    (ContextV2Function)_contextGetGarbageWordID,
    (ContextV2Function)_contextIsSpellingContext,
    (ContextV2Function)_contextGetpLex,
    (ContextV2Function)_contextGetpBeginOfWords,
    (ContextV2Function)_contextGetpWordProp,
    (ContextV2Function)_contextGetpSyntax,
    (ContextV2Function)_contextIsIsoWordContext,
};

void FillContextV2VirtualTable(ContextV2Function *table)
{
    memcpy(table, _ContextV2VirtualTable, sizeof(_ContextV2VirtualTable));
}
