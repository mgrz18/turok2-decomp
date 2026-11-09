#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "protos/scene.h"
#include "protos/player.h"

s32 Cinema_RequestById(s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x9F0];
    /* 0x9F0 */ s32 unk9F0;
} Unk002532A8;

s32 CEngineApp__StartWarp(sptr, s32);                /* extern */

void EngineMode15_Ending_Init(sptr arg0) {
    void *temp_v0;

    gFinalEnding = 0;
    sCreditsLine = 0;
    gCreditsScroll = (f32) gScreenHeight;
    gCreditsDone = 0;
    temp_v0 = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if ((temp_v0 != NULL) && (((Unk002532A8 *)temp_v0)->unk9F0 != 0)) {
        gEndingPrimagenDefeated = 1;
    } else {
        gEndingPrimagenDefeated = 0;
    }
    if (gEndingPrimagenDefeated != 0) {
        CScene__RequestWarp(&gScene, ~Cinema_RequestById(0x82), 0);
        return;
    }
    CEngineApp__StartWarp(arg0, 0x82);
}
