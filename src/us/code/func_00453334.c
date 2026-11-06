/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DCE4.c, func_0043DCE4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"
#include "structs/game.h"

extern s32 gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount;
extern void func_00469250(void *a);
s32 ContinueMenu_No_2(s32 a, Menu *b) {
    if (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_nCount == 1) func_00469250(&gEngineApp); else b->player->m_bActive = 0;
    return 1;
}
