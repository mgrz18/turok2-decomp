#include "common.h"
#include "globals/scene.h"

s32 Cinema_RequestById(s32);                         /* extern */
s32 CScene__LoadBitSections(s32 *);                   /* extern */
s32 CScene__LoadBitSections2(s32 *);                   /* extern */
s32 CScene__RequestWarp(s32 *, s32, s32);     /* extern */
s32 CPlayerPool__NewGame(s32, s32, s32);       /* extern */
s32 CCameraPool__NewGame(s32, s32);                /* extern */

void CEngineApp__StartWarp(s32 arg0, s32 arg1) {
    CCameraPool__NewGame(arg0 + 0x22C00, 1);
    CPlayerPool__NewGame(arg0 + 0x22BC0, 1, 0);
    CScene__RequestWarp(&gScene, ~Cinema_RequestById(arg1), 0);
    CScene__LoadBitSections(&gScene);
    CScene__LoadBitSections2(&gScene);
}
