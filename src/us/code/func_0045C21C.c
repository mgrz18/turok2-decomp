/* Second-module copy of func_004232FC (first module; this one at link address 0x0042321C), calling this module's own functions at their link addresses. */
#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 func_00253640(s32, s32, s32);

s32 func_0045C21C(s32 arg0, Menu *arg1) {
    CPlayer *temp_a0;
    MenuItem *temp_a0_2;
    MenuItem *temp_v0;

    temp_a0 = arg1->player;
    if (func_00253640((s32) temp_a0, 0, temp_a0->m_Data.m_CheckPointWarpID) != 0) {
        temp_v0 = arg1->items;
        temp_v0[2].flags = temp_v0[2].flags | 0x01000000;
    } else {
        temp_a0_2 = arg1->items;
        temp_a0_2[2].flags = temp_a0_2[2].flags & 0xFEFFFFFF;
    }
    return 0;
}
