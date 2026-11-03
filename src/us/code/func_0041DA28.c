#include "common.h"

/* Greys out the controller pak notes that cannot be picked: a used note is
 * selectable unless it is this game's save (D_800B45C0 when loading, mode 1,
 * D_800B45CC in mode 2) with Turok 2's game and company codes. */

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 flags;
    /* 0x0C */ u8 padC[0x28 - 0xC];
} MenuRow;

typedef struct {
    /* 0x00 */ u8 pad0[0x78];
    /* 0x78 */ MenuRow rows[16];
} MenuWindow;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 slot;
} MenuCtl;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ MenuWindow *win;
    /* 0x10 */ u8 pad10[0x20 - 0x10];
    /* 0x20 */ MenuCtl *ctl;
} Menu;

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_800C2024;
extern char D_800B45C0[];
extern char D_800B45CC[];
extern char D_800B45D4[];
extern char D_800B45D8[];

extern s32 func_00425E74(s32 slot, s32 note, s32 *used, char *name, char *ext, s32 *size, char *company, char *game);
extern s32 func_00426D8C(char *s);
extern s32 func_00426D54(char *a, char *b);

/* Game code / company code check on the controller-pak note (as in func_0041F32C). */
static __inline__ s32 k7_chk(char *gc, char *cc) {
    if (func_00426D8C(gc) == 4 && func_00426D8C(cc) == 2 &&
        func_00426D54(gc, D_800B45D8) == 0 && func_00426D54(cc, D_800B45D4) == 0) {
        return 1;
    }
    return 0;
}

static __inline__ void rowEnable(MenuRow *row) {
    row->flags &= ~0x01000000;
}

static __inline__ void rowDisable(MenuRow *row) {
    row->flags |= 0x01000000;
}

void func_0041DA28(Menu *menu) {
    char ext[8];
    char company[8];
    char game[8];
    char name[16];
    s32 used;
    s32 size;
    s32 slot;
    s32 i;
    s32 ok;
    s32 ours;
    s32 err;

    if (D_80130990) {
        slot = D_800C2028;
    } else {
        slot = menu->ctl->slot;
    }
    for (i = 0; i < 16; i++) {
        err = func_00425E74(slot, i, &used, name, ext, &size, company, game);
        ok = 1;
        if (err == 0 && used != 0) {
            switch (D_800C2024) {
            case 1:
                ours = k7_chk(game, company);
                if (func_00426D54(name, D_800B45C0) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 2:
                ours = k7_chk(game, company);
                if (func_00426D54(name, D_800B45CC) == 0 && ours) {
                    ok = 0;
                }
                break;
            case 0:
            default:
                ok = 0;
                break;
            }
        }
        if (ok) {
            rowEnable(&menu->win->rows[i]);
        } else {
            rowDisable(&menu->win->rows[i]);
        }
    }
}
