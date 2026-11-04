#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 D_801198A0;
extern char D_800F6CB0[];
extern void func_00430330(char *);

s32 func_0041A3D4(s32 menu, Menu *c) {
    if (D_801198A0 == 1) {
        func_00430330(D_800F6CB0);
    } else {
        c->player->m_bActive = 0;
    }
    return 1;
}
