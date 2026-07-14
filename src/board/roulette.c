#include "game/board/roulette.h"
#include "game/board/audio.h"
#include "game/board/player.h"
#include "game/board/status.h"
#include "game/board/pause.h"

#include "game/sprite.h"
#include "game/pad.h"
#include "game/flag.h"
#include "game/frand.h"

#include "humath.h"
#include "math.h"

static s8 rouletteAngleNo = -1;
static HUSPR_GROUPID rouletteSprGrpId = HUSPR_GRP_NONE;
static int rouletteSeNo = MSM_SENO_NONE;
static float rouletteDecel = 0.92f;

static s8 rouletteChoiceTbl[GW_PLAYER_MAX];
static s16 rouletteComDelay;
static s16 rouletteCounter;
static float rouletteMaxSpeed;
static s16 rouletteComValue;
static float rouletteOffset;
static s16 rouletteComValueF;
static u8 rouletteCapsuleF;
static OMOBJ *rouletteOMObj;

typedef struct RouletteWork_s {
    unsigned killF : 1;
    unsigned stopF : 1;
    unsigned mode : 4;
    u8 maxPlayer;
    s8 playerNo;
    s8 time;
    u8 hideTime;
    u8 angleNo;
    s16 maxCounter;
    s16 angle;
    s16 speed;
} ROULETTE_WORK;

static void RouletteOMExec(OMOBJ *obj);
static void RouletteInit(ROULETTE_WORK *work);
static void RouletteWait(ROULETTE_WORK *work);
static void RouletteResultDisp(ROULETTE_WORK *work);
static void RouletteDispOn(ROULETTE_WORK *work);
static void RouletteDispOff(ROULETTE_WORK *work);
static void RouletteKill(void);
static void RouletteChoiceTblGet(int maxPlayer, int playerNo, s8 *result);
static int RoulettePadGet(ROULETTE_WORK *work);
static s16 RouletteComDelayGet(int playerNo, int maxPlayer);
static int RouletteComPadGet(ROULETTE_WORK *work);
static int RouletteAngleNoGet(ROULETTE_WORK *work);
static void RoulettePauseHook(BOOL pauseF);

static BOOL RouletteCreate(int playerNo, int maxPlayer, BOOL capsuleF)
{
    ROULETTE_WORK *work;
    int i;
    int num;
    if(capsuleF) {
        maxPlayer = 3;
        rouletteCapsuleF = TRUE;
    } else {
        rouletteCapsuleF = FALSE;
    }
    if(rouletteCapsuleF) {
        rouletteDecel = ((frand() & 0x7F)*(1.0f/127.0f)*0.05f)+0.92f;
    } else {
        rouletteDecel = 0.92f;
    }
    maxPlayer = 3;
    for(i=0; i<GW_PLAYER_MAX; i++) {
        rouletteChoiceTbl[i] = -1;
    }
    if(!rouletteCapsuleF) {
        for(i=0, num=0; i<GW_PLAYER_MAX; i++) {
            if(maxPlayer != 3 || i != playerNo) {
                rouletteChoiceTbl[num++] = i;
            }
        }
    } else {
        for(i=0, num=0; i<3; i++) {
            rouletteChoiceTbl[num++] = i;
        }
    }
    maxPlayer = num;
    if(maxPlayer <= 1) {
        if(num == 0) {
            rouletteChoiceTbl[0] = playerNo;
        }
        rouletteAngleNo = 0;
        return FALSE;
    }
    rouletteOMObj = omAddObj(mbObjMan, 259, 0, 0, RouletteOMExec);
    work = omObjGetWork(rouletteOMObj, ROULETTE_WORK);
    work->killF = FALSE;
    work->stopF = FALSE;
    work->maxPlayer = maxPlayer;
    work->playerNo = playerNo;
    work->time = 0;
    work->hideTime = 0;
    work->angleNo = 0;
    work->angle = 0;
    rouletteMaxSpeed = 18;
    if(GwPlayer[work->playerNo].comF) {
        work->maxCounter = 180;
    } else {
        work->maxCounter = 3600;
    }
    return TRUE;
}

