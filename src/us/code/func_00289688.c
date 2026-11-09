#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"
#include "globals/scene.h"
#include "globals/menu_desc.h"
#include "protos/scene.h"
#include "protos/menu.h"

s32 Music_SetTrack(s32);                     /* extern */
s32 func_002888BC(CEngineApp *, s32, s32);       /* extern */
s32 Demo_End();                            /* extern */

void EngineMode08_MainMenu_Init(CEngineApp *app) {
    Demo_End();
    func_002888BC(app, 0, 0);
    Music_SetTrack(0x10);
    CScene__LoadLevel(&gScene, 0, 0);
    MenuPool_OpenMenu(app->m_Game.m_MenuPool, &gTitleMenuDesc, 0, &gMergedController, 0);
}
