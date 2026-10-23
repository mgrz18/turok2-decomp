#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ void *def;
    /* 0x08 */ s32 hasAmmo;
    /* 0x0C */ f32 position;
    /* 0x10 */ u8 pad10[0x8];
} WeaponEntry; /* size 0x18 */

typedef struct {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ f32 position;
    /* 0x010 */ u8 pad10[0x8];
    /* 0x018 */ s32 count;
    /* 0x01C */ WeaponEntry entries[(0x37C - 0x1C) / 0x18];
    /* 0x37C */ s32 selected;
} WeaponEntryList;

extern void *D_800AF618[];
extern s32 func_0024E700(CPlayer *player, s32 weapon);

/* s16 weapon id at 0xB14 (LibTEngine's CPlayer has m_RequestedWeapon at 0xB1C). */
#define PLAYER_B14(p) (*(s16 *)((u8 *)(p) + 0xB14))

/* Fills entry index with a weapon, selecting it if none is and it is the
 * player's weapon at 0xB14. */
void func_002550E8(WeaponEntryList *list, s32 weapon, s32 index, CPlayer *player) {
    void *def = D_800AF618[weapon];
    s32 hasAmmo = func_0024E700(player, weapon) != 0;

    list->entries[index].weapon = weapon;
    list->entries[index].hasAmmo = hasAmmo;
    list->entries[index].def = def;
    if (list->selected == -1 && weapon == PLAYER_B14(player)) {
        list->selected = index;
        list->position = list->entries[index].position;
    }
    list->count++;
}
