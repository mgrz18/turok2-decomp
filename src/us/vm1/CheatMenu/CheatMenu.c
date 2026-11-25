#include "common.h"
#include "globals/menu.h"
#include "globals/game.h"
#include "structs/menu.h"

void CheatsMenu_Init(Menu *menu) {
    s32 weapon = 0;
    s32 tries;
    s32 mask;

    for (tries = weapon; tries < 11; tries++) {
        switch (weapon) {
            default:
            case 0:
                mask = 0x10000;
                break;
            case 1:
                mask = 0x20000;
                break;
            case 2:
                mask = 0x40000;
                break;
            case 3:
                mask = 0x80000;
                break;
            case 4:
                mask = 0x100000;
                break;
            case 5:
                mask = 0x200000;
                break;
            case 6:
                mask = 0x800000;
                break;
            case 7:
                mask = 0x400000;
                break;
            case 8:
                mask = 0x1000000;
                break;
            case 9:
                mask = 0x2000000;
                break;
            case 10:
                mask = 0x4000000;
                break;
        }
        if (gEngineApp_m_Settings_unlockedCheats & mask) {
            goto found;
        }
        weapon++;
        if (weapon >= 11) {
            weapon = 0;
        } else if (weapon < 0) {
            weapon = 10;
        }
    }
    weapon = -1;
found:
    sCheatsMenuSelection = weapon;
    /* the N64 word at items + 0x2D8: the 19th item's flags (MenuItem is larger on the host) */
    if (weapon != -1) {
        menu->items[18].flags |= 0x1800000;
    } else {
        menu->items[18].flags &= ~0x1800000;
    }
}

#include "structs/menu.h"
#include "structs/game.h"
#include "globals/scene.h"
#include "protos/scene.h"

/* Level select (warp) menu input: A warps to the selected level (or, on the
 * last entry, starts the credits/next mode), up and down move the selection
 * to the previous/next level whose key the player owns. */




extern s32 Controller_ConfirmPressed(sptr pad);
extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
extern void Menu_ClaimPressingController(Menu *c);
extern void CEngineApp__SetupFadeTo(void *game, s32 mode);
extern void CEngineApp__StartGameAtWarp(void *game, s32 level);

static __inline__ u32 levelKey(s32 level) {
    u32 bit;

    switch (level) {
    case 0:
    default:
        bit = 0x10000;
        break;
    case 1:
        bit = 0x20000;
        break;
    case 2:
        bit = 0x40000;
        break;
    case 3:
        bit = 0x80000;
        break;
    case 4:
        bit = 0x100000;
        break;
    case 5:
        bit = 0x200000;
        break;
    case 6:
        bit = 0x800000;
        break;
    case 7:
        bit = 0x400000;
        break;
    case 8:
        bit = 0x1000000;
        break;
    case 9:
        bit = 0x2000000;
        break;
    case 10:
        bit = 0x4000000;
        break;
    }
    return bit;
}

/* The next level from the selection in direction step whose key is owned, or -1. */
static __inline__ s32 findLevel(s32 step) {
    s32 sel;
    s32 n;
    u32 keys;

    sel = sCheatsMenuSelection;
    keys = gEngineApp_m_Game.m_Settings.unlockedCheats;
    for (n = 0; n < 11; n++) {
        sel += step;
        if (sel > 10) {
            sel = 0;
        } else if (sel < 0) {
            sel = 10;
        }
        if (keys & levelKey(sel)) {
            return sel;
        }
    }
    return -1;
}

