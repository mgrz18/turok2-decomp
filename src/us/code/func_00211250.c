/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GAME/CODE_8027302C.CPP, func_80274380_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern f32 Math_AcosLookup(f32 arg0);
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
extern void CQuaternion__TurnToward(CQuatern *rotation, CQuatern *target);
extern f32 D_800B6D28;
void CQuaternion__SlerpFast(void *, f32, void *, void *);
void CQuaternion__TurnToward(CQuatern *rotation, CQuatern *target) {
    CQuatern result;
    f32 angle;
    f32 blend;
    f32 t;
    angle = Math_AcosLookup((rotation->x * target->x) + (rotation->y * target->y) + (rotation->z * target->z) + (rotation->w * target->w));
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
        CQuaternion__SlerpFast(&result, blend, rotation, target);
        *rotation = result;
    }
}
