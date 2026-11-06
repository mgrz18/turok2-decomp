/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00206604.c, func_00206604.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"
typedef struct { u8 p[0x1D]; u8 f; } GFx;
extern GFx D_8011AAD4;
void InteractiveAnim_Step(CGameObjectInstance *actor, CAI *state);
void InteractiveAnim_DrawAllModels(CGameObjectInstance *actor, CAI *state);
s32 Cinema_GetScriptId(void);
void InteractiveAnim_Setup(CGameObjectInstance *actor, CAI *state) {
    state->m_pModeTable = gInteractiveAnimModeTable;
    state->m_pfStep = InteractiveAnim_Step;
    actor->m_dwFlags |= 0x1000000;
    actor->m_dwFlags |= 0x2000000;
    actor->m_dwFlags |= 0x20000;
    if (D_8011AAD4.f == 0) {
        actor->m_dwFlags |= 0x10000000;
    }
    if (actor->m_nTypeFlag == 0x453 && Cinema_GetScriptId() == 0x6F) {
        state->m_pfDraw = InteractiveAnim_DrawAllModels;
    }
}
