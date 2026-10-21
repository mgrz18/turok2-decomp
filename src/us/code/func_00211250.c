/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GAME/CODE_8027302C.CPP, func_80274380_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern f32 func_0021153C(f32 arg0);
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
extern void func_00211250(CQuatern *rotation, CQuatern *target);
extern f32 D_800B6D28;
void func_0020DCC8(void *, f32, void *, void *);
void func_00211250(CQuatern *rotation, CQuatern *target) {
    CQuatern result;
    f32 angle;
    f32 blend;
    f32 t;
    angle = func_0021153C((rotation->x * target->x) + (rotation->y * target->y) + (rotation->z * target->z) + (rotation->w * target->w));
    if (angle != 0.0f) {
        t = 1.0f - ((angle - (((angle * 0.1388889f) + 1.7453293e-4f) * D_800B6D28)) / angle);
        if ((1.0f < t) || !(t < 0.0f)) {
            blend = 1.0f;
            if (!(blend < t)) {
                blend = t;
            }
        } else {
            blend = 0.0f;
        }
        func_0020DCC8(&result, blend, rotation, target);
        *rotation = result;
    }
}
