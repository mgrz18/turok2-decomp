#include "common.h"

typedef struct WeaponMenu WeaponMenu;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 upgrade;         /* the weapon that replaces this one */
    /* 0x0E */ u8 padE[6];
    /* 0x14 */ u32 flags;           /* 1: in the wheel, 2: in the wheel underwater */
} WeaponInfo;

typedef struct Player {
    /* 0x000 */ u8 pad0[0x3C8];
    /* 0x3C8 */ u32 aiFlags;
    /* 0x3CC */ u8 pad3CC[0x972 - 0x3CC];
    /* 0x972 */ s8 owned[0x23];
} Player;

extern WeaponInfo *D_800AF618[];
extern s32 D_80110058;
extern s32 D_8011005C;

void func_0024BF1C(WeaponMenu *menu, s32 rows, s32 *owned, s32 perRow, Player *player);

/* Opens the weapon wheel: picks the weapons to show (owned, allowed here,
 * not superseded by an owned upgrade) and how many times round to repeat
 * them so that the slots are at most 20 degrees apart. */
void func_0024C47C(WeaponMenu *menu, Player *player) {
    s32 shown[0x23];
    s32 count = 0;
    s32 range = D_8011005C - D_80110058 + 1;
    s32 rows;
    s32 slots;
    s32 i;

    for (i = 0; i < 0x23; i++) {
        WeaponInfo *info = D_800AF618[i];
        s32 show;

        if (!player->owned[i]) {
            show = 0;
        } else if (!((player->aiFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            show = 0;
        } else {
            s16 upgrade = info->upgrade;

            show = 1;
            if (player->owned[upgrade]) {
                show = !(i < upgrade);
            }
        }
        shown[i] = show;
        if (show) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    slots = rows * count;
    while (360 / slots > 20) {
        slots += count;
        rows++;
    }
    func_0024BF1C(menu, rows, shown, count, player);
}
