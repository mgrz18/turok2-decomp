/* WRONG CUT: real function 0x0041BA50, size 0x1B4 (through 0x0041BC04): splat cut its tail
 * off as func_0041BBB0. */
#include "common.h"
#include "structs/game.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80243A90_de_9.c, func_802458D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void GameSettings_SetDefaults(GameSettings *s);
void PlayerOptions_SetDefaults(void *arg0, s8 arg1);
void PlayerOptions_SetDefaultName(CPlayerOptions *w);

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
    PlayerOptions_SetDefaultName(slot);
}

/* Default multiplayer setup. */
DEFERRED void GameSettings_SetDefaults(GameSettings *s) {
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

#include "m2c_macros.h"

DEFERRED void PlayerOptions_SetDefaults(void *arg0, s8 arg1)
{
  int new_var;
  new_var = 0x80;
  *((s8 *) (((s8 *) arg0) + 0x29)) = new_var;
  *((s8 *) (((s8 *) arg0) + 0x2A)) = new_var;
  *((s8 *) (((s8 *) arg0) + 0x28)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2C)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2B)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2D)) = 1;
  *((s8 *) (((s8 *) arg0) + 0x2E)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x2F)) = arg1;
  *((s8 *) (((s8 *) arg0) + 0x30)) = 0;
  *((s8 *) (((s8 *) arg0) + 0x31)) = 1;
  *((s8 *) (((s8 *) arg0) + 0x32)) = 0;
  PlayerOptions_SetDefaultName((sptr) arg0);
}

#include "globals/menu_text.h"
#include "structs/player_options.h"

DEFERRED void PlayerOptions_SetDefaultName(CPlayerOptions *w) {
    s32 i;

    switch (w->m_nController) {
    case 0:
    default:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player1[i];
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player2[i];
        }
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player3[i];
        }
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player4[i];
        }
        break;
    }
}

#include "globals/menu.h"

void OptionsMenu_Init(void);

DEFERRED void OptionsMenu_Init(void) {
    sResolutionToggleDelay = 0;
}

#include "structs/menu.h"
#include "structs/player.h"
#include "globals/game.h"

DEFERRED s32 *OptionsMenu_GetPlayerOptions(Menu *arg0) {
    s32 *temp_v1_2;
    s32 *var_v0;
    CPlayer *temp_v1;

    temp_v1 = arg0->player;
    var_v0 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v1 != NULL) {
        temp_v1_2 = (s32 *) temp_v1->m_pOptions;
        if (temp_v1_2 != NULL) {
            var_v0 = temp_v1_2;
        }
    }
    return var_v0;
}

extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
s32 Controller_ConfirmPressed(char *);

