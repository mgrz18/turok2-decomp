#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ u32 flags;
} WeaponDef;

typedef struct {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ u8 pad4[0x14];
} WeaponEntry; /* size 0x18 */

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 count;
    /* 0x1C */ WeaponEntry entries[1];
} WeaponEntryList;

extern WeaponDef *D_800AF618[];

/* Whether the player holds a usable weapon that is not in the list. */
s32 func_00254F88(WeaponEntryList *list, CPlayer *player) {
    WeaponDef *def;
    s32 usable;
    s32 found;
    s32 w;
    s32 i;
    s32 *aiFlags = &player->m_WeaponActor.m_AI.m_dwFlags;

    for (w = 0; w < 35; w++) {
        def = D_800AF618[w];
        usable = 0;
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        if (*aiFlags & 0x80) {
            if (def->flags & 2) {
                usable = 1;
            }
        } else if (def->flags & 1) {
            usable = 1;
        }
        if (usable) {
            found = 0;
            for (i = 0; i < list->count; i++) {
                if (list->entries[i].weapon == w) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 1;
            }
        }
    }
    return 0;
}
