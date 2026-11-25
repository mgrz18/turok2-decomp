/* Second-module copy of MpMenu_StartFragTag (build/manual/W/MpMenu_StartFragTag.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"
#include "globals/game.h"
/* Second-module copy of MpMenu_StartBloodLust (build/manual/W/MpMenu_StartBloodLust.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
/* Second-module copy of MpMenu_StartTeamBlood (build/manual/W/MpMenu_StartTeamBlood.c), with its first-module addresses 0xE0 lower. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MpMenu/func_00417910.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8043D54C_4.CPP, func_8043DE50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* The second VM module's copy of src/us/vm1/MpMenu/func_00417BB0.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/vector.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/player_options.h"
#include "structs/list.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "common.h"
#include "m2c_macros.h"
#include "globals/menu.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E1C0.c, func_0043E1C0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
/* The second VM module's copy of src/us/vm1/MpMenu/func_00417DD4.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
/* The second VM module's copy of src/us/vm1/MpMenu/func_00417E28.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "protos/game.h"
#include "structs/player.h"
#include "globals/menu_desc.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E520.c, func_0043E520.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/pak.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043E65C.c, func_0043E65C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/camera.h"
#include "globals/menu_data.h"

s32 Controller_ConfirmPressed(ControllerProfile *pad);
extern s32 Random_Next(void);
extern void CEngineApp__StartGame_2(void *a, s32 level);

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

s32 MpMenu_StartFragTag_2(void) {
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
    CEngineApp__StartGame_2(&gEngineApp, arg);
    return 1;
}

/* Mp_SetupMatchRules, inlined; here written on the global (the copies in MpMenu_StartFragTag_2/MpMenu_StartTeamBlood_2 go
 * through a MatchState pointer, which this one does not match with). */
static __inline__ void StartMatch_45060C(f32 minutes, s32 a1, s32 started, s32 a3) {
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

s32 MpMenu_StartBloodLust_2(void) {
    GameSettings *setup = &gEngineApp_m_Settings;
    s32 level;
    s32 arg;
    s32 i;
    ControllerProfile *ctl;

    level = setup->dmArena;
    StartMatch_45060C(setup->dmTimeLimit, setup->dmScoreLimit, 0, 0);
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
    CEngineApp__StartGame_2(&gEngineApp, arg);
    return 1;
}

s32 MpMenu_StartTeamBlood_2(void) {
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
    CEngineApp__StartGame_2(&gEngineApp, arg);
    return 1;
}

/* Label of a weapon toggle in the weapons menu: "<weapon> on/off". */
s32 WeaponsMenu_ToggleLabel_2(MenuItem *a) {
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
struct SharedRecAt148;
typedef struct SharedRecAt148 SharedRecAt148;
struct Shared_func_8043DEDC_S3;
typedef struct Shared_func_8043DEDC_S3 Shared_func_8043DEDC_S3;
void Menu_ClaimPressingController_2(func_80250BD4_S1 *arg0);
struct SharedRecAt148 {
    char pad0[0xA8];
    s8 unkA8; /* Turok 2: 0x40-byte records */
};
struct Shared_func_8043DEDC_S3 {
    char pad0[0x28];
    s8 unk28;
    char pad29[0x6];
    s8 unk2F;
};
void Music_RestoreVolume(void);

void Menu_ClaimPressingController_2(func_80250BD4_S1 *arg0) {
    Shared_func_8043DEDC_S3 *settings;
    s8 *status;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    i = 3;
    status = (s8 *)(struct SharedRecAt148 *) &gEngineApp_m_Settings.slots[1];
    for (; i >= 0; i--) {
        ((SharedRecAt148 *)status)->unkA8 = 0;
        status -= 0x40;
    }
    i = 0;
    settings = (Shared_func_8043DEDC_S3 *)&gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++, profile++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->unk28 = on;
            settings->unk2F = i;
            arg0->unk20 = profile;
            break;
        }
    }
    Music_RestoreVolume();
}

#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif

struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
typedef struct PakMenuSlot PakMenuSlot;
struct OwnerCode8043DF84;
struct OwnerCode8043DF84 {
    char pad0[0x85C];
    s32 unk85C;
};
struct MenuRecAt20V2;
typedef struct MenuRecAt20V2 MenuRecAt20V2;
struct MenuRecAt20V2 {
    char pad0[0x1C];
    struct OwnerCode8043DF84 *owner;
    PakMenuSlot *widget;
};
void Menu_SetPakWatch_2(void *arg0, s32 arg1);
void Controller_WatchPak(s32 on);
extern void Controller_UnwatchPak(sptr arg);

