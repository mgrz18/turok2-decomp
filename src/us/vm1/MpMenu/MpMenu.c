#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"
#include "globals/game.h"

extern s32 Controller_ConfirmPressed(ControllerProfile *ctl);
extern s32 Random_Next(void);
extern void CEngineApp__StartGame(void *a, s32 level);

#ifndef TARGET_PORT /* the port keeps the generated copies */
/* The object's .rodata starts with these strings, before MpMenu_StartFragTag's literals (an earlier
 * definition emitted them: the original's inline label helpers or its menu tables, whose items point
 * at " "). Here they are data in that place, and the label functions read them by name. */
const char sFinalScoreMenuFmt_04d_def[] __asm__("sFinalScoreMenuFmt_04d") = "%04d";
const char sMpMenuText_Space_def[] __asm__("D_800C0E78") = " "; /* the vm1 menu tables' blank items */
const char sMpMenuFmt_02d_def[] __asm__("D_800C0E7C") = "%02d";
const char sLimitLabelFmt_02d_def[] __asm__("sLimitLabelFmt_02d") = "  %02d";
const char sLimitLabelText_None_def[] __asm__("sLimitLabelText_None") = "none";
#define MPMENU_FMT_02D sMpMenuFmt_02d_def
#else
#define MPMENU_FMT_02D "%02d"
#endif

/* Mp_SetupMatchRules, inlined. */
static __inline__ void StartMatch(f32 minutes, s32 a1, s32 started, s32 a3) {
    GameMatch *m = &gEngineApp_m_Settings.match;

    m->tag = started;
    m->teams = a3;
    m->scoreLimit = a1;
    m->over = 0;
    m->teamScore[0] = 0;
    m->teamScore[1] = 0;
    m->teamOver = 0;
    m->done = 0;
    m->timeLeft = minutes * 900.0f;
    if (m->timeLeft == 0.0f) {
        m->timeLeft = -1.0f;
    }
}

s32 MpMenu_StartFragTag(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->tagArena;
    StartMatch(setup->tagTimeLimit, setup->tagScoreLimit, 1, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = gControllers; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &gEngineApp_m_Settings.slots[i];

            slot->m_bActive = 1;
            slot->m_nController = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 6) {
            arg = level + 0x2336;
        } else {
            arg = Random_Next() % 6 + 0x2337;
        }
    } else {
        arg = Random_Next() % 6 + 0x2337;
    }
    CEngineApp__StartGame(&gEngineApp, arg);
    return 1;
}

#ifndef TARGET_PORT /* the port keeps the generated copy */
/* "%3d": vm1/vm2 MpMenu_PlayerSpeedLabel read it by name (globals/menu_text.h); its place in the
 * object's .rodata is here, after MpMenu_StartFragTag's literals */
const char sMpMenuFmt_3d_def[] __asm__("sMpMenuFmt_3d") = "%3d";
#endif

/* Mp_SetupMatchRules, inlined. */
static __inline__ void StartMatch_41760C(f32 minutes, s32 a1, s32 started, s32 a3) {
    gEngineApp_m_Settings.match.tag = started;
    gEngineApp_m_Settings.match.teams = a3;
    gEngineApp_m_Settings.match.scoreLimit = a1;
    gEngineApp_m_Settings.match.over = 0;
    gEngineApp_m_Settings.match.teamScore[0] = 0;
    gEngineApp_m_Settings.match.teamScore[1] = 0;
    gEngineApp_m_Settings.match.teamOver = 0;
    gEngineApp_m_Settings.match.done = 0;
    gEngineApp_m_Settings.match.timeLeft = minutes * 900.0f;
    if (gEngineApp_m_Settings.match.timeLeft == 0.0f) {
        gEngineApp_m_Settings.match.timeLeft = -1.0f;
    }
}

s32 MpMenu_StartBloodLust(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->dmArena;
    StartMatch_41760C(setup->dmTimeLimit, setup->dmScoreLimit, 0, 0);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = gControllers; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &gEngineApp_m_Settings.slots[i];

            slot->m_bActive = 1;
            slot->m_nController = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 13) {
            arg = level + 0x2328;
        } else {
            arg = Random_Next() % 13 + 0x2329;
        }
    } else {
        arg = Random_Next() % 13 + 0x2329;
    }
    CEngineApp__StartGame(&gEngineApp, arg);
    return 1;
}

