#include "types.h"

#include "gssdk/langdata.h"

BOOL _langCheckDataType(LanguageData *language)
{
    return language->data->dataType == 0x1003;
}

u32 _langGetSize(LanguageData *language)
{
    return language->data->size;
}

void *_langGetVersionInfo(LanguageData *language)
{
    return language->data->versionInfo;
}

u32 _langGetNbrCodeBook(LanguageData *language)
{
    return language->data->nbrCodeBook;
}

u32 _langGetNbrState(LanguageData *language)
{
    return language->data->nbrState;
}

u32 _langGetNbrPhenome(LanguageData *language)
{
    return language->data->nbrErgodicPhenomes +
           language->data->nbrNonErgodicPhenomes;
}

u32 _langGetAdaptSilState(LanguageData *language)
{
    return language->data->adaptSilState;
}

u32 _langGetNbrErgodicStates(LanguageData *language)
{
    return language->data->nbrErgodicStates;
}

u32 _langGetNbrStateErgodicStates(LanguageData *language)
{
    return language->data->nbrStateErgodicStates;
}

u32 _langGetNbrErgodicPhenomes(LanguageData *language)
{
    return language->data->nbrErgodicPhenomes;
}

u32 _langGetNbrStateErgodicPhenomes(LanguageData *language)
{
    return language->data->nbrStateErgodicPhenomes;
}

u32 _langGetNbrNonErgodicPhenomes(LanguageData *language)
{
    return language->data->nbrNonErgodicPhenomes;
}

u32 _langGetSilencePhenome(LanguageData *language)
{
    return language->data->silencePhenome;
}

u32 _langGetUWSilencePhenome(LanguageData *language)
{
    return language->data->userWordSilencePhenome;
}

u32 _langGetSingleWordGarbagePhenome(LanguageData *language)
{
    return language->data->singleWordGarbagePhenome;
}

u32 _langGetSentenceGarbagePhenome(LanguageData *language)
{
    return language->data->sentenceGarbagePhenome;
}

u32 _langGetAnySpeechGarbagePhenome(LanguageData *language)
{
    return language->data->anySpeechGarbagePhenome;
}

u32 _langGetNbrSpeechUnit(LanguageData *language)
{
    return language->data->nbrSpeechUnit;
}

u32 _langGetNbrWarpFactors(LanguageData *language)
{
    return language->data->nbrWarpFactors;
}

u32 _langGetNbrSpeechUnitClass(LanguageData *language)
{
    return language->data->nbrSpeechUnitClass;
}

u32 _langGetNbrTones(LanguageData *language)
{
    return language->data->nbrTones;
}

u32 _langGetUserWordSpeechUnitClass(LanguageData *language)
{
    return language->data->userWordSpeechUnitClass;
}

u32 _langGetNbrTransWord(LanguageData *language)
{
    return language->data->nbrTransWord;
}

u32 _langGetNbrPhenUserWordTraining(LanguageData *language)
{
    return language->data->nbrPhenUserWordTraining;
}

s32 _langGetRecogWTP(LanguageData *language)
{
    return language->data->recogWTP;
}

s32 _langGetSpellingWTP(LanguageData *language)
{
    return language->data->spellingWTP;
}

u32 _cdbGetCodeBookDim(CodeBookData *codeBook)
{
    return codeBook->dimension;
}

u32 _cdbGetFirstCdbSize(CodeBookData *codeBook)
{
    return codeBook->firstSize;
}

u32 _cdbGetNbrInSecSearch(CodeBookData *codeBook)
{
    return codeBook->nbrInSecondSearch;
}

u32 _cdbGetSecondCdbSize(CodeBookData *codeBook)
{
    return codeBook->secondSize;
}

u32 _cdbGetCompStart(CodeBookData *codeBook)
{
    return codeBook->compressedStart;
}

u32 _cdbGetNbrGastone(CodeBookData *codeBook)
{
    return codeBook->nbrGastone;
}

u16 _langGetSilPhenome(LanguageData *language)
{
    return language->data->silencePhenome;
}

void *_langGetpWarpFactors(LanguageData *language)
{
    return language->data->payload;
}

void *_langGetpGastone(LanguageData *language)
{
    return language->data->payload + language->data->nbrWarpFactors * 8;
}

void *_langGetpErgodicStates(LanguageData *language)
{
    LanguageDataV2 *data = language->data;

    return data->payload + data->nbrWarpFactors * 8 +
           data->nbrState * data->nbrCodeBook * 2;
}

void *_langGetpErgodicPhenomes(LanguageData *language)
{
    LanguageDataV2 *data = language->data;

    return data->payload + data->nbrWarpFactors * 8 +
           data->nbrState * data->nbrCodeBook * 2 +
           (data->nbrErgodicStates + 1) * 2 +
           data->nbrStateErgodicStates * 2;
}

void *_langGetpErgodicPenalty(LanguageData *language)
{
    LanguageDataV2 *data = language->data;

    return data->payload + data->nbrWarpFactors * 8 +
           data->nbrState * data->nbrCodeBook * 2 +
           (data->nbrErgodicStates + 1) * 2 +
           data->nbrStateErgodicStates * 2 +
           (data->nbrErgodicPhenomes + 1) * 2 +
           data->nbrStateErgodicPhenomes * 2;
}
