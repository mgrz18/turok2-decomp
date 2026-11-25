#include "common.h"
#include "structs/menu.h"

s32 Pak_ProbeSlot(s32);
void Controller_WatchPak(s32 on);

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

DEFERRED void SingleMenu_Init(Menu *arg0) {
    s8 temp_s0;

    temp_s0 = arg0->controller->port;
    Controller_WatchPak(temp_s0);
    Pak_ProbeSlot((s32) temp_s0);
}

#include "globals/pak.h"

DEFERRED void SingleMenu_InitAfterPak(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}

#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "structs/twin_titlemenu.h"

DEFERRED s32 SingleMenu_NewGame(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakMsgRumblePakBDesc, arg1->player, (sptr) arg1->controller, 0);
    return 1;
}

s32 Pak_ClearDamagedContinued();                            /* extern */

DEFERRED s32 SingleMenu_Load(s32 arg0, Menu *arg1) {
    Pak_ClearDamagedContinued();
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc, (sptr) arg1->player, (sptr) arg1->controller, 0);
    return 1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043EA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"

extern void CEngineApp__SetupFadeTo(struct GameSession *object, int value);
s32 SingleMenu_GoBack(void *arg0, Outer8043E56C *arg1);
extern void Controller_UnwatchPak(sptr arg0);

DEFERRED s32 SingleMenu_GoBack(void *arg0, Outer8043E56C *arg1) {
    CEngineApp__SetupFadeTo(&gEngineApp, 8);
    gInSinglePlayerMenu = 0;
    Controller_UnwatchPak(arg1->unk20->unk4);
    return 1;
}

#include "structs/vector.h"
#include "globals/game.h"
#include "ultra_types.h"
#include "gfx.h"
#include "structs/controller.h"
#include "structs/list.h"
#include "structs/table.h"

void TitleMenu_Single(void *arg0, Menu *menu);
void Music_RestoreVolume(void);
extern s32 Controller_ConfirmPressed(char *);
extern void CEngineApp__StartGame(char *, s32);

/* The window's controller by member (an m2c view wrote it at N64 offset 0x20: Menu.desc on the host). */
DEFERRED void TitleMenu_Single(void *arg0, Menu *menu) {
    GameSettings *globals;
    CPlayerOptions *settings;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    globals = &gEngineApp_m_Settings;
    globals->multiplayer = 0;
    globals->playerCount = 1;
    for (i = 3; i >= 0; i--) {
        globals->slots[i].m_bActive = 0;
    }
    i = 0;
    settings = &gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->m_bActive = on;
            gEngineApp_m_Settings.defaultSlot.m_nController = i;
            menu->controller = profile;
            break;
        }
        profile++;
    }
    Music_RestoreVolume();
    CEngineApp__StartGame(&gEngineApp, -1);
    gInSinglePlayerMenu = 1;
}

#include "structs/player.h"

s32 Controller_CountConnected();                                /* extern */

DEFERRED s32 TitleMenu_MultiUpdate(s32 arg0, Menu *arg1) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (Controller_CountConnected() < 2) {
        temp_a0 = arg1->items;
        temp_a0[2].flags = temp_a0[2].flags & 0xFEFFFFFF;
        if (arg1->cursor == 2) {
            arg1->cursor = 1;
        }
    } else {
        temp_v0 = arg1->items;
        temp_v0[2].flags = temp_v0[2].flags | 0x01000000;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EBE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */


DEFERRED s32 TitleMenu_Multi(void *first, void *second, void *third) {
    GameSettings *options = &gEngineApp_m_Settings;
    s32 i;
    options->multiplayer = 1;
    options->playerCount = 4;
    for (i = 0; i < 4; i++) {
        options->slots[i].m_bActive = 0;
    }
    MenuPool_OpenChildMenu(third, second, &gDeathmatchMenuDesc);
    return 1;
}

DEFERRED s32 TitleMenu_Options(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gOptionsMenuDesc);
    return 1;
}

DEFERRED s32 TitleMenu_EnterCheat(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gEnterCheatMenuDesc);
    return 1;
}

DEFERRED s32 TitleMenu_Cheats(sptr arg0, sptr arg1, sptr arg2) { /* sptr: menu callback args (--high-memory) */
    MenuPool_OpenChildMenu(arg2, arg1, &gCheatsMenuDesc);
    return 1;
}

