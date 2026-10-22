/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00259B30.c, func_00259B30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
f32 func_002982F0(f32 x);
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
extern f32 D_800A80F0;
extern f32 D_800A80F4;
extern f32 D_800A80F8;
void func_0020EF2C(M04Vec3 *out, void *src, s32 a);
void func_0020FB90(void *src, M04Vec3 *a, M04Vec3 *out);
void func_0020F040(M04Vec3 *v);
f32 func_0021153C(f32 x);
f32 func_002982F0(f32 x);
f32 __builtin_fabsf(f32 x);
s32 func_00271E20(s32 a0, u8 *obj) {
    M04Vec3 r;
    M04Vec3 d;
    M04Vec3 w;
    f32 s;
    f32 zero;
    if (obj == 0) {
        return 0x40;
    }
    func_0020EF2C(&d, obj + 0x114, a0);
    if (d.x * d.x + d.z * d.z < D_800A80F0) {
        return 0x40;
    }
    func_0020FB90(obj + 0x14C, &d, &w);
    w.y = 0.0f;
    zero = w.y;
    r.y = zero;
    r.x = zero;
    r.z = D_800A80F4;
    func_0020F040(&w);
    s = func_002982F0(func_0021153C(w.x * r.x + w.y * r.y + w.z * r.z));
    if (w.x == zero) {
        return 0x40;
    }
    if (zero < w.x) {
        if (s < zero) {
            s = -s;
        }
    } else {
        s = -__builtin_fabsf(s);
    }
    return (s16)(s32)(s * D_800A80F8 + D_800A80F8);
}
