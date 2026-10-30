/* NON-MATCHING: 1 of 77 words differ: GCC cross-jumps the two stores of `kind`; the ROM keeps
 * one in the delay slot of the jump. */
/* WRONG CUT: real function 0x0041DC5C, size 0x134 (through 0x0041DD90): splat cut it in two
 * (func_0041DC5C, func_0041DCB0) at a load in a delay slot. */
#include "common.h"

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s8 kind;
} MenuCtl;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ MenuCtl *ctl;
} MenuCaller;

extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_80130994;
extern s32 D_80130998;
extern s32 D_801309B4;
extern s32 D_800C2028;
extern u8 D_0043882C[];
extern u8 D_00439190[];
extern u8 D_0043AC80[];
extern u8 D_0043ACA4[];
extern u8 D_0043ACC8[];
extern u8 D_0043ACEC[];
extern u8 D_0043B738[];
extern u8 D_0043BEF4[];

static __inline__ void *TmplForKind(s32 kind) {
    void *t;

    switch (kind) {
    case 0:
    default:
        t = D_0043AC80;
        break;
    case 1:
        t = D_0043ACA4;
        break;
    case 2:
        t = D_0043ACC8;
        break;
    case 3:
        t = D_0043ACEC;
        break;
    }
    return t;
}

/* Picks the menu template to open for the current game mode. */
void *func_0041DC5C(MenuCaller *c) {
    void *tmpl = NULL;
    volatile s32 kind;

    if (D_80130984) {
        if (D_801309B4) {
            if (D_80130990) {
                kind = D_800C2028;
            } else {
                kind = c->ctl->kind;
            }
            tmpl = TmplForKind(kind);
        } else {
            tmpl = D_0043BEF4;
        }
    } else if (D_80130994) {
        if (D_801309B4) {
            tmpl = D_0043882C;
        } else if (D_80130998) {
            tmpl = D_0043BEF4;
        } else {
            tmpl = D_00439190;
        }
    } else if (D_80130990) {
        tmpl = D_0043B738;
    }
    return tmpl;
}
