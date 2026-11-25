/* WRONG CUT: the real function is CheatsMenu_Init_2, size 0x10C: splat's CheatsMenu_Init_2 starts with a stray
 * never-executed word (a jal) ahead of the 16-aligned real start. */
#include "common.h"
#include "globals/menu.h"
#include "globals/game.h"

#include "structs/menu.h"

/* Picks the first of the 11 options whose flag is set in the settings (-1 if none). */
void CheatsMenu_Init_2(Menu *menu) {
    s32 index = 0;
    s32 n;
    u32 bit;

    for (n = 0; n < 11; n++) {
        switch (index) {
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
        if (gEngineApp_m_Settings_unlockedCheats & bit) {
            goto found;
        }
        index++;
        if (index > 10) {
            index = 0;
        } else if (index < 0) {
            index = 10;
        }
    }
    index = -1;
found:
    sCheatsMenuSelection = index;
    if (index != -1) {
        menu->items[18].flags |= 0x1800000;
    } else {
        menu->items[18].flags &= ~0x1800000;
    }
}

#include "structs/menu.h"
#include "structs/game.h"
#include "globals/scene.h"
/* Level select (warp) menu input: A warps to the selected level (or, on the
 * last entry, starts the credits/next mode), up and down move the selection
 * to the previous/next level whose key the player owns. */



#define PROGRESS (&gEngineApp_m_Game.m_Settings)

extern s32 Controller_ConfirmPressed(sptr pad);
extern int Controller_MenuLeft(struct ControllerProfile *object);
extern int Controller_MenuRight(struct ControllerProfile *object);
extern void Menu_ClaimPressingController_2(Menu *c);
extern void CEngineApp__SetupFadeTo(void *game, s32 mode);
extern void CEngineApp__StartGameAtWarp(void *game, s32 level);
extern void CScene__RequestWarp_2(void *level, s32 id, s32 how);

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

s32 CheatsMenu_WarpSelect_2(void *menu, Menu *c) {
    s32 id;

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
            PROGRESS->multiplayer = 0;
            PROGRESS->playerCount = 1;
            Menu_ClaimPressingController_2(c);
            CEngineApp__SetupFadeTo(&gEngineApp, 16);
            return 1;
        }
        if (gEngineApp_m_Game.m_Settings.mode == 13) {
            CScene__RequestWarp_2(&gScene, id, 2);
            return 0;
        }
        PROGRESS->multiplayer = 0;
        PROGRESS->playerCount = 1;
        Menu_ClaimPressingController_2(c);
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

void EnterCheatMenu_Init_2(void) {
    sEnterCheatResult = -1;
}

#include "globals/render.h"

/* Text entry item: draws the item, with a blinking 2-pixel cursor bar under the character being edited
 * (sibling of NameEntryMenu_DrawName_2, for a single entry). */


void Hud_SetupFillMode(void);
void Hud_DrawBevelBox(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, s32 invert, s32 red, s32 green, s32 blue, s32 alpha);
void MenuItem_DrawText_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event);

void EnterCheatMenu_DrawTextField_2(void *menu, MenuItemLayout *item, void *arg2, Menu *event) {
    f32 charWidth;
    f32 alpha;
    s32 x;

    if (gCheatEntry.timer <= 6.0f) {
        charWidth = (gScreenWidth * 11) / 284;
        x = item->left + gCheatEntry.cursor * charWidth;
        alpha = event->fade * (event->alpha * 128.0f);
        gMenuCurrentFont = -1;
        Hud_SetupFillMode();
        Hud_DrawBevelBox(x, item->bottom, x + charWidth, item->bottom + 2, 0, 0, 255, 255, 255, (u32)alpha & 0xFF);
    }
    MenuItem_DrawText_2(menu, item, arg2, event);
}

#include "structs/vector.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/list.h"
#include "globals/engine.h"

extern int Controller_MenuLeft(ControllerProfile *object);
extern int Controller_MenuRight(ControllerProfile *object);
#include "rw/abu_types.h"
/* the menu by its host layout (round 10 y4): the view's s32 input at N64 0x20 is
 * Menu.controller, a pointer at another host offset, and the call cast it to s32 (macOS, statics
 * above 4 GB: Controller_MenuLeft crashed when a soak run opened the cheat entry; Linux non-PIE
 * statics fit in 32 bits) */
#define T2_MENU_INPUT(m) (((Menu *) (m))->controller)
#define T2_MENU_LEFT(m) Controller_MenuLeft(T2_MENU_INPUT(m))
#define T2_MENU_RIGHT(m) Controller_MenuRight(T2_MENU_INPUT(m))