void Music_SetTrack(s32 track);
void Sound__StopAllExcept(s32 volume);
void Sound__SetForce2D(s32 on);
s32 Music_GetTrack();                                /* extern */
void Controller_DisableMenuSounds(void);

DEFERRED s32 TitleMenu_SoundOptions(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Sound__SetForce2D(1);
    sSoundMenuActive = 0;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu(arg2, arg1, &gSoundOptionsMenuDesc);
    return 1;
}

s32 CEngineApp__QuitToLegal(sptr);
s32 Music_Stop();                            /* extern */

DEFERRED s32 QuitMenu_Yes(void) {
    Music_SetTrack(-1);
    Music_RestoreVolume();
    Music_Stop();
    CEngineApp__QuitToLegal((sptr) &gEngineApp);
    return 1;
}

#include "m2c_macros.h"

DEFERRED void QuitMenu_Init(s16 *arg0) {
    *arg0 = 4;
}

DEFERRED s32 Menu_StepDebugMode(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk18;
    *option = Menu_StepValue(second, *option, 1, 0, 2, 1);
    return 0;
}

#include "globals/menu_text.h"

DEFERRED s32 Menu_DebugModeLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.unk18) {
    case 0:
        a->text = sDebugMenuText_DebugOff;
        break;
    case 1:
        a->text = sDebugMenuText_DebugOn;
        break;
    case 2:
        a->text = sDebugMenuText_DebugVis;
        break;
    case 3:
        a->text = sDebugMenuText_DebugAis;
        break;
    case 4:
        a->text = sDebugMenuText_DebugSnd;
        break;
    case 5:
        a->text = sDebugMenuText_DebugLinks;
        break;
    }
    return 0;
}

#include "protos/game.h"

void Sound__EnableAmbientTrack();                            /* extern */
void LoopSounds_Resume(sptr);                         /* extern */

DEFERRED s32 PauseMenu_Resume(void) {
    LoopSounds_Resume(LoopSounds_Get());
    Music_RestoreVolume();
    Sound__EnableAmbientTrack();
    return 1;
}

DEFERRED s32 PauseMenu_MissionStatus(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gMissionStatusMenuDesc);
    return 1;
}

DEFERRED s32 PauseMenu_Inventory(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gInventoryMenuDesc);
    return 1;
}

DEFERRED s32 PauseMenu_Options(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gOptionsMenuDesc);
    return 1;
}

DEFERRED s32 PauseMenu_EnterCheat(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gEnterCheatMenuDesc);
    return 1;
}

DEFERRED s32 PauseMenu_Cheats(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gCheatsMenuDesc);
    return 1;
}

DEFERRED s32 PauseMenu_Quit(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gQuitMenuDesc);
    return 1;
}

s32 MenuItem_SelectSubmenu(sptr, sptr, sptr);
s32 Music_DuckVolume();                            /* extern */
s32 Controller_EnableMenuSounds();                            /* extern */

DEFERRED void SoundMenu_GoBack(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    Sound__SetForce2D(0);
    if (sSoundMenuActive != 0) {
        Music_DuckVolume();
    }
    Music_SetTrack(sSoundMenuSavedTrack);
    Controller_EnableMenuSounds();
    MenuItem_SelectSubmenu(arg0, arg1, arg2);
}

DEFERRED s32 SoundMenu_StepSpeechVolume(sptr arg0, sptr arg1)
{
  s32 var_v1;
  s32 new_var;
  var_v1 = gEngineApp_m_Settings_unk16[0];
  new_var = (s32) var_v1;
  var_v1 = Menu_StepValue(arg1, new_var, 8, 0, 0xFF, 0);
  if (var_v1 == 0xF7)
  {
    var_v1 = 0xF8;
  }
  gEngineApp_m_Settings_unk16[0] = (u8) var_v1;
  return 0;
}

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
DEFERRED s32 SoundMenu_SpeechVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk16 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_SpeechVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_SpeechVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_SpeechVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "  %2d", v);
    } else {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), " %3d", v);
    }
    return 0;
}

DEFERRED s32 SoundMenu_StepMusicVolume(sptr arg0, sptr arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = gEngineApp_m_Settings_unk14[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk14[0] = (u8) var_a1;
  if (Music_GetTrack() <= 0)
  {
    Music_SetTrack(1);
  }
  return 0;
}

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
DEFERRED s32 SoundMenu_MusicVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk14 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_MusicVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_MusicVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_MusicVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_2d, v);
    } else {
        a->text = sSoundMenuText_MusicVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_3d, v);
    }
    return 0;
}

