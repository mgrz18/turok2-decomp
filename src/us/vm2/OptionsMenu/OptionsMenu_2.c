/* Module-2 copy of GameSettings_SetDefaults (same code; callee at its link address). */
#include "common.h"
#include "structs/game.h"
#include "common.h"
#include "structs/player_options.h"
#include "globals/menu_text.h"
#include "m2c_macros.h"
#include "globals/menu.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"
#include "structs/vector.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044421C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444314_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444594.c, func_00444594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044465C.c, func_0044465C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_80444424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044472C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444D58.c, func_00444D58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* The same code as OptionsMenu_ToggleWeaponSelect (the other module). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030.CPP, func_80444C68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
#include "protos/cache.h"

typedef struct {
    /* 0x00 */ char pad0[0x2F];
    /* 0x2F */ s8 kind;
    /* 0x30 */ char pad30[0x3];
    /* 0x33 */ u8 name[8];
} NamedThing;
void PlayerOptions_SetDefaultName_2(NamedThing *t);

static __inline__ void InitSlot(CPlayerOptions *slot, u8 index) {
    slot->m_bActive = 0;
    slot->m_SettingX = 0x80;
    slot->m_SettingY = 0x80;
    slot->m_ControlSide = 0;
    slot->m_ControlType = 0;
    slot->m_bLookSpring = 1;
    slot->m_bAutoAim = 0;
    slot->m_nController = index;
    slot->m_nCharacter = 0;
    slot->m_WeaponSelect = 1;
    slot->m_Handicap = 0;
    PlayerOptions_SetDefaultName_2(slot);
}

/* Default multiplayer setup. */
void GameSettings_SetDefaults_2(GameSettings *s) {
    s32 i;

    s->unk13 = 1;
    s->unk14 = 0x80;
    s->unk15 = 0xFA;
    s->unk16 = 0xFF;
    s->hudAlphaSetting.value = 0x80;
    s->difficulty = 0x10;
    s->unkC = 0;
    s->unk17 = 1;
    s->unk18 = 0;
    s->cheatFlags = 0;
    s->unlockedCheats = 0;
    s->unk1B = 0;
    s->unk180 = 0;
    s->dmTimeLimit = 10;
    s->dmScoreLimit = 0;
    s->dmArena = 0;
    s->dmTextureSet = 0;
    s->dmCrosshairs = 1;
    s->tagTimeLimit = 0;
    s->tagScoreLimit = 0;
    s->tagArena = 0;
    s->tagTextureSet = 0;
    s->tagCrosshairs = 1;
    s->teamTimeLimit = 10;
    s->teamScoreLimit = 0;
    s->teamArena = 0;
    s->teamTextureSet = 0;
    s->teamCrosshairs = 1;
    s->hudAlpha = s->hudAlphaSetting.value;
    s->speed = 1.0f;
    for (i = 0; i < 10; i++) {
        s->weaponEnabled[i] = 1;
    }
    s->unk3C = 0;
    s->unk3D = 3;
    s->unk3E = 0;
    s->unk3F = 3;
    for (i = 0; i < 4; i++) {
        InitSlot((CPlayerOptions *) ((u8 *) s + 0x80 + i * 0x40), i);
    }
    InitSlot((CPlayerOptions *) ((u8 *) s + 0x40), 0);
    s->defaultSlot.m_bActive = 1;
    s->multiplayer = 0;
    s->playerCount = 1;
    s->cheatFlags = 0x4000000;
    s->unlockedCheats = 0x4000000;
}

void PlayerOptions_SetDefaults_2(CPlayerOptions *w, u8 arg1) {
    w->m_SettingX = 0x80;
    w->m_SettingY = 0x80;
    w->m_bActive = 0;
    w->m_ControlSide = 0;
    w->m_ControlType = 0;
    w->m_bLookSpring = 1;
    w->m_bAutoAim = 0;
    w->m_nController = arg1;
    w->m_nCharacter = 0;
    w->m_WeaponSelect = 1;
    w->m_Handicap = 0;
    PlayerOptions_SetDefaultName_2(w);
}

