/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408E1C.c, func_00408E1C, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

/* Controller pak menu: closes the current message box and shows the next one for the menu state. */

typedef struct {
    u8 pad0[4];
    s8 pad; /* 0x04 */
} MenuPad;

typedef struct {
    u8 pad0[0x1C];
    CPlayer *player; /* 0x1C */
    MenuPad *pad;   /* 0x20 */
    void *msg;      /* 0x24 */
} MenuBox;

extern s32 D_801309C0; /* menu state */
extern s32 D_80130940;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_8013098C;
extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_80110054;
extern s32 D_8011F148;
extern s32 D_80130998;
extern u8 D_800F7078[];
extern u8 D_800F6CB0[];
extern u8 D_80119E2C[];
extern u8 D_0043BE14[];
extern u8 D_0043BDF0[];
extern u8 D_0043B610[];
extern u8 D_004390B0[];
extern u8 D_0043B634[];
extern u8 D_0043B5EC[];
void func_00285AE0(MenuPad *a);
void func_00285BD0(MenuPad *a, s32 b);
void func_00285A80(s32 a);
s32 func_002858CC(s32 a);
void func_004263A0(s32 a);
s32 func_0042647C(s32 a);
void func_0042C620(u8 *a, s32 b, s32 c);
void func_004302CC(u8 *a);
void func_00416644(u8 *a, void *b, CPlayer *c, MenuPad *d, s32 e);

s32 func_0045977C(s32 a0, MenuBox *o) {
    s32 s0, s1, v;
    u8 *p;

    if (D_801309C0 == 8) {
        return 0;
    }
    if (o->msg == D_0043BE14 || o->msg == D_0043BDF0) {
        D_80130940 = 0;
    }
    if (D_801309C0 == 9 || D_801309C0 == 5) {
        func_00285AE0(o->pad);
        func_00285BD0(o->pad, 1);
    }
    if (D_801309C0 == 9) {
        if (D_80130994 != 0 && D_801309B4 == 0 && D_8013098C == 0) {
            if (D_80130990 != 0) {
                s1 = D_800C2028;
            } else {
                s1 = o->pad->pad;
            }
            func_00285A80(s1);
            func_0042C620(D_800F7078, o->player->m_Data.m_CheckPointWarpID, 2);
            return 1;
        }
        func_004302CC(D_800F6CB0);
        return 1;
    }
    if (D_80130990 != 0) {
        s1 = D_800C2028;
    } else {
        s1 = o->pad->pad;
    }
    if (D_801309C0 == 5) {
        func_00285A80(s1);
        func_0042C620(D_800F7078, D_80110054, 2);
    }
    if (D_80130990 != 0) {
        if (D_801309C0 == 6) {
            D_8011F148 = 1;
        }
    }
    if (D_801309C0 == 4 && D_80130994 != 0 && D_801309B4 == 0) {
        func_004263A0(s1);
        s0 = func_0042647C(s1);
        v = func_002858CC(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->msg == (void *)1) {
            func_00416644(D_80119E2C, D_0043B610, o->player, o->pad, 0);
        } else {
            func_00416644(D_80119E2C, D_004390B0, o->player, o->pad, 0);
        }
    } else if (D_801309C0 == 3 && D_80130998 != 0) {
        func_004263A0(s1);
        s0 = func_0042647C(s1);
        v = func_002858CC(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->msg == (void *)1) {
            func_00416644(D_80119E2C, D_0043B634, o->player, o->pad, 0);
        } else {
            if (o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = D_80119E2C;
            }
            func_00416644(p, D_0043BE14, o->player, o->pad, 0);
            D_80130940 = 0;
        }
    } else {
        if (o->msg == (void *)1) {
            o->msg = NULL;
        }
        if (o->msg != NULL) {
            if (o->msg != D_0043B5EC && o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = D_80119E2C;
            }
            func_00416644(p, o->msg, o->player, o->pad, s1);
        }
    }
    return 1;
}
