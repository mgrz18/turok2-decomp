/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E8B0.c, func_0044E8B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct S7Light {
               u8 pad0[0x14];
               Vec3f dir;
               s32 enabled;
} S7Light;
void func_0020F040(Vec3f *v);
void func_0042D1C0(S7Light *l) {
    l->dir.x = -0.667424f;
    l->dir.y = 0.953462f;
    l->dir.z = -0.572078f;
    func_0020F040(&l->dir);
    l->enabled = 1;
}
