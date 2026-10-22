/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C33C.c, func_0024C33C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B11Box B11Box;
typedef union { s32 i; f32 f; } B11UIF;
typedef struct Obj { /* Turok 2 layout */
    u8 type;
    u8 pad1[3];
    Vec3f pos;          /* 0x04 */
    B11Box *unk10;      /* 0x10: floor region */
    u8 pad14[0x20];
    f32 unk34;          /* 0x34: scale x */
    u8 pad38[4];
    f32 unk3C;          /* 0x3C: scale z */
    u8 pad40[0x10];
    B11UIF unk50;       /* 0x50: yaw */
} Obj;
void func_00215AE0(f32 *p, s32 a, f32 t, f32 *out);
void func_002105A4(f32 *m, s32 a);
void func_0020F85C(f32 *m, f32 *v, f32 *out);
void func_002688C8(Obj *a, Vec3f v, Vec3f *out, B11Box **box, s32 e, s32 f);
void func_0021F668(Obj *a, f32 *p) {
    f32 t0[3];
    f32 t1[3];
    f32 d[3];
    f32 r[3];
    Vec3f w;
    f32 m[16];
    func_00215AE0(p, 0, 0.0f, t0);
    func_00215AE0(p, 0, *p, t1);
    d[0] = t1[0] - t0[0];
    d[1] = 0.0f;
    d[2] = t1[1] - t0[1];
    func_002105A4(m, a->unk50.i);
    func_0020F85C(m, d, r);
    w.x = a->pos.x + r[0] * a->unk34;
    w.y = a->pos.y;
    w.z = a->pos.z + r[2] * a->unk3C;
    func_002688C8(a, w, &a->pos, (B11Box **)&a->unk10, 0, 0);
}
