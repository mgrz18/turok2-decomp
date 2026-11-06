/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408870.c, func_00408870, with Turok 2's layouts. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "structs/save.h"

/* Controller pak menu, load: after the pak check, copies the loaded save into the player and the game state
 * and shows the "loaded" message; otherwise reports whether the note could be read. */

extern s32 D_80130990;
extern s32 D_800C2028; /* pad */
extern s32 D_801309BC;
extern s32 D_801309B8;
extern s32 D_801309C4;
extern CScene gScene[];
extern u8 D_8011AC58;
extern s32 D_8011AADC;
extern f32 D_8011ACB4[];
extern f32 D_8011ACB8[];
extern f32 D_8011ACBC[];
extern s32 D_80110060;
extern u8 D_800EBAC0[];
extern u8 D_80119E2C[];
extern u8 D_0043BC40[];
extern u8 D_0043BCD0[];
extern u8 D_0043BD3C[];
s32 PakMenu_CheckPak_2(Menu *o, s32 pad, s32 z);
CPlayer *CPlayerPool__FindByController(CPlayerPool *list, ControllerProfile *s);
s32 Pak_DeleteNote_2(s32 pad, s32 idx);
void MenuPool_OpenMenu_2(u8 *a, void *b, void *c, s32 d, s32 e);

s32 GameNoteMenu_SelectYes_2(s32 unused, Menu *own) {
    f32 *new_var;
    s32 pad;
    CPlayer *plr;
    CScene *g;
    u8 save;
    s32 i;
    SaveGame *c;
    u8 *src;
    u8 *dp;

    if (D_80130990 != 0) {
        pad = D_800C2028;
    } else {
        pad = own->controller->port;
    }
    if (PakMenu_CheckPak_2(own, pad, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309B8 != 0) {
        plr = CPlayerPool__FindByController(&D_80119870.m_PlayerList, own->controller);
        g = gScene;
        save = D_8011AC58;
        plr->m_Data = D_800C201C->player;
        {
            *(GameSettingsBlock *)&D_8011AAD8 = D_800C201C->settings;
            c = D_800C201C;
        }
        new_var = &c->seconds;
        D_8011ACB0.frames = c->frames;
        D_8011ACB4[0] = *new_var;
        D_8011ACB8[0] = c->minutes;
        D_80110060 = D_8011AADC;
        D_8011AC58 = save;
        D_8011ACBC[0] = c->hours;
        src = c->levelFlags;
        dp = D_800EBAC0;
        for (i = 0; i < g->unk_30; i++) {
            *dp++ = *src++;
        }
        D_80119870.m_PlayerList.m_bCinema = 0;
        D_80119870.m_PlayerList.m_bCinemaStarted = 0;
        MenuPool_OpenMenu_2(D_80119870.m_MenuPool, D_0043BC40, plr, plr->m_ContState.m_pContState, 0);
        return 1;
    }
    if (Pak_DeleteNote_2(pad, D_801309C4) == 0) {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BCD0, own->player, own->controller, own->context);
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BD3C, own->player, own->controller, own->context);
    }
    return 1;
}
