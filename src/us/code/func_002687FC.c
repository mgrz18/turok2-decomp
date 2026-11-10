/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E73C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef float f32;
struct FloatState3C;
typedef struct FloatState3C FloatState3C;
struct FloatState3C;
struct FloatState3C {
    unsigned char padding_0[56];
    f32 unk_38;
};
#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"

/* The port: the AI at its host offset (N64 0x140), the turn by member (N64 buffer + 0x38). */
void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o);
f32 GameObject_GetFrameTurn(u8 *arg0) {
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        return motion.m_Turn;
    }
    return 0.0f;
}
#else
extern void AI_ComputeFrameMotion(void *arg0, void *arg1, void *arg2);
f32 GameObject_GetFrameTurn(u8 *arg0) {
    char buf[0x40];
    if (*arg0 == 1) {
        AI_ComputeFrameMotion(arg0, arg0 + 0x140, buf);
        return ((struct FloatState3C *) buf)->unk_38;
    }
    return 0.0f;
}
#endif
