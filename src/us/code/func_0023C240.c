/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/ANIM/80241170.CPP, func_80241260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Plane;
typedef struct Plane Plane;
struct Plane;
struct Plane {
    CVector3 pos; /* 0x00 in Turok 2 */
    char pad24[0x24];
    CVector3 normal;
};
extern void func_0020EF2C(CVector3 *, CVector3 *, CVector3 *);
s32 func_0023C240(void *arg0, CVector3 *arg1) {
    Plane *object = arg0;
    CVector3 sp10;
    s32 var_v0;
    func_0020EF2C(&sp10, arg1, &object->pos);
    var_v0 = 0;
    if (!((object->normal.x * sp10.x) + (object->normal.y * sp10.y) + (object->normal.z * sp10.z) >= 0.0f)) {
        var_v0 = 1;
    }
    return var_v0;
}
