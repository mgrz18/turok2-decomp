#include "common.h"
#include "m2c_macros.h"

#include "structs/region.h"
#include "protos/region.h"

f32 CGameRegion__SetGroupHeight(u16 *arg0, f32 arg1) {
    f32 var_f0;

    var_f0 = arg1;
    if (arg0 != NULL) {
#ifdef TARGET_PORT
        CGameRegion__FloodSetHeight((CGameRegion *)arg0, arg1, *arg0);   /* returns void; no caller of CGameRegion__SetGroupHeight uses its result */
#else
        var_f0 = ((f32 (*)(CGameRegion *, f32, u16))CGameRegion__FloodSetHeight)((CGameRegion *)arg0, arg1, *arg0);   /* the ROM reads the $f0 it leaves */
#endif
    }
    return var_f0;
}
