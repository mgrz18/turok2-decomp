#include "common.h"
#include "m2c_macros.h"

extern f32 CGameObjectInstance__GetAverageScale(s32);
extern f32 D_800A65B4[];

f32 Blast_GetSourceScale(u8 *arg0) {
    if (*arg0 == 1) {
        return CGameObjectInstance__GetAverageScale((s32) arg0);
    }
    return D_800A65B4[0];
}
