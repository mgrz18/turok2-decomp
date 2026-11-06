/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_00408870.c, func_00408870, with Turok 2's layouts. */
#include "common.h"
#include "structs/world.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "structs/save.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/pak.h"
#include "globals/menu_desc.h"

/* Controller pak menu, load: after the pak check, copies the loaded save into the player and the game state
 * and shows the "loaded" message; otherwise reports whether the note could be read. */

extern f32 gEngineApp_m_Settings_match_seconds[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_minutes[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern f32 gEngineApp_m_Settings_match_hours[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern u8 gEngineApp_m_Game_m_MenuPool[]; /* local type: globals/menu.h declares it; this view (its type here) is what keeps the ROM's code */
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

    if (gPakMenuAnyPak != 0) {
        pad = gPakMenuPort;
    } else {
        pad = own->controller->port;
    }
    if (PakMenu_CheckPak_2(own, pad, 0) != 0) {
        gPakMenuLeaving = 1;
        return 1;
    }
    if (sGameNoteLoad != 0) {
        plr = CPlayerPool__FindByController(&gEngineApp_m_Game.m_PlayerList, own->controller);
        g = &gScene;
        save = gEngineApp_m_Settings_unk180;
        plr->m_Data = gLoadBuffer->player;
        {
            *(GameSettingsBlock *)&gEngineApp_m_Settings = gLoadBuffer->settings;
            c = gLoadBuffer;
        }
        new_var = &c->seconds;
        gEngineApp_m_Settings_match.frames = c->frames;
        gEngineApp_m_Settings_match_seconds[0] = *new_var;
        gEngineApp_m_Settings_match_minutes[0] = c->minutes;
        gScene_m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
        gEngineApp_m_Settings_unk180 = save;
        gEngineApp_m_Settings_match_hours[0] = c->hours;
        src = c->levelFlags;
        dp = gLoadSavePersistentData;
        for (i = 0; i < g->unk_30; i++) {
            *dp++ = *src++;
        }
        gEngineApp_m_Game.m_PlayerList.m_bCinema = 0;
        gEngineApp_m_Game.m_PlayerList.m_bCinemaStarted = 0;
        MenuPool_OpenMenu_2(gEngineApp_m_Game.m_MenuPool, &gPakMsgRumblePakDesc_2, plr, plr->m_ContState.m_pContState, 0);
        return 1;
    }
    if (Pak_DeleteNote_2(pad, gPakNoteIndex) == 0) {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, &gPakMsgDeleteDoneDesc_2, own->player, own->controller, own->context);
    } else {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, &gPakMsgDeleteFailedDesc_2, own->player, own->controller, own->context);
    }
    return 1;
}