DEFERRED s32 OptionsMenu_CycleDifficulty(void) {
    extern u32 gEngineApp_m_Settings_difficulty[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
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

DEFERRED s32 OptionsMenu_DifficultyText(MenuItemRecAt14 *item) {
    extern u32 gEngineApp_m_Settings_difficulty; /* local type: the field's real type (or the member path) changes the ROM's code here */
    if (gEngineApp_m_Settings.mode == 8) {
        item->flags |= 0x01000000;
    } else {
        item->flags &= ~0x01000000;
    }
    switch (gEngineApp_m_Settings_difficulty) {
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

#include "protos/menu.h"

DEFERRED s32 OptionsMenu_StepControlType(MenuItem *a, Menu *b) {
    CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
    if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
    p->m_ControlType = Menu_StepValue(b, p->m_ControlType, 1, 0, 1, 1);
    return 0;
}

DEFERRED s32 OptionsMenu_ControlTypeText(MenuItem *a, Menu *b) {
CPlayerOptions *p = &gEngineApp_m_Settings.defaultSlot;
if (b->player) { CPlayerOptions *q = b->player->m_pOptions; if (q) p = q; }
 switch (p->m_ControlType) { case 0: a->text = sOptionsMenuText_ControlTypeExpert; break; case 1: a->text = sOptionsMenuText_ControlTypeArcade; break; }
 return 0; }

DEFERRED s32 OptionsMenu_StepControlSide(void *first, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    settings->m_ControlSide = Menu_StepValue(menu, settings->m_ControlSide, 1, 0, 1, 1);
    return 0;
}

#define T2_TEXT(s) ((void *) (s))
#ifdef TARGET_PORT
#else
#endif

DEFERRED s32 OptionsMenu_ControlSideText(MenuItem *arg0, Menu *arg1) {
    CPlayerOptions *temp_v0_2;
    CPlayerOptions *var_v1;
    u8 temp_v1;
    CPlayer *temp_v0;

    temp_v0 = arg1->player;
    var_v1 = &gEngineApp_m_Settings.defaultSlot;
    if (temp_v0 != NULL) {
        temp_v0_2 = temp_v0->m_pOptions;
        if (temp_v0_2 != NULL) {
            var_v1 = temp_v0_2;
        }
    }
    temp_v1 = var_v1->m_ControlSide;
    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            arg0->table = T2_TEXT(sOptionsMenuText_ControlSideLeft);
        }
    } else {
        arg0->table = T2_TEXT(sOptionsMenuText_ControlSideRight);
    }
    return 0;
}

DEFERRED s32 OptionsMenu_StepLookSpring(void *first, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    settings->m_bLookSpring = Menu_StepValue(menu, settings->m_bLookSpring, 1, 0, 1, 1);
    return 0;
}

DEFERRED s32 OptionsMenu_LookSpringText(MenuItem *item, Menu *menu) {
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

DEFERRED s32 OptionsMenu_StepAutoAim(void *first, Menu *menu) {
    CPlayerOptions *settings = &gEngineApp_m_Settings.defaultSlot;
    if (menu->player != 0 && menu->player->m_pOptions != 0) {
        settings = menu->player->m_pOptions;
    }
    settings->m_bAutoAim = Menu_StepValue(menu, settings->m_bAutoAim, 1, 0, 1, 1);
    return 0;
}

DEFERRED s32 OptionsMenu_AutoAimText(MenuItem *item, Menu *menu) {
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

DEFERRED s32 OptionsMenu_StepBloodColor(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk17;
    *option = Menu_StepValue(second, *option, 1, 0, 2, 1);
    return 0;
}

DEFERRED s32 OptionsMenu_BloodColorText(struct FieldRecAt14 *field) {
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

DEFERRED s32 OptionsMenu_StepTwoPlayerLayout(void *first, void *second) {
    u8 *option = &gEngineApp_m_Settings.unk1B;
    *option = Menu_StepValue(second, *option, 1, 0, 1, 1);
    return 0;
}

DEFERRED s32 OptionsMenu_TwoPlayerLayoutText(MenuItem *a) {
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0) {
        a->flags &= ~0x1000000;
    } else {
        a->flags |= 0x1000000;
    }
    switch (gEngineApp_m_Settings.unk1B) {
    case 0:
        a->text = sOptionsMenuText_2PlyrLayoutHorz;
        break;
    case 1:
        a->text = sOptionsMenuText_2PlyrLayoutVert;
        break;
    }
    return 0;
}

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

DEFERRED s32 OptionsMenu_OpacityText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.hudAlphaSetting.value >> 4;

    if (h == 0) {
        a->text = sOptionsMenuText_OpacityClear;
    } else if (h == 0x10) {
        a->text = sOptionsMenuText_OpacityOpaque;
    } else {
        a->text = sOptionsMenuText_Opacity;
        sprintf(a->text + (MenuItem_TextLength(a) - 2), "%02d", h);
    }
    return 0;
}

DEFERRED s32 OptionsMenu_StepOpacity(sptr arg0, sptr arg1) {
    u16 temp_v0;
    u8 var_v1;

    temp_v0 = Menu_StepValue(arg1, gEngineApp_m_Settings.hudAlphaSetting.value, 0x10, 0, 0x100, 0);
    gEngineApp_m_Settings.hudAlphaSetting.value = temp_v0;
    var_v1 = 0xFF;
    if ((u32) (temp_v0 & 0xFFFF) < 0xFFU) {
        var_v1 = gEngineApp_m_Settings.hudAlphaSetting.bytes[1];
    }
    gEngineApp_m_Settings.hudAlpha = var_v1;
    return 0;
}

DEFERRED s32 OptionsMenu_StepStickHorz(s32 arg0, Menu *arg1) {
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
    var_s0->m_SettingX = Menu_StepValue(arg1, (s32) var_s0->m_SettingX, 8, 8, 0xF8, 0);
    return 0;
}

/* Label of the player's 0x80-centred setting at options+0x29. */
DEFERRED s32 OptionsMenu_StickHorzText(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingX - 0x80) / 8;
    if (v == 0) {
        a->text = sOptionsMenuText_ControlStickHorzNorm;
    } else if (v > 0) {
        a->text = sOptionsMenuText_ControlStickHorz;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "  %02d", v);
    } else {
        a->text = sOptionsMenuText_ControlStickHorz;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), " %03d", v);
    }
    return 0;
}