s32 CheatsMenu_WarpSelect(void *menu, Menu *c) {
    s32 id;
    GameSettings *progress;

    switch (sCheatsMenuSelection) {
    case 0:
    default:
        id = 1000;
        break;
    case 1:
        id = 2000;
        break;
    case 2:
        id = 3000;
        break;
    case 3:
        id = 4000;
        break;
    case 4:
        id = 5000;
        break;
    case 5:
        id = 6000;
        break;
    case 6:
        id = 0x1CE9;
        break;
    case 7:
        id = 0x1D4D;
        break;
    case 8:
        id = 0x1DB1;
        break;
    case 9:
        id = 0x1DB2;
        break;
    case 10:
        id = 8000;
        break;
    }
    if (Controller_ConfirmPressed(c->controller) != 0) {
        if (id == 8000) {
            gEngineApp_m_Game.m_Settings.playerCount = 1;
            Menu_ClaimPressingController(c);
            CEngineApp__SetupFadeTo(&gEngineApp, 16);
            return 1;
        }
        if (gEngineApp_m_Game.m_Settings.mode == 13) {
            CScene__RequestWarp(&gScene, id, 2);
            return 0;
        }
        progress = &gEngineApp_m_Game.m_Settings;
        progress->multiplayer = 0;
        progress->playerCount = 1;
        Menu_ClaimPressingController(c);
        CEngineApp__StartGameAtWarp(&gEngineApp, id);
        return 1;
    }
    if (Controller_MenuLeft(c->controller) != 0) {
        sCheatsMenuSelection = findLevel(-1);
    } else if (Controller_MenuRight(c->controller) != 0) {
        sCheatsMenuSelection = findLevel(1);
    } else {
        return 0;
    }
    return 0;
}

#include "m2c_macros.h"

void EnterCheatMenu_Init(void) {
    sEnterCheatResult = -1;
}

#include "globals/render.h"

/* Menu item draw callback: while the cursor blink timer is low, draws the
 * highlight box (11 virtual pixels wide, scaled to the screen) at the cursor
 * column, faded by the item's alpha, then draws the item as usual. */


extern void Hud_SetupFillMode(void);
extern void Hud_DrawBevelBox(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j);
extern void MenuItem_DrawText(void *menu, MenuItemLayout *item, void *arg2, Menu *win);

void EnterCheatMenu_DrawTextField(void *menu, MenuItemLayout *item, void *arg2, Menu *win) {
    f32 width;
    f32 alpha;
    s32 x;

    if (gCheatEntry.timer <= 6.0f) {
        width = gScreenWidth * 11 / 284;
        x = item->left + gCheatEntry.cursor * width;
        alpha = win->fade * (win->alpha * 128.0f);
        gMenuCurrentFont = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom, x + width, item->bottom + 2, 0, 0, 0xFF, 0xFF, 0xFF, (u8) alpha);
    }
    MenuItem_DrawText(menu, item, arg2, win);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"
/* the menu by its host layout (round 10 y4): the view's s32 input at N64 0x20 is
 * Menu.controller, a pointer at another host offset, and the call cast it to s32 (macOS, statics
 * above 4 GB: Controller_MenuLeft crashed when a soak run opened the cheat entry; Linux non-PIE
 * statics fit in 32 bits) */
#define T2_MENU_INPUT(m) (((Menu *) (m))->controller)
#define T2_MENU_LEFT(m) Controller_MenuLeft(T2_MENU_INPUT(m))
#define T2_MENU_RIGHT(m) Controller_MenuRight(T2_MENU_INPUT(m))

s32 EnterCheatMenu_UpdateTextField(void *unused, MenuRecAt20V4 *menu) {
    f32 t;
    t = gCheatEntry.timer + gFrameIncrement;
    gCheatEntry.timer = t;
    if (t >= 15.0f) {
        gCheatEntry.timer = t - 15.0f;
    }
    if (menu->state == 2) {
        if (T2_MENU_LEFT(menu) != 0 && gCheatEntry.cursor > 0) {
            gCheatEntry.cursor--;
            gCheatEntry.timer = 0.0f;
        } else if (T2_MENU_RIGHT(menu) != 0) {
            if (gCheatEntry.cursor < gCheatEntry.length) {
                gCheatEntry.cursor++;
                gCheatEntry.timer = 0.0f;
            }
        }
    }
    return 0;
}

s32 EnterCheatMenu_TextFieldSelect(void) {
    return 0;
}

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *key;
} KeyEvent;

/* Inserts the event's character at the cursor (max 22 chars). */
s32 EnterCheatMenu_InsertLetter(KeyEvent *ev) {
    s32 i;

    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 22) {
        return 0;
    }
    for (i = 22; gCheatEntry.cursor < i; i--) {
        gCheatEntryText[i] = gCheatEntryText[i - 1];
    }
    gCheatEntryText[gCheatEntry.cursor] = *ev->key;
    gCheatEntry.cursor++;
    gCheatEntry.length++;
    gCheatEntryText[gCheatEntry.length] = 0;
    return 0;
}

