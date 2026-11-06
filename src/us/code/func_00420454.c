/* WRONG CUT: real function 0x00420454, size 0x264 (through 0x004206B8): splat cut it into
 * GameNoteMenu_SelectYes, func_004205B8 and func_004205C4 inside its straight-line code. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/scene.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"

/* local: globals/game.h declares these, but its gEngineApp_m_Settings_unk180 (u8) clashes with the array view below, which the ROM needs here */
extern GameSettings gEngineApp_m_Settings;
extern GameMatch gEngineApp_m_Settings_match; /* local type: globals/game.h declares it; this view (its type here) is what keeps the ROM's code */
extern CGame gEngineApp_m_Game;

/* Load game menu "select" handler: with the pak checked, either restores the
 * loaded save (player block, game progress, level times, the level's key
 * bytes) and opens the "game loaded" dialog, or opens the "no save"/"bad
 * save" dialog for the pak note. */

extern u32 gEngineApp_m_Settings_unlockedCheats[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u8 gEngineApp_m_Settings_unk180[]; /* local type: GameSettings.unk180 read as an array (other registers otherwise) */
extern f32 gEngineApp_m_Settings_match_seconds[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_minutes[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_hours[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern s32 gEngineApp_m_Game_m_PlayerList_m_bCinema[2]; /* local type: the field's real type (or the member path) changes the ROM's code here */

extern s32 PakMenu_CheckPak(Menu *c, s32 slot, s32 quiet);
extern CPlayer *CPlayerPool__FindByController(void *players, ControllerProfile *ctl);
extern s32 Pak_DeleteNote(s32 slot, s32 note);
extern void MenuPool_OpenMenu(void *menus, void *tmpl, s32 a, void *b, void *parent);

s32 GameNoteMenu_SelectYes(void *menu, Menu *c) {
    s32 slot;
    CPlayer *player;
    u8 keep;
    s32 i;
    u8 *src;
    u8 *dst;
    CScene *world;

    if (gPakMenuAnyPak) {
        slot = gPakMenuPort;
    } else {
        slot = c->controller->port;
    }
    if (PakMenu_CheckPak(c, slot, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sGameNoteLoad != 0) {
        player = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, c->controller);
        world = &gScene;
        keep = gEngineApp_m_Settings_unk180[0];
        player->m_Data = gLoadBuffer->player;
        *(GameSettingsBlock *)&gEngineApp_m_Settings = gLoadBuffer->settings;
        gEngineApp_m_Settings_match.frames = gLoadBuffer->frames;
        gEngineApp_m_Settings_match_seconds[0] = gLoadBuffer->seconds;
        gEngineApp_m_Settings_match_minutes[0] = gLoadBuffer->minutes;
        gScene_m_PreviewOptions = gEngineApp_m_Settings_unlockedCheats[0];
        gEngineApp_m_Settings_match_hours[0] = gLoadBuffer->hours;
        src = gLoadBuffer->levelFlags;
        dst = gLoadSavePersistentData;
        gEngineApp_m_Settings_unk180[0] = keep;
        for (i = 0; i < world->unk_30; i++) {
            *dst++ = *src++;
        }
        gEngineApp_m_Game_m_PlayerList_m_bCinema[0] = 0;
        gEngineApp_m_Game_m_PlayerList_m_bCinema[1] = 0;
        MenuPool_OpenMenu((u8 *) gEngineApp_m_Game_m_PlayerList_m_bCinema + 0x588, &gPakMsgRumblePakDesc, (s32) player, player->m_ContState.m_pContState, NULL);
        return 1;
    }
    if (Pak_DeleteNote(slot, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakMsgDeleteDoneDesc, c->player, c->controller, c->context);
    } else {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakMsgDeleteFailedDesc, c->player, c->controller, c->context);
    }
    return 1;
}