void Menu_SetPakWatch_2(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        Controller_WatchPak(val);
    } else {
        Controller_UnwatchPak(val);
    }
}

void Mp_SetupMatchRules_2(f32 minutes, s32 a1, s32 started, s32 a3) {
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

struct StatusView;
struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (0x150) */
};
typedef struct StatusView StatusView;
struct StatusStep;
typedef struct StatusStep StatusStep;
struct StatusStep {
    char pad[0x40];
};
void Mp_JoinPressingController_2(void);

void Mp_JoinPressingController_2(void) {
    StatusStep *status;
    CPlayerOptions *settings;
    s32 i;
    s32 active;
    i = 3; /* 4 entries in Turok 2 (8 in Rage Wars) */
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
ControllerProfile *Menu_FindPressingController_2(s32 *out);

ControllerProfile *Menu_FindPressingController_2(s32 *out) {
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

/* The game options seen from one player: each player has a 0x40-byte block, so the
 * view moves 0x40 bytes per player and its ready flag sits at 0xA8. */
typedef struct {
    /* 0x00 */ u8 pad0[0xA8];
    /* 0xA8 */ u8 ready;
} PlayerOptionsView;

/* Number of active players, or 0 if any of them is not ready. */
s32 Mp_CountJoinedControllers_2(void) {
    PlayerOptionsView *var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_a2 = 0;
    var_a1 = 0;
    var_a0 = (PlayerOptionsView *) &gEngineApp_m_Settings;
    var_v1 = 0;
loop_1:
    if (((ControllerProfile *) ((u8 *) gControllers + var_v1))->connected != 0) {
        var_a2 += 1;
        if (var_a0->ready == 0) {
            return 0;
        }
    }
    var_a0 = (PlayerOptionsView *) ((u8 *) var_a0 + 0x40);
    var_a1 += 1;
    var_v1 += sizeof(ControllerProfile);
    if (var_a1 >= 4) {
        return var_a2;
    }
    goto loop_1;
}

void FinalScoreMenu_Init_2(void) {
    sFinalScoreTimer = 0;
}

/* gEngineApp_m_Settings_match (GameMatch in structs/game.h; unk24 is teams). Kept as a local
 * array view: through it the compiler bases the store and the argument on
 * gEngineApp_m_Settings_match + 0x24, as the ROM does; through the struct it bases them on
 * gEngineApp_m_Settings_match. */
typedef struct S10Game {
               u8 pad0[0x14];
               f32 unk14;
               s32 unk18;
               s32 unk1C;
               s32 unk20;
               s32 unk24;
               s32 unk28;
               s32 unk2C[5];
               s32 unk40[5];
               s32 unk54;
               u8 pad58[0x6C - 0x58];
               s32 unk6C;
               u8 pad70[0x78 - 0x70];
               s32 unk78;
               u8 pad7C[4];
               s32 unk80;
} S10Game;
extern S10Game gMatchView[] __asm__("gEngineApp_m_Settings_match"); /* the array view (GameMatch in globals/game.h) keeps the ROM's code: the stores base on the symbol */
extern void CPlayerPool__EndMatch(void *p);

s32 FinalScoreMenu_Poll_2(void) {
    sFinalScoreTimer += gFrameIncrement;
    if (sFinalScoreTimer >= 120.0f) {
        gMatchView->unk24 = 0;
        CPlayerPool__EndMatch((u8 *)gMatchView - 0x1440);
        return 1;
    }
    return 0;
}

extern s32 MenuItem_TextLength_2(MenuItem *a);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, char *fmt, ...);

/* Prints gEngineApp_m_Settings_match_teamScore over the last four characters of the label. */
s32 FinalScoreMenu_BlueScoreLabel_2(MenuItem *a) {
    a->text = sFinalScoreMenuText_BlueBloodScore;
    sprintf(a->text + (MenuItem_TextLength_2(a) - 4), "%04d", gEngineApp_m_Settings_match_teamScore[0]);
    return 0;
}

/* Prints gEngineApp_m_Settings.match.teamScore[1] over the last four characters of the label. */
s32 FinalScoreMenu_RedScoreLabel_2(MenuItem *a) {
    a->text = sFinalScoreMenuText_RedBloodScore;
    sprintf(a->text + (MenuItem_TextLength_2(a) - 4), sFinalScoreMenuFmt_04d, gEngineApp_m_Settings.match.teamScore[1]);
    return 0;
}

s32 FinalScoreMenu_PleaseWaitSelect_2(void) {
    return 0;
}

s32 RumbleMenu_Poll_2(void) {
    return gEngineApp_m_Settings.match.over;
}

void Controller_UnwatchPak(sptr);
s32 Controller_ResetRumble(sptr);
#ifdef TARGET_PORT
/* The port (round 13 b5): the select callback of the rumble notice's last item (a MenuItemDesc
 * select: the item, its window). The m2c body read the window's player and controller at their N64
 * offsets (0x1C, 0x20; host 0x30, 0x38: it took the descriptor for the controller and wrote 0
 * through a pointer made of padding); the host body reads Menu's members. 0xBF8 is
 * CPlayer.m_bEnteringDeathmatch. (n64viewcheck: the a5 sweep's unresolved RumbleMenu_Play.) */
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/controller.h"
s32 RumbleMenu_Play_2(s32 arg0, void *arg1) {
    Menu *menu = (Menu *) arg1;
    gEngineApp_m_Settings.match.tagPhase = 0;
    Controller_UnwatchPak((sptr) menu->controller->port);
    Controller_ResetRumble((sptr) menu->controller);
    Controller_SetRumbleOption(menu->controller, 1);
    if (menu->player != NULL) {
        menu->player->m_bEnteringDeathmatch = 0;
    }
    return 1;
}
#else
s32 RumbleMenu_Play_2(s32 arg0, void *arg1)
{
  void *new_var;
  gEngineApp_m_Settings.match.tagPhase = 0;
  Controller_UnwatchPak((s32) (*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))));
  Controller_ResetRumble((sptr) (*((void **) (((s8 *) arg1) + 0x20))));
 do { Controller_SetRumbleOption((sptr) (*((void **) (((s8 *) arg1) + 0x20))), 1); new_var = *((void **) (((s8 *) arg1) + 0x1C)); } while (0);
  *((s32 *) (((s8 *) new_var) + 0xBF8)) = 0;
  return 1;
}
#endif

