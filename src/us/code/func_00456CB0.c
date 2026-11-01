/* Second-module copy of func_0041DD90 (build/manual/W/func_0041DD90.c), with its first-module addresses 0xE0 lower. */
#include "common.h"

/* Checks the controller pak in the given slot before a save/load menu:
 * returns the pak status, or -1 after opening the "pak full" dialog; opens the
 * matching warning dialog (templates D_0043xxxx) when the pak is missing,
 * damaged or unformatted. */

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} MenuCaller;

extern u8 D_80119E2C[];     /* D_80119870 + 0x5BC: the menu system */
extern u8 D_0043BB20[];
extern u8 D_0043BB44[];
extern u8 D_0043BB68[];
extern u8 D_0043BB8C[];
extern u8 D_0043BBF8[];
extern u8 D_0043BF7C[];
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_801309BC;

extern void func_004263A0(s32 slot);
extern s32 func_0042647C(s32 slot);
extern void *func_0041DB7C(MenuCaller *c);
extern s32 func_002858CC(s32 slot);
extern s32 func_004264B4(s32 slot);
extern s32 func_00426AF8(s32 slot);
extern void func_00416644(void *menus, void *tmpl, s32 a, s32 b, void *parent);

s32 func_00456CB0(MenuCaller *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    func_004263A0(slot);
    status = func_0042647C(slot);
    parent = func_0041DB7C(c);
    if (parent != NULL) {
        full = func_002858CC(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                func_00416644(D_80119E2C, D_0043BBF8, c->unk1C, c->unk20, parent);
                return -1;
            }
        } else if (func_004264B4(slot) != 0) {
            D_801309BC = 1;
            if (D_80130990 != 0) {
                tmpl = D_0043BB44;
            } else if (D_80130984 != 0) {
                tmpl = D_0043BB68;
            } else {
                tmpl = D_0043BB8C;
            }
            func_00416644(D_80119E2C, tmpl, c->unk1C, c->unk20, parent);
        } else if (quiet == 0) {
            D_801309BC = 1;
            if (func_00426AF8(slot) != 0) {
                func_00416644(D_80119E2C, D_0043BF7C, c->unk1C, c->unk20, parent);
            } else {
                func_00416644(D_80119E2C, D_0043BB20, c->unk1C, c->unk20, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}
