#include "common.h"

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 enabled;
    /* 0x0C */ u8 padC[0x18 - 0xC];
} Entry; /* size 0x18 */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 count;
    /* 0x1C */ Entry entries[1];
} EntryList;

extern s32 CPlayer__GetWeaponAmmo(void *player, s32 id);

/* Marks each entry enabled when CPlayer__GetWeaponAmmo accepts its id. */
void CBarrel__MarkUsable(EntryList *list, void *player) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (CPlayer__GetWeaponAmmo(player, list->entries[i].id)) {
            list->entries[i].enabled = 1;
        } else {
            list->entries[i].enabled = 0;
        }
    }
}
