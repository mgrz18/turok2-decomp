/* WRONG CUT: real function 0x0041E1D8, size 0x19C (through 0x0041E374): splat cut it in two
 * (func_0041E1D8, func_0041E294) at a jal's delay slot. */
#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 kind;
} MenuCtl;

typedef struct {
    /* 0x000 */ u8 pad0[0x328];
    /* 0x328 */ u32 flags;
} MenuScreen;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *pools;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ MenuScreen *screen;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ Player *player;
    /* 0x20 */ MenuCtl *ctl;
} Menu;

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_801309A0;
extern s32 D_801309B4;
extern s32 D_801309BC;
extern s32 D_800C2020;
extern s32 D_800C2024;
extern u8 D_80119E2C[];   /* menu pool */
extern u8 D_0043882C[];
extern u8 D_0043B6A8[];
extern u8 D_0043BC00[];
extern u8 D_0043BC6C[];
extern u8 D_0043C05C[];

extern void func_00416644(void *pool, void *tmpl, Player *player, MenuCtl *ctl, void *back);
extern void func_00426480(s32 kind);
extern s32 func_0042655C(s32 kind);
extern s32 func_00426594(s32 kind);
extern s32 func_00426BD8(s32 kind);
extern void func_0041DA28(Menu *m);

void func_0041E1D8(Menu *m) {
    s32 kind;

    D_80130984 = 0;
    D_801309A0 = 0;
    D_80130994 = 1;
    D_801309B4 = 1;
    D_800C2024 = 1;
    D_801309BC = 0;
    D_80130990 = 0;
    m->screen->flags &= ~0x1000000;
    kind = m->ctl->kind;
    if (D_800C2020) {
        D_801309BC = 1;
        func_00416644(m->player->pools + 0x53C, D_0043882C, m->player, m->ctl, NULL);
        return;
    }
    func_00426480(kind);
    if (!func_0042655C(kind)) {
        func_0041DA28(m);
        return;
    }
    D_801309BC = 1;
    if (func_00426594(kind)) {
        func_00416644(D_80119E2C, D_0043BC6C, m->player, m->ctl, D_0043882C);
    } else if (func_00426BD8(kind)) {
        func_00416644(D_80119E2C, D_0043C05C, m->player, m->ctl, D_0043B6A8);
    } else {
        func_00416644(D_80119E2C, D_0043BC00, m->player, m->ctl, D_0043882C);
    }
}
