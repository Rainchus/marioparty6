#ifndef _BOARD_BRANCH_H
#define _BOARD_BRANCH_H

#include "dolphin.h"

typedef int (*MBBRANCHCOMSTARHOOK)(int playerNo, int linkNum, s16 *linkTbl, BOOL debugF);

void mbBranchInit(void);
BOOL mbev_Branch(int playerNo, s16 *masuId);
BOOL mbev_BranchDebug(int playerNo, s16 *masuId);

void mbBranchAttrSet(u16 attr);
void mbBranchMAttrSet(u32 attr);
void mbBranchAttrReset(u16 attr);
void mbBranchMAttrReset(u32 attr);
void mbBranchAttrInit(void);
void mbBranchMAttrInit(void);
u16 mbBranchAttrGet(void);
u32 mbBranchMAttrGet(void);
BOOL mbBranchAttrCheck(int masuId);
void mbBranchComStarHookSet(MBBRANCHCOMSTARHOOK hook);

#endif
