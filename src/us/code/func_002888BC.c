#include "common.h"

s32 CPlayerPool__NewGame(s32, s32, s32);       /* extern */
s32 CCameraPool__NewGame(s32);                         /* extern */

void func_002888BC(s32 arg0, s32 arg1, s32 arg2) {
    CCameraPool__NewGame(arg0 + 0x22C00);
    CPlayerPool__NewGame(arg0 + 0x22BC0, arg1, arg2);
}
