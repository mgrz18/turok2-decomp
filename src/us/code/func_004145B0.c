/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00440618.c, func_00440618, with Turok 2's layouts. */
#include "common.h"

/* 0x28-byte menu item, also copied by value to lay out its parts. */
typedef struct MenuItem {
    /* 0x00 */ s32 idx;
    /* 0x04 */ s16 kind;
    /* 0x06 */ u8 pad6[2];
    /* 0x08 */ u32 flags;
    /* 0x0C */ u16 x;
    /* 0x0E */ u16 y;
    /* 0x10 */ u8 b10, b11, b12, b13;
    /* 0x14 */ s32 data;
    /* 0x18 */ void *def;
    /* 0x1C */ s32 value; /* slider position, 0-256 */
    /* 0x20 */ void *p20;
    /* 0x24 */ void *owner;
} MenuItem;

/* Layout of an item (func_0041363C). */
typedef struct {
    /* 0x00 */ s32 f0;
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ f32 scaleX;
    /* 0x10 */ f32 scaleY;
    /* 0x14 */ s32 x0, x1, y0, y1;
    /* 0x24 */ s32 pad24;
} MenuInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 x;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
    /* 0x1C */ s32 y;
} MenuBox;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 left;
    /* 0x18 */ s32 right;
} MenuColumn;

extern void func_0041363C(MenuItem *item, MenuInfo *info);
extern void func_004143DC(MenuItem *item, MenuBox *box, MenuColumn *column, s32 window);
extern void func_00413F74(MenuItem *item, MenuBox *box, MenuColumn *column, s32 window, s32 data);

static __inline__ void fin(MenuItem *item, MenuBox *box, MenuColumn *column, s32 window) {
    func_00413F74(item, box, column, window, item->data);
}

/* Draws a slider: its label, its bar and its knob at the item's value. */
void func_004145B0(MenuItem *it, MenuBox *box, MenuColumn *column, s32 window) {
    MenuItem cp;
    MenuInfo o1;
    MenuInfo o2;
    MenuInfo o3;
    s32 mid;
    s32 t;

    mid = (column->left + column->right) / 2;
    cp = *it;
    cp.kind = 0;
    func_0041363C(&cp, &o1);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C(&cp, &o2);
    cp = *it;
    cp.kind = 1;
    cp.data = 0;
    func_0041363C(&cp, &o3);
    cp = *it;
    cp.data = 0;
    box->y = box->y + o1.height;
    if (it->flags & 1) {
        box->x = mid - ((o2.width + o3.width) / 2);
    }
    box->x = box->x + o3.width / 2;
    func_004143DC(&cp, box, column, window);
    cp = *it;
    cp.data = 0;
    t = it->value * o2.width;
    box->x = box->x + (t / 256 - o3.width / 2);
    box->y = box->y + (o2.height / 2 - o3.height / 2);
    func_004143DC(&cp, box, column, window);
    cp = *it;
    if (it->flags & 1) {
        box->x = mid - o1.width / 2;
    }
    fin(&cp, box, column, window);
}
