#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
} Quad; /* passed by value: starts in $a3, the rest on the stack */

extern void Sound__PlaySpeech(s32);

#ifdef TARGET_PORT
#include "structs/vector.h"
/* ABI (docs/port/ABI.md section 5, abicheck indirect struct_class/struct_scalar): the dispatcher
 * (ImpactEvent_Send/Call) passes (actor, actor, kind, CVector3 pos, 8-byte event value). On MIPS
 * the position and the value fill a3 and the stack words this definition names q (q.xC: the value); on the host
 * the vector travels in SSE/FP registers and the value in one general register, so the host
 * definition takes them as they are passed: the value's first word is what MIPS read. */
void ImpactEvent_PlaySound(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, s32 value) {
    Sound__PlaySpeech(value);
}
#else
void ImpactEvent_PlaySound(s32 arg0, s32 arg1, s32 arg2, Quad q) {
    Sound__PlaySpeech(q.xC);
}
#endif
