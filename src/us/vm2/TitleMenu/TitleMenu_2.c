#include "common.h"
#include "structs/menu.h"

s32 Pak_ProbeSlot_2(s32);
void Controller_WatchPak(s32 on);

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void SingleMenu_Init_2(Menu *arg0) {
    s8 temp_s0;

    temp_s0 = arg0->controller->port;
    Controller_WatchPak(temp_s0);
    Pak_ProbeSlot_2((s32) temp_s0);
}

#include "globals/pak.h"

void SingleMenu_InitAfterPak_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}

#include "globals/menu.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"

s32 SingleMenu_NewGame_2(s32 arg0, Menu *arg1) {
    Controller_WatchPak(arg1->controller->port);
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakMsgRumblePakBDesc_2, arg1->player, (sptr) arg1->controller, 0);
    return 1;
}

s32 Pak_ClearDamagedContinued_2();                            /* extern */
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Unk00419618;

s32 SingleMenu_Load_2(s32 arg0, Menu *arg1) {
    Pak_ClearDamagedContinued_2();
    MenuPool_OpenMenu_2(&gEngineApp_m_Game_m_MenuPool, &gPakLoadGameMenuDesc_2, (sptr) arg1->player, (sptr) arg1->controller, 0);
    return 1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E830_de.c, func_8043EA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/game.h"

extern void CEngineApp__SetupFadeTo(struct GameSession *object, int value);
s32 SingleMenu_GoBack_2(void *arg0, Outer8043E56C *arg1);
extern void Controller_UnwatchPak(sptr arg0);

s32 SingleMenu_GoBack_2(void *arg0, Outer8043E56C *arg1) {
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

void TitleMenu_Single_2(void *arg0, Menu *menu);
void Music_RestoreVolume(void);
extern s32 Controller_ConfirmPressed(char *);
extern void CEngineApp__StartGame_2(char *, s32);

/* The window's controller by member (an m2c view wrote it at N64 offset 0x20: Menu.desc on the host). */
void TitleMenu_Single_2(void *arg0, Menu *menu) {
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
    CEngineApp__StartGame_2(&gEngineApp, -1);
    gInSinglePlayerMenu = 1;
}

#include "structs/player.h"

s32 Controller_CountConnected();                                /* extern */

s32 TitleMenu_MultiUpdate_2(s32 arg0, Menu *arg1) {
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


s32 TitleMenu_Multi_2(void *first, void *second, void *third) {
    GameSettings *options = &gEngineApp_m_Settings;
    s32 i;
    options->multiplayer = 1;
    options->playerCount = 4;
    for (i = 0; i < 4; i++) {
        options->slots[i].m_bActive = 0;
    }
    MenuPool_OpenChildMenu_2(third, second, &gDeathmatchMenuDesc_2);
    return 1;
}

s32 TitleMenu_Options_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gOptionsMenuDesc_2);
    return 1;
}

s32 TitleMenu_EnterCheat_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gEnterCheatMenuDesc_2);
    return 1;
}

s32 TitleMenu_Cheats_2(sptr arg0, sptr arg1, sptr arg2) { /* sptr: menu callback args (--high-memory) */
    MenuPool_OpenChildMenu_2(arg2, arg1, &gCheatsMenuDesc_2);
    return 1;
}

void Music_SetTrack(s32 track);
void Sound__StopAllExcept(s32 volume);
void Sound__SetForce2D(s32 on);
s32 Music_GetTrack();                                /* extern */
void Controller_DisableMenuSounds(void);

s32 TitleMenu_SoundOptions_2(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Sound__SetForce2D(1);
    sSoundMenuActive = 0;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, &gSoundOptionsMenuDesc_2);
    return 1;
}

s32 CEngineApp__QuitToLegal_2(sptr);
s32 Music_Stop();                            /* extern */

s32 QuitMenu_Yes_2(void) {
    Music_SetTrack(-1);
    Music_RestoreVolume();
    Music_Stop();
    CEngineApp__QuitToLegal_2((sptr) &gEngineApp);
    return 1;
}

#include "m2c_macros.h"

void QuitMenu_Init_2(s16 *arg0) {
    *arg0 = 4;
}

