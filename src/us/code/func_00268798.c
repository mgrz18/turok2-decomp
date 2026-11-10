/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8024E130_3.CPP, func_8024E6D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"

/* The port: the AI at its host offset (N64 0x140) and the whole CAIFrameMotion as the buffer
 * (the N64 code passes a 12-byte Triple that AI_ComputeFrameMotion fills to 0x3C bytes, safe only
 * in the MIPS frame layout: on the host it overwrote the caller's frame). */
void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o);
void GameObject_GetFrameDelta(u8 *arg0, void *arg1) {
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        *(CVector3 *) arg1 = motion.m_vDelta;
        return;
    }
    ((CVector3 *) arg1)->x = 0.0f;
    ((CVector3 *) arg1)->y = 0.0f;
    ((CVector3 *) arg1)->z = 0.0f;
}
#else
extern void AI_ComputeFrameMotion(void *arg0, void *arg1, void *arg2);
void GameObject_GetFrameDelta(u8 *arg0, void *arg1) {
    Triple buf;
    char pad[0x30];
    if (*arg0 == 1) {
        AI_ComputeFrameMotion(arg0, arg0 + 0x140, &buf);
        *(Triple *)arg1 = buf;
        return;
    }
    *(s32 *)arg1 = 0;
    ((Triple *)arg1)->y = 0;
    ((Triple *)arg1)->z = 0;
}
#endif