s32 MpMenu_StartTeamBlood(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->teamArena;
    StartMatch(setup->teamTimeLimit, setup->teamScoreLimit, 0, 1);
    for (i = 3; i >= 0; i--) {
        setup->slots[i].m_nCharacter = 0;
    }
    for (i = 0, ctl = gControllers; i < 4; i++, ctl++) {
        if (Controller_ConfirmPressed(ctl)) {
            CPlayerOptions *slot = &gEngineApp_m_Settings.slots[i];

            slot->m_bActive = 1;
            slot->m_nController = i;
            break;
        }
    }
    if (level != 0) {
        if ((u32) (level - 1) < 13) {
            arg = level + 0x2328;
        } else {
            arg = Random_Next() % 13 + 0x2329;
        }
    } else {
        arg = Random_Next() % 13 + 0x2329;
    }
    CEngineApp__StartGame(&gEngineApp, arg);
    return 1;
}

#include "globals/menu_text.h"
#include "structs/menu.h"

/* Label of a weapon toggle in the weapons menu: "<weapon> on/off". */
s32 WeaponsMenu_ToggleLabel(MenuItem *a) {
    switch (a->index - 2) {
    default:
        a->text = "unknown weapon";
        break;
    case 0:
        if (gEngineApp_m_Settings.weaponEnabled[1]) {
            a->text = sWeaponsMenuText_ChargeDartRifleOn;
        } else {
            a->text = sWeaponsMenuText_ChargeDartRifleOff;
        }
        break;
    case 1:
        if (gEngineApp_m_Settings.weaponEnabled[2]) {
            a->text = sWeaponsMenuText_FirestormOn;
        } else {
            a->text = sWeaponsMenuText_FirestormOff;
        }
        break;
    case 2:
        if (gEngineApp_m_Settings.weaponEnabled[3]) {
            a->text = sWeaponsMenuText_AssaultRifleOn;
        } else {
            a->text = sWeaponsMenuText_AssaultRifleOff;
        }
        break;
    case 3:
        if (gEngineApp_m_Settings.weaponEnabled[4]) {
            a->text = sWeaponsMenuText_PlasmaRifleOn;
        } else {
            a->text = sWeaponsMenuText_PlasmaRifleOff;
        }
        break;
    case 4:
        if (gEngineApp_m_Settings.weaponEnabled[5]) {
            a->text = sWeaponsMenuText_CerebralBoreOn;
        } else {
            a->text = sWeaponsMenuText_CerebralBoreOff;
        }
        break;
    case 5:
        if (gEngineApp_m_Settings.weaponEnabled[6]) {
            a->text = sWeaponsMenuText_GrenadeLauncherOn;
        } else {
            a->text = sWeaponsMenuText_GrenadeLauncherOff;
        }
        break;
    case 6:
        if (gEngineApp_m_Settings.weaponEnabled[7]) {
            a->text = sWeaponsMenuText_ScorpionLauncherOn;
        } else {
            a->text = sWeaponsMenuText_ScorpionLauncherOff;
        }
        break;
    case 7:
        if (gEngineApp_m_Settings.weaponEnabled[9]) {
            a->text = sWeaponsMenuText_TorpedoLauncherOn;
        } else {
            a->text = sWeaponsMenuText_TorpedoLauncherOff;
        }
        break;
    }
    return 0;
}

#include "structs/player_options.h"

struct func_80250BD4_S1;
#ifdef TARGET_PORT
/* Menu::controller at its host offset (the N64 0x20 is inside Menu::desc on the host: a menu that
 * claimed a controller had its desc replaced by the profile; soak S5, the cheats menu) */
#include "structs/menu.h"
struct func_80250BD4_S1 {
    char pad0[__builtin_offsetof(Menu, controller)];
    ControllerProfile *unk20; /* Menu::controller */
};
#else
struct func_80250BD4_S1 {
    char pad0[0x20];
    ControllerProfile *unk20; /* Menu::controller */
};
#endif