s32 Menu_StepDebugMode_2(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk18;
    *option = Menu_StepValue_2(second, *option, 1, 0, 2, 1);
    return 0;
}

#include "globals/menu_text.h"

s32 Menu_DebugModeLabel_2(MenuItem *a) {
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

s32 PauseMenu_Resume_2(void) {
    LoopSounds_Resume(LoopSounds_Get());
    Music_RestoreVolume();
    Sound__EnableAmbientTrack();
    return 1;
}

s32 PauseMenu_MissionStatus_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gMissionStatusMenuDesc_2);
    return 1;
}

s32 PauseMenu_Inventory_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gInventoryMenuDesc_2);
    return 1;
}

s32 PauseMenu_Options_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gOptionsMenuDesc_2);
    return 1;
}

s32 PauseMenu_EnterCheat_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gEnterCheatMenuDesc_2);
    return 1;
}

s32 PauseMenu_Cheats_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gCheatsMenuDesc_2);
    return 1;
}

s32 PauseMenu_Quit_2(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, &gQuitMenuDesc_2);
    return 1;
}

s32 MenuItem_SelectSubmenu_2(sptr, sptr, sptr);
s32 Music_DuckVolume();                            /* extern */
s32 Controller_EnableMenuSounds();                            /* extern */

void SoundMenu_GoBack_2(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(-1);
    Sound__SetForce2D(0);
    if (sSoundMenuActive != 0) {
        Music_DuckVolume();
    }
    Music_SetTrack(sSoundMenuSavedTrack);
    Controller_EnableMenuSounds();
    MenuItem_SelectSubmenu_2(arg0, arg1, arg2);
}

s32 SoundMenu_StepSpeechVolume_2(sptr arg0, sptr arg1)
{
  s32 var_v1;
  s32 new_var;
  var_v1 = gEngineApp_m_Settings_unk16[0];
  new_var = (s32) var_v1;
  var_v1 = Menu_StepValue_2(arg1, new_var, 8, 0, 0xFF, 0);
  if (var_v1 == 0xF7)
  {
    var_v1 = 0xF8;
  }
  gEngineApp_m_Settings_unk16[0] = (u8) var_v1;
  return 0;
}

extern s32 MenuItem_TextLength_2(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Label of a slider setting: its value from -16 to 15, in steps of 8 raw units. */
s32 SoundMenu_SpeechVolumeLabel_2(MenuItem *a) {
    s32 v = (gEngineApp_m_Settings.unk16 - 0x80) / 8;

    if (v == 0) {
        a->text = sSoundMenuText_SpeechVolumeNorm;
    } else if (v == 15) {
        a->text = sSoundMenuText_SpeechVolumeMax;
    } else if (v == -16) {
        a->text = sSoundMenuText_SpeechVolumeOff;
    } else if (v > 0) {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength_2(a) - 4), "  %2d", v);
    } else {
        a->text = sSoundMenuText_SpeechVolume;
        sprintf(a->text + (MenuItem_TextLength_2(a) - 4), " %3d", v);
    }
    return 0;
}