s32 PlayerSetupMenu_Poll_2(void) {
    return gEngineApp_m_Settings.match.over;
}

s32 Menu_StepValue_2(struct Menu *, s32, s32, s32, s32, s32);

s32 PlayerSetupMenu_StepHandicap_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_s0;

    temp_s0 = ((CPlayer *) arg1->player)->m_pOptions;
    temp_s0->m_Handicap = Menu_StepValue_2((sptr) arg1, (s32) temp_s0->m_Handicap, 1, 0, 0xA, 0);
    return 0;
}

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */

/* Shows the player's handicap: two special values, otherwise a per-port line with the number. */
s32 PlayerSetupMenu_HandicapLabel_2(MenuItem *item, Menu *ctx) {
    s32 handicap = ctx->player->m_pOptions->m_Handicap;

    if (handicap == 0) {
        item->text = sPlayerSetupMenuText_HandicapNone;
    } else if (handicap == 10) {
        item->text = sPlayerSetupMenuText_HandicapMax;
    } else {
        switch (ctx->controller->port) {
        case 0:
        default:
            item->text = sPlayerSetupMenuText_Handicap_B41B0;
            break;
        case 1:
            item->text = sPlayerSetupMenuText_Handicap_B41C0;
            break;
        case 2:
            item->text = sPlayerSetupMenuText_Handicap_B41D0;
            break;
        case 3:
            item->text = sPlayerSetupMenuText_Handicap_B41E0;
            break;
        }
        sprintf(&item->text[MenuItem_TextLength_2(item) - 2], "%02d", handicap);
    }
    return 0;
}

s32 PlayerSetupMenu_StepControlType_2(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_s0;

    temp_s0 = arg1->player->m_pOptions;
    temp_s0->m_ControlType = Menu_StepValue_2((sptr) arg1, (s32) temp_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}

sptr MenuPool_OpenChildMenu_2(struct MenuPool *, struct Menu *, struct MenuDesc *);

s32 PlayerSetupMenu_EditName_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gNameEntryMenuDesc_2);
    return 1;
}

