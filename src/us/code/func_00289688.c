#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"

s32 func_002759C4(s32);                     /* extern */
s32 func_002888BC(CEngineApp *, s32, s32);       /* extern */
s32 func_00416644(void *, s32 *, s32, ControllerProfile *, s32); /* extern */
s32 func_0042AC0C(s32 *, s32, s32); /* extern */
s32 func_0042F514();                            /* extern */
extern s32 D_004392B0;
extern s32 D_800F7078;

void func_00289688(CEngineApp *app) {
    func_0042F514();
    func_002888BC(app, 0, 0);
    func_002759C4(0x10);
    func_0042AC0C(&D_800F7078, 0, 0);
    func_00416644(app->m_Game.m_MenuPool, &D_004392B0, 0, &D_800F5C78, 0);
}
