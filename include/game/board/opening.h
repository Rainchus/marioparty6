#ifndef _BOARD_OPENING_H
#define _BOARD_OPENING_H

#include "game/board/object_data.h"
#include "humath.h"

void mbev_Opening(void);
void mbev_OpeningParty(void);
void mbev_OpeningSingle(void);

void mbOpeningViewSet(HuVecF *rot, HuVecF *pos, float zoom);
float mbOpeningZoomGet(void);
void mbOpeningRotGet(HuVecF *rot);
void mbOpeningPosGet(HuVecF *pos);
void mbOpeningInstHookSet(void (*hook)(void));
void mbOpeningStarInstHookSet(void (*hook)(void));
MBMODELID mbOpeningGuideObjIdGet(void);
void mbOpeningGuidePosRestore(void);
void mbOpeningCameraPosRestore(void);

#endif