s32 EnterCheatMenu_Backspace(void) {
    s32 index;
    s32 next;
    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 0) {
        return 0;
    }
    if (gCheatEntry.cursor == 0) {
        return 0;
    }
    index = gCheatEntry.cursor - 1;
    if (index < 0x17) {
        do {
            next = index + 1;
            gCheatEntry.text[index] = gCheatEntry.text[next];
            index = next;
        } while (index < 0x17);
    }
    gCheatEntry.cursor--;
    gCheatEntry.length--;
    return 0;
}

s32 strlen(u8 *);

s32 EnterCheatMenu_MatchesCode(u8 *arg0) {
    u8 *input = arg0;
    s32 index = 0;
    if (strlen(input) != strlen(gCheatEntryText)) {
        return 0;
    }
    for (; index < strlen(input); index++) {
        if ((input[index] ^ index) != gCheatEntryText[index]) {
            return 0;
        }
    }
    return 1;
}

#include "globals/cheat.h"

extern s32 strlen(u8 *);
extern void Sound__Play2D(s32);

s32 EnterCheatMenu_Enter(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    GameSettings *settings;
    u8 *code;
    sEnterCheatResult = -1;
    for (i = 0; i < 12; i++) {
        code = sCheatCodes[i].code;
        j = 0;
        if (strlen(code) != strlen(gCheatEntryText)) {
            match = 0;
        } else {
            for (;;) {
                if (j < strlen(code)) {
                    if ((code[j] ^ j) != gCheatEntryText[j]) {
                        match = 0;
                        break;
                    }
                    j++;
                } else {
                    match = 1;
                    break;
                }
            }
        }
        if (match) {
            gEngineApp_m_Settings.unlockedCheats |= sCheatCodes[i].flags;
            settings = &gEngineApp_m_Settings;
            do {
                gScene_m_PreviewOptions = settings->unlockedCheats;
            } while (0);
            hasSound = sCheatCodes[i].sound != 0;
            sEnterCheatResult = i;
            if (hasSound) {
                Sound__Play2D((s16)sCheatCodes[i].sound);
            }
        }
    }
    return 0;
}

/* The cheat keyboard's labels: one-character strings a word apart (" ", "Z", "Y", ...), drawn
 * as text. Declared as bytes so that the port generates them in ROM byte order: as s32 words
 * they were swapped and every key of the Enter Cheat screen drew an empty string. */
extern u8 sEnterCheatKeyLabels[]; /* local: globals/menu_text.h is included further down */

s32 EnterCheatMenu_ResultLabel(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = (s32 *) sCheatCodes[sEnterCheatResult].unk4;
    } else {
        arg0->table = (s32 *) sEnterCheatKeyLabels;
    }
    return 0;
}

s32 Cheat_FindWarpLevel(s32 index, s32 step, s32 mode) {
    s32 i;
    s32 mask;
    s32 *flags;
    for (i = 0; i < 11; i++) {
        if (mode == 0) {
            index += step;
            if (index >= 11) {
                index = 0;
            } else if (index < 0) {
                index = 10;
            }
        }
        switch (index) {
        case 0:
        default:
            mask = 0x10000;
            break;
        case 1:
            mask = 0x20000;
            break;
        case 2:
            mask = 0x40000;
            break;
        case 3:
            mask = 0x80000;
            break;
        case 4:
            mask = 0x100000;
            break;
        case 5:
            mask = 0x200000;
            break;
        case 6:
            mask = 0x800000;
            break;
        case 7:
            mask = 0x400000;
            break;
        case 8:
            mask = 0x1000000;
            break;
        case 9:
            mask = 0x2000000;
            break;
        case 10:
            mask = 0x4000000;
            break;
        }
        flags = &gEngineApp_m_Settings.unlockedCheats;
        if (*flags & mask) {
            return index;
        }
        if (mode != 0) {
            index += step;
            if (index >= 11) {
                index = 0;
            } else if (index < 0) {
                index = 10;
            }
        }
    }
    return -1;
}

s32 CheatsMenu_SetToggle(struct Func8043D458Arg *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    if (gEngineApp_m_Settings.unlockedCheats & arg1) arg0->flags |= 0x01000000;
    else arg0->flags &= 0xFEFFFFFF;
    if (gEngineApp_m_Settings.cheatFlags & arg1) arg0->text = arg2;
    else arg0->text = arg3;
    return 0;
}