BOOL mbRouletteCreate(int playerNo, int maxPlayer)
{
    return RouletteCreate(playerNo, maxPlayer, FALSE);
}

BOOL mbRouletteKaneCreate(int playerNo)
{
    return RouletteCreate(playerNo, 3, TRUE);
}

BOOL mbRouletteCheck(void)
{
    return (rouletteOMObj != NULL) ? FALSE : TRUE;
}

void mbRouletteWait(void)
{
    while(!mbRouletteCheck()) {
        HuPrcVSleep();
    }
}

int mbRouletteResultGet(void)
{
    return rouletteChoiceTbl[rouletteAngleNo];
}

void mbRouletteMaxSpeedSet(float speed)
{
    if(speed <= 0) {
        speed = 18;
    }
    rouletteMaxSpeed = speed;
}

static void RouletteOMExec(OMOBJ *obj)
{
    ROULETTE_WORK *work = omObjGetWork(obj, ROULETTE_WORK);
    if(work->killF || mbExitCheck()) {
        RouletteKill();
        rouletteOMObj = NULL;
        omDelObjEx(HuPrcCurrentGet(), obj);
        return;
    }
    if(rouletteCounter < work->maxCounter) {
        rouletteCounter++;
    }
    switch(work->mode) {
        case 0:
            RouletteInit(work);
            break;

        case 1:
            RouletteDispOn(work);
            break;

        case 2:
            RouletteWait(work);
            break;

        case 3:
            RouletteResultDisp(work);
            break;

        case 4:
            RouletteDispOff(work);
            break;
    }
}

static int rouletteFileTbl[6] = {
    DATANUM(DATA_board, 117),
    DATANUM(DATA_board, 120),
    DATANUM(DATA_board, 119),
    DATANUM(DATA_board, 120),
    DATANUM(DATA_board, 118),
    DATANUM(DATA_board, 118),
};

static s8 roulettePrioTbl[8+GW_PLAYER_MAX] = {
    60,
    80,
    80,
    80,
    20,
    30,
    70,
    70,
    70,
    70,
};

static int charFileTbl[GW_CHARA_MAX] = {
    DATANUM(DATA_board, 121),
    DATANUM(DATA_board, 122),
    DATANUM(DATA_board, 123),
    DATANUM(DATA_board, 124),
    DATANUM(DATA_board, 125),
    DATANUM(DATA_board, 126),
    DATANUM(DATA_board, 127),
    DATANUM(DATA_board, 128),
    DATANUM(DATA_board, 129),
    DATANUM(DATA_board, 130),
    DATANUM(DATA_board, 131),
    DATANUM(DATA_board, 130),
    DATANUM(DATA_board, 130),
    DATANUM(DATA_board, 130),
};

static int rouletteCapsuleFileTbl[4] = {
    DATANUM(DATA_board, 121),
    DATANUM(DATA_board, 122),
    DATANUM(DATA_board, 123),
    DATANUM(DATA_board, 124),
};

