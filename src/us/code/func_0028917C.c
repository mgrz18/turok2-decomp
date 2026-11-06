#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"
#include "globals/engine.h"

s32 Controller_UnwatchAllPaks();                            /* extern */
s32 CEngineApp__SetupFadeTo(CEngineApp *, s32);                /* extern */
s32 MenuPool_OpenMenu(void *, s32 *, s32, ControllerProfile *, s32); /* extern */
extern s32 gPakSaveGameAnyPakMenuDesc;
extern f32 gPakCheckTimer;
extern s32 gPakCheckTimedOut;
extern s32 gPakCheckDone;

void EngineMode00_PakCheck_Retrace(CEngineApp *app) {
    if (gPakCheckStarted != 0) {
        if (gPakCheckTimer > 0.0f) {
            gPakCheckTimer -= gFrameIncrement;
        }
        if ((gPakCheckTimer <= 0.0f) && (gPakCheckTimedOut == 0)) {
            gPakCheckTimer = 0.0f;
            gPakCheckTimedOut = 1;
            MenuPool_OpenMenu(app->m_Game.m_MenuPool, &gPakSaveGameAnyPakMenuDesc, 0, &gMergedController, 0);
        }
        if (gPakCheckDone != 0) {
            CEngineApp__SetupFadeTo(app, 1);
            Controller_UnwatchAllPaks();
        }
    }
}