s32 Sound__Play2D(s32);

DEFERRED s32 SoundMenu_StepSfxVolume(sptr arg0, sptr arg1)
{
  s32 var_a1;
  s32 new_var;
  var_a1 = gEngineApp_m_Settings_unk15[0];
  new_var = Menu_StepValue(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk15[0] = (u8) var_a1;
  Sound__Play2D(0x19E);
  return 0;
}

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
DEFERRED s32 SoundMenu_SfxVolumeLabel(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk15 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_SfxVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_SfxVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_SfxVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_SfxVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_2d, v);
    } else {
        a->text = sSoundMenuText_SfxVolume;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sSoundMenuFmt_3d, v);
    }
    return 0;
}

DEFERRED s32 SoundMenu_StepMusicTrack(s32 arg0, struct RecordRecAt20V1 *arg1) {
    s32 var_a0;
    var_a0 = sSoundMenuTrack;
    if (arg1->inner->unkB0 & 0x20202) {
        var_a0 -= 1;
        if (var_a0 < 0) {
            var_a0 = 0x16;
        } else if (var_a0 == 0xE) {
            var_a0 = 0xB;
        }
    }
    if (arg1->inner->unkB0 & 0x4D101) {
        var_a0 += 1;
        if (var_a0 >= 0x17) {
            var_a0 = 0;
        } else if (var_a0 == 0xC) {
            var_a0 = 0xF;
        }
    }
    sSoundMenuTrack = var_a0;
    Music_SetTrack(var_a0);
    return 0;
}

DEFERRED s32 SoundMenu_MusicTrackLabel(MenuItem *a) {
    if (sSoundMenuTrack == 0) {
        a->text = sSoundMenuText_MusicNone;
    } else {
        a->text = sSoundMenuText_Music;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), "%2d", sSoundMenuTrack);
    }
    return 0;
}

extern void Sound__DisableAmbientTrack(void);
extern s32 Music_GetTrack(void);

/* Opens the menu gSoundOptionsMenuDesc, switching the music (restored by SoundMenu_GoBack). */
DEFERRED s32 PauseMenu_SoundOptions(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(300);
    Sound__DisableAmbientTrack();
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    sSoundMenuActive = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu(arg2, arg1, &gSoundOptionsMenuDesc);
    return 1;
}

sptr Video_GetResolutionText(void);
s32 GetMemSize();                                /* extern */

DEFERRED s32 PauseMenu_ResolutionText(void *arg0)
{
  s32 var_v0;
  int new_var;
  if (GetMemSize() != 0x400000)
  {
    new_var = 0x800000;
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) | new_var;
  }
  else
  {
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) & 0xFF7FFFFF;
  }
  new_var = var_v0;
  *((s32 *) (((s8 *) arg0) + 8)) = new_var;
  *((s32 *) (((s8 *) arg0) + 0x14)) = Video_GetResolutionText();
  return 0;
}

s32 Video_StartResolutionChange(void);

DEFERRED s32 PauseMenu_ToggleResolution(void) {
    Video_StartResolutionChange();
    return 0;
}

#include "globals/rumble.h"

void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(RumblePool *pool);
extern void LoopSounds_Pause(sptr);

DEFERRED void PauseMenu_Init(A254Menu *menu) {
    s32 i;

    menu->state = 2;
    for (i = 0; i < 4; i++) {
        sPauseRumbleOn[i] = gControllers[i].rumbleOn;
    }
    RumblePool_FreeAll(&gRumblePool);
    Controller_ResetAllRumble();
    if (gEngineApp_m_Settings.multiplayer) {
        menu->screen->flags80 &= ~0x800000;
        menu->screen->flagsA8 &= ~0x800000;
    } else {
        menu->screen->flags80 |= 0x800000;
        menu->screen->flagsA8 |= 0x800000;
    }
    LoopSounds_Pause(LoopSounds_Get());
}

/* Restores the per-controller words saved by PauseMenu_Init. */
DEFERRED void PauseMenu_Close(void) {
    s32 i;

    Controller_ResetAllRumble();
    for (i = 0; i < 4; i++) {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
    }
    RumblePool_FreeAll(&gRumblePool);
}

s32 Menu_Draw(sptr, sptr);

DEFERRED void PauseMenu_Update(sptr arg0, sptr arg1) { /* sptr: the menu and its pool (soak S4, --high-memory) */
    Menu_Draw(arg0, arg1);
}
