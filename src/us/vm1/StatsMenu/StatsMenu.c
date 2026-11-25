#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
#include "protos/player.h"
#include "structs/menu.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "globals/menu_text.h"
#include "structs/player_options.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443E44.c, func_00443E44.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/pak.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443F40.c, func_00443F40.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xA50];
    /* 0xA50 */ void *controller;
} C960Player;
typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ C960Player *player;
} C960Arg;
extern void Pak_ClearDamagedContinued(void);

s32 StatsMenu_SaveProfile(s32 arg0, C960Arg *arg1) {
    C960Player *player;

    player = arg1->player;
    CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    Pak_ClearDamagedContinued();
    MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gPakSaveProfileMenuDesc, player, player->controller, 0);
    return 1;
}

struct Leaf;
struct Leaf {
    char pad[0x3D];
    u8 flag;
};
struct Middle;
struct Middle {
    char pad[0x518]; /* player options pointer (0x5D8 in Rage Wars) */
    struct Leaf *leaf;
};
struct RootRecAt1C;
struct RootRecAt1C {
    char pad[0x1C];
    struct Middle *middle;
};
s32 StatsMenu_Done(void *unused, struct RootRecAt1C *root);

s32 StatsMenu_Done(void *unused, struct RootRecAt1C *root) {
    root->middle->leaf->flag = 1;
    return 1;
}

extern void CPlayer__StartGame(void *player);

s32 StatsMenu_ContinueYes(s32 menu, Menu *c) {
    CPlayer__StartGame(c->player);
    return 1;
}

extern int gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern void CEngineApp__QuitToLegal(char *);

int StatsMenu_ContinueNo(int unused, Menu *menu) {
    GameMatch *sync = &gEngineApp_m_Settings.match;
    int team;
    if (sync->tag != 0) {
        team = menu->player->m_pOptions->m_TagMode;
        if (team == 1) {
            sync->tagPhase = team;
        }
    }
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) {
        CEngineApp__QuitToLegal(&gEngineApp);
    } else {
        menu->player->m_bActive = 0;
    }
    return 1;
}

s32 StatsMenu_IsMatchOver(void)
{
  s32 var_a0;
  s32 *new_var;
  var_a0 = 0;
  new_var = &gEngineApp_m_Settings.match;
  if ((((*((s32 *) (((s8 *) new_var) + 0x28))) != 0) || ((*((s32 *) (((s8 *) new_var) + 0x1C))) != 0)) || ((*((s32 *) (((s8 *) (&gEngineApp_m_Settings.match)) + 0x20))) != 0))
  {
    var_a0 = 1;
  }
  return var_a0;
}

s32 StatsMenu_HeaderText(MenuItem *arg0) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        arg0->text = (char *)sStatsMenuText_YouTagged;
    } else {
        arg0->text = (char *)sStatsMenuText_YouFragged;
    }
    return 0;
}

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the player's tags count into the label. */
s32 StatsMenu_TagsText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Tags;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), "%04d", n);
    return 0;
}

/* Prints the player's kills count into the label. */
s32 StatsMenu_FragsText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Kills;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), sStatsMenuFmt_04d, n);
    return 0;
}

/* Prints the player's deaths count into the label. */
s32 StatsMenu_DeathsText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Deaths;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), sStatsMenuFmt_04d, n);
    return 0;
}

/* Prints the player's suicides count into the label. */
s32 StatsMenu_SuicidesText(MenuItem *a, Menu *c) {
    s32 n = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, c->player)].m_Suicides;

    sprintf(a->text + (MenuItem_TextLength(a) - 4), sStatsMenuFmt_04d, n);
    return 0;
}

