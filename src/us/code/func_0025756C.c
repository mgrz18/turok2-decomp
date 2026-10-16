/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022E948_de.c, func_8022EA3C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
extern void func_00275B30(f32 arg0);
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
extern float D_800A7238;
extern float D_800A7234;
extern float D_800A7230;
typedef struct Shared_Func80243A80Collision Shared_Func80243A80Collision;
struct Shared_Func80243A80Collision {
    u16 id;
    u16 flags;
};
extern f32 func_00269C3C(void *arg0, f32 arg1, f32 arg2);
extern f32 func_0026BE60(s32, f32, f32);
void func_0025756C(void *arg0, Vec3 *arg1) {
    f32 first;
    f32 amount;
    if (arg0 != 0 && arg1 != 0 &&
        (((Shared_Func80243A80Collision *)arg0)->flags & 0x40)) {
        first = func_00269C3C(arg0,
            arg1->x, arg1->z);
        amount = (f32)(s32)(first - func_0026BE60(arg0,
            arg1->x, arg1->z));
        if (amount < D_800A7230) {
            func_00275B30(D_800A7238 - (amount * D_800A7234));
            return;
        }
    }
    func_00275B30(0.0f);
}
