#include "common.h"
#include "structs/menu.h"
#include "rw/abu_protos.h"
#include "structs/player_options.h"

extern s32 D_8011ACE4;

/* A CPlayer view: rw/abu_protos.h (its Mtx) clashes with structs/player.h. */
typedef struct {
    /* 0x000 */ u8 pad0[0x514];
    /* 0x514 */ s32 unk514;
    /* 0x518 */ CPlayerOptions *options;
    /* 0x51C */ s32 unk51C;
    /* 0x520 */ u8 pad520[0x92C - 0x520];
    /* 0x92C */ s32 unk92C;
    /* 0x930 */ u8 pad930[0xA50 - 0x930];
    /* 0xA50 */ s32 unkA50;
    /* 0xA54 */ u8 padA54[0xBF4 - 0xA54];
    /* 0xBF4 */ s32 unkBF4;
    /* 0xBF8 */ s32 unkBF8;
} MenuData;

/* Byte-offset field access: member references here would let the
 * scheduler move the D_8011ACE4 load and change the register choice. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void StatsMenu_InitContinue(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;
    MenuData *temp_v1;
    MenuItem *temp_v1_2;
    MenuItem *temp_v1_3;
    MenuItem *temp_v1_4;
    MenuItem *temp_v1_5;
    MenuItem *temp_v1_6;

    temp_v1 = arg0->player;
    if (temp_v1 != NULL) {
        temp_v1->unkBF8 = 1;
    }
    temp_v1_2 = arg0->items;
    FIELD_AT(&temp_v1_2[11], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_2[11], s32, MenuItem, flags) | 0x01800000);
    temp_v1_3 = arg0->items;
    FIELD_AT(&temp_v1_3[12], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_3[12], s32, MenuItem, flags) | 0x01800000);
    temp_v1_4 = arg0->items;
    FIELD_AT(&temp_v1_4[13], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_4[13], s32, MenuItem, flags) | 0x01800000);
    temp_v1_5 = arg0->items;
    FIELD_AT(&temp_v1_5[14], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_5[14], s32, MenuItem, flags) & 0xFE7FFFFF);
    temp_v1_6 = arg0->items;
    FIELD_AT(&temp_v1_6[15], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_6[15], s32, MenuItem, flags) & 0xFE7FFFFF);
    if (D_8011ACE4 != 0) {
        temp_v0 = arg0->items;
        FIELD_AT(&temp_v0[7], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v0[7], s32, MenuItem, flags) | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    FIELD_AT(&temp_a0[7], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_a0[7], s32, MenuItem, flags) & 0xFEFFFFFF);
}
