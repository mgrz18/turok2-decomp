#include "common.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu_desc.h"
#include "protos/player.h"

/* Reset the current player state and open a menu page on the player's controller. */

void Pak_ClearDamagedContinued_2(void);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *controller, sptr flags);

s32 StatsMenu_SaveProfile_2(s32 arg0, Menu *event) {
    u8 *player = (u8 *) event->player;

    CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    Pak_ClearDamagedContinued_2();
    MenuPool_OpenMenu_2(gEngineApp_m_Game.m_MenuPool, &gPakSaveProfileMenuDesc_2, player, *(void **)(player + 0xA50), 0);
    return 1;
}

#include "structs/player.h"

s32 StatsMenu_Done_2(s32 arg0, Menu *arg1) {
    ((CPlayer *) arg1->player)->m_pOptions->unk_0x3D = 1;
    return 1;
}

s32 CPlayer__StartGame_2(void *player);                         /* extern */

s32 StatsMenu_ContinueYes_2(s32 menu, Menu *c) {
    CPlayer__StartGame_2(c->player);
    return 1;
}

typedef struct RenderCounters {
    u8 pad0[0x34];
    s32 option34;                /* 0x34 (0x54) */
    u8 pad38[8];
    s32 unk40;                   /* 0x40 (0x74) */
} RenderCounters;
typedef struct Shared_Game {
    u8 pad0[0x5D8];
    RenderCounters counters;     /* 0x5D8 */
} Shared_Game;
typedef struct GameSession GameSession;
extern int gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern void CEngineApp__QuitToLegal_2(GameSession *);

int StatsMenu_ContinueNo_2(int unused, Menu *menu) {
    RenderCounters *sync = (RenderCounters *) &gEngineApp_m_Settings.match;
    int team;
    if (sync->option34 != 0) {
        team = menu->player->m_pOptions->m_TagMode;
        if (team == 1) {
            sync->unk40 = team;
        }
    }
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) {
        CEngineApp__QuitToLegal_2(&gEngineApp);
    } else {
        menu->player->m_bActive = 0;
    }
    return 1;
}

s32 StatsMenu_IsMatchOver_2(void)
{
  s8 *new_var;
  s32 var_v1;
  new_var = (s8 *) (&gEngineApp_m_Settings.match);
  var_v1 = 0;
  if (((*((s32 *) (new_var + 0x1C))) != 0) || ((*((s32 *) (new_var + 0x20))) != 0))
  {
    var_v1 = 1;
  }
  return var_v1;
}

#include "globals/menu_text.h"

struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};

s32 StatsMenu_HeaderText_2(struct FieldRecAt14 *field) {
    if (gEngineApp_m_Settings.match.tag != 0) {
        field->text = sStatsMenuText_YouTagged;
    } else {
        field->text = sStatsMenuText_YouFragged;
    }
    return 0;
}

/* Menu text item: print one u16 stat of the current player's record (64-byte
 * records, field at 0x8011AB5E) at the end of the item's text. */


s32 MenuItem_TextLength_2(MenuItem *item);
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
void sprintf(char *dst, const char *fmt, ...);

s32 StatsMenu_TagsText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Tags;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), sStatsMenuFmt_04d, value);
    return 0;
}

s32 StatsMenu_FragsText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Kills;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), sStatsMenuFmt_04d, value);
    return 0;
}

s32 StatsMenu_DeathsText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Deaths;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), sStatsMenuFmt_04d, value);
    return 0;
}

#include "structs/player_options.h"

s32 StatsMenu_SuicidesText_2(MenuItem *item, Menu *event) {
    s32 value = gEngineApp_m_Settings.slots[CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, event->player)].m_Suicides;

    sprintf(item->text + (MenuItem_TextLength_2(item) - 4), sStatsMenuFmt_04d, value);
    return 0;
}