typedef struct func_80250BD4_S1 func_80250BD4_S1;
struct SharedRecAtA8;
typedef struct SharedRecAtA8 SharedRecAtA8;
void Menu_ClaimPressingController(func_80250BD4_S1 *arg0);
struct SharedRecAtA8 {
    char pad0[0xA8];
    s8 unkA8;
};
void Music_RestoreVolume(void);

void Menu_ClaimPressingController(func_80250BD4_S1 *arg0) {
    CPlayerOptions *settings;
    s8 *status;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    i = 3;
    status = (s8 *)(struct SharedRecAtA8 *) &gEngineApp_m_Settings.slots[1];
    for (; i >= 0; i--) {
        ((SharedRecAtA8 *)status)->unkA8 = 0;
        status -= 0x40;
    }
    i = 0;
    settings = &gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++, profile++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->m_bActive = on;
            settings->m_nController = i;
            arg0->unk20 = profile;
            break;
        }
    }
    Music_RestoreVolume();
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de.c, func_8043DEF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Menu_SetPakWatch(void *arg0, s32 arg1);
extern void Controller_WatchPak(s32 arg);
extern void Controller_UnwatchPak(sptr arg);

void Menu_SetPakWatch(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        Controller_WatchPak(val);
    } else {
        Controller_UnwatchPak(val);
    }
}

void Mp_SetupMatchRules(f32 minutes, s32 a1, s32 started, s32 a3) {
    GameMatch *m = &gEngineApp_m_Settings.match;

    m->tag = started;
    m->teams = a3;
    m->scoreLimit = a1;
    m->over = 0;
    m->teamScore[0] = 0;
    m->teamScore[1] = 0;
    m->teamOver = 0;
    m->done = 0;
    m->timeLeft = minutes * 900.0f;
    if (m->timeLeft == 0.0f) {
        m->timeLeft = -1.0f;
    }
}

typedef struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (Rage Wars 0x150) */
} StatusView;
typedef struct StatusStep {
    char pad[0x40];
} StatusStep;

void Mp_JoinPressingController(void) {
    StatusStep *status;
    CPlayerOptions *settings;
    s32 i;
    s32 active;
    i = 3;
    status = ((StatusStep *) &gEngineApp_m_Settings.slots[1]);
    for (; i >= 0; i--) {
        ((StatusView *)status)->active = 0;
        status--;
    }
    i = 0;
    active = 1;
    settings = gEngineApp_m_Settings.slots;
    for (; i < 4; i++) {
        if (Controller_ConfirmPressed(&gControllers[i]) != 0) {
            settings[i].m_bActive = active;
            settings[i].m_nController = i;
            break;
        }
    }
}
ControllerProfile *Menu_FindPressingController(s32 *out);

ControllerProfile *Menu_FindPressingController(s32 *out) {
    s32 i;
    ControllerProfile *s;
    i = 0;
    s = gControllers;
    while (i < 4) {
        if (Controller_ConfirmPressed(s) != 0) {
            *out = i;
            return s;
        }
        i++;
        s++;
    }
    *out = -1;
    return 0;
}

/* Counts the connected controllers; 0 if one of their players is not ready. */
s32 Mp_CountJoinedControllers(void) {
    u8 *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (u8 *) &gEngineApp_m_Settings;
    var_v1 = 0;
loop_1:
    if (((ControllerProfile *) ((u8 *) gControllers + var_v1))->connected != 0) {
        var_a2 += 1;
        if (var_a0[0xA8] == 0) {
            return 0;
        }
    }
    var_a0 = var_a0 + 0x40;
    var_a1 += 1;
    var_v1 += sizeof(ControllerProfile);
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}

#include "m2c_macros.h"
#include "globals/menu.h"

void FinalScoreMenu_Init(void) {
    sFinalScoreTimer = 0;
}

#include "globals/engine.h"