s32 EnterCheatMenu_UpdateTextField_2(void *unused, MenuRecAt20V4 *menu) {
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

s32 EnterCheatMenu_TextFieldSelect_2(void) {
    return 0;
}

typedef struct {
    /* 0x00 */ char pad0[0x14];
    /* 0x14 */ u8 *chr;
} ObjRecAt14;

/* Inserts the selected character at the cursor of the name entry. */
s32 EnterCheatMenu_InsertLetter_2(ObjRecAt14 *arg0) {
    s32 index;
    s32 count;

    sEnterCheatResult = -1;
    gCheatEntry.timer = 0;
    if (gCheatEntry.length == 0x16) {
        return 0;
    }
    index = 0x16;
    if (gCheatEntry.cursor < 0x16) {
        do {
            gCheatEntry.text[index] = gCheatEntry.text[index - 1];
            index--;
        } while (gCheatEntry.cursor < index);
    }
    gCheatEntry.text[gCheatEntry.cursor] = *arg0->chr;
    count = gCheatEntry.length + 1;
    gCheatEntry.cursor++;
    gCheatEntry.length = count;
    gCheatEntry.text[count] = 0;
    return 0;
}

s32 EnterCheatMenu_Backspace_2(void) {
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

s32 EnterCheatMenu_MatchesCode_2(u8 *arg0) {
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

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU_2/80408AC4_41.CPP, func_8043CDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

GameSettings;
s32 Sound__Play2D(s32 arg0);
extern s32 strlen(u8 *);

s32 EnterCheatMenu_Enter_2(void)
{
    u32 i;
    s32 j;
    s32 match;
    s32 hasSound;
    GameSettings *settings;
    u8 *code;
    sEnterCheatResult = -1;
    for (i = 0; i < 12; i++) {
        code = sCheatCodes_2[i].code;
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
            gEngineApp_m_Settings.unlockedCheats |= sCheatCodes_2[i].flags;
            settings = &gEngineApp_m_Settings;
            do {
                gScene_m_PreviewOptions = settings->unlockedCheats;
            } while (0);
            hasSound = sCheatCodes_2[i].sound != 0;
            sEnterCheatResult = i;
            if (hasSound) {
                Sound__Play2D((s16)sCheatCodes_2[i].sound);
            }
        }
    }
    return 0;
}

/* The cheat keyboard's labels (see vm1's EnterCheatMenu_ResultLabel). */
extern u8 sEnterCheatKeyLabels[]; /* local: globals/menu_text.h is included further down */

s32 EnterCheatMenu_ResultLabel_2(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = (s32 *) sCheatCodes_2[sEnterCheatResult].unk4;
    } else {
        arg0->table = (s32 *) sEnterCheatKeyLabels;
    }
    return 0;
}

s32 Cheat_FindWarpLevel_2(s32 index, s32 step, s32 mode) {
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

s32 CheatsMenu_SetToggle_2(struct Func8043D458Arg *arg0, s32 arg1, s32 *arg2, s32 *arg3) {
    if (gEngineApp_m_Settings.unlockedCheats & arg1) arg0->flags |= 0x01000000;
    else arg0->flags &= 0xFEFFFFFF;
    if (gEngineApp_m_Settings.cheatFlags & arg1) arg0->text = arg2;
    else arg0->text = arg3;
    return 0;
}

s32 CheatMenu_ToggleFlag_2(s32 arg0, s32 arg1) {
    gEngineApp_m_Settings.cheatFlags ^= arg1;
    return 0;
}

#include "globals/menu_text.h"

s32 CheatMenu_WarpText_2(MenuItem *o) {
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

/* The level of the selected option. */
s32 CheatMenu_GetWarpLevel_2(void) {
    switch (sCheatsMenuSelection) {
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

s32 CheatMenu_ToggleInvincibility_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 1;
    return 0;
}

struct TextBoundsText;
struct TextBoundsDescriptor;
typedef struct TextBoundsDescriptor TextBoundsDescriptor;
struct TextBoundsMetrics;
typedef struct TextBoundsMetrics TextBoundsMetrics;
typedef struct TextBoundsText TextBoundsText;
struct TextBoundsDescriptor {
    char pad0[0x44];
    s8 * unk_44;
};
struct TextBoundsMetrics {
    s32 field0;
    s32 width;
    s32 height;
    f32 scaleX;
    f32 scaleY;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
};
struct TextBoundsHandlers {
    char pad0[0x14];
    void (*draw)(TextBoundsText *, TextBoundsMetrics *, TextBoundsMetrics *, void *);
};
struct TextBoundsText {
    s32 value;
    s16 mode;
    s16 reserved;
    s32 flags;
    s16 x;
    s16 y;
    s32 field10;
    void *text;
    struct TextBoundsHandlers *field18;
    s32 spacing;
    s32 field20;
    struct TextBoundsDescriptor *descriptor;
};

s32 CheatMenu_InvincibilityText_2(TextBoundsText *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 1) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 1) {
        arg0->text = &sCheatMenuText_InvincibilityOn;
    } else {
        arg0->text = &sCheatMenuText_InvincibilityOff;
    }
    return 0;
}

s32 CheatMenu_ToggleInfiniteLives_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 2;
    return 0;
}

s32 CheatMenu_InfiniteLivesText_2(struct Func8043D458Arg *arg0) {
    if (gEngineApp_m_Settings.unlockedCheats & 2) {
        arg0->flags |= 0x01000000;
    } else {
        arg0->flags &= 0xFEFFFFFF;
    }
    if (gEngineApp_m_Settings.cheatFlags & 2) {
        arg0->text = &sCheatMenuText_InfiniteLivesOn;
    } else {
        arg0->text = &sCheatMenuText_InfiniteLivesOff;
    }
    return 0;
}

s32 CheatMenu_ToggleAllWeapons_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 4;
    return 0;
}

