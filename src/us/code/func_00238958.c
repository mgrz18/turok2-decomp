/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_3.c, func_80267DB4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern MenuMatchGlobals gEngineApp_m_Game_m_CameraPool;
extern s32 CCameraPool__StartShake(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, Triple arg6);
extern f32 D_800A65C8[2];
void func_00238958(s32 arg0, s32 arg1, s32 arg2, Triple t, Vec4s v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) v.x, (f32) v.y, (f32) v.z, (f32) v.w * D_800A65C8[1], 1, t);
}