void PlayerOptions_SetDefaultName_2(NamedThing *t) {
    s32 i;

    switch (t->kind) {
        case 0:
        default:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player1[i];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player2[i];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player3[i];
            }
            break;
        case 3:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player4[i];
            }
            break;
    }
}

void OptionsMenu_Init_2(void) {
    sResolutionToggleDelay = 0;
}

/* The player's options, or the defaults at gEngineApp_m_Settings_defaultSlot. */
CPlayerOptions *OptionsMenu_GetPlayerOptions_2(Menu *arg0) {
    CPlayerOptions *temp_v1_2;
    CPlayerOptions *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v1 != NULL) {
        temp_v1_2 = temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}

extern int Controller_MenuLeft(ControllerProfile *object);
extern int Controller_MenuRight(ControllerProfile *object);
s32 Controller_ConfirmPressed(char *);
extern u32 gEngineApp_m_Settings_difficulty[]; /* local type: the field's real type (or the member path) changes the ROM's code here */

s32 OptionsMenu_CycleDifficulty_2(void) {
 if(((s32 (*)(char *))Controller_MenuLeft)((char *)&gMergedController)) {
 switch(gEngineApp_m_Settings_difficulty[0]) {
 case 8: gEngineApp_m_Settings_difficulty[0]=32; break;
 case 16: gEngineApp_m_Settings_difficulty[0]=8; break;
 case 32: gEngineApp_m_Settings_difficulty[0]=16; break;
 }
 }
 if(((s32 (*)(char *))Controller_MenuRight)((char *)&gMergedController) || Controller_ConfirmPressed((char *)&gMergedController)) {
 switch(gEngineApp_m_Settings_difficulty[0]) {
 case 8: gEngineApp_m_Settings_difficulty[0]=16; break;
 case 16: gEngineApp_m_Settings_difficulty[0]=32; break;
 case 32: gEngineApp_m_Settings_difficulty[0]=8; break;
 }
 }
 return 0;
}

struct MenuItemRecAt14;
struct MenuItemRecAt14 {
    char pad0[8];
    u32 flags;
    char padC[8];
    void *table;
};
typedef struct MenuItemRecAt14 MenuItemRecAt14;

s32 OptionsMenu_DifficultyText_2(MenuItemRecAt14 *item) {
    if (gEngineApp_m_Settings.mode == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (gEngineApp_m_Settings_difficulty[0]) {
    case 8:
        item->table = sOptionsMenuText_DifficultyEasy;
        break;
    case 0x10:
        item->table = sOptionsMenuText_DifficultyNormal;
        break;
    case 0x20:
        item->table = sOptionsMenuText_DifficultyHard;
        break;
    }
    return 0;
}

s32 Menu_StepValue_2(s32, s32, s32, s32, s32, s32);

s32 OptionsMenu_StepControlType_2(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_ControlType = Menu_StepValue_2((sptr) arg1, (s32) var_s0->m_ControlType, 1, 0, 1, 1);
    return 0;
}

s32 OptionsMenu_ControlTypeText_2(MenuItem *a, Menu *b) {
CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 switch (p->m_ControlType) { case 0: a->text = sOptionsMenuText_ControlTypeExpert; break; case 1: a->text = sOptionsMenuText_ControlTypeArcade; break; }
 return 0; }

s32 OptionsMenu_StepControlSide_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_ControlSide = Menu_StepValue_2((sptr) arg1, (s32) var_s0->m_ControlSide, 1, 0, 1, 1);
    return 0;
}

s32 OptionsMenu_ControlSideText_2(MenuItem *a, Menu *b) {
CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 if (p->m_ControlSide != 0) { if (p->m_ControlSide == 1) a->text = sOptionsMenuText_ControlSideLeft; } else a->text = sOptionsMenuText_ControlSideRight;
 return 0; }

s32 OptionsMenu_StepLookSpring_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_bLookSpring = Menu_StepValue_2((sptr) arg1, var_s0->m_bLookSpring, 1, 0, 1, 1);
    return 0;
}

