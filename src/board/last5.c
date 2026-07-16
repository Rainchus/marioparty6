#include "game/board/coin.h"
#include "game/board/guide.h"
#include "game/board/main.h"
#include "game/board/player.h"

#include "game/charman.h"
#include "game/frand.h"
#include "game/process.h"

#define LAST5_COIN_NUM 40

typedef struct Last5CoinWork_s {
    s16 delay;
    float velocity;
} LAST5COINWORK;

extern int mbCoinAddProcExec(int playerNo, int coinNum, BOOL dispF, BOOL fastF);

static void Last5PlayerOrderGet(int *playerOrder, int playerNum)
{
    int rank[GW_PLAYER_MAX];
    int i;
    int j;
    int swap;

    for (i = 0; i < playerNum; i++) {
        playerOrder[i] = i;
    }
    for (i = 0; i < playerNum - 1; i++) {
        j = i + mbRandMod(playerNum - i);
        swap = playerOrder[j];
        playerOrder[j] = playerOrder[i];
        playerOrder[i] = swap;
    }

    if (!GWTeamFGet()) {
        if (GwPlayer[playerOrder[playerNum - 1]].comF) {
            for (i = playerNum - 1; i >= 0; i--) {
                if (!GwPlayer[playerOrder[i]].comF) {
                    swap = playerOrder[playerNum - 1];
                    playerOrder[playerNum - 1] = playerOrder[i];
                    playerOrder[i] = swap;
                    break;
                }
            }
        }
    } else if (GwPlayer[mbPlayerTeamFindPlayer(playerOrder[playerNum - 1], 0)].comF
        && GwPlayer[mbPlayerTeamFindPlayer(playerOrder[playerNum - 1], 1)].comF) {
        for (i = playerNum - 1; i >= 0; i--) {
            if (!GwPlayer[mbPlayerTeamFindPlayer(playerOrder[i], 0)].comF
                || !GwPlayer[mbPlayerTeamFindPlayer(playerOrder[i], 1)].comF) {
                swap = playerOrder[playerNum - 1];
                playerOrder[playerNum - 1] = playerOrder[i];
                playerOrder[i] = swap;
                break;
            }
        }
    }

    for (i = 0; i < playerNum; i++) {
        if (playerNum == GW_PLAYER_MAX) {
            rank[i] = GwPlayer[playerOrder[i]].rank;
        } else {
            rank[i] = mbPlayerTeamRankGet(playerOrder[i]);
        }
    }
    for (i = 0; i < playerNum - 1; i++) {
        for (j = i + 1; j < playerNum; j++) {
            if (rank[i] > rank[j]) {
                swap = rank[j];
                rank[j] = rank[i];
                rank[i] = swap;
                swap = playerOrder[j];
                playerOrder[j] = playerOrder[i];
                playerOrder[i] = swap;
            }
        }
    }
}

static void ev_Last5Coin40(int playerNo, OMOBJ *guideObj)
{
    HuVecF playerPos;
    MBCOINOBJ *coinObj;
    LAST5COINWORK *coinWork;
    s16 coinObjId[LAST5_COIN_NUM];
    int activeNum;
    int coinNum = 1;
    int i;

    mbGuideMotionShiftSet(guideObj, 6, TRUE);
    mbGuideMotionStop(guideObj);
    HuPrcSleep(48);
    coinNum = LAST5_COIN_NUM;
    mbPlayerPosGet(playerNo, &playerPos);
    for (i = 0; i < coinNum; i++) {
        coinObjId[i] = mbCoinCreate2();
        mbCoinObjDispSet(coinObjId[i], FALSE);
        coinObj = mbCoinObjGet(coinObjId[i]);
        coinObj->pos.x = playerPos.x + (60.000004f * (frandf() - 0.5f));
        coinObj->pos.y = 800.0f + (playerPos.y + (50.0f * frandf()));
        coinObj->pos.z = playerPos.z + (60.000004f * (frandf() - 0.5f));
        coinObj->rot.x = 40.0f * (frandf() - 0.5f);
        coinObj->rot.y = 360.0f * frandf();
        coinObj->scale.x = coinObj->scale.y = coinObj->scale.z = 0.7f;
        coinWork = (LAST5COINWORK *)coinObj->work;
        coinWork->delay = (float)(i * 30) / coinNum;
        coinWork->velocity = -13.333334f;
    }

    activeNum = coinNum;
    while (activeNum != 0) {
        activeNum = 0;
        for (i = 0; i < coinNum; i++) {
            if (coinObjId[i] == 0) {
                continue;
            }
            activeNum++;
            coinObj = mbCoinObjGet(coinObjId[i]);
            coinWork = (LAST5COINWORK *)coinObj->work;
            if (coinWork->delay != 0) {
                coinWork->delay--;
                continue;
            }
            mbCoinObjDispSet(coinObjId[i], TRUE);
            coinWork->velocity += -0.5444445f;
            coinObj->pos.y += coinWork->velocity;
            if (coinObj->pos.y < 100.0f + playerPos.y) {
                mbCoinObjKill(coinObjId[i]);
                coinObjId[i] = 0;
            }
        }
        HuPrcVSleep();
    }

    mbGuideMotionShiftSet(guideObj, 1, TRUE);
    mbPlayerWinLoseVoicePlay(playerNo, 12, CHARVOICEID(6));
    mbPlayerMotionShiftSet(playerNo, 12, 0.0f, 12.0f, HU3D_MOTATTR_NONE);
    mbCoinAddProcExec(playerNo, coinNum, TRUE, TRUE);
    mbPlayerMotionEndWait(playerNo);
    mbPlayerMotIdleSet(playerNo);
}