static void RouletteInit(ROULETTE_WORK *work)
{
    GXColor colorTbl[13] = {
        { 227, 67, 67, 255 },
        { 68, 67, 227, 255 },
        { 241, 158, 220, 255 },
        { 67, 228, 68, 255 },
        { 138, 60, 180, 255 },
        { 227, 228, 68, 255 },
        { 40, 40, 40, 255 },
        { 227, 227, 227, 255 },
        { 40, 227, 227, 255 },
        { 227, 139, 40, 255 },
        { 180, 40, 40, 255 },
        { 40, 180, 40, 255 },
        { 40, 40, 180, 255 },
    };

    s16 i;
    s16 num;
    HUSPRID sprId;

    s16 comDelay;

    float rate;
    float zRotScale;

    rouletteSeNo = MSM_SENO_NONE;
    rouletteCounter = 0;
    switch(work->maxPlayer) {
        case 3:
            num = 3;
            rate = 60;
            break;

        case 2:
            num = 2;
            rate = 60;
            break;
    }
    rouletteSprGrpId = HuSprGrpCreate(num+9);
    RouletteChoiceTblGet(work->maxPlayer, work->playerNo, rouletteChoiceTbl);
    if(GwPlayer[work->playerNo].comF) {
        rouletteComDelay = RouletteComDelayGet(work->playerNo, work->maxPlayer);
        switch(GwPlayer[work->playerNo].comDif) {
            case GW_PLAYER_COM_DIF_EASY:
                comDelay = mbRandMod(90);
                break;

            case GW_PLAYER_COM_DIF_NORMAL:
                comDelay = mbRandMod(60);
                break;

            case GW_PLAYER_COM_DIF_HARD:
                comDelay = mbRandMod(30);
                break;

            case GW_PLAYER_COM_DIF_VERYHARD:
                comDelay = mbRandMod(10);
                break;
        }
        comDelay -= mbRandMod(20);
        rouletteComDelay += comDelay;
        if(rouletteComDelay > 360.0f) {
            rouletteComDelay -= 360.0f;
        }
        if(rouletteComDelay < 0) {
            rouletteComDelay += 360.0f;
        }
    } else {
        rouletteComDelay = 0;
    }
    for(i=0; i<=4; i++) {
        mbSprCreate(mbBoardDataNumGet(rouletteFileTbl[i]), roulettePrioTbl[i], NULL, &sprId);
        HuSprGrpMemberSet(rouletteSprGrpId, i, sprId);
        HuSprAttrSet(rouletteSprGrpId, i, HUSPR_ATTR_LINEAR);
    }
    HuSprZRotSet(rouletteSprGrpId, 4, 180);
    for(i=1; i<4; i++) {
        HuSprDispOff(rouletteSprGrpId, i);
    }
    switch(work->maxPlayer) {
        case 4:
            HuSprDispOn(rouletteSprGrpId, 3);
            break;

        case 3:
            HuSprDispOn(rouletteSprGrpId, 2);
            break;

        case 2:
            HuSprDispOn(rouletteSprGrpId, 1);
            break;
    }
    for(i=0; i<num; i++) {
        if(!rouletteCapsuleF) {
            s16 charNo = GwPlayer[rouletteChoiceTbl[i]].charNo;
            mbSprCreate(mbBoardDataNumGet(charFileTbl[charNo]), roulettePrioTbl[i+5], NULL, &sprId);
        } else {
            mbSprCreate(mbBoardDataNumGet(rouletteCapsuleFileTbl[rouletteChoiceTbl[i]]), roulettePrioTbl[i+5], NULL, &sprId);
        }
        HuSprGrpMemberSet(rouletteSprGrpId, i+5, sprId);
        HuSprAttrSet(rouletteSprGrpId, i+5, HUSPR_ATTR_LINEAR);
    }
    HuSprGrpPosSet(rouletteSprGrpId, 288, 240);
    HuSprGrpScaleSet(rouletteSprGrpId, 0.01f, 0.01f);
    OSs16tof32(&num, &zRotScale);
    zRotScale = 360/zRotScale;
    for(i=0; i<num; i++) {
        switch(work->maxPlayer) {
            case 3:
                switch(i) {
                    case 0:
                        HuSprPosSet(rouletteSprGrpId, i+5, 50, -35);
                        break;

                    case 1:
                        HuSprPosSet(rouletteSprGrpId, i+5, 0, 57);
                        break;

                    case 2:
                        HuSprPosSet(rouletteSprGrpId, i+5, -50, -35);
                        break;
                }
                break;

            case 2:
                switch(i) {
                    case 0:
                        HuSprPosSet(rouletteSprGrpId, i+5, 50, 0);
                        break;

                    case 1:
                        HuSprPosSet(rouletteSprGrpId, i+5, -50, 0);
                        break;
                }
                break;
        }
    }
    mbPauseHookPush(RoulettePauseHook);
    for(i=0; i<3; i++) {
        if(rouletteChoiceTbl[i] == rouletteComValue) {
            break;
        }
    }
    rouletteOffset = i*120.0f;
    if(rouletteOffset > 180) {
        rouletteOffset -= 360;
    }
    work->mode = 1;
}