s32 CheatMenu_ToggleFlag(s32 arg0, s32 arg1) {
    gEngineApp_m_Settings.cheatFlags ^= arg1;
    return 0;
}

#include "globals/menu_text.h"

s32 CheatMenu_WarpText(MenuItem *o) {
    switch (sCheatsMenuSelection) {
    default: o->text = (char *)sEnterCheatKeyLabels; break;
    case 0: o->text = (char *)sCheatMenuText_WarpPortOfAdia; break;
    case 1: o->text = (char *)sCheatMenuText_WarpRiverOfSouls; break;
    case 2: o->text = (char *)sCheatMenuText_WarpDeathMarshes; break;
    case 3: o->text = (char *)sCheatMenuText_WarpLairOfTheBlindOnes; break;
    case 4: o->text = (char *)sCheatMenuText_WarpHiveOfTheMantids; break;
    case 5: o->text = (char *)sCheatMenuText_WarpPrimagensLightship; break;
    case 6: o->text = (char *)sCheatMenuText_WarpBlindOneBoss; break;
    case 7: o->text = (char *)sCheatMenuText_WarpQueenBoss; break;
    case 8: o->text = (char *)sCheatMenuText_WarpMotherBoss; break;
    case 9: o->text = (char *)sCheatMenuText_WarpPrimagenBoss; break;
    case 10: o->text = (char *)sCheatMenuText_WarpCredits; break;
    }
    return 0;
}

/* A time/score threshold per setting index. */
s32 CheatMenu_GetWarpLevel(void) {
    switch ((u32) sCheatsMenuSelection) {
    case 0:
    default:
        return 1000;
    case 1:
        return 2000;
    case 2:
        return 3000;
    case 3:
        return 4000;
    case 4:
        return 5000;
    case 5:
        return 6000;
    case 6:
        return 7401;
    case 7:
        return 7501;
    case 8:
        return 7601;
    case 9:
        return 7602;
    case 10:
        return 8000;
    }
}

s32 CheatMenu_ToggleInvincibility(void) {
    gEngineApp_m_Settings_cheatFlags ^= 1;
    return 0;
}

#define T2_TEXT(s) ((void *) (s))
#ifdef TARGET_PORT
#else
#endif

s32 CheatMenu_InvincibilityText(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 1) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 1) {
        arg0->text = T2_TEXT(sCheatMenuText_InvincibilityOn);
    } else {
        arg0->text = T2_TEXT(sCheatMenuText_InvincibilityOff);
    }
    return 0;
}

s32 CheatMenu_ToggleInfiniteLives(void) {
    gEngineApp_m_Settings_cheatFlags ^= 2;
    return 0;
}

s32 CheatMenu_InfiniteLivesText(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 2) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 2) {
        arg0->text = T2_TEXT(sCheatMenuText_InfiniteLivesOn);
    } else {
        arg0->text = T2_TEXT(sCheatMenuText_InfiniteLivesOff);
    }
    return 0;
}

s32 CheatMenu_ToggleAllWeapons(void) {
    gEngineApp_m_Settings_cheatFlags ^= 4;
    return 0;
}

s32 CheatMenu_AllWeaponsText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x4) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x4) {
        item->unk14 = sCheatMenuText_AllWeaponsOn;
    } else {
        item->unk14 = sCheatMenuText_AllWeaponsOff;
    }
    return 0;
}

s32 CheatMenu_ToggleUnlimitedAmmo(void) {
    gEngineApp_m_Settings_cheatFlags ^= 8;
    return 0;
}

s32 CheatMenu_UnlimitedAmmoText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x8) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x8) {
        item->unk14 = sCheatMenuText_UnlimitedAmmoOn;
    } else {
        item->unk14 = sCheatMenuText_UnlimitedAmmoOff;
    }
    return 0;
}

s32 CheatMenu_ToggleAllSpecialObjects(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x10;
    return 0;
}

s32 CheatMenu_AllSpecialObjectsText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x10) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        item->unk14 = sCheatMenuText_AllSpecialObjectsOn;
    } else {
        item->unk14 = sCheatMenuText_AllSpecialObjectsOff;
    }
    return 0;
}

s32 CheatMenu_ToggleBigHeadMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x20;
    return 0;
}