s32 SoundMenu_StepMusicVolume_2(sptr arg0, sptr arg1)
{
  s32 new_var;
  s32 var_a1;
  var_a1 = gEngineApp_m_Settings_unk14[0];
  new_var = Menu_StepValue_2(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
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

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */

/* Shows setting unk14 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 SoundMenu_MusicVolumeLabel_2(MenuItem *item) {
    s32 value = (gEngineApp_m_Settings.unk14 - 0x80) / 8;

    if (value == 0) {
        item->text = sSoundMenuText_MusicVolumeNorm;
    } else if (value == 15) {
        item->text = sSoundMenuText_MusicVolumeMax;
    } else if (value == -16) {
        item->text = sSoundMenuText_MusicVolumeOff;
    } else if (value > 0) {
        item->text = sSoundMenuText_MusicVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %2d", value);
    } else {
        item->text = sSoundMenuText_MusicVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %3d", value);
    }
    return 0;
}

s32 Sound__Play2D(s32);

s32 SoundMenu_StepSfxVolume_2(sptr arg0, sptr arg1)
{
  s32 var_a1;
  s32 new_var;
  var_a1 = gEngineApp_m_Settings_unk15[0];
  new_var = Menu_StepValue_2(arg1, (s32) var_a1, 8, 0, 0xFF, 0);
  var_a1 = new_var;
  if (var_a1 == 0xF7)
  {
    var_a1 = 0xF8;
  }
  gEngineApp_m_Settings_unk15[0] = (u8) var_a1;
  Sound__Play2D(0x19E);
  return 0;
}

/* Shows setting unk15 (0-255, centred on 0x80) in steps of 8 from -16 to 15. */
s32 SoundMenu_SfxVolumeLabel_2(MenuItem *item) {
    s32 value = (gEngineApp_m_Settings.unk15 - 0x80) / 8;

    if (value == 0) {
        item->text = sSoundMenuText_SfxVolumeNorm;
    } else if (value == 15) {
        item->text = sSoundMenuText_SfxVolumeMax;
    } else if (value == -16) {
        item->text = sSoundMenuText_SfxVolumeOff;
    } else if (value > 0) {
        item->text = sSoundMenuText_SfxVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %2d", value);
    } else {
        item->text = sSoundMenuText_SfxVolume;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %3d", value);
    }
    return 0;
}

s32 SoundMenu_StepMusicTrack_2(s32 arg0, struct RecordRecAt20V1 *arg1) {
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

s32 SoundMenu_MusicTrackLabel_2(MenuItem *a) {
    if (sSoundMenuTrack == 0) {
        a->text = sSoundMenuText_MusicNone;
    } else {
        a->text = sSoundMenuText_Music;
        sprintf(a->text + (MenuItem_TextLength_2(a) - 2), "%2d", sSoundMenuTrack);
    }
    return 0;
}

extern void Sound__DisableAmbientTrack(void);
extern s32 Music_GetTrack(void);

/* Opens the menu gSoundOptionsMenuDesc_2, switching the music (restored by SoundMenu_GoBack_2). */
s32 PauseMenu_SoundOptions_2(sptr arg0, sptr arg1, sptr arg2) {
    Sound__StopAllExcept(300);
    Sound__DisableAmbientTrack();
    sSoundMenuSavedTrack = Music_GetTrack();
    Music_SetTrack(sSoundMenuTrack);
    Music_RestoreVolume();
    Sound__SetForce2D(1);
    sSoundMenuActive = 1;
    Controller_DisableMenuSounds();
    MenuPool_OpenChildMenu_2(arg2, arg1, &gSoundOptionsMenuDesc_2);
    return 1;
}

sptr Video_GetResolutionText_2(void);
s32 GetMemSize();                                /* extern */

s32 PauseMenu_ResolutionText_2(void *arg0)
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
  *((s32 *) (((s8 *) arg0) + 0x14)) = Video_GetResolutionText_2();
  return 0;
}

s32 Video_StartResolutionChange_2(void);

s32 PauseMenu_ToggleResolution_2(void) {
    Video_StartResolutionChange_2();
    return 0;
}

#include "globals/rumble.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ u32 flags80;
    /* 0x84 */ u8 pad84[0x24];
    /* 0xA8 */ u32 flagsA8;
} A254Screen;
typedef struct {
    /* 0x00 */ s16 state;
    /* 0x02 */ u8 pad2[0xA];
    /* 0x0C */ A254Screen *screen;
} A254Menu;
void Controller_ResetAllRumble(void);
extern void RumblePool_FreeAll(RumblePool *pool);
extern void LoopSounds_Pause(sptr);

void PauseMenu_Init_2(A254Menu *menu) {
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

/* Restores the per-controller words saved by PauseMenu_Init_2. */
void PauseMenu_Close_2(void) {
    s32 i;

    Controller_ResetAllRumble();
    for (i = 0; i < 4; i++) {
        gControllers[i].rumbleOn = sPauseRumbleOn[i];
    }
    RumblePool_FreeAll(&gRumblePool);
}

s32 Menu_Draw_2(sptr, sptr);

void PauseMenu_Update_2(sptr arg0, sptr arg1) { /* sptr: the menu and its pool (soak S4, --high-memory) */
    Menu_Draw_2(arg0, arg1);
}