s32 OptionsMenu_LookSpringText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    switch (settings->m_bLookSpring) {
    case 0:
        item->text = sOptionsMenuText_LookSpringOff;
        break;
    case 1:
        item->text = sOptionsMenuText_LookSpringOn;
        break;
    }
    return 0;
}

s32 OptionsMenu_StepAutoAim_2(s32 arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_bAutoAim = Menu_StepValue_2((sptr) arg1, (s32) var_s0->m_bAutoAim, 1, 0, 1, 1);
    return 0;
}

s32 OptionsMenu_AutoAimText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    switch (settings->m_bAutoAim) {
    case 0:
        item->text = sOptionsMenuText_AutoAimOff;
        break;
    case 1:
        item->text = sOptionsMenuText_AutoAimOn;
        break;
    }
    return 0;
}

s32 OptionsMenu_StepBloodColor_2(s32 arg0, s32 arg1) {
    u8 *value = &gEngineApp_m_Settings_unk17; /* through a pointer: the ROM builds the address once */

    *value = Menu_StepValue_2(arg1, *value, 1, 0, 2, 1);
    return 0;
}

struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};

s32 OptionsMenu_BloodColorText_2(struct FieldRecAt14 *field) {
    switch (gEngineApp_m_Settings.unk17) {
    case 0:
        field->text = sOptionsMenuText_BloodColorOff;
        break;
    case 1:
        field->text = sOptionsMenuText_BloodColorRed;
        break;
    case 2:
        field->text = sOptionsMenuText_BloodColorGreen;
        break;
    default:
        return 0;
    }
    return 0;
}

s32 OptionsMenu_StepTwoPlayerLayout_2(s32 arg0, s32 arg1) {
    u8 *value = &gEngineApp_m_Settings_unk1B; /* through a pointer: the ROM builds the address once */

    *value = Menu_StepValue_2(arg1, *value, 1, 0, 1, 1);
    return 0;
}

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

s32 OptionsMenu_TwoPlayerLayoutText_2(MenuItem *item) {
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0) {
        item->flags &= ~0x1000000;
    } else {
        item->flags |= 0x1000000;
    }
    switch (gEngineApp_m_Settings.unk1B) {
    case 0:
        item->text = sOptionsMenuText_2PlyrLayoutHorz;
        break;
    case 1:
        item->text = sOptionsMenuText_2PlyrLayoutVert;
        break;
    }
    return 0;
}

/* Shows the HUD alpha setting in sixteenths: two fixed lines for 0 and 16, otherwise the number. */
s32 OptionsMenu_OpacityText_2(MenuItem *item) {
    s32 value = gEngineApp_m_Settings.hudAlphaSetting.value >> 4;

    if (value == 0) {
        item->text = sOptionsMenuText_OpacityClear;
    } else if (value == 16) {
        item->text = sOptionsMenuText_OpacityOpaque;
    } else {
        item->text = sOptionsMenuText_Opacity;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 2], "%02d", value);
    }
    return 0;
}

s32 OptionsMenu_StepOpacity_2(s32 arg0, s32 arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = Menu_StepValue_2(arg1, (s32) gEngineApp_m_Settings.hudAlphaSetting.value, 0x10, 0, 0x100, 0);
    gEngineApp_m_Settings.hudAlphaSetting.value = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = gEngineApp_m_Settings.hudAlphaSetting.bytes[1];
    }
    gEngineApp_m_Settings.hudAlpha = var_v1;
    return 0;
}

s32 OptionsMenu_StepStickHorz_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_SettingX = Menu_StepValue_2((sptr) arg1, (s32) var_s0->m_SettingX, 8, 8, 0xF8, 0);
    return 0;
}

