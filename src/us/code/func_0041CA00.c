/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

typedef struct RenderCounters {
    char pad0[0x34];
    s32 option34;              /* 0x34 (Rage Wars 0x54) */
    char pad38[0x40 - 0x38];
    s32 unk40;                 /* 0x40 (Rage Wars 0x74) */
} RenderCounters;

typedef struct Shared_Game {
    char pad0[0x5D8];
    RenderCounters counters;   /* 0x5D8 */
} Shared_Game;

extern Shared_Game D_8011A6D8;
extern int gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount; /* local type: the field's real type (or the member path) changes the ROM's code here */
extern char gEngineApp[];
extern void func_00430330(char *);

int StatsMenu_ContinueNo(int unused, Menu *menu) {
    RenderCounters *sync = &D_8011A6D8.counters;
    int team;
    if (sync->option34 != 0) {
        team = menu->player->m_pOptions->m_TagMode;
        if (team == 1) {
            sync->unk40 = team;
        }
    }
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) {
        func_00430330(gEngineApp);
    } else {
        menu->player->m_bActive = 0;
    }
    return 1;
}
