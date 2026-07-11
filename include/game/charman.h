#ifndef _GAME_CHARMAN_H
#define _GAME_CHARMAN_H

#include "dolphin.h"
#include "game/hu3d.h"
#include "game/armem.h"

#include "msm_se.h"
#include "datanum/charmot.h"


#define CHARVOICEID(no) (MSM_SE_CHARVOICE_MARIO+(no))
#define CHARSEID(no) (MSM_SE_CHAR_MARIO+(no))

#define CHARNO_NONE -1
#define CHARNO_MARIO 0
#define CHARNO_LUIGI 1
#define CHARNO_PEACH 2
#define CHARNO_YOSHI 3
#define CHARNO_WARIO 4
#define CHARNO_DAISY 5
#define CHARNO_WALUIGI 6
#define CHARNO_KINOPIO 7
#define CHARNO_TERESA 8
#define CHARNO_MINIKOOPA 9
#define CHARNO_KINOPICO 10
#define CHARNO_MINIKOOPAR 11
#define CHARNO_MINIKOOPAG 12
#define CHARNO_MINIKOOPAB 13
#define CHARNO_MAX 14

#define CHAR_MODEL0 (1 << 0)
#define CHAR_MODEL1 (1 << 1)
#define CHAR_MODEL2 (1 << 2)
#define CHAR_MODEL3 (1 << 3)
#define CHAR_MODEL_MAX 4

#define CHAR_MOT_MAX 32

#define CHAR_NPC_MAX 14
#define CHAR_NPC_NO_BASE 15
#define CHAR_NPC_NONE -1

#define CHAR_MOTNO(dataNum) FILENUM(dataNum)

#define CHARMOT_DATANUM(dataNum, charName) (DATANUM(DATA_##charName##mot, FILENUM(dataNum)))
#define CHAR_DATANUM(dataNum, charName) (DATANUM(DATA_##charName, FILENUM(dataNum)))

void CharInit(void);
AMEM_PTR CharMotionAMemPGet(s16 charNo);
void CharMotionInit(s16 charNo);
void CharMotionInitAsync(s16 charNo);
void CharMotionClose(s16 charNo);
void CharDataClose(s16 charNo);
void CharClose(s16 charNo);
HU3D_MODELID CharModelCreate(s16 charNo, s16 model);

HU3D_MOTIONID CharMotionCreate(s16 charNo, unsigned int dataNum);
void CharMotionNoSet(s16 charNo, HU3D_MOTIONID motId, unsigned int motNo);
void CharMotionKill(s16 charNo, unsigned int motId);
void CharMotionDataClose(s16 charNo);
void CharModelDataClose(s16 charNo);
void CharModelKill(s16 charNo);
void CharMotionSet(s16 charNo, HU3D_MOTIONID motId);

char **CharModelEyeBmpGet(s16 charNo, s16 model);
char *CharModelItemHookGet(s16 charNo, s16 model, s16 hookNo);

void CharMotionTimeSet(s16 charNo, float time);
float CharMotionTimeGet(s16 charNo);
float CharMotionMaxTimeGet(s16 charNo);
BOOL CharMotionEndCheck(s16 charNo);
HU3D_MOTIONID CharMotionShiftIDGet(s16 charNo);
void CharMotionShiftSet(s16 charNo, HU3D_MOTIONID motId, float start, float end, u32 attr);
float CharMotionShiftTimeGet(s16 charNo);
void CharMotionSpeedSet(s16 charNo, float speed);
void CharMotionStartEndSet(s16 charNo, float start, float end);
void CharModelAttrSet(s16 charNo, u32 attr);
void CharModelAttrReset(s16 charNo, u32 attr);
float CharModelHeightGet(s16 charNo);
HU3D_MODELID CharModelMotListCreate(s16 charNo, s16 model, unsigned int *motDataNum, HU3D_MOTIONID *motId);
s32 CharModelTimingHookNoGet(s16 charNo);
void CharModelHookDustCreate(s16 charNo, char *objName);
void CharEffectSmokeCreate(s16 cameraBit, HuVecF *pos);
void CharEffectCoinGlowCreate(s16 cameraBit, HuVecF *pos);
void CharModelHitCreate(s16 charNo);
void CharEffectHitCreate(s16 cameraBit, HuVecF *pos, HuVecF *rot);
void CharModelShoeHitCreate(s16 charNo);
void CharEffectShoeHitCreate(s16 cameraBit, HuVecF *pos, HuVecF *rot);
void CharEffectLayerSet(s16 layerNo);
void CharMotionVoiceOnSet(s16 charNo, s16 motNo, BOOL voiceOn);
void CharModelVoicePanAutoSet(s16 charNo, BOOL voicePanAuto);
void CharModelVoiceFlagSet(s16 charNo, BOOL fxFlag);
void CharMotionUpdateSet(s16 charNo, unsigned int dataNum, BOOL updateF);
s32 CharNpcDustSet(HU3D_MODELID modelId, HU3D_MOTIONID motId, s16 type, s16 npcNo);
s32 CharNpcDustVoiceOffSet(HU3D_MODELID modelId, HU3D_MOTIONID motId, s16 type);
void CharModelStepSet(s16 charNo, s16 stepFx);

void CharModelLandDustCreate(s16 charNo, HuVecF *pos);
void CharEffectDustCreate(s16 cameraBit, float scale, HuVecF *pos);
void CharEffectCryCreate(s16 cameraBit, HuVecF *pos, HuVecF *offset, float scale);
void CharModelCryCreate(s16 charNo, float yOfs, float ofsY);

unsigned int CharModelFileNumGet(s16 charNo, s16 model);
unsigned int CharMotionFileNumGet(s16 charNo, u16 motId);
void CharModelVoicePanSet(s16 charNo, s16 pan);
void CharModelVoiceVolSet(s16 charNo, s16 vol);

void CharWinLoseVoicePlay(s16 charNo, unsigned int motId, s16 seId);
void CharLoseVoicePlay(s16 charNo1, s16 charNo2, s16 charNo3, s16 charNo4);

s16 CharMotionTotalTimeGet(s16 charNo, int motNo);
s16 CharMotionExtraTimeGet(s16 charNo, int motNo);

void CharMotionLoad(s16 *statList);
void CharEffectSmokeCreateScale(s16 cameraBit, HuVecF *pos, float scale);
void CharModelLandDustCreateStep(s16 charNo, HuVecF *pos);
void CharEffectHipDropCreate(s16 charNo, HuVecF *pos);
void CharEffectWarnCreate(s16 charNo, float scale);
u32 CharAttrGet(s16 charNo);
HU3D_LIGHTID CharLightCreateV(HuVecF *pos, HuVecF *dir, GXColor *color);
HU3D_LIGHTID CharLightCreate(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, u8 r, u8 g, u8 b);
void CharLightSpotSet(s32 func, float cutoff);
void CharLightInfinitytSet(void);
void CharLightPointSet(s32 func, float cutoff, float brightness);
void CharLightColorSet(u8 r, u8 g, u8 b, u8 a);
void CharLightPosAimSetV(HuVecF *pos, HuVecF *aim);
void CharLightPosAimSet(float posX, float posY, float posZ, float aimX, float aimY, float aimZ);
void CharLightStaticSet(BOOL staticF);

extern unsigned int CharDataDirTbl[CHARNO_MAX][6];

#endif