typedef struct Shared_PlayerSettingsRecord Shared_PlayerSettingsRecord;
struct Shared_PlayerSettingsRecord {
    char pad0[4];
    s16 score;
    char pad6[0x4C - 6];
    u8 weapons[0x16];
    char pad62[0x78 - 0x62];
    u8 enabled;
    char pad79[6];
    u8 slot;
    s8 character;
    u8 variant;
    char pad82[2];
    u8 name[13];
    u8 locked;
    char pad92[3];
    u8 active;
};
typedef struct RenderCounters {
    f32 ticks, seconds, minutes, hours;
    f32 countdown;
    f32 timeLimit;
    s32 mode;
    s32 elapsed;
    s32 state;
    s32 teams;
    s32 round;
    s32 scores[5];
    s32 counts[5];
    s32 option54;
    s32 option58;
    s32 unk5C;
    s32 unk60;
    s32 setupState;
    s32 restrictionActive;
    s32 mask;
    s32 teamColour;
    s32 unk74;
    s32 option78;
    s32 unk7C;
    s32 counter80;
    s32 unk84;
    s32 first, second;
    s32 kills;
    s32 unk94;
    s32 enabled;
    s32 unk9C;
    s32 timer;
    s32 cooldown;
    s32 humanWon;
    s32 transitionActive;
} RenderCounters;
typedef struct Shared_GameSlot Shared_GameSlot;
struct Shared_GameSlot {
    u8 enabled;
    u8 unknown01[6];
    u8 slot;
    s8 kind;
    u8 unknown09[0x10];
    u8 controller;
    u8 unknown1A[2];
    u8 role;
    u8 escort;
    u8 secondListMode;
    u8 unknown1F[0x77];
};
typedef struct Shared_Game Shared_Game;
struct Shared_Game {
    u32 flags;
    u8 unknown04[4];
    u32 sceneMask;
    u8 unknown0C;
    u8 controllerMode;
    u8 unknown0E[2];
    s32 buttons;
    u16 packedOptions;
    u8 hudFade;
    u8 unknown17;
    u8 volume[3];
    u8 firstListMode;
    u8 unknown1C;
    u8 local;
    u8 mode1E;
    u8 unknown1F;
    union {
        struct {
            f32 speedFactor;
            s8 laps;
            s8 players;
            s8 trialOther;
            s8 trialScore;
            u8 unknown28[0x120];
            Shared_GameSlot slots[8];
            u8 unknown5F8[0x34];
            s32 unk62C;
            u8 unknown630[0x40];
            s32 split;
            u32 unknown674[3];
            s32 humanWon;
            s32 threePlayerLayout;
        };
        struct {
            u8 reserved20[0xB0];
            Shared_PlayerSettingsRecord playerSettings[8];
            u8 reserved580;
            u8 language;
            u8 reserved582[0x32];
            s32 state;
            u8 reserved5B8[0x20];
            RenderCounters counters;
        };
    };
    u8 reserved688[0xCAC - 0x688];
};
s32 FinalScoreMenu_Poll(void);
void CPlayerPool__EndMatch(void *arg0);

s32 FinalScoreMenu_Poll(void) {
    f32 time;
    RenderCounters *base;
    time = sFinalScoreTimer + gFrameIncrement;
    sFinalScoreTimer = time;
    if ((120.0f) <= time) {
        base = (RenderCounters *) &gEngineApp_m_Settings.match;
        base->teams = 0;
        CPlayerPool__EndMatch((void *) ((char *) base - 0x1440));
        return 1;
    }
    return 0;
}

/* the blue team's score read as a scalar: globals/game.h's array (or the member path) changes the ROM's code here */
extern s32 gMatchBlueTeamScore __asm__("gEngineApp_m_Settings_match_teamScore");
extern s32 MenuItem_TextLength(MenuItem *a);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);

/* Prints gEngineApp_m_Settings_match_teamScore over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel(MenuItem *a) {
    a->text = sFinalScoreMenuText_BlueBloodScore;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), sFinalScoreMenuFmt_04d, gMatchBlueTeamScore);
    return 0;
}

/* Prints gEngineApp_m_Settings.match.teamScore[1] over the last four characters of the label. */
s32 FinalScoreMenu_RedScoreLabel(MenuItem *a) {
    a->text = sFinalScoreMenuText_RedBloodScore;
    sprintf(a->text + (MenuItem_TextLength(a) - 4), sFinalScoreMenuFmt_04d, gEngineApp_m_Settings.match.teamScore[1]);
    return 0;
}

s32 FinalScoreMenu_PleaseWaitSelect(void) {
    return 0;
}

