/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004070F4.c, func_004070F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern u8 D_80119E2C[];
extern u8 D_004390B0[];
extern u8 D_0043BB8C[];
extern u8 D_0043BF7C[];
extern u8 D_0043B610[];
extern u8 D_0043BB20[];
void func_004263A0(s32 ch);
s32 func_0042647C(s32 ch);
s32 func_004264B4(s32 ch);
s32 func_00426AF8(s32 ch);
void func_0041D948(Menu *o);
void MenuPool_OpenMenu(u8 *a, void *b, void *c, void *d, void *e);
void PakMenu_InitLoadGame_2(Menu *own) {
    s32 pad;
    register s32 o1 asm("$8");
    register s32 o2 asm("$18");
    D_800C2024 = 2;
    D_80130984 = 0;
    D_801309A0 = 0;
    o1 = 1;
    D_80130994 = o1;
    D_801309B4 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    pad = own->controller->port;
    if (D_800C2020 != 0) {
        D_801309BC = o1;
        MenuPool_OpenMenu(D_80119E2C, D_004390B0, own->player, own->controller, 0);
    } else {
        own->items[20].flags &= 0xFEFFFFFF;
        func_004263A0(own->controller->port);
        if (func_0042647C(pad) == 0) {
            func_0041D948(own);
        } else {
            o2 = 1;
            D_801309BC = o2;
            if (func_004264B4(pad) != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BB8C, own->player, own->controller, (void *)o2);
            } else if (func_00426AF8(pad) != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BF7C, own->player, own->controller, D_0043B610);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BB20, own->player, own->controller, D_004390B0);
            }
        }
    }
}
