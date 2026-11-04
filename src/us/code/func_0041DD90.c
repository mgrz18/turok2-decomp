#include "common.h"
#include "structs/menu.h"

/* Checks the controller pak in the given slot before a save/load menu:
 * returns the pak status, or -1 after opening the "pak full" dialog; opens the
 * matching warning dialog (templates D_0043xxxx) when the pak is missing,
 * damaged or unformatted. */

extern u8 D_80119E2C[];     /* D_80119870 + 0x5BC: the menu system */
extern u8 D_0043BC00[];
extern u8 D_0043BC24[];
extern u8 D_0043BC48[];
extern u8 D_0043BC6C[];
extern u8 D_0043BCD8[];
extern u8 D_0043C05C[];
extern s32 D_80130984;
extern s32 D_80130990;
extern s32 D_801309BC;

extern void func_00426480(s32 slot);
extern s32 func_0042655C(s32 slot);
extern void *func_0041DC5C(Menu *c);
extern s32 func_002858CC(s32 slot);
extern s32 func_00426594(s32 slot);
extern s32 func_00426BD8(s32 slot);
extern void func_00416644(void *menus, void *tmpl, s32 a, s32 b, void *parent);

s32 func_0041DD90(Menu *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    func_00426480(slot);
    status = func_0042655C(slot);
    parent = func_0041DC5C(c);
    if (parent != NULL) {
        full = func_002858CC(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                func_00416644(D_80119E2C, D_0043BCD8, c->player, c->controller, parent);
                return -1;
            }
        } else if (func_00426594(slot) != 0) {
            D_801309BC = 1;
            if (D_80130990 != 0) {
                tmpl = D_0043BC24;
            } else if (D_80130984 != 0) {
                tmpl = D_0043BC48;
            } else {
                tmpl = D_0043BC6C;
            }
            func_00416644(D_80119E2C, tmpl, c->player, c->controller, parent);
        } else if (quiet == 0) {
            D_801309BC = 1;
            if (func_00426BD8(slot) != 0) {
                func_00416644(D_80119E2C, D_0043C05C, c->player, c->controller, parent);
            } else {
                func_00416644(D_80119E2C, D_0043BC00, c->player, c->controller, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}