/* Shows the player's option unk29 (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 OptionsMenu_StickHorzText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->m_pOptions != NULL) {
        options = menu->player->m_pOptions;
    }
    value = (options->m_SettingX - 0x80) / 8;
    if (value == 0) {
        item->text = sOptionsMenuText_ControlStickHorzNorm;
    } else if (value > 0) {
        item->text = sOptionsMenuText_ControlStickHorz;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    } else {
        item->text = sOptionsMenuText_ControlStickHorz;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %03d", value);
    }
    return 0;
}

s32 OptionsMenu_StepStickVert_2(s32 arg0, Menu *arg1) {
    struct CPlayerOptions *temp_v0_2;
    struct CPlayerOptions *var_s0;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_s0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_s0 = temp_v0_2;
        }
    }
    var_s0->m_SettingY = Menu_StepValue_2((sptr) arg1, var_s0->m_SettingY, 8, 8, 0xF8, 0);
    return 0;
}

/* Shows the player's option unk2A (0-255, centred on 0x80) in steps of 8, falling back to the
 * default options when there is no player. */
s32 OptionsMenu_StickVertText_2(MenuItem *item, Menu *menu) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 value;

    if (menu->player != NULL && menu->player->m_pOptions != NULL) {
        options = menu->player->m_pOptions;
    }
    value = (options->m_SettingY - 0x80) / 8;
    if (value == 0) {
        item->text = sOptionsMenuText_ControlStickVertNorm;
    } else if (value > 0) {
        item->text = sOptionsMenuText_ControlStickVert;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    } else {
        item->text = sOptionsMenuText_ControlStickVert;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], " %03d", value);
    }
    return 0;
}

s32 OptionsMenu_WeaponSelectText_2(MenuItem *a, Menu *b) {
 CPlayer *q = b->player;
 CPlayerOptions *p;
 if (q) p = q->m_pOptions; else p = &gEngineApp_m_Settings.defaultSlot;
 if (p->m_WeaponSelect == 1) a->text = sOptionsMenuText_WeaponSelectQuick; else a->text = sOptionsMenuText_WeaponSelectNorm;
 return 0;
}

s32 OptionsMenu_ToggleWeaponSelect_2(void *unused, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0) {
        settings = menu->player->m_pOptions;
    }
    if (settings->m_WeaponSelect == 1) {
        settings->m_WeaponSelect = 0;
    } else {
        settings->m_WeaponSelect = 1;
    }
    return 0;
}

struct RecordRecAt14;
struct RecordRecAt14 {
    char pad[8];
    s32 flags;
    char padC[0x14 - 0xC];
    s32 value;
};
extern s32 GetMemSize(void);
extern sptr Video_GetResolutionText_2();

s32 OptionsMenu_ResolutionText_2(struct RecordRecAt14 *record) {
    sResolutionToggleDelay -= gFrameIncrement;
    if (sResolutionToggleDelay <= 0.0f) {
        sResolutionToggleDelay = 0.0f;
    }
    if (GetMemSize() != 0x400000) {
        record->flags |= 0x1000000;
    } else {
        record->flags &= ~0x1000000;
    }
    record->value = Video_GetResolutionText_2();
    return 0;
}

s32 Video_StartResolutionChange_2();                            /* extern */
#include "globals/render.h"   /* RESOLUTION_LOW_BYTE */

s32 OptionsMenu_ToggleResolution_2(void) {
    if (sResolutionToggleDelay == 0.0f) {
        Video_StartResolutionChange_2();
        sResolutionToggleDelay = 18.0f;
        gEngineApp_m_Settings.unk180 = RESOLUTION_LOW_BYTE;
    }
    return 0;
}

s32 OptionsMenu_StepBrightness_2(s32 arg0, s32 arg1) {
    GameSettings *settings = &gEngineApp_m_Settings;

    settings->unkC = Menu_StepValue_2(arg1, settings->unkC, 0x10, 0, 0x80, 0);
    return 0;
}

/* Shows setting 0x0C in sixteenths: a fixed line for 0, otherwise the number. */
s32 OptionsMenu_BrightnessText_2(MenuItem *item) {
    u32 value = gEngineApp_m_Settings.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (value == 0) {
        item->text = sOptionsMenuText_BrightnessNorm;
    } else {
        item->text = sOptionsMenuText_Brightness;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", value);
    }
    return 0;
}
