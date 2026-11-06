/* Second-module copy of PakMenu_CheckPak (build/manual/W/PakMenu_CheckPak.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
#include "structs/menu.h"

/* Checks the controller pak in the given slot before a save/load menu:
 * returns the pak status, or -1 after opening the "pak full" dialog; opens the
 * matching warning dialog (templates D_0043xxxx) when the pak is missing,
 * damaged or unformatted. */

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

extern void Pak_ProbeSlot_2(s32 slot);
extern s32 Pak_GetStatus_2(s32 slot);
extern void *PakMenu_GetReturnMenu_2(Menu *c);
extern s32 Controller_TakePakChanged(s32 slot);
extern s32 Pak_IsRumblePak_2(s32 slot);
extern s32 Pak_NeedsRepair_2(s32 slot);
extern void MenuPool_OpenMenu_2(void *menus, void *tmpl, s32 a, s32 b, void *parent);

s32 PakMenu_CheckPak_2(Menu *c, s32 slot, s32 quiet) {
    s32 status;
    void *parent;
    s32 full;
    void *tmpl;

    Pak_ProbeSlot_2(slot);
    status = Pak_GetStatus_2(slot);
    parent = PakMenu_GetReturnMenu_2(c);
    if (parent != NULL) {
        full = Controller_TakePakChanged(slot);
        if ((status == 0 || full != 0) && status != -2) {
            if (full != 0) {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043BBF8, c->player, c->controller, parent);
                return -1;
            }
        } else if (Pak_IsRumblePak_2(slot) != 0) {
            D_801309BC = 1;
            if (D_80130990 != 0) {
                tmpl = D_0043BB44;
            } else if (D_80130984 != 0) {
                tmpl = D_0043BB68;
            } else {
                tmpl = D_0043BB8C;
            }
            MenuPool_OpenMenu_2(D_80119E2C, tmpl, c->player, c->controller, parent);
        } else if (quiet == 0) {
            D_801309BC = 1;
            if (Pak_NeedsRepair_2(slot) != 0) {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043BF7C, c->player, c->controller, parent);
            } else {
                MenuPool_OpenMenu_2(D_80119E2C, D_0043BB20, c->player, c->controller, parent);
            }
        } else if (status != -2) {
            status = 0;
        }
        return status;
    }
    return 0;
}