s32 CheatMenu_AllWeaponsText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 4) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 4) a->text = sCheatMenuText_AllWeaponsOn; else a->text = sCheatMenuText_AllWeaponsOff;
    return 0;
}

s32 CheatMenu_ToggleUnlimitedAmmo_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 8;
    return 0;
}

s32 CheatMenu_UnlimitedAmmoText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x8) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x8) a->text = sCheatMenuText_UnlimitedAmmoOn; else a->text = sCheatMenuText_UnlimitedAmmoOff;
    return 0;
}

s32 CheatMenu_ToggleAllSpecialObjects_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x10;
    return 0;
}

s32 CheatMenu_AllSpecialObjectsText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x10) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x10) a->text = sCheatMenuText_AllSpecialObjectsOn; else a->text = sCheatMenuText_AllSpecialObjectsOff;
    return 0;
}

s32 CheatMenu_ToggleBigHeadMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x20;
    return 0;
}

s32 CheatMenu_BigHeadModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x20) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x20) a->text = sCheatMenuText_BigHeadModeOn; else a->text = sCheatMenuText_BigHeadModeOff;
    return 0;
}

s32 CheatMenu_ToggleTinyMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x40;
    return 0;
}

s32 CheatMenu_TinyModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x40) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x40) a->text = sCheatMenuText_TinyModeOn; else a->text = sCheatMenuText_TinyModeOff;
    return 0;
}

s32 CheatMenu_ToggleStickMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x80;
    return 0;
}

s32 CheatMenu_StickModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x80) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x80) a->text = sCheatMenuText_StickModeOn; else a->text = sCheatMenuText_StickModeOff;
    return 0;
}

s32 CheatMenu_ToggleBigHandsFeetMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x100;
    return 0;
}

s32 CheatMenu_BigHandsFeetModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x100) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x100) a->text = sCheatMenuText_BigHandsFeetModeOn; else a->text = sCheatMenuText_BigHandsFeetModeOff;
    return 0;
}

s32 CheatMenu_TogglePenAndInkMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x200;
    return 0;
}

s32 CheatMenu_PenAndInkModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x200) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x200) a->text = sCheatMenuText_PenAndInkModeOn; else a->text = sCheatMenuText_PenAndInkModeOff;
    return 0;
}

s32 CheatMenu_ToggleGouraudMode_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x400;
    return 0;
}

s32 CheatMenu_GouraudModeText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x400) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x400) a->text = sCheatMenuText_GouraudModeOn; else a->text = sCheatMenuText_GouraudModeOff;
    return 0;
}

s32 CheatMenu_ToggleJuansCheat_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x800;
    return 0;
}

s32 CheatMenu_JuansCheatText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x800) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x800) a->text = sCheatMenuText_JuansCheatOn; else a->text = sCheatMenuText_JuansCheatOff;
    return 0;
}

s32 CheatMenu_ToggleZachAttack_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x1000;
    return 0;
}

s32 CheatMenu_ZachAttackText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x1000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x1000) a->text = sCheatMenuText_ZachAttackCheatOn; else a->text = sCheatMenuText_ZachAttackCheatOff;
    return 0;
}

s32 CheatMenu_ToggleAllMap_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x2000;
    return 0;
}

s32 CheatMenu_AllMapText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x2000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x2000) a->text = sCheatMenuText_AllMapOn; else a->text = sCheatMenuText_AllMapOff;
    return 0;
}

s32 CheatMenu_ToggleBlackout_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x4000;
    return 0;
}

s32 CheatMenu_BlackoutText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x4000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) a->text = sCheatMenuText_BlackoutOn; else a->text = sCheatMenuText_BlackoutOff;
    return 0;
}

s32 CheatMenu_ToggleFrootyStripes_2(void) {
    gEngineApp_m_Settings_cheatFlags ^= 0x8000;
    return 0;
}

s32 CheatMenu_FrootyStripesText_2(MenuItem *a) {
    if (gEngineApp_m_Settings.unlockedCheats & 0x8000) a->flags |= 0x1000000; else a->flags &= ~0x1000000;
    if (gEngineApp_m_Settings.cheatFlags & 0x8000) a->text = sCheatMenuText_FrootyStripesOn; else a->text = sCheatMenuText_FrootyStripesOff;
    return 0;
}
