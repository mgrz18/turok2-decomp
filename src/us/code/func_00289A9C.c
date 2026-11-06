#include "common.h"
#include "structs/game.h"

s32 Cinema_RequestById(s32);
s32 CScene__RequestWarp(s32, s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0x9F0];
    /* 0x9F0 */ s32 unk9F0;
} Unk002532A8;

void *CPlayerPool__GetFirstActive(CPlayerPool *);                     /* extern */
s32 CEngineApp__StartWarp(s32, s32);                /* extern */
extern s32 gFinalEnding;
extern s32 gEndingPrimagenDefeated;
extern s32 gScreenHeight;
extern s32 sCreditsLine;
extern s32 gScene;
extern s32 gCreditsDone;
extern f32 gCreditsScroll;

void EngineMode15_Ending_Init(s32 arg0) {
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
        CScene__RequestWarp((s32) &gScene, ~Cinema_RequestById(0x82), 0);
        return;
    }
    CEngineApp__StartWarp(arg0, 0x82);
}
