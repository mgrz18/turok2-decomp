/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80271624_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern CVector3 *CVector3__RandomPerturbZ(CVector3 *out, CVector3 *in, f32 amount);
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
extern s32 Random_Next(void);
extern f32 func_00298470(f32);
CVector3 *CVector3__RandomPerturbZ(CVector3 *out, CVector3 *in, f32 amount) {
    CVector3 random;
    CVector3 result;
    CVector3 *randomPtr;
    random.x = in->x;
    random.y = in->y;
    random.z = ((f32)(Random_Next() % 20000) - 1.0e4f) *
               0.0001f;
    randomPtr = &random;
    {
        f32 magnitude;
        f32 scale;
        magnitude = func_00298470((randomPtr->x * randomPtr->x) +
                                  (randomPtr->y * randomPtr->y) +
                                  (randomPtr->z * randomPtr->z));
        if (magnitude != 0.0f) {
            scale = 1.0f / magnitude;
            randomPtr->x *= scale;
            randomPtr->y *= scale;
            randomPtr->z *= scale;
        }
    }
    {
        f32 magnitude;
        f32 scale;
        magnitude = func_00298470((in->x * in->x) +
                                  (in->y * in->y) +
                                  (in->z * in->z));
        if (magnitude != 0.0f) {
            scale = 1.0f / magnitude;
            in->x *= scale;
            in->y *= scale;
            in->z *= scale;
        }
    }
    result.x = in->x + amount * (randomPtr->x - in->x);
    result.y = in->y + amount * (randomPtr->y - in->y);
    result.z = in->z + amount * (randomPtr->z - in->z);
    *out = result;
    return out;
}
extern f32 func_00298470(f32);