s32 RumbleMenu_Poll(void)
{
  s32 *new_var;
  s32 var_a0;
  var_a0 = 0;
  new_var = &gEngineApp_m_Settings.match;
  if ((((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x28))) != 0) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) new_var) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}

#include "protos/game.h"
#include "structs/player.h"

void Controller_UnwatchPak(sptr);
void Controller_ResetRumble(ControllerProfile *);

/* The select callback of the rumble notice's last item (a MenuItemDesc select: the item, its
 * window): stop watching the pak, reset and enable the rumble, and clear the player's
 * m_bEnteringDeathmatch. Round 22 de-faked it: Controller_ResetRumble returns void (its
 * definition, func_00285AE0.c); declared s32, the draft needed an if/else with two equal arms
 * to keep the player pointer out of $v1. The N64 stores through the player unconditionally;
 * the port (round 13 b5) keeps a NULL test. */
s32 RumbleMenu_Play(s32 arg0, void *arg1) {
    Menu *menu = (Menu *) arg1;
    gEngineApp_m_Settings.match.tagPhase = 0;
    Controller_UnwatchPak((sptr) menu->controller->port);
    Controller_ResetRumble(menu->controller);
    Controller_SetRumbleOption(menu->controller, 1);
#ifdef TARGET_PORT
    if (menu->player != NULL)
#endif
        menu->player->m_bEnteringDeathmatch = 0;
    return 1;
}

