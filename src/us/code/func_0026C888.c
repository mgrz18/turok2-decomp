#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
extern void CGameRegion__FloodSetHeight(void *, f32, s32); /* returns void; no caller of CGameRegion__SetGroupHeight uses its result */
#else
extern f32 CGameRegion__FloodSetHeight(void *, f32, u16);
#endif

f32 CGameRegion__SetGroupHeight(u16 *arg0, f32 arg1) {
    f32 var_f0;

    var_f0 = arg1;
    if (arg0 != NULL) {
#ifdef TARGET_PORT
        CGameRegion__FloodSetHeight(arg0, arg1, *arg0);
#else
        var_f0 = CGameRegion__FloodSetHeight(arg0, arg1, *arg0);
#endif
    }
    return var_f0;
}
