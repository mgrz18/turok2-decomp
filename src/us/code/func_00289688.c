#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"

s32 Music_SetTrack(s32);                     /* extern */
s32 func_002888BC(CEngineApp *, s32, s32);       /* extern */
s32 MenuPool_OpenMenu(void *, s32 *, s32, ControllerProfile *, s32); /* extern */
s32 CScene__LoadLevel(s32 *, s32, s32); /* extern */
s32 Demo_End();                            /* extern */
extern s32 D_004392B0;
extern s32 gScene;

void EngineMode08_MainMenu_Init(CEngineApp *app) {
    Demo_End();
    func_002888BC(app, 0, 0);
    Music_SetTrack(0x10);
    CScene__LoadLevel(&gScene, 0, 0);
    MenuPool_OpenMenu(app->m_Game.m_MenuPool, &D_004392B0, 0, &D_800F5C78, 0);
}
