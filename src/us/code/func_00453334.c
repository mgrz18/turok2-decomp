/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DCE4.c, func_0043DCE4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/menu.h"
#include "structs/player.h"

extern s32 D_801198A0;
extern u8 gEngineApp[];
extern void func_00430250(void *a);
s32 ContinueMenu_No_2(s32 a, Menu *b) {
    if (D_801198A0 == 1) func_00430250(gEngineApp); else b->player->m_bActive = 0;
    return 1;
}
