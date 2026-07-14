#ifndef GSSDK_LANGDATA_H
#define GSSDK_LANGDATA_H

#include "types.h"

typedef struct LanguageDataV2 {
    u32 dataType;
    u32 size;
    u8 versionInfo[0x80];
    u32 nbrCodeBook;
    u32 nbrState;
    u32 nbrNonErgodicPhenomes;
    u32 nbrErgodicStates;
    u32 nbrStateErgodicStates;
    u32 nbrErgodicPhenomes;
    u32 nbrStateErgodicPhenomes;
    u32 silencePhenome;
    u32 userWordSilencePhenome;
    u32 adaptSilState;
    u32 singleWordGarbagePhenome;
    u32 sentenceGarbagePhenome;
    u32 anySpeechGarbagePhenome;
    u32 nbrSpeechUnit;
    u32 nbrSpeechUnitClass;
    u32 nbrTones;
    u32 userWordSpeechUnitClass;
    u32 nbrWarpFactors;
    u32 nbrTransWord;
    u32 nbrPhenUserWordTraining;
    u32 flags;
    s16 recogWTP;
    s16 spellingWTP;
    u8 payload[];
} LanguageDataV2;

typedef struct LanguageData {
    LanguageDataV2 *data;
} LanguageData;

typedef struct CodeBookData {
    u32 dimension;
    u32 firstSize;
    u32 nbrInSecondSearch;
    u32 secondSize;
    u32 compressedStart;
    u32 nbrGastone;
} CodeBookData;

#endif