/* The module-2 copy of PlayerSetupMenu_NameLabel (func_00418108.c): the name line of the player
 * setup window shows the name in the settings slot of the window's controller. Round 23: through
 * the members (the m2c body used the N64 offsets MenuItem +0x14 and Menu +0x20). */
s32 PlayerSetupMenu_NameLabel_2(MenuItem *item, Menu *menu) {
    item->text = (char *) gEngineApp_m_Settings.slots[menu->controller->port].m_Name;
    return 0;
}

s32 PlayerSetupMenu_Play_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gRumbleNoticeMenuDesc_2);
    return 1;
}

s32 PlayerSetupMenu_ControlTypeLabel_2(MenuItem *a, Menu *m) {
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

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void CharacterMenu_Init_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
}

/* A menu window callback: the window (structs/menu.h) and the controller it reads. */

void CharacterMenu_InitAfterPak_2(Menu *arg0) {
    Controller_WatchPak(arg0->controller->port);
    gDamagedPakContinued = 0;
}

s32 CharacterMenu_Poll_2(void) {
    return gEngineApp_m_Settings.match.over;
}

sptr MenuPool_OpenMenu_2(struct MenuPool *, struct MenuDesc *, sptr, sptr, sptr);
s32 Sound__StopAllExcept(s32);                     /* extern */
s32 Pak_ClearDamagedContinued_2();                            /* extern */

s32 CharacterMenu_LoadSelect_2(s32 arg0, Menu *arg1) {
    sptr temp_s1;

    temp_s1 = arg1->player;
    Pak_ClearDamagedContinued_2();
    Sound__StopAllExcept(-1);
    MenuPool_OpenMenu_2((sptr) &gEngineApp_m_Game_m_MenuPool, (sptr) &gPakLoadProfileMenuDesc_2, temp_s1, arg1->controller, 0);
    return 1;
}

s32 CharacterMenu_LoadUpdate_2(void) {
    return 0;
}

extern s32 Menu_StepValueLR_2(Menu *a, s32 cur, s32 step, s32 lo, s32 hi, s32 wrap);
extern void CPlayer__SetCharacter_2(CPlayer *player, s32 v);
extern void MenuItem_SetModel_2(void *a, s32 b, s32 c);
extern void Sound__Play2D(s32 a);

s32 CharacterMenu_SelectCharacter_2(void *x, Menu *m, void *y) {
    CPlayer *player = m->player;
    CPlayerOptions *options = player->m_pOptions;
    if (Controller_ConfirmPressed(m->controller)) {
        CPlayer__SetCharacter_2(player, options->m_nCharacter);
        Sound__Play2D(gCharacterSelectSounds[player->m_Data.m_nDeathmatchCharacter].sound);
        MenuPool_OpenChildMenu_2((sptr)y, (sptr)m, &gPlayerSetupMenuDesc_2);
        return 1;
    } else {
        s8 v = Menu_StepValueLR_2(m, options->m_nCharacter, 1, 0, 0xA, 1);
        options->m_nCharacter = v;
        CPlayer__SetCharacter_2(player, v);
        MenuItem_SetModel_2(x, 0xB, options->m_nCharacter);
        return 0;
    }
}

s32 CharacterMenu_ModelUpdate_2(void) {
    return 0;
}

s32 TeamMenu_Poll_2(void) {
    return gEngineApp_m_Settings.match.over;
}

s32 TeamMenu_ChooseBlue_2(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 0;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu_2((sptr) temp_a2->m_pCamera->m_MenuPool, (sptr) &gCharacterMenuDesc_2, (sptr) temp_a2, (sptr) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

s32 TeamMenu_ChooseRed_2(s32 arg0, Menu *arg1) {
    CPlayer *temp_a2;

    temp_a2 = arg1->player;
    temp_a2->m_nDeathmatchTeamSelected = 1;
    temp_a2->m_pOptions->m_nCharacter = 0;
    MenuPool_OpenMenu_2((sptr) temp_a2->m_pCamera->m_MenuPool, (sptr) &gCharacterMenuDesc_2, (sptr) temp_a2, (sptr) temp_a2->m_ContState.m_pContState, temp_a2->m_nController);
    return 1;
}

s32 FragTagMenu_StepTimeLimit_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagTimeLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.tagTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}

/* Writes setting unk25 (or "none" when 0) over the end of the item's text. */
s32 FragTagMenu_TimeLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->tagTimeLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->tagTimeLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 FragTagMenu_StepTagLimit_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagScoreLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.tagScoreLimit, 1, 0, 0x63, 1);
    return 0;
}

