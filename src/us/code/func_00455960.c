/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

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

extern Shared_Game D_8011A6D8;
extern int D_801198A0;
extern GameSession gEngineApp;
extern void func_00469250(GameSession *);

int StatsMenu_ContinueNo_2(int unused, Menu *menu) {
    RenderCounters *sync = &D_8011A6D8.counters;
    int team;
    if (sync->option34 != 0) {
        team = menu->player->m_pOptions->m_TagMode;
        if (team == 1) {
            sync->unk40 = team;
        }
    }
    if (D_801198A0 == 1) {
        func_00469250(&gEngineApp);
    } else {
        menu->player->m_bActive = 0;
    }
    return 1;
}
