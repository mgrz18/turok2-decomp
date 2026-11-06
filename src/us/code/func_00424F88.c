#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

extern s32 D_800B436C;
extern s32 D_800B437C;

s32 MissionMenu_UpdateTitle(MenuItem *arg0) {
    if (sMissionFailed != 0) {
        arg0->table = &D_800B437C;
    } else {
        arg0->table = &D_800B436C;
    }
    return 1;
}
