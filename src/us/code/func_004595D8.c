/* WRONG CUT: the real function starts at 0x004595D8 (size 0x1A4 = 420): splat
 * cut its first two words (lui/lw D_80130990) as CharNoteMenu_SelectYes_2. */
#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player.h"
#include "structs/camera.h"

/* Level chosen: load it, or set up the new player from the saved settings, or
 * open the next menu page. */

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130960;
extern s32 D_801309C4;
extern u8 D_80130970;
extern u16 D_80130968[];
extern u16 D_8013096A[];
extern u16 D_8013096C[];
extern u16 D_8013096E[];
extern u8 D_80130974[8];
extern s8 D_80130950;
extern u8 D_80119E2C[];
extern u8 D_00438668[];
extern u8 D_0043BCD0[];
extern u8 D_0043BD3C[];

s32 PakMenu_CheckPak_2(Menu *event, s32 mode, s32 arg2);
CPlayer *CPlayerPool__FindByController(void *state, ControllerProfile *args);
void CPlayer__SetCharacter_2(CPlayer *player, s32 arg1);
void MenuPool_OpenChildMenu_2(void *menu, Menu *event, void *page);
s32 Pak_DeleteNote_2(s32 mode, s32 arg1);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *args, void *prev);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->controller->port;
    }
    return mode;
}

s32 CharNoteMenu_SelectYes_2(void *arg0, Menu *event) {
    s32 mode = currentMode(event);
    CPlayer *player;
    CPlayerOptions *options;
    s32 i;

    if (PakMenu_CheckPak_2(event, mode, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_80130960 != 0) {
        player = CPlayerPool__FindByController(&D_80119870.m_PlayerList, event->controller);
        event->player = player;
        options = player->m_pOptions;
        options->m_nCharacter = D_80130970;
        options->m_Suicides = D_80130968[0];
        options->m_Deaths = D_8013096A[0];
        options->m_Kills = D_8013096C[0];
        options->m_Tags = D_8013096E[0];
        for (i = 0; i < 8; i++) {
            options->m_Name[i] = D_80130974[i];
        }
        CPlayer__SetCharacter_2(player, D_80130950);
        MenuPool_OpenChildMenu_2(player->m_pCamera->m_MenuPool, event, D_00438668);
        return 1;
    }
    if (Pak_DeleteNote_2(mode, D_801309C4) == 0) {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BCD0, event->player, event->controller, event->context);
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043BD3C, event->player, event->controller, event->context);
    }
    return 1;
}
