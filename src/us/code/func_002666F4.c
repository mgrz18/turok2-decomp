/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00206604.c, func_00206604.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
typedef struct { u8 p[0x1D]; u8 f; } GFx;
extern GFx D_8011AAD4;
extern u8 D_800B2510[];
void D_00266814(CGameObjectInstance *actor, CAI *state);
void D_00266AB4(CGameObjectInstance *actor, CAI *state);
s32 func_00284408(void);
void func_002666F4(CGameObjectInstance *actor, CAI *state) {
    state->m_pModeTable = D_800B2510;
    state->m_pfStep = D_00266814;
    actor->m_dwFlags |= 0x1000000;
    actor->m_dwFlags |= 0x2000000;
    actor->m_dwFlags |= 0x20000;
    if (D_8011AAD4.f == 0) {
        actor->m_dwFlags |= 0x10000000;
    }
    if (actor->m_nTypeFlag == 0x453 && func_00284408() == 0x6F) {
        state->m_pfDraw = D_00266AB4;
    }
}
