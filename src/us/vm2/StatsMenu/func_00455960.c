/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "structs/game.h"
#include "globals/game.h"

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
