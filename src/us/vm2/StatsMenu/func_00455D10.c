#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"

/* The window's item records are 0x28 bytes apart, item i's flags word at
 * 0x120 + i * 0x28 of the block. */
#define ITEM_FLAGS(i) (0x120 + (i) * 0x28)

#include "rw/abu_protos.h"


void StatsMenu_InitContinue_2(Menu *arg0) {
    void *temp_a0;
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *temp_v1_6;

    temp_v1 = arg0->player;
    if (temp_v1 != NULL) {
        ((CPlayer *) temp_v1)->m_bEnteringDeathmatch = 1;
    }
    temp_v1_2 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_2 + ITEM_FLAGS(4)) = (s32) (*(s32 *) ((s8 *) temp_v1_2 + ITEM_FLAGS(4)) | 0x01800000);
    temp_v1_3 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_3 + ITEM_FLAGS(5)) = (s32) (*(s32 *) ((s8 *) temp_v1_3 + ITEM_FLAGS(5)) | 0x01800000);
    temp_v1_4 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_4 + ITEM_FLAGS(6)) = (s32) (*(s32 *) ((s8 *) temp_v1_4 + ITEM_FLAGS(6)) | 0x01800000);
    temp_v1_5 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_5 + ITEM_FLAGS(7)) = (s32) (*(s32 *) ((s8 *) temp_v1_5 + ITEM_FLAGS(7)) & 0xFE7FFFFF);
    temp_v1_6 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_6 + ITEM_FLAGS(8)) = (s32) (*(s32 *) ((s8 *) temp_v1_6 + ITEM_FLAGS(8)) & 0xFE7FFFFF);
    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = arg0->items;
        *(s32 *) ((s8 *) temp_v0 + ITEM_FLAGS(0)) = (s32) (*(s32 *) ((s8 *) temp_v0 + ITEM_FLAGS(0)) | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    *(s32 *) ((s8 *) temp_a0 + ITEM_FLAGS(0)) = (s32) (*(s32 *) ((s8 *) temp_a0 + ITEM_FLAGS(0)) & 0xFEFFFFFF);
}
