/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0020612C.c, func_0020612C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
typedef struct B01Info {
    u32 flags;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    u8 padC[0x8];
    s16 unk14;
    s16 unk16;
} B01Info;
void func_00243414(CGameObjectInstance *actor, CAI *state, s32 event);
void func_00263640(CGameObjectInstance *actor, CAI *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->header.m_pIntelligence) + 0x14));
    if (info->unk4 == 0) {
        if (info->unk14 == -1) {
            state->m_ModeTime = 0.0f;
            return;
        }
        if (info->unk16 == -1) {
            state->m_ModeTime = 0.0f;
            return;
        }
    }
    if (state->m_VarS32[0] == info->unkA) {
        state->m_ModeTime = 0.0f;
        return;
    }
    if ((info->flags & 1) || state->m_VarS32[1] > 0) {
        if ((info->flags & 2) && (actor->m_dwFlags & 0x200)) {
            state->m_ModeTime = 0.0f;
            return;
        }
        if (state->m_ModeTime >= state->m_ConditionTimer) {
            func_00243414(actor, state, 1);
        }
    }
}