/* "<name> <count>" line of the score table, for the slot of item index - 2. */
s32 StatsMenu_PlayerRowText(MenuItem *a, Menu *c) {
    s32 slot = a->index - 2;
    CPlayer *player = c->player;
    s32 n;
    char *src;
    char *dst;
    s32 i;

    if (gEngineApp_m_Settings.match.tag) {
        n = player->m_pOptions->m_Tagged[slot];
    } else {
        n = player->m_pOptions->m_Killed[slot];
    }
    dst = a->text;
    src = (char *)gEngineApp_m_Settings.slots[slot].m_Name;
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    sprintf(a->text + (MenuItem_TextLength(a) - 2), sStatsMenuFmt_02d, n);
    return 0;
}

/* Sets or clears flag 0x01000000 of the item from gEngineApp_m_Settings_match_tag[0]. */
void StatsMenu_ShowTagsRow(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;

    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = arg0->items;
        temp_v0[7].flags = (s32) (temp_v0[7].flags | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    temp_a0[7].flags = (s32) (temp_a0[7].flags & 0xFEFFFFFF);
}

/* Byte-offset field access: member references here would let the
 * scheduler move the gEngineApp_m_Settings_match_tag[0] load and change the register choice. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void StatsMenu_InitContinue(Menu *arg0) {
    MenuItem *temp_a0;
    MenuItem *temp_v0;
    CPlayer *temp_v1;
    MenuItem *temp_v1_2;
    MenuItem *temp_v1_3;
    MenuItem *temp_v1_4;
    MenuItem *temp_v1_5;
    MenuItem *temp_v1_6;

    temp_v1 = arg0->player;
    if (temp_v1 != NULL) {
        temp_v1->m_bEnteringDeathmatch = 1;
    }
    temp_v1_2 = arg0->items;
    FIELD_AT(&temp_v1_2[11], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_2[11], s32, MenuItem, flags) | 0x01800000);
    temp_v1_3 = arg0->items;
    FIELD_AT(&temp_v1_3[12], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_3[12], s32, MenuItem, flags) | 0x01800000);
    temp_v1_4 = arg0->items;
    FIELD_AT(&temp_v1_4[13], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_4[13], s32, MenuItem, flags) | 0x01800000);
    temp_v1_5 = arg0->items;
    FIELD_AT(&temp_v1_5[14], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_5[14], s32, MenuItem, flags) & 0xFE7FFFFF);
    temp_v1_6 = arg0->items;
    FIELD_AT(&temp_v1_6[15], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v1_6[15], s32, MenuItem, flags) & 0xFE7FFFFF);
    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = arg0->items;
        FIELD_AT(&temp_v0[7], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_v0[7], s32, MenuItem, flags) | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    FIELD_AT(&temp_a0[7], s32, MenuItem, flags) = (s32) (FIELD_AT(&temp_a0[7], s32, MenuItem, flags) & 0xFEFFFFFF);
}

extern void Controller_WatchPak(s32 a);
extern void Pak_ProbeSlot(s32 a);

void StatsMenu_InitSaveDone(Menu *a) {
    s32 n;
    a->items[14].flags |= 0x1800000;
    a->items[15].flags |= 0x1800000;
    a->items[11].flags &= ~0x1800000;
    a->items[12].flags &= ~0x1800000;
    a->items[13].flags &= ~0x1800000;
    gPakMenuAnyPak = 0;
    gDamagedPakContinued = 0;
    n = 0;
    if (gEngineApp_m_Settings_match_tag[0]) a->items[7].flags |= 0x1000000; else a->items[7].flags &= ~0x1000000;
    if (a->player) n = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, a->player);
    Controller_WatchPak(n);
    Pak_ProbeSlot(n);
}

void StatsMenu_InitAfterPak(Menu *a) {
    s32 n;
    gDamagedPakContinued = 0;
    n = 0;
    if (a->player) n = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, a->player);
    Controller_WatchPak(n);
    a->items[14].flags |= 0x1800000;
    a->items[15].flags |= 0x1800000;
    a->items[11].flags &= ~0x1800000;
    a->items[12].flags &= ~0x1800000;
    a->items[13].flags &= ~0x1800000;
    if (gEngineApp_m_Settings_match_tag[0]) a->items[7].flags |= 0x1000000; else a->items[7].flags &= ~0x1000000;
}
