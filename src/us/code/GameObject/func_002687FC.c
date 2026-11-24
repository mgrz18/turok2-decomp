/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E73C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"

void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o);

/* The turn a live object made this frame (0 for the others). Round 28 joined the N64 view and the
 * port's (the turn by member, N64 buffer + 0x38). */
f32 GameObject_GetFrameTurn(CGameObjectInstance *obj) {
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        return motion.m_Turn;
    }
    return 0.0f;
}
