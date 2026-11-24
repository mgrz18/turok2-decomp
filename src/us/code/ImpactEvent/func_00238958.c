/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_3.c, func_80267DB4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/camera.h"
#include "structs/triple.h"
#include "structs/particle_event.h"

extern s32 CCameraPool__StartShake(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, Triple arg6);

void ImpactEvent_StartShakeType1(s32 arg0, s32 arg1, s32 arg2, Triple t, CImpactEventWords v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) (s16) (v.w0 >> 16), (f32) (s16) v.w0,
                            (f32) (s16) (v.w1 >> 16), (f32) (s16) v.w1 * 10.24f, 1, t);
}
