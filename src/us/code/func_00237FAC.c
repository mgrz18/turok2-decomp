/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CGameRegion__FloodClearFlag4(void *arg0);
extern void CGameRegion__FloodSetFlag4(void *arg0);
#ifdef TARGET_PORT
#include "structs/vector.h"

/* Host body: the position comes as a CVector3 (floats: SSE registers on x86-64). The Rage Wars
 * Triple is three s32, passed in integer registers, so with it every later argument of this
 * function and of GameObject_TraceMove arrived in the wrong register. */
extern void GameObject_TraceMove(void *arg0, CVector3 input, CVector3 *output, void **region,
                                 s32 unused0, s32 unused1);
void func_00237FAC(void *arg0, CVector3 input, s32 arg4) {
    CVector3 output;
    void *region;

    GameObject_TraceMove(arg0, input, &output, &region, 0, 0);
    if (arg4 != 0) {
        CGameRegion__FloodSetFlag4(region);
    } else {
        CGameRegion__FloodClearFlag4(region);
    }
}
#else
extern void GameObject_TraceMove(void *arg0, Triple input, Triple *output,
                          s32 *value, s32 unused0, s32 unused1);
void func_00237FAC(void *arg0, Triple input, s32 arg4) {
    Triple output;
    sptr value;
    GameObject_TraceMove(arg0, input, &output, &value, 0, 0);
    if (arg4 != 0) {
        CGameRegion__FloodSetFlag4((void *)value);
    } else {
        CGameRegion__FloodClearFlag4((void *)value);
    }
}
#endif
