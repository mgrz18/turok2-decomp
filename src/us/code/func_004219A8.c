#include "common.h"
#include "structs/menu.h"

extern s32 D_800C2028;
extern s32 D_80130990;

s32 PakMenu_GetPlayerAndPort(Menu *arg0, s32 *arg1) {
    s32 var_a0;
    s32 var_v0;

    if (D_80130990 != 0) {
        var_v0 = D_800C2028;
        var_a0 = 0;
    } else {
        var_a0 = arg0->player;
        var_v0 = arg0->controller->port;
    }
    *arg1 = var_v0;
    return var_a0;
}
