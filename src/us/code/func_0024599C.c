/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002165F8.c, func_002165F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
void func_0020EF2C(void *, Vec3f *, Vec3f *);
void func_0020F040(Vec3f *);
f32 func_00298470(f32); /* sqrtf */
typedef struct { u8 pad0[4]; Vec3f pos; u8 pad10[0x40]; f32 unk50; } B04A; /* pos 0x4, unk50 (0x6C in Rage Wars) */
typedef struct { u8 pad0[0x48]; Vec3f pos; u8 pad54[0xC]; f32 unk60; } B04B;
typedef struct { Vec3f d; Vec3f n; f32 len; Vec3f e; Vec3f e2; f32 elen; f32 dy; } B04Dir;
void func_0024599C(B04A *a, B04B *b, B04Dir *o) {
    func_0020EF2C(o, &a->pos, &b->pos);
    o->n = o->d;
    func_0020F040(&o->n);
    o->len = func_00298470(o->d.x * o->d.x + o->d.y * o->d.y + o->d.z * o->d.z);
    o->e.y = 0;
    o->e.x = o->d.x;
    o->e.z = o->d.z;
    o->e2 = o->e;
    func_0020F040(&o->e2);
    o->elen = func_00298470(o->e.x * o->e.x + o->e.y * o->e.y + o->e.z * o->e.z);
    o->dy = a->unk50 - b->unk60;
}
