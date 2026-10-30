#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ char *text;
} MenuItemTmpl;

typedef struct {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u8 handicap;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} Player;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 kind;
} MenuCtl;

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ Player *player;
    /* 0x20 */ MenuCtl *ctl;
} MenuCaller;

extern char D_800B41B0[];
extern char D_800B41C0[];
extern char D_800B41D0[];
extern char D_800B41E0[];
extern char D_800B41F0[];
extern char D_800B4200[];
extern char D_800C0E7C[]; /* number format */
extern s32 func_004160F0(MenuItemTmpl *a);
extern void func_0029E3E0(char *dst, char *fmt, s32 value);

/* Label of the handicap item, with the handicap value printed into it. */
s32 func_00417F9C(MenuItemTmpl *a, MenuCaller *c) {
    s32 handicap = c->player->options->handicap;

    if (handicap == 0) {
        a->text = D_800B41F0;
    } else if (handicap == 10) {
        a->text = D_800B4200;
    } else {
        switch (c->ctl->kind) {
        case 0:
        default:
            a->text = D_800B41B0;
            break;
        case 1:
            a->text = D_800B41C0;
            break;
        case 2:
            a->text = D_800B41D0;
            break;
        case 3:
            a->text = D_800B41E0;
            break;
        }
        func_0029E3E0(a->text + (func_004160F0(a) - 2), D_800C0E7C, handicap);
    }
    return 0;
}
