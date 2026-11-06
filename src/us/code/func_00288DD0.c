#include "common.h"

s32 Cinema_RequestById(s32);                         /* extern */
s32 CScene__LoadBitSections(s32 *);                   /* extern */
s32 CScene__LoadBitSections2(s32 *);                   /* extern */
s32 CScene__RequestWarp(s32 *, s32, s32);     /* extern */
s32 CPlayerPool__NewGame(s32, s32, s32);       /* extern */
s32 CCameraPool__NewGame(s32, s32);                /* extern */
extern s32 D_800F7078;

void CEngineApp__StartWarp(s32 arg0, s32 arg1) {
    CCameraPool__NewGame(arg0 + 0x22C00, 1);
    CPlayerPool__NewGame(arg0 + 0x22BC0, 1, 0);
    CScene__RequestWarp(&D_800F7078, ~Cinema_RequestById(arg1), 0);
    CScene__LoadBitSections(&D_800F7078);
    CScene__LoadBitSections2(&D_800F7078);
}
