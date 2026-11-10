/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_3.c, func_80267DB4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/camera.h"
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif


extern s32 CCameraPool__StartShake(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, Triple arg6);
#ifdef TARGET_PORT
/* Host body (round 7 v2): an event's 8-byte value reaches the handler as two host-order
 * words (the archive swaps the animation events' values as two u32: struct IntPair), word 0
 * the N64's first big-endian word; the bytes/halves this handler reads are taken from them. */
typedef struct { u32 w0, w1; } T2EventWords;

void func_00238958(s32 arg0, s32 arg1, s32 arg2, Triple t, T2EventWords v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) (s16) (v.w0 >> 16), (f32) (s16) v.w0,
                            (f32) (s16) (v.w1 >> 16), (f32) (s16) v.w1 * 10.24f, 1, t);
}
#else
void func_00238958(s32 arg0, s32 arg1, s32 arg2, Triple t, Vec4s v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) v.x, (f32) v.y, (f32) v.z, (f32) v.w * 10.24f, 1, t);
}
#endif
