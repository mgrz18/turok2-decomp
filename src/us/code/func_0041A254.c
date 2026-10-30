#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ u32 flags80;
    /* 0x84 */ u8 pad84[0x24];
    /* 0xA8 */ u32 flagsA8;
} A254Screen;

typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ A254Screen *screen;
} A254Menu;

typedef struct {
    /* 0x000 */ u8 pad0[0xCC];
    /* 0x0CC */ s32 unkCC;
    /* 0x0D0 */ u8 padD0[0x224 - 0xD0];
} Controller;

extern Controller D_800F5EC0[4]; /* controllers */
extern s32 D_80130910[4];
extern u8 D_800F5A50[];
extern u8 D_8011AAF1;   /* D_8011AAD8 + 0x19 */
extern void func_00285304(void);
extern void func_0026EDA8(void *);
extern s32 func_00275F7C(void);
extern void func_00275DC0(s32);

void func_0041A254(A254Menu *menu) {
    s32 i;

    menu->state = 2;
    for (i = 0; i < 4; i++) {
        D_80130910[i] = D_800F5EC0[i].unkCC;
    }
    func_0026EDA8(D_800F5A50);
    func_00285304();
    if (D_8011AAF1) {
        menu->screen->flags80 &= ~0x800000;
        menu->screen->flagsA8 &= ~0x800000;
    } else {
        menu->screen->flags80 |= 0x800000;
        menu->screen->flagsA8 |= 0x800000;
    }
    func_00275DC0(func_00275F7C());
}
