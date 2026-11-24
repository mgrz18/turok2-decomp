/* FAKEMATCH (round 22): the `if (state)` with two arms that differ only in statement order (state is never
 * NULL) and the goto into the else; without them (one arm, the plain if/else on the typed definition)
 * Generator_Advance keeps m_pIntelligence and the body pointer in two $s registers: 64-66 of 66 words. */
#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/game.h"
#include "protos/ai.h"
#include "structs/word.h"
#include "structs/generator.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205F18.c, func_00205F18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 Random_Blend(f32 a, f32 b);
void Generator_Advance(CGameObjectInstance *actor, CAI *state) {
    CGameObjectInstance *a2;
    B01Info *new_var;
    B01Info *info;
    a2 = actor;
    new_var = (B01Info *)(((u8 *)a2->header.m_pIntelligence) + 0x14);
    if (!(a2->header.m_pIntelligence->m_dwTypeFlags & 0x10) || (gEngineApp_m_Settings.multiplayer != 0 && gEngineApp_m_Settings.playerCount == 1)) {
        if (state) {
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            info = new_var;
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        } else {
            info = new_var;
            state->m_ConditionTimer = Random_Blend(45.0f, 60.0f);
            state->m_VarS32[0] = 0;
            state->m_VarS32[1] = info->unk8;
        }
        if (a2->m_nTypeFlag == 0x644 || (info->flags & 8)) {
            a2->m_dwFlags &= ~0x2000;
        }
        CAI__SetMode(a2, state, 0);
        if ((info->flags & 0x10) && gEngineApp_m_Settings.playerCount != 1) {
            goto clear;
        }
    } else {
clear:
        a2->m_dwFlags &= ~0x100;
    }
}
