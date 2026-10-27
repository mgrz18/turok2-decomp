/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217928_de, with Turok 2's layouts. */

#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct MenuSlot {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ u8 pad4[8];
    /* 0x0C */ f32 angle;
    /* 0x10 */ f32 lift;
    /* 0x14 */ f32 scale;
} MenuSlot;

typedef struct WeaponMenu {
    /* 0x000 */ s32 active;
    /* 0x004 */ u8 pad4[4];
    /* 0x008 */ f32 open;
    /* 0x00C */ u8 padC[0x1C - 0xC];
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
} WeaponMenu;

typedef struct MenuItem {
    /* 0x00 */ s32 slot;
    /* 0x04 */ Vec3f position;
    /* 0x10 */ s32 icon;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ f32 size;
} MenuItem;

typedef struct WeaponInfo {
    /* 0x0 */ u8 pad0[6];
    /* 0x6 */ u16 icon;
} WeaponInfo;

extern f32 D_800AE2D8;              /* slot scale */
extern WeaponInfo *D_800AF618[];

f32 func_00211774(f32 value, f32 target, f32 rate);
void func_0020F79C(f32 *m);
void func_00210B38(f32 *m, f32 angle);
void func_0020F85C(f32 *m, Vec3f *in, Vec3f *out);
void func_0024AF68(WeaponMenu *menu, MenuItem *items, s32 count, s32 arg3);

/* Lays out the eight slots of the weapon wheel, dropping in as it opens,
 * and draws them. */
void func_00254D5C(WeaponMenu *menu, s32 arg1, s32 arg2) {
    f32 m[16];
    Vec3f position;
    MenuItem items[36];
    Vec3f offset;
    MenuItem *item;
    MenuSlot *slot;
    s32 count;
    s32 i;
    f32 target;

    if (menu->active == 0) {
        return;
    }
    item = items;
    count = 0;
    offset.x = 0.0f;
    offset.y = -0.6f - (0.4f - menu->open * 0.4f);
    offset.z = 0.0f;
    for (i = 0; i < 8; i++) {
        slot = &menu->slots[i];
        if (slot->weapon == -1) {
            continue;
        }
        if (i == menu->cursor) {
            target = D_800AE2D8 * 1.3f;
        } else {
            target = D_800AE2D8;
        }
        count++;
        slot->scale = func_00211774(slot->scale, target, D_800AE2D8 * 0.1f);
        func_0020F79C(m);
        func_00210B38(m, slot->angle);
        func_0020F85C(m, &offset, &position);
        item->slot = i;
        item->position = position;
        item->icon = D_800AF618[slot->weapon]->icon;
        item->unk14 = 1.0f;
        item->size = 1.0f;
        item++;
    }
    func_0024AF68(menu, items, count, arg2);
}