static void RouletteWait(ROULETTE_WORK *work)
{
    float speed;
    float angle;

    float pitch;

    s16 angle2;
    s16 angleDist;
    u32 btn;
    s16 pitchVal;

    OSs16tof32(&work->angle, &angle);
    OSs16tof32(&work->speed, &speed);
    angle = 0.0625f*angle;
    speed = 0.0625f*speed;
    if(!work->stopF) {
        if(speed < 18) {
            speed += 0.7f;
            if(speed > rouletteMaxSpeed) {
                speed = rouletteMaxSpeed;
            }
        } else {
            btn = RoulettePadGet(work);
            if(btn & PAD_BUTTON_A) {
                work->stopF = TRUE;
            }
        }
    } else {
        OSf32tos16(&angle, &angle2);
        switch(work->maxPlayer) {
            case 3:
                angleDist = 120;
                break;

            case 2:
                angleDist = 180;
                break;
        }
        angle2 %= angleDist;
        if(speed < 0.5f && (angle2 < 2 || angle2 >= angleDist-2)) {
            speed += speed/2;
        }
        speed *= rouletteDecel;
    }
    if(work->angleNo != RouletteAngleNoGet(work)) {
        work->angleNo = RouletteAngleNoGet(work);
    }
    pitch = 8191-(8191*(speed/rouletteMaxSpeed));
    OSf32tos16(&pitch, &pitchVal);
    HuAudFXPitchSet(rouletteSeNo, -pitchVal);
    if(speed > -0.0000001f && speed < 0.0000001f) {
        work->mode = 3;
        if(rouletteSeNo != MSM_SENO_NONE) {
            mbAudFXStop(rouletteSeNo);
            rouletteSeNo = MSM_SENO_NONE;
        }
        mbAudFXPlay(0x3F7);
    }
    angle += speed;
    if(angle > 360) {
        angle -= 360;
    }
    HuSprZRotSet(rouletteSprGrpId, 4, fmod(180+angle, 360));
    angle *= 16;
    speed *= 16;
    OSf32tos16(&angle, &work->angle);
    OSf32tos16(&speed, &work->speed);
}

static void RouletteResultDisp(ROULETTE_WORK *work)
{
    u8 hideTime;
    if(work->hideTime == 0) {
        work->angle >>= 4;
        rouletteAngleNo = RouletteAngleNoGet(work);
    }
    hideTime = work->hideTime%6;
    if(hideTime < 3) {
        HuSprDispOff(rouletteSprGrpId, rouletteAngleNo+5);
    } else {
        HuSprDispOn(rouletteSprGrpId, rouletteAngleNo+5);
    }
    if(work->hideTime < 90) {
        work->hideTime++;
    } else {
        HuSprDispOn(rouletteSprGrpId, rouletteAngleNo+5);
        work->mode = 4;
    }
}

static void RouletteDispOn(ROULETTE_WORK *work)
{
    float angle;
    if(work->time < 90) {
        work->time += 3;
    } else {
        _CheckFlag(FLAG_BOARD_TUTORIAL);
        work->time = 90;
        work->mode = 2;
        rouletteSeNo = mbAudFXPlay(0x3F5);
    }
    OSs8tof32(&work->time, &angle);
    HuSprGrpScaleSet(rouletteSprGrpId, HuSin(angle), HuSin(angle));
}

static void RouletteDispOff(ROULETTE_WORK *work)
{
    float angle;
    if(work->time > 0) {
        work->time -= 3;
    } else {
        work->time = 0;
        work->killF = TRUE;
        work->mode = 255;
        rouletteComValueF = FALSE;
    }
    OSs8tof32(&work->time, &angle);
    HuSprGrpScaleSet(rouletteSprGrpId, HuSin(angle), HuSin(angle));
}