DEFERRED s32 OptionsMenu_StepStickVert(s32 arg0, Menu *arg1) {
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
    var_s0->m_SettingY = Menu_StepValue(arg1, (s32) var_s0->m_SettingY, 8, 8, 0xF8, 0);
    return 0;
}

/* Label of the player's 0x80-centred setting at options+0x2A. */
DEFERRED s32 OptionsMenu_StickVertText(MenuItem *a, Menu *c) {
    CPlayerOptions *options = &gEngineApp_m_Settings.defaultSlot;
    s32 v;

    if (c->player != NULL && c->player->m_pOptions != NULL) {
        options = c->player->m_pOptions;
    }
    v = (options->m_SettingY - 0x80) / 8;
    if (v == 0) {
        a->text = sOptionsMenuText_ControlStickVertNorm;
    } else if (v > 0) {
        a->text = sOptionsMenuText_ControlStickVert;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_02d, v);
    } else {
        a->text = sOptionsMenuText_ControlStickVert;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_03d, v);
    }
    return 0;
}

/* The weapon select item's text: quick or normal. */
DEFERRED s32 OptionsMenu_WeaponSelectText(MenuItem *item, Menu *menu)
{
  char *quick;
  CPlayerOptions *settings;
  u8 *select;
  CPlayer *player;
  player = menu->player;
  quick = sOptionsMenuText_WeaponSelectQuick;
  settings = &gEngineApp_m_Settings.defaultSlot;
  if (player != 0)
  {
    settings = player->m_pOptions;
  }
  select = &settings->m_WeaponSelect;
  if (*select == 1)
  {
    item->text = quick;
  }
  else
  {
    item->text = sOptionsMenuText_WeaponSelectNorm;
  }
  return 0;
}

DEFERRED s32 OptionsMenu_ToggleWeaponSelect(void *unused, Menu *menu) {
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

#include "globals/engine.h"
#ifdef TARGET_PORT
#include "structs/menu.h"
#endif

sptr Video_GetResolutionText(void);
s32 GetMemSize();                                /* extern */

DEFERRED s32 OptionsMenu_ResolutionText(void *arg0)
{
  f32 temp_f0;
  s32 var_v0;
  int new_var2;
  s8 *new_var;
  temp_f0 = sResolutionToggleDelay - gFrameIncrement;
  sResolutionToggleDelay = temp_f0;
  if (temp_f0 <= 0.0f)
  {
    sResolutionToggleDelay = 0.0f;
  }
  if (GetMemSize() != 0x400000)
  {
    new_var2 = 0x01000000;
    var_v0 = (*((s32 *) (((s8 *) arg0) + 8))) | new_var2;
  }
  else
  {
    var_v0 = (*((s32 *) (((s8 *) arg0) - -8))) & 0xFEFFFFFF;
  }
  new_var2 = var_v0;
  *((s32 *) (((s8 *) arg0) + 8)) = new_var2;
#ifdef TARGET_PORT
  /* Round 23: the item's text by the member (host 0x18; the N64 store below is an s32 at 0x14) */
  ((MenuItem *) arg0)->text = (char *) Video_GetResolutionText();
#else
  new_var = ((s8 *) arg0) + 0x14;
  *((s32 *) new_var) = Video_GetResolutionText();
#endif
  return 0;
}

s32 Video_StartResolutionChange(void);
extern u8 D_800C203B;

DEFERRED s32 OptionsMenu_ToggleResolution(void) {
    if (sResolutionToggleDelay == 0.0f) {
        Video_StartResolutionChange();
        sResolutionToggleDelay = 18.0f;
        gEngineApp_m_Settings.unk180 = D_800C203B;
    }
    return 0;
}

DEFERRED s32 OptionsMenu_StepBrightness(s32 menu, void *c) {
    GameSettings *s = &gEngineApp_m_Settings;

    s->unkC = Menu_StepValue(c, s->unkC, 0x10, 0, 0x80, 0);
    return 0;
}

#include "protos/cache.h"

DEFERRED s32 OptionsMenu_BrightnessText(MenuItem *a) {
    u32 h = gEngineApp_m_Settings.unkC >> 4;

    CCache__SetFrameDelay(CCache__GetFrameDelay() + 2);
    if (h == 0) {
        a->text = sOptionsMenuText_BrightnessNorm;
    } else {
        a->text = sOptionsMenuText_Brightness;
        sprintf(a->text + (MenuItem_TextLength(a) - 4), sOptionsMenuFmt_02d, h);
    }
    return 0;
}
