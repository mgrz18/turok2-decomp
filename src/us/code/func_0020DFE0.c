/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80270FE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A5514;
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
CVector3 CVector3__RandomPerturb(CVector3 *in, f32 amount) {
    CVector3 random;
    CVector3 result;
    CVector3 *randomPtr;
    f32 randomMagnitude;
    f32 randomScale;
    f32 inputMagnitude;
    f32 inputScale;
    random.x = (f32)((Random_Next() % 20000) - 0x2710);
    random.y = (f32)((Random_Next() % 20000) - 0x2710);
    random.z = (f32)((Random_Next() % 20000) - 0x2710);
    randomPtr = &random;
    randomMagnitude = func_00298470((randomPtr->x * randomPtr->x) +
                                    (randomPtr->y * randomPtr->y) +
                                    (randomPtr->z * randomPtr->z));
    if (randomMagnitude != 0.0f) {
        randomScale = 1.0f / randomMagnitude;
        randomPtr->x *= randomScale;
        randomPtr->y *= randomScale;
        randomPtr->z *= randomScale;
    }
    inputMagnitude = func_00298470((in->x * in->x) +
                                   (in->y * in->y) +
                                   (in->z * in->z));
    if (inputMagnitude != 0.0f) {
        inputScale = D_800A5514 / inputMagnitude;
        in->x *= inputScale;
        in->y *= inputScale;
        in->z *= inputScale;
    }
    result.x = in->x + amount * (randomPtr->x - in->x);
    result.y = in->y + amount * (randomPtr->y - in->y);
    result.z = in->z + amount * (randomPtr->z - in->z);
    return result;
}
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
