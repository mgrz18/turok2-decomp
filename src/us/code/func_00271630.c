/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258A7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, s32, s32);
void CAudioWorld__PlaySoundPitched(ContextRecAt2BC0 *arg0, s32 arg1, CVector3 arg2, s32 arg3, s32 arg4, f32 arg5) {
    arg0->alpha = arg5;
    CAudioWorld__PlaySound(arg0, arg1, arg2, arg3, arg4);
    arg0->alpha = 1.0f;
}
extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, s32, s32);
