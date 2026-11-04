/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408E1C.c, func_00408E1C, with Turok 2's layouts. */
/* WRONG CUT: the real function is 0x0042085C-0x00420C30 (size 0x3D4, 245 words): splat split it into func_0042085C (0x1C),
 * func_00420878 (0x68) and func_004208E0 (0x350). All 245 words match (checked over the real extent).
 * Twin of src/us/code/func_0045977C.c (first-module callees and data names, +0xE0). */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

/* Controller pak menu: closes the current message box and shows the next one for the menu state. */

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
extern u8 D_0043BEF4[];
extern u8 D_0043BED0[];
extern u8 D_0043B6F0[];
extern u8 D_00439190[];
extern u8 D_0043B714[];
extern u8 D_0043B6CC[];
void func_00285AE0(MenuController *a);
void func_00285BD0(MenuController *a, s32 b);
void func_00285A80(s32 a);
s32 func_002858CC(s32 a);
void func_00426480(s32 a);
s32 func_0042655C(s32 a);
void func_0042C700(u8 *a, s32 b, s32 c);
void func_004303AC(u8 *a);
void func_00416644(u8 *a, void *b, CPlayer *c, MenuController *d, s32 e);

s32 func_0042085C(s32 a0, Menu *o) {
    s32 s0, s1, v;
    u8 *p;

    if (D_801309C0 == 8) {
        return 0;
    }
    if (o->context == D_0043BEF4 || o->context == D_0043BED0) {
        D_80130940 = 0;
    }
    if (D_801309C0 == 9 || D_801309C0 == 5) {
        func_00285AE0(o->controller);
        func_00285BD0(o->controller, 1);
    }
    if (D_801309C0 == 9) {
        if (D_80130994 != 0 && D_801309B4 == 0 && D_8013098C == 0) {
            if (D_80130990 != 0) {
                s1 = D_800C2028;
            } else {
                s1 = o->controller->port;
            }
            func_00285A80(s1);
            func_0042C700(D_800F7078, o->player->m_Data.m_CheckPointWarpID, 2);
            return 1;
        }
        func_004303AC(D_800F6CB0);
        return 1;
    }
    if (D_80130990 != 0) {
        s1 = D_800C2028;
    } else {
        s1 = o->controller->port;
    }
    if (D_801309C0 == 5) {
        func_00285A80(s1);
        func_0042C700(D_800F7078, D_80110054, 2);
    }
    if (D_80130990 != 0) {
        if (D_801309C0 == 6) {
            D_8011F148 = 1;
        }
    }
    if (D_801309C0 == 4 && D_80130994 != 0 && D_801309B4 == 0) {
        func_00426480(s1);
        s0 = func_0042655C(s1);
        v = func_002858CC(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            func_00416644(D_80119E2C, D_0043B6F0, o->player, o->controller, 0);
        } else {
            func_00416644(D_80119E2C, D_00439190, o->player, o->controller, 0);
        }
    } else if (D_801309C0 == 3 && D_80130998 != 0) {
        func_00426480(s1);
        s0 = func_0042655C(s1);
        v = func_002858CC(s1);
        if ((s0 == 0 || v != 0) && s0 != -2 && o->context == (void *)1) {
            func_00416644(D_80119E2C, D_0043B714, o->player, o->controller, 0);
        } else {
            if (o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = D_80119E2C;
            }
            func_00416644(p, D_0043BEF4, o->player, o->controller, 0);
            D_80130940 = 0;
        }
    } else {
        if (o->context == (void *)1) {
            o->context = NULL;
        }
        if (o->context != NULL) {
            if (o->context != D_0043B6CC && o->player != NULL) {
                p = ((u8 *)o->player->m_pCamera) + 0x53C;
            } else {
                p = D_80119E2C;
            }
            func_00416644(p, o->context, o->player, o->controller, s1);
        }
    }
    return 1;
}