static void RouletteKill(void)
{
    if(rouletteSprGrpId != HUSPR_GRP_NONE) {
        mbPauseHookPop(RoulettePauseHook);
        HuSprGrpKill(rouletteSprGrpId);
        rouletteSprGrpId = HUSPR_GRP_NONE;
    }
}

static void RouletteChoiceTblGet(int maxPlayer, int playerNo, s8 *result)
{
    int i;
    for(i=0; i<255; i++) {
        int index1 = (frand() & 0x7FFF)%maxPlayer;
        int index2 = (frand() & 0x7FFF)%maxPlayer;
        if(index1 != index2) {
            s8 temp = result[index1];
            result[index1] = result[index2];
            result[index2] = temp;
        }
    }
    for(i=0; i<maxPlayer; i++) {

    }
}

static int RoulettePadGet(ROULETTE_WORK *work)
{
    int btn;
    if(rouletteCounter >= work->maxCounter) {
        return PAD_BUTTON_A;
    }
    if(!GwPlayer[work->playerNo].comF) {
        btn = HuPadBtnDown[GwPlayer[work->playerNo].padNo];
        return btn;
    } else {
        btn = RouletteComPadGet(work);
        if(rouletteComValueF) {
            ROULETTE_WORK *workP = omObjGetWork(rouletteOMObj, ROULETTE_WORK);
            float angleStart;
            float angleEnd;
            float angle;
            float speed;
            OSs16tof32(&workP->angle, &angle);
            OSs16tof32(&workP->speed, &speed);
            angle = 0.0625f*angle;
            speed = 0.0625f*speed;
            angleStart = angle+(speed*46);
            angleStart = fmod(angleStart, 360);
            if(angleStart > 180) {
                angleStart -= 360;
            }
            angleEnd = rouletteOffset-angleStart;
            if(angleEnd >= 360) {
                angleEnd += 360;
            }
            if(angleEnd >= 0 && angleEnd < (120-(2*speed))) {
                btn = PAD_BUTTON_A;
            } else {
                btn = 0;
            }
        }
        return btn;
    }
}

void mbRouletteComValueSet(s16 value)
{
    rouletteComValueF = TRUE;
    rouletteComValue = value;
}

static s16 RouletteComDelayGet(int playerNo, int maxPlayer)
{
    int playerBest;
    int num;
    int i;
    s16 delay;
    if(GWPartyGet() == TRUE) {
        playerBest = mbPlayerBestPathGet();
    }
    if(playerBest == -1 || playerBest == playerNo) {
        do {
            playerBest = mbRandMod(GW_PLAYER_MAX);
        } while(playerBest == playerNo);

    }
    num = maxPlayer;
    for(delay=i=0; i<num; i++) {
        if(playerBest == rouletteChoiceTbl[i]) {
            float angle = i*(360/num);
            angle += 45;
            angle -= 180;
            if(angle < 0) {
                angle += 360;
            }
            if(angle >= 360) {
                angle -= 360;
            }
            OSf32tos16(&angle, &delay);
            break;
        }
    }
    return delay;
}

static int RouletteComPadGet(ROULETTE_WORK *work)
{
    int angle;
    s16 origAngle;
    int delayMax;
    int delayMin;
    origAngle = work->angle >> 4;
    switch(work->maxPlayer) {
        case 3:
            angle = 120;
            break;

        case 2:
            angle = 180;
            break;
    }
    delayMin = rouletteComDelay;
    delayMax = rouletteComDelay+angle;
    if(origAngle >= delayMin && origAngle < delayMax) {
        return PAD_BUTTON_A;
    } else {
        return 0;
    }
}

static int RouletteAngleNoGet(ROULETTE_WORK *work)
{
    int result;
    switch(work->maxPlayer) {
        case 3:
            result = work->angle/120;
            break;

        case 2:
            result = work->angle/180;
            break;
    }
    return result;
}

static void RoulettePauseHook(BOOL pauseF)
{
    if(pauseF == 0) {
        HuSprGrpPosSet(rouletteSprGrpId, 1024, 1024);
    } else {
        HuSprGrpPosSet(rouletteSprGrpId, 288, 240);
    }
}