s32 PlayerSetupMenu_Poll(void)
{
  s8 *new_var;
  s32 var_a0;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  var_a0 = 0;
  if ((((*((s32 *) (new_var + 0x28))) != 0) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x1C))) != 0)) || ((*((s32 *) (new_var + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}

#include "protos/menu.h"

s32 PlayerSetupMenu_StepHandicap(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = (CPlayerOptions *) ((CPlayer *) arg1->player)->m_pOptions;
    temp_s0->m_Handicap = Menu_StepValue(arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}

/* Label of the handicap item, with the handicap value printed into it. */
s32 PlayerSetupMenu_HandicapLabel(MenuItem *a, Menu *c) {
    s32 handicap = c->player->m_pOptions->m_Handicap;

    if (handicap == 0) {
        a->text = sPlayerSetupMenuText_HandicapNone;
    } else if (handicap == 10) {
        a->text = sPlayerSetupMenuText_HandicapMax;
    } else {
        switch (c->controller->port) {
        case 0:
        default:
            a->text = sPlayerSetupMenuText_Handicap_B41B0;
            break;
        case 1:
            a->text = sPlayerSetupMenuText_Handicap_B41C0;
            break;
        case 2:
            a->text = sPlayerSetupMenuText_Handicap_B41D0;
            break;
        case 3:
            a->text = sPlayerSetupMenuText_Handicap_B41E0;
            break;
        }
        sprintf(a->text + (MenuItem_TextLength(a) - 2), MPMENU_FMT_02D, handicap);
    }
    return 0;
}

s32 PlayerSetupMenu_StepControlType(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_ControlType = Menu_StepValue(arg1, (s32) temp_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}

#include "globals/menu_desc.h"

s32 PlayerSetupMenu_EditName(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gNameEntryMenuDesc);
    return 1;
}

/* The update callback of the player setup window's name line: it shows the name in the settings
 * slot of the window's controller. Round 23: through the members (the m2c body wrote the text at
 * MenuItem +0x14 and read Menu +0x20, the N64 offsets: on the host the text is at 0x18 and
 * Menu.controller at 0x30). */
s32 PlayerSetupMenu_NameLabel(MenuItem *item, Menu *menu) {
    item->text = (char *) gEngineApp_m_Settings.slots[menu->controller->port].m_Name;
    return 0;
}

s32 PlayerSetupMenu_Play(void *first, void *second, void *third) {
    MenuPool_OpenChildMenu(third, second, &gRumbleNoticeMenuDesc);
    return 1;
}

s32 PlayerSetupMenu_ControlTypeLabel(MenuItem *a, Menu *m) {
    switch (m->player->m_pOptions->m_ControlType) {
    case 0:
        a->text = (char *)sPlayerSetupMenuText_ControlExpert;
        break;
    case 1:
        a->text = (char *)sPlayerSetupMenuText_ControlArcade;
        break;
    }
    return 0;
}

void CharacterMenu_Init(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}

#include "globals/pak.h"

void CharacterMenu_InitAfterPak(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}

s32 CharacterMenu_Poll(void)
{
  s8 *new_var;
  s32 var_a0;
  var_a0 = 0;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  if ((((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + ((0x28 & 0xFFFFFFFF) & 0xFFFFFFFF)))) != 0) || ((*((s32 *) (new_var + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}

s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Pak_ClearDamagedContinued();                            /* extern */

s32 CharacterMenu_LoadSelect(s32 arg0, Menu *arg1) {
    sptr temp_s1;

    temp_s1 = arg1->player;
    Pak_ClearDamagedContinued();
    Sound__StopAllExcept(-1);
    MenuPool_OpenMenu(&gEngineApp_m_Game_m_MenuPool, &gPakLoadProfileMenuDesc, temp_s1, arg1->controller, 0);
    return 1;
}

s32 CharacterMenu_LoadUpdate(void) {
    return 0;
}

extern s32 Menu_StepValueLR(Menu *c, s32 value, s32 step, s32 min, s32 max, s32 wrap);
extern void CPlayer__SetCharacter(CPlayer *player, s32 team);
extern void MenuItem_SetModel(sptr menu, s32 id, s32 arg);
extern void Sound__Play2D(s32 sound);

/* Team selection item: left/right cycles the team, A confirms. */
s32 CharacterMenu_SelectCharacter(s32 menu, Menu *c, s32 pool) {
    CPlayer *player = c->player;
    CPlayerOptions *options = player->m_pOptions;

    if (Controller_ConfirmPressed(c->controller)) {
        CPlayer__SetCharacter(player, options->m_nCharacter);
        Sound__Play2D(gCharacterSelectSounds[player->m_Data.m_nDeathmatchCharacter].sound);
        MenuPool_OpenChildMenu(pool, c, &gPlayerSetupMenuDesc);
        return 1;
    } else {
        s8 team = Menu_StepValueLR(c, options->m_nCharacter, 1, 0, 10, 1);

        options->m_nCharacter = team;
        CPlayer__SetCharacter(player, team);
        MenuItem_SetModel(menu, 0xB, options->m_nCharacter);
        return 0;
    }
}

s32 CharacterMenu_ModelUpdate(void) {
    return 0;
}

s32 TeamMenu_Poll(void)
{
  s32 var_a0;
  s8 *new_var;
  var_a0 = 0;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  if ((((*((s32 *) (new_var + 0x28))) != 0) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}

#include "structs/camera.h"

s32 TeamMenu_ChooseBlue(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 0;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu((sptr)temp_a2->m_pCamera->m_MenuPool, &gCharacterMenuDesc, (sptr) temp_a2, (sptr) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

s32 TeamMenu_ChooseRed(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 1;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu(temp_a2->m_pCamera->m_MenuPool, &gCharacterMenuDesc, (sptr) temp_a2, temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* game options block; Turok 2 keeps this option as a signed byte at +0x25 */
s32 FragTagMenu_StepTimeLimit(void *first, void *second) {
    GameSettings *opts = &gEngineApp_m_Settings;
    opts->tagTimeLimit = Menu_StepValue(second, opts->tagTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}

/* Prints the time limit into the label, or "none". */
s32 FragTagMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->tagTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->tagTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 FragTagMenu_StepTagLimit(void *first, void *second) {
    GameSettings *options = &gEngineApp_m_Settings;
    options->tagScoreLimit = Menu_StepValue(second, options->tagScoreLimit, 1, 0, 0x63, 1);
    return 0;
}

/* Prints the second limit (gEngineApp_m_Settings+0x26) into the label, or "none". */
s32 FragTagMenu_TagLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->tagScoreLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->tagScoreLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: the option lives at +0x27 of this options block (s8). */
s32 FragTagMenu_StepLevel(void *first, void *second) {
    GameSettings *options = &gEngineApp_m_Settings;
    options->tagArena = Menu_StepValue(second, options->tagArena, 1, 0, 6, 1);
    return 0;
}

/* Label of the level item. */
s32 FragTagMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagArena) {
    case 0:
        a->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        a->text = sFragTagMenuText_LevelMadhouse;
        break;
    case 2:
        a->text = sFragTagMenuText_LevelSnap;
        break;
    case 3:
        a->text = sFragTagMenuText_LevelSneaksNLadders;
        break;
    case 4:
        a->text = sFragTagMenuText_LevelPrimattica;
        break;
    case 5:
        a->text = sFragTagMenuText_LevelCrossfire;
        break;
    case 6:
        a->text = sFragTagMenuText_LevelYNGWTSF;
        break;
    }
    return 0;
}

s32 FragTagMenu_StepTextureSet(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagTextureSet = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.tagTextureSet, 1, 0, 8, 1);
    return 0;
}

s32 FragTagMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagTextureSet) {
    case 0:
        a->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        a->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        a->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        a->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        a->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        a->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        a->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        a->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        a->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

s32 FragTagMenu_StepCrosshairs(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagCrosshairs = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.tagCrosshairs, 1, 0, 1, 1);
    return 0;
}

s32 FragTagMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagCrosshairs) {
    case 0:
        a->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        a->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

#include "globals/menu_data.h"

extern s32 Controller_MenuRight(sptr);

/* Steps the selection (twice when the controller reports busy) and sets the scale
 * from the table plus one, clamped to [0.5, 2.0]. */
s32 MpMenu_StepPlayerSpeed(void *arg0, Menu *item) {
    f32 scale;

    gMpPlayerSpeed = Menu_StepValue(item, gMpPlayerSpeed, 1, 0, 15, 0);
    if (Controller_MenuRight(item->controller) != 0 || Controller_ConfirmPressed((ControllerProfile *) item->controller) != 0) {
        Menu_StepValue(item, gMpPlayerSpeed, 1, 0, 15, 0);
    }
    scale = sPlayerSpeedTable.scale[gMpPlayerSpeed] + 1.0f;
    if (!(scale < 0.5f)) {
        if (!(scale > 2.0f)) {
            if (scale < 0.5f) {
                scale = 0.5f;
            }
        } else {
            scale = 2.0f;
        }
    } else {
        scale = 0.5f;
    }
    gEngineApp_m_Settings.speed = scale;
    return 0;
}

s32 MpMenu_PlayerSpeedLabel(MenuItem *a) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        a->text = sMpMenuText_PlayerSpeedDefault;
    } else {
        a->text = sMpMenuText_PlayerSpeed;
        sprintf(a->text + (MenuItem_TextLength(a) - 3), sMpMenuFmt_3d, gMpPlayerSpeed - 5);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Options block; Turok 2 keeps this option as a signed byte at 0x20. */
s32 BloodLustMenu_StepTimeLimit(void *first, void *second) {
    GameSettings *opts = &gEngineApp_m_Settings;
    opts->dmTimeLimit = Menu_StepValue(second, opts->dmTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}

/* Prints the setting at gEngineApp_m_Settings+0x20 into the label, or "none". */
s32 BloodLustMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->dmTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->dmTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 BloodLustMenu_StepFragLimit(void *first, void *second) {
    GameSettings *options = &gEngineApp_m_Settings;
    options->dmScoreLimit = Menu_StepValue(second, options->dmScoreLimit, 1, 0, 0x63, 1);
    return 0;
}

/* Prints the setting at gEngineApp_m_Settings+0x21 into the label, or "none". */
s32 BloodLustMenu_FragLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->dmScoreLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->dmScoreLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

s32 BloodLustMenu_StepLevel(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmArena = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.dmArena, 1, 0, 0xD, 1);
    return 0;
}

s32 BloodLustMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmArena) {
    case 0:
        a->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        a->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        a->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        a->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        a->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        a->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        a->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        a->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        a->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        a->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        a->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        a->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        a->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        a->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}

s32 BloodLustMenu_StepTextureSet(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmTextureSet = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.dmTextureSet, 1, 0, 8, 1);
    return 0;
}

s32 BloodLustMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmTextureSet) {
    case 0:
        a->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        a->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        a->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        a->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        a->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        a->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        a->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        a->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        a->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

/* The multiplayer settings, gEngineApp_m_Settings (GameSettings in PlayerSetupMenu_HandicapLabel_2; 0x184 bytes, see Demo_End). */


s32 BloodLustMenu_StepCrosshairs(sptr arg0, sptr arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->dmCrosshairs = Menu_StepValue(arg1, settings->dmCrosshairs, 1, 0, 1, 1);
    return 0;
}

s32 BloodLustMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmCrosshairs) {
    case 0:
        a->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        a->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

s32 TeamBloodMenu_StepTimeLimit(void *first, void *second) {
    GameSettings *opts = &gEngineApp_m_Settings;
    opts->teamTimeLimit = Menu_StepValue(second, opts->teamTimeLimit, 5, 0, 30, 1);
    return 0;
}

/* Prints the setting at gEngineApp_m_Settings+0x2A into the label, or "none". */
s32 TeamBloodMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->teamTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->teamTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_804446D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
s32 TeamBloodMenu_StepFragLimit(void *first, void *second) {
    GameSettings *game = &gEngineApp_m_Settings;
    game->teamScoreLimit = Menu_StepValue(second, game->teamScoreLimit, 1, 0, 99, 1);
    return 0;
}

/* Prints the setting at gEngineApp_m_Settings+0x2B into the label, or "none". */
s32 TeamBloodMenu_FragLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->teamScoreLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelFmt_02d, setup->teamScoreLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sLimitLabelText_None);
    }
    return 0;
}

s32 TeamBloodMenu_StepLevel(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamArena = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.teamArena, 1, 0, 0xD, 1);
    return 0;
}

s32 TeamBloodMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.teamArena) {
    case 0:
        a->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        a->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        a->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        a->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        a->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        a->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        a->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        a->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        a->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        a->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        a->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        a->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        a->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        a->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}

s32 TeamBloodMenu_StepTextureSet(sptr arg0, sptr arg1)
{
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings);
  *((s8 *) (((s8 *) (&gEngineApp_m_Settings)) + 0x2D)) = Menu_StepValue(arg1, (s32) (*((s8 *) (new_var + 0x2D))), 1, 0, 8, 1);
  return 0;
}

s32 TeamBloodMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.teamTextureSet) {
    case 0:
        a->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        a->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        a->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        a->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        a->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        a->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        a->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        a->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        a->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

s32 TeamBloodMenu_StepCrosshairs(sptr arg0, sptr arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamCrosshairs = Menu_StepValue(arg1, (s32) gEngineApp_m_Settings.teamCrosshairs, 1, 0, 1, 1);
    return 0;
}

s32 TeamBloodMenu_CrosshairsLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.teamCrosshairs) {
    case 0:
        a->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        a->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8.CPP, func_8043E830_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Menu callback: toggle the option selected by arg0[0]. */
s32 WeaponsMenu_Toggle(s32 *arg0) {
    GameSettings *base;
    switch (arg0[0]) {
        case 2:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[1] ^= 1;
            break;
        case 3:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[2] ^= 1;
            break;
        case 4:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[3] ^= 1;
            break;
        case 5:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[4] ^= 1;
            break;
        case 6:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[5] ^= 1;
            break;
        case 7:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[6] ^= 1;
            break;
        case 8:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[7] ^= 1;
            break;
        case 9:
            base = &gEngineApp_m_Settings;
            base->weaponEnabled[9] ^= 1;
            break;
    }
    return 0;
}

s32 DeathmatchMenu_OpenBloodLust(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gBloodLustMenuDesc);
    return 1;
}

s32 DeathmatchMenu_OpenFragTag(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gFragTagMenuDesc);
    return 1;
}

s32 DeathmatchMenu_OpenTeamBlood(sptr arg0, sptr arg1, sptr arg2)
{
  GameMatch *match;
  match = &gEngineApp_m_Settings.match;
  match->teamScore[0] = 0;
  match->teamScore[1] = 0;
  match->teamOver = 0;
  MenuPool_OpenChildMenu(arg2, arg1, &gTeamBloodMenuDesc);
  return 1;
}

s32 DeathmatchMenu_OpenWeapons(sptr arg0, sptr arg1, sptr arg2) {
    MenuPool_OpenChildMenu(arg2, arg1, &gWeaponsMenuDesc);
    return 1;
}
