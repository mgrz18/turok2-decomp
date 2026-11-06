/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_30.c, func_80239D1C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 Random_Blend(f32 arg0, f32 arg1);
extern float func_002982F0(float x);
extern float D_800A9C2C;
extern f32 CShakeAxis__GetOffset(struct Wave *wave);
f32 CShakeAxis__GetOffset(struct Wave *wave) {
    switch (wave->kind) {
    case 1:
        return func_002982F0(wave->phase * D_800A9C2C) * wave->amplitude;
    case 0:
        return Random_Blend(-wave->amplitude, wave->amplitude);
    }
    return 0.0f;
}
