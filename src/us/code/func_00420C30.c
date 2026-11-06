/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_13.c, func_80409144_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_0043BDB0[];
extern s32 D_801309BC;
extern int PakConfirmMenu_SelectYes(void * owner, struct MenuRecAt24 * menu, void * arg);
extern int D_801309C4;
extern char D_0043BE1C[];
extern s32 D_800C2028;
extern s32 D_8011B114;
extern s32 D_80130990;
extern char D_80119E2C[];
extern s32 Pak_DeleteNote(s32 ch, s32 index);
extern void Pak_ProbeSlot(s32 ch);
extern s32 D_801309B0;
extern s32 D_80130988;
extern char D_0043BDD4[];
extern char D_0043C038[];
extern s32 Pak_RepairIfDamaged(s32 ch);
extern s32 PakMenu_WriteSaveGame(void *owner, MenuRecAt24 *menu, void *arg);
s32 PakConfirmMenu_SelectYes(void *owner, MenuRecAt24 *menu, void *arg) {
    extern void MenuPool_OpenMenu(char *, char *, void *, PakMenuSlot *, char *);
    extern s32 PakMenu_CheckPak(MenuRecAt24 *menu, s32 ch, s32 quiet);
    s32 ch;
    switch (D_801309B0) {
        case 4:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, D_80130988) == 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BDB0, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043BE1C, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 3:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_DeleteNote(ch, D_801309C4) == 0) {
                PakMenu_WriteSaveGame(owner, menu, arg);
                return 1;
            }
            MenuPool_OpenMenu(D_80119E2C, D_0043BE1C, menu->player, menu->slot, menu->prompt);
            break;
        case 0:
            if (D_80130990 != 0) {
                ch = D_800C2028;
            } else {
                ch = menu->slot->unk4;
            }
            if (PakMenu_CheckPak(menu, ch, 1) != 0) {
                D_801309BC = 1;
                MenuPool_OpenMenu(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
                break;
            }
            Pak_ProbeSlot(ch);
            if (Pak_RepairIfDamaged(ch) == 0) {
                MenuPool_OpenMenu(D_80119E2C, D_0043BDD4, menu->player, menu->slot, menu->prompt);
            } else {
                MenuPool_OpenMenu(D_80119E2C, D_0043C038, menu->player, menu->slot, menu->prompt);
            }
            break;
        case 1:
        case 2:
            if (D_80130990 != 0) {
                D_8011B114 = 1;
            }
            MenuPool_OpenMenu(D_80119E2C, menu->prompt, menu->player, menu->slot, 0);
            break;
    }
    return 1;
}