s32 CheatMenu_BigHeadModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x20) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x20) {
        item->unk14 = sCheatMenuText_BigHeadModeOn;
    } else {
        item->unk14 = sCheatMenuText_BigHeadModeOff;
    }
    return 0;
}

s32 CheatMenu_ToggleTinyMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x40;
    return 0;
}

s32 CheatMenu_TinyModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x40) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x40) {
        item->unk14 = sCheatMenuText_TinyModeOn;
    } else {
        item->unk14 = sCheatMenuText_TinyModeOff;
    }
    return 0;
}

s32 CheatMenu_ToggleStickMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x80;
    return 0;
}

s32 CheatMenu_StickModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x80) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x80) {
        item->unk14 = sCheatMenuText_StickModeOn;
    } else {
        item->unk14 = sCheatMenuText_StickModeOff;
    }
    return 0;
}

s32 CheatMenu_ToggleBigHandsFeetMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x100;
    return 0;
}

s32 CheatMenu_BigHandsFeetModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x100) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x100) {
        item->unk14 = sCheatMenuText_BigHandsFeetModeOn;
    } else {
        item->unk14 = sCheatMenuText_BigHandsFeetModeOff;
    }
    return 0;
}

s32 CheatMenu_TogglePenAndInkMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x200;
    return 0;
}

s32 CheatMenu_PenAndInkModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x200) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x200) {
        item->unk14 = sCheatMenuText_PenAndInkModeOn;
    } else {
        item->unk14 = sCheatMenuText_PenAndInkModeOff;
    }
    return 0;
}

s32 CheatMenu_ToggleGouraudMode(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x400;
    return 0;
}

s32 CheatMenu_GouraudModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x400) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x400) {
        item->unk14 = sCheatMenuText_GouraudModeOn;
    } else {
        item->unk14 = sCheatMenuText_GouraudModeOff;
    }
    return 0;
}

s32 CheatMenu_ToggleJuansCheat(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x800;
    return 0;
}

s32 CheatMenu_JuansCheatText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x800) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x800) {
        item->unk14 = sCheatMenuText_JuansCheatOn;
    } else {
        item->unk14 = sCheatMenuText_JuansCheatOff;
    }
    return 0;
}

s32 CheatMenu_ToggleZachAttack(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x1000;
    return 0;
}

s32 CheatMenu_ZachAttackText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x1000) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x1000) {
        item->unk14 = sCheatMenuText_ZachAttackCheatOn;
    } else {
        item->unk14 = sCheatMenuText_ZachAttackCheatOff;
    }
    return 0;
}

s32 CheatMenu_ToggleAllMap(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x2000;
    return 0;
}

/* On/off option item for bit 0x2000: shown only when unlocked. */
s32 CheatMenu_AllMapText(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x2000) {
        a->flags |= 0x1000000;
    } else {
        a->flags &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x2000) {
        a->text = sCheatMenuText_AllMapOn;
    } else {
        a->text = sCheatMenuText_AllMapOff;
    }
    return 0;
}

s32 CheatMenu_ToggleBlackout(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x4000;
    return 0;
}

/* On/off option item for bit 0x4000: shown only when unlocked. */
s32 CheatMenu_BlackoutText(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x4000) {
        a->flags |= 0x1000000;
    } else {
        a->flags &= ~0x1000000;
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        a->text = sCheatMenuText_BlackoutOn;
    } else {
        a->text = sCheatMenuText_BlackoutOff;
    }
    return 0;
}

s32 CheatMenu_ToggleFrootyStripes(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x8000;
    return 0;
}

s32 CheatMenu_FrootyStripesText(MenuItem *item)
{
  s32 var_v0;
  int new_var;
  if (gEngineApp_m_Settings.unlockedCheats & 0x8000)
  {
 new_var = 0x01000000; do { var_v0 = item->flags | new_var; } while (0);
  }
  else
  {
    var_v0 = item->flags & 0xFEFFFFFF;
  }
  *(s32 *)&item->flags = var_v0; /* not a member store: then it may alias gEngineApp_m_Settings and stays first, as in the ROM */
  if (gEngineApp_m_Settings.cheatFlags & 0x8000)
  {
    item->text = sCheatMenuText_FrootyStripesOn;
  }
  else
  {
    item->text = sCheatMenuText_FrootyStripesOff;
  }
  return 0;
}