/* Writes setting unk26 (or "none" when 0) over the end of the item's text. */
s32 FragTagMenu_TagLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->tagScoreLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->tagScoreLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 FragTagMenu_StepLevel_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagArena = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.tagArena, 1, 0, 6, 1);
    return 0;
}

/* Names setting 0x27. */
s32 FragTagMenu_LevelLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagArena) {
    case 0:
        item->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        item->text = sFragTagMenuText_LevelMadhouse;
        break;
    case 2:
        item->text = sFragTagMenuText_LevelSnap;
        break;
    case 3:
        item->text = sFragTagMenuText_LevelSneaksNLadders;
        break;
    case 4:
        item->text = sFragTagMenuText_LevelPrimattica;
        break;
    case 5:
        item->text = sFragTagMenuText_LevelCrossfire;
        break;
    case 6:
        item->text = sFragTagMenuText_LevelYNGWTSF;
        break;
    }
    return 0;
}

s32 FragTagMenu_StepTextureSet_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagTextureSet = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.tagTextureSet, 1, 0, 8, 1);
    return 0;
}

/* Names setting 0x28. */
s32 FragTagMenu_TextureSetLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagTextureSet) {
    case 0:
        item->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        item->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        item->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        item->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        item->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        item->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        item->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        item->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        item->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

s32 FragTagMenu_StepCrosshairs_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.tagCrosshairs = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.tagCrosshairs, 1, 0, 1, 1);
    return 0;
}

/* Names setting 0x29. */
s32 FragTagMenu_CrosshairsLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

s32 Controller_MenuRight(sptr controller);
#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Adjusts the game speed setting and stores its multiplier, clamped to [0.5, 2]. */
s32 MpMenu_StepPlayerSpeed_2(void *item, Menu *menu) {
    f32 speed;

    gMpPlayerSpeed = Menu_StepValue_2(menu, gMpPlayerSpeed, 1, 0, 15, 0);
    if (Controller_MenuRight(menu->controller) || Controller_ConfirmPressed(menu->controller)) {
        Menu_StepValue_2(menu, gMpPlayerSpeed, 1, 0, 15, 0);
    }
    speed = sPlayerSpeedTable_scale_2[gMpPlayerSpeed] + 1.0f;
    speed = MIN(MAX(speed, 0.5f), 2.0f);
    gEngineApp_m_Settings.speed = speed;
    return 0;
}

/* Names the game speed: a fixed line at 1.0, otherwise the setting relative to 5. */
s32 MpMenu_PlayerSpeedLabel_2(MenuItem *item) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        item->text = sMpMenuText_PlayerSpeedDefault;
    } else {
        item->text = sMpMenuText_PlayerSpeed;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 3], sMpMenuFmt_3d, gMpPlayerSpeed - 5);
    }
    return 0;
}

s32 BloodLustMenu_StepTimeLimit_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmTimeLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.dmTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}

/* Writes setting unk20 (or "none" when 0) over the end of the item's text. */
s32 BloodLustMenu_TimeLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->dmTimeLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->dmTimeLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 BloodLustMenu_StepFragLimit_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmScoreLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.dmScoreLimit, 1, 0, 0x63, 1);
    return 0;
}

/* Writes setting unk21 (or "none" when 0) over the end of the item's text. */
s32 BloodLustMenu_FragLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->dmScoreLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->dmScoreLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 BloodLustMenu_StepLevel_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmArena = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.dmArena, 1, 0, 0xD, 1);
    return 0;
}

/* Names setting 0x22. */
s32 BloodLustMenu_LevelLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.dmArena) {
    case 0:
        item->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        item->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        item->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        item->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        item->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        item->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        item->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        item->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        item->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        item->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        item->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        item->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        item->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        item->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}

s32 BloodLustMenu_StepTextureSet_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->dmTextureSet = Menu_StepValue_2(arg1, settings->dmTextureSet, 1, 0, 8, 1);
    return 0;
}

/* Names setting 0x23. */
s32 BloodLustMenu_TextureSetLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.dmTextureSet) {
    case 0:
        item->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        item->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        item->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        item->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        item->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        item->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        item->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        item->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        item->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

s32 BloodLustMenu_StepCrosshairs_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.dmCrosshairs = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.dmCrosshairs, 1, 0, 1, 1);
    return 0;
}

/* Names setting 0x24. */
s32 BloodLustMenu_CrosshairsLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.dmCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

