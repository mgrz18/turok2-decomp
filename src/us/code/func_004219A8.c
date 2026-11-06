#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern s32 gPakMenuAnyPak;

s32 PakMenu_GetPlayerAndPort(Menu *arg0, s32 *arg1) {
    s32 var_a0;
    s32 var_v0;

    if (gPakMenuAnyPak != 0) {
        var_v0 = gPakMenuPort;
        var_a0 = 0;
    } else {
        var_a0 = arg0->player;
        var_v0 = arg0->controller->port;
    }
    *arg1 = var_v0;
    return var_a0;
}
