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

typedef struct CodeBookData {
    u32 dimension;
    u32 firstSize;
    u32 nbrInSecondSearch;
    u32 secondSize;
    u32 compressedStart;
    u32 nbrGastone;
} CodeBookData;

typedef struct LanguageData LanguageData;
typedef void (*LanguageDataMethod)(void);

struct LanguageData {
    LanguageDataV2 *data;
    BOOL (*checkDataType)(LanguageData *language);
    u32 (*getSize)(LanguageData *language);
    void *(*getVersionInfo)(LanguageData *language);
    u32 (*getNbrCodeBook)(LanguageData *language);
    u32 (*getNbrState)(LanguageData *language);
    u32 (*getNbrPhenome)(LanguageData *language);
    LanguageDataMethod reserved1C;
    u32 (*getAdaptSilState)(LanguageData *language);
    u32 (*getNbrErgodicStates)(LanguageData *language);
    u32 (*getNbrStateErgodicStates)(LanguageData *language);
    u32 (*getNbrErgodicPhenomes)(LanguageData *language);
    u32 (*getNbrStateErgodicPhenomes)(LanguageData *language);
    u32 (*getNbrNonErgodicPhenomes)(LanguageData *language);
    u32 (*getSilencePhenome)(LanguageData *language);
    u32 (*getUWSilencePhenome)(LanguageData *language);
    u32 (*getSingleWordGarbagePhenome)(LanguageData *language);
    u32 (*getSentenceGarbagePhenome)(LanguageData *language);
    u32 (*getAnySpeechGarbagePhenome)(LanguageData *language);
    u32 (*getNbrSpeechUnit)(LanguageData *language);
    LanguageDataMethod reserved50;
    u32 (*getNbrSpeechUnitClass)(LanguageData *language);
    u32 (*getNbrTones)(LanguageData *language);
    u32 (*getUserWordSpeechUnitClass)(LanguageData *language);
    u32 (*getNbrTransWord)(LanguageData *language);
    u32 (*getNbrPhenUserWordTraining)(LanguageData *language);
    s32 (*getRecogWTP)(LanguageData *language);
    s32 (*getSpellingWTP)(LanguageData *language);
    u32 (*getCodeBookDim)(CodeBookData *codeBook);
    u32 (*getFirstCdbSize)(CodeBookData *codeBook);
    u32 (*getNbrInSecSearch)(CodeBookData *codeBook);
    u32 (*getSecondCdbSize)(CodeBookData *codeBook);
    u32 (*getCompStart)(CodeBookData *codeBook);
    u32 (*getNbrGastone)(CodeBookData *codeBook);
    u32 *(*getpWarpFactors)(LanguageData *language);
    void *(*getpGastone)(LanguageData *language);
    LanguageDataMethod reserved90;
    u16 (*getSilPhenome)(LanguageData *language);
    void *(*getpErgodicStates)(LanguageData *language);
    u16 *(*getpErgodicPhenomes)(LanguageData *language);
    void *(*getpErgodicPenalty)(LanguageData *language);
    CodeBookData *(*getpCodeBook)(LanguageData *language, u32 index);
    LanguageDataMethod reservedA8[10];
    BOOL (*checkBitField)(LanguageData *language, u32 bitField);
    LanguageDataMethod reservedD4[5];
    f32 *(*getpFirstCdb)(CodeBookData *codeBook);
    u32 *(*getpIndexInSecCdb)(CodeBookData *codeBook);
    f32 *(*getpSecondCdb)(CodeBookData *codeBook);
    LanguageDataMethod reservedF4[18];
};

#endif
