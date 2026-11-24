/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E6D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"

void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o);

/* The distance a live object moved this frame (zero for the others). Round 28 joined the N64 view
 * (a 12-byte Triple that AI_ComputeFrameMotion fills to 0x3C bytes) and the port's (the whole
 * CAIFrameMotion: on the host the Triple overwrote the caller's frame). */
void GameObject_GetFrameDelta(CGameObjectInstance *obj, CVector3 *delta) {
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        *delta = motion.m_vDelta;
        return;
    }
    delta->x = 0.0f;
    delta->y = 0.0f;
    delta->z = 0.0f;
}
