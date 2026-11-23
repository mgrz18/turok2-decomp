#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"
#include "globals/pak.h"
#include "globals/engine.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 Controller_UnwatchAllPaks();                            /* extern */
s32 CEngineApp__SetupFadeTo(CEngineApp *, s32);                /* extern */

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