s32 TeamBloodMenu_StepTimeLimit_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->teamTimeLimit = Menu_StepValue_2(arg1, settings->teamTimeLimit, 5, 0, 0x1E, 1);
    return 0;
}

/* Writes setting unk2A (or "none" when 0) over the end of the item's text. */
s32 TeamBloodMenu_TimeLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->teamTimeLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->teamTimeLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 TeamBloodMenu_StepFragLimit_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamScoreLimit = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.teamScoreLimit, 1, 0, 0x63, 1);
    return 0;
}

/* Writes setting unk2B (or "none" when 0) over the end of the item's text. */
s32 TeamBloodMenu_FragLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->teamScoreLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->teamScoreLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}

s32 TeamBloodMenu_StepLevel_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->teamArena = Menu_StepValue_2(arg1, settings->teamArena, 1, 0, 0xD, 1);
    return 0;
}

/* Names setting 0x2C. */
s32 TeamBloodMenu_LevelLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.teamArena) {
    case 0:
        item->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        item->text = sBloodLustMenuText_LevelExperimentata;
        break;
    case 2:
        item->text = sBloodLustMenuText_LevelMoonlitMayhem;
        break;
    case 3:
        item->text = sBloodLustMenuText_LevelCloseQuartered;
        break;
    case 4:
        item->text = sBloodLustMenuText_LevelVulcansForge;
        break;
    case 5:
        item->text = sBloodLustMenuText_LevelTeleportastic;
        break;
    case 6:
        item->text = sBloodLustMenuText_LevelMirrororrim;
        break;
    case 7:
        item->text = sBloodLustMenuText_LevelFishInABarrel;
        break;
    case 8:
        item->text = sBloodLustMenuText_LevelH2Whoa;
        break;
    case 9:
        item->text = sBloodLustMenuText_LevelEscheria;
        break;
    case 10:
        item->text = sBloodLustMenuText_LevelMoshCrypt;
        break;
    case 11:
        item->text = sBloodLustMenuText_LevelSquaresCubed;
        break;
    case 12:
        item->text = sBloodLustMenuText_LevelCrazyEight;
        break;
    case 13:
        item->text = sBloodLustMenuText_LevelBullseye;
        break;
    }
    return 0;
}

s32 TeamBloodMenu_StepTextureSet_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamTextureSet = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.teamTextureSet, 1, 0, 8, 1);
    return 0;
}

/* Names setting 0x2D. */
s32 TeamBloodMenu_TextureSetLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.teamTextureSet) {
    case 0:
        item->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        item->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        item->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        item->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        item->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        item->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        item->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        item->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        item->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}

s32 TeamBloodMenu_StepCrosshairs_2(s32 arg0, s32 arg1) {
    GameSettings *var_s0;

    var_s0 = &gEngineApp_m_Settings;
    var_s0 = &gEngineApp_m_Settings;
    gEngineApp_m_Settings.teamCrosshairs = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.teamCrosshairs, 1, 0, 1, 1);
    return 0;
}

/* Names setting 0x2E. */
s32 TeamBloodMenu_CrosshairsLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.teamCrosshairs) {
    case 0:
        item->text = sBloodLustMenuText_CrosshairsOff;
        break;
    case 1:
        item->text = sBloodLustMenuText_CrosshairsOn;
        break;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8.CPP, func_8043E830_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Menu callback: toggle the option selected by arg0[0]. */
s32 WeaponsMenu_Toggle_2(s32 *arg0) {
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

s32 DeathmatchMenu_OpenBloodLust_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gBloodLustMenuDesc_2);
    return 1;
}

s32 DeathmatchMenu_OpenFragTag_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gFragTagMenuDesc_2);
    return 1;
}

s32 DeathmatchMenu_OpenTeamBlood_2(s32 arg0, s32 arg1, s32 arg2)
{
  s8 *new_var;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  *((s32 *) (new_var + 0x2C)) = 0;
  *((s32 *) (new_var + 0x30)) = 0;
  *((s32 *) (new_var + 0x28)) = 0;
  MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) (&gTeamBloodMenuDesc_2));
  return 1;
}

s32 DeathmatchMenu_OpenWeapons_2(s32 arg0, s32 arg1, s32 arg2) {
    MenuPool_OpenChildMenu_2(arg2, arg1, (sptr) &gWeaponsMenuDesc_2);
    return 1;
}
