/* WRONG CUT: real function 0x0041531C, size 0x3E0 (through 0x004156FC): splat cut it into
 * func_0041531C, func_004154C4, func_00415500, func_00415508, func_004155AC,
 * func_0041560C and func_004156D8 at branch targets. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00441384.c, func_00441384, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ u32 buttons;
} MenuController;

typedef struct MenuItemDef {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ void (*update)(void *item, void *menu, void *arg);
    /* 0x1C */ s32 (*select)(void *item, void *menu, void *arg);
} MenuItemDef;

typedef struct {
    /* 0x00 */ s32 index;
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u8 padC[0xC];
    /* 0x18 */ MenuItemDef *def;
    /* 0x1C */ s32 value;
    /* 0x20 */ u8 pad20[8];
} MenuItem; /* 0x28 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 (*poll)(void);
} MenuTemplate;

typedef struct {
    /* 0x00 */ s16 cursor;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ MenuItem *items;
    /* 0x10 */ s32 count;
    /* 0x14 */ MenuTemplate *tmpl;
    /* 0x18 */ u8 pad18[8];
    /* 0x20 */ MenuController *ctl;
    /* 0x24 */ u8 pad24[4];
    /* 0x28 */ s16 state;
    /* 0x2A */ u8 pad2A[2];
    /* 0x2C */ f32 time;
    /* 0x30 */ f32 fade;
    /* 0x34 */ f32 open;
} Menu;

extern f32 D_800B6D28; /* frame delta */
extern s32 func_002855E8(MenuController *ctl);
extern s32 func_00285628(MenuController *ctl);
extern s32 func_00285670(MenuController *ctl);
extern s32 func_002856B8(MenuController *ctl);
extern s32 func_00285700(MenuController *ctl);
extern void func_004151B4(Menu *menu, u8 dir);
extern void func_00416C28(Menu *menu);

s32 func_0041531C(Menu *menu, void *arg) {
    MenuItem *item;
    s32 quit;
    s32 i;
    s32 moveV;
    u32 horizontal;

    quit = 0;
    if (menu->tmpl->poll != NULL) {
        quit = menu->tmpl->poll();
    }
    i = menu->count;
    item = menu->items;
    i--;
    if (i != -1) {
        do {
            if (item->def->update != NULL) {
                item->def->update(item, menu, arg);
            }
            i--;
            item++;
        } while (i != -1);
    }
    item = (MenuItem *) ((u8 *) menu->items + menu->cursor * 0x28);
    menu->time = menu->time + D_800B6D28;
    horizontal = item->flags & 0x400000;
    func_00416C28(menu);
    switch (menu->state) {
    case 0:
        menu->fade = menu->fade + D_800B6D28;
        if (1.0f <= menu->fade) {
            menu->fade = 1.0f;
            menu->state = 1;
        }
        break;
    case 1:
        menu->open = menu->open + D_800B6D28 * 0.4f;
        if (1.0f <= menu->open) {
            menu->open = 1.0f;
            menu->state = 2;
        }
        break;
    case 3:
        menu->open = menu->open - D_800B6D28 * 0.4f;
        if (menu->open <= 0.0f) {
            menu->open = 0.0f;
            menu->state = 4;
        }
        break;
    case 4:
        menu->fade = menu->fade - D_800B6D28;
        if (menu->fade <= 0.0f) {
            menu->fade = 0.0f;
            return 1;
        }
        break;
    }
    if (menu->state < 3) if (menu->state >= 0) {
        if (quit != 0) {
            menu->state = 4;
        }
        if (!(item->flags & 8)) {
            u32 buttons;

            moveV = 1;
            if (item->type == 2) {
                buttons = menu->ctl->buttons;
                if (buttons & 0x20202) {
                    moveV = 0;
                    item->value -= 2;
                    if (item->value < 0) {
                        item->value = 0;
                    }
                } else if (buttons & 0x40101) {
                    item->value += 2;
                    moveV = 0;
                    if (item->value >= 0x100) {
                        item->value = 0xFF;
                    }
                }
            }
            if (horizontal == 0) {
                if (func_00285628(menu->ctl) != 0) {
                    func_004151B4(menu, 0x10);
                }
                if (func_00285670(menu->ctl) != 0) {
                    func_004151B4(menu, 0x11);
                }
            } else if (menu->ctl->buttons & 0x60303) {
                moveV = 0;
            }
            if (moveV != 0) {
                if (func_002856B8(menu->ctl) != 0) {
                    func_004151B4(menu, 0x12);
                }
                if (func_00285700(menu->ctl) != 0) {
                    func_004151B4(menu, 0x13);
                }
            }
            if (menu->state == 2 && (func_002855E8(menu->ctl) != 0 || (func_00285628(menu->ctl) != 0 && horizontal != 0)
                                     || (func_00285670(menu->ctl) != 0 && horizontal != 0))) {
                if (item->def->select == NULL || item->def->select(item, menu, arg) != 0) {
                    menu->state = 3;
                }
            }
        }
    }
    return 0;
}
