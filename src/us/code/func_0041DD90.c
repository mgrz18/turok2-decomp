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

extern void Pak_ProbeSlot(s32 slot);
extern s32 Pak_GetStatus(s32 slot);
extern void *PakMenu_GetReturnMenu(Menu *c);
extern s32 Controller_TakePakChanged(s32 slot);
extern s32 Pak_IsRumblePak(s32 slot);
extern s32 Pak_NeedsRepair(s32 slot);
extern void MenuPool_OpenMenu(void *menus, void *tmpl, s32 a, s32 b, void *parent);

s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    Pak_ProbeSlot(slot);
    status = Pak_GetStatus(slot);
    parent = PakMenu_GetReturnMenu(c);
    if (parent != NULL) {
        full = Controller_TakePakChanged(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BCD8, c->player, c->controller, parent);
                return -1;
            }
        } else if (Pak_IsRumblePak(slot) != 0) {
            D_801309BC = 1;
            if (D_80130990 != 0) {
                tmpl = D_0043BC24;
            } else if (D_80130984 != 0) {
                tmpl = D_0043BC48;
            } else {
                tmpl = D_0043BC6C;
            }
            MenuPool_OpenMenu(D_80119E2C, tmpl, c->player, c->controller, parent);
        } else if (quiet == 0) {
            D_801309BC = 1;
            if (Pak_NeedsRepair(slot) != 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043C05C, c->player, c->controller, parent);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BC00, c->player, c->controller, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}
