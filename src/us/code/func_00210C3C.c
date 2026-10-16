/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273D6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct Mtx;
struct Mtx {
    Vec3 right;
    f32 m03;
    Vec3 up;
    f32 m13;
    Vec3 forward;
    f32 m23;
    f32 m30;
    f32 m31;
    f32 m32;
    f32 m33;
};
struct Mtx;
typedef struct Mtx Mtx;
void func_00210C3C(struct Mtx *object) {
    float t;
    t = object->up.x;
    object->up.x = object->forward.x;
    object->forward.x = t;
    t = object->up.y;
    object->up.y = object->forward.y;
    object->forward.y = t;
    t = object->up.z;
    object->up.z = object->forward.z;
    object->forward.z = t;
}
