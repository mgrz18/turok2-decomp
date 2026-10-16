/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80271624_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern float D_800A5534;
extern Vec3 *func_0020E61C(Vec3 *out, Vec3 *in, f32 amount);
extern float D_800A553C;
extern f32 func_00298470(f32);
extern f32 func_00298470(f32);
extern s32 func_002113A4(void);
extern f32 func_00298470(f32);
extern s32 func_002113A4(void);
extern f32 func_00298470(f32);
extern s32 func_002113A4(void);
extern f32 func_00298470(f32);
extern s32 func_002113A4(void);
extern f32 func_00298470(f32);
Vec3 *func_0020E61C(Vec3 *out, Vec3 *in, f32 amount) {
    Vec3 random;
    Vec3 result;
    Vec3 *randomPtr;
    random.x = in->x;
    random.y = in->y;
    random.z = ((f32)(func_002113A4() % 20000) - D_800A5534) *
               *(&D_800A5534 + 1);
    randomPtr = &random;
    {
        f32 magnitude;
        f32 scale;
        magnitude = func_00298470((randomPtr->x * randomPtr->x) +
                                  (randomPtr->y * randomPtr->y) +
                                  (randomPtr->z * randomPtr->z));
        if (magnitude != 0.0f) {
            scale = D_800A553C / magnitude;
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
            scale = *(&D_800A553C + 1) / magnitude;
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
