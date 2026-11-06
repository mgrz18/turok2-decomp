/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00409170.c, func_00409170.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 D_801309B0;
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130988;
extern s32 D_801309C4;
extern s32 D_8011B114;
extern u8 D_80119E2C[];
extern u8 D_0043BCD0[];
extern u8 D_0043BD3C[];
extern u8 D_0043BCF4[];
extern u8 D_0043BF58[];
s32 func_0041DCB0(Menu *o, s32 pad, s32 z);
void func_004263A0(s32 ch);
s32 func_00425F84(s32 pad, s32 idx);
s32 func_00426734(s32 pad);
s32 func_0041EFE8(s32 a, Menu *o, s32 c);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, void *d, void *e);
s32 PakConfirmMenu_SelectYes_2(s32 a, Menu *own, s32 c) {
    s32 pad;
    switch (D_801309B0) {
    case 4:
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->controller->port;
        }
        if (func_0041DCB0(own, pad, 1) != 0) {
            D_801309BC = 1;
            MenuPool_OpenMenu(D_80119E2C, own->context, own->player, own->controller, 0);
        } else {
            func_004263A0(pad);
            if (func_00425F84(pad, D_80130988) == 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BCD0, own->player, own->controller, own->context);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BD3C, own->player, own->controller, own->context);
            }
        }
        break;
    case 3:
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->controller->port;
        }
        if (func_0041DCB0(own, pad, 1) != 0) {
            D_801309BC = 1;
            MenuPool_OpenMenu(D_80119E2C, own->context, own->player, own->controller, 0);
        } else {
            func_004263A0(pad);
            if (func_00425F84(pad, D_801309C4) == 0) {
                func_0041EFE8(a, own, c);
                return 1;
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BD3C, own->player, own->controller, own->context);
            }
        }
        break;
    case 0:
        if (D_80130990 != 0) {
            pad = D_800C2028;
        } else {
            pad = own->controller->port;
        }
        if (func_0041DCB0(own, pad, 1) != 0) {
            D_801309BC = 1;
            MenuPool_OpenMenu(D_80119E2C, own->context, own->player, own->controller, 0);
        } else {
            func_004263A0(pad);
            if (func_00426734(pad) == 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BCF4, own->player, own->controller, own->context);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BF58, own->player, own->controller, own->context);
            }
        }
        break;
    case 1:
    case 2:
        if (D_80130990 != 0) {
            D_8011B114 = 1;
        }
        MenuPool_OpenMenu(D_80119E2C, own->context, own->player, own->controller, 0);
        break;
    }
    return 1;
}
