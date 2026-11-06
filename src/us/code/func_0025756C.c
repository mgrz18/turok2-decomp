/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022E948_de.c, func_8022EA3C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Sound__SetReverb(f32 arg0);
extern f32 CGameRegion__GetCeilingHeight(void *arg0, f32 arg1, f32 arg2);
extern f32 CGameRegion__GetHeight(s32, f32, f32);
void Sound_SetUnderwaterFilter(void *arg0, CVector3 *arg1) {
    f32 first;
    f32 amount;
    if (arg0 != 0 && arg1 != 0 &&
        (((Shared_Func80243A80Collision *)arg0)->flags & 0x40)) {
        first = CGameRegion__GetCeilingHeight(arg0,
            arg1->x, arg1->z);
        amount = (f32)(s32)(first - CGameRegion__GetHeight(arg0,
            arg1->x, arg1->z));
        if (amount < 1024.0f) {
            Sound__SetReverb(1.0f - (amount * 0.0009765625f));
            return;
        }
    }
    Sound__SetReverb(0.0f);
}
