/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443E44.c, func_00443E44.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/pak.h"
#include "protos/player.h"

extern void Controller_WatchPak(s32 a);
extern void Pak_ProbeSlot(s32 a);
extern s32 gEngineApp_m_Settings_match_tag[]; /* local type: GameMatch.tag read as an array (the ROM bases the load on a register); so globals/game.h is not included here */
extern CGame gEngineApp_m_Game; /* local: see above (globals/game.h) */
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