s32 StatsMenu_PlayerRowText_2(MenuItem *item, Menu *event) {
    s32 other = item->index - 2;
    u8 *player = (u8 *) event->player;
    s32 value;
    char *dst;
    char *src;
    s32 i;

    if (gEngineApp_m_Settings.match.tag != 0) {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Tagged[other];
    } else {
        value = ((CPlayerOptions *)*(void **)(player + 0x518))->m_Killed[other];
    }
    dst = item->text;
    src = gEngineApp_m_Settings.slots[other].m_Name;
    for (i = 0; i < 8; i++) {
        *dst++ = *src++;
    }
    sprintf(item->text + (MenuItem_TextLength_2(item) - 2), sStatsMenuFmt_02d, value);
    return 0;
}

typedef struct {
    /* 0x000 */ u8 pad0[0x120];
    /* 0x120 */ s32 flags;
} Target;

void StatsMenu_ShowTagsRow_2(Menu *arg0) {
    Target *temp_a0;
    Target *temp_v0;

    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = (Target *) arg0->items;
        temp_v0->flags = (s32) (temp_v0->flags | 0x01000000);
        return;
    }
    temp_a0 = (Target *) arg0->items;
    temp_a0->flags = (s32) (temp_a0->flags & 0xFEFFFFFF);
}

/* The window's item records are 0x28 bytes apart, item i's flags word at
 * 0x120 + i * 0x28 of the block. */
#define ITEM_FLAGS(i) (0x120 + (i) * 0x28)

void StatsMenu_InitContinue_2(Menu *arg0) {
    void *temp_a0;
    void *temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    void *temp_v1_3;
    void *temp_v1_4;
    void *temp_v1_5;
    void *temp_v1_6;

    temp_v1 = arg0->player;
    if (temp_v1 != NULL) {
        ((CPlayer *) temp_v1)->m_bEnteringDeathmatch = 1;
    }
    temp_v1_2 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_2 + ITEM_FLAGS(4)) = (s32) (*(s32 *) ((s8 *) temp_v1_2 + ITEM_FLAGS(4)) | 0x01800000);
    temp_v1_3 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_3 + ITEM_FLAGS(5)) = (s32) (*(s32 *) ((s8 *) temp_v1_3 + ITEM_FLAGS(5)) | 0x01800000);
    temp_v1_4 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_4 + ITEM_FLAGS(6)) = (s32) (*(s32 *) ((s8 *) temp_v1_4 + ITEM_FLAGS(6)) | 0x01800000);
    temp_v1_5 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_5 + ITEM_FLAGS(7)) = (s32) (*(s32 *) ((s8 *) temp_v1_5 + ITEM_FLAGS(7)) & 0xFE7FFFFF);
    temp_v1_6 = arg0->items;
    *(s32 *) ((s8 *) temp_v1_6 + ITEM_FLAGS(8)) = (s32) (*(s32 *) ((s8 *) temp_v1_6 + ITEM_FLAGS(8)) & 0xFE7FFFFF);
    if (gEngineApp_m_Settings.match.tag != 0) {
        temp_v0 = arg0->items;
        *(s32 *) ((s8 *) temp_v0 + ITEM_FLAGS(0)) = (s32) (*(s32 *) ((s8 *) temp_v0 + ITEM_FLAGS(0)) | 0x01000000);
        return;
    }
    temp_a0 = arg0->items;
    *(s32 *) ((s8 *) temp_a0 + ITEM_FLAGS(0)) = (s32) (*(s32 *) ((s8 *) temp_a0 + ITEM_FLAGS(0)) & 0xFEFFFFFF);
}

#include "globals/pak.h"

extern void Controller_WatchPak(s32 a);
extern void Pak_ProbeSlot_2(s32 a);

void StatsMenu_InitSaveDone_2(Menu *a) {
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
    Pak_ProbeSlot_2(n);
}

void StatsMenu_InitAfterPak_2(Menu *a) {
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
