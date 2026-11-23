/* WRONG CUT: the real function starts at 0x004595D8 (size 0x1A4 = 420): splat
 * cut its first two words (lui/lw gPakMenuAnyPak) as CharNoteMenu_SelectYes_2. */
#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/menu.h"
#include "globals/menu_desc.h"

/* Level chosen: load it, or set up the new player from the saved settings, or
 * open the next menu page. */

extern s32 gPakMenuAnyPak; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gPakMenuPort; /* local: globals/pak.h is not included here (its gPakProfile would clash with this file's local-type view) */
extern s32 gPakMenuLeaving; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 sCharNoteLoad; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gPakNoteIndex; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern u8 gPakProfile_character; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u16 gPakProfile[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern u16 gPakProfile_deaths[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u16 gPakProfile_kills[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u16 gPakProfile_tags[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u8 gPakProfile_name[8]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s8 gPakProfileRead_character; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 PakMenu_CheckPak_2(Menu *event, s32 mode, s32 arg2);
CPlayer *CPlayerPool__FindByController(void *state, ControllerProfile *args);
void CPlayer__SetCharacter_2(CPlayer *player, s32 arg1);
void MenuPool_OpenChildMenu_2(void *menu, Menu *event, void *page);
s32 Pak_DeleteNote_2(s32 mode, s32 arg1);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *args, void *prev);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (gPakMenuAnyPak != 0) {
        mode = gPakMenuPort;
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
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sCharNoteLoad != 0) {
        player = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, event->controller);
        event->player = player;
        options = player->m_pOptions;
        options->m_nCharacter = gPakProfile_character;
        options->m_Suicides = gPakProfile[0];
        options->m_Deaths = gPakProfile_deaths[0];
        options->m_Kills = gPakProfile_kills[0];
        options->m_Tags = gPakProfile_tags[0];
        for (i = 0; i < 8; i++) {
            options->m_Name[i] = gPakProfile_name[i];
        }
        CPlayer__SetCharacter_2(player, gPakProfileRead_character);
        MenuPool_OpenChildMenu_2(player->m_pCamera->m_MenuPool, event, &gPlayerSetupMenuDesc_2);
        return 1;
    }
    if (Pak_DeleteNote_2(mode, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgDeleteDoneDesc_2, event->player, event->controller, event->context);
    } else {
        MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgDeleteFailedDesc_2, event->player, event->controller, event->context);
    }
    return 1;
}
