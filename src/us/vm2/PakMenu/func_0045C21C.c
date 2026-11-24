#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

s32 CPlayer__CanGetRefill(struct CPlayer *player, s32 kind, s32 arg); /* code/CPlayer's definition */

s32 SavePortalMenu_UpdateAmmo_2(s32 arg0, Menu *arg1) {
    CPlayer *temp_a0;
    MenuItem *temp_a0_2;
    MenuItem *temp_v0;

    temp_a0 = arg1->player;
    if (CPlayer__CanGetRefill((struct CPlayer *) temp_a0, 0, temp_a0->m_Data.m_CheckPointWarpID) != 0) {
        temp_v0 = arg1->items;
        temp_v0[2].flags |= 0x01000000;
    } else {
        temp_a0_2 = arg1->items;
        temp_a0_2[2].flags &= 0xFEFFFFFF;
    }
    return 0;
}
