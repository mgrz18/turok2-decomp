/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00216D3C.c, func_00216D3C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct VecDst {
               s32 unk0;
               s32 unk4;
               s32 unk8;
               Vec3f a;
               Vec3f b;
               s32 unk24;
               Vec3f c;
               Vec3f d;
               s32 unk40;
} VecDst;
extern f32 D_800B6D28;
typedef struct { u8 pad0[4]; Vec3f pos; } B04A; /* pos at 0x4 in Turok 2 (0x8 in Rage Wars) */
typedef struct { u8 pad0[0x78]; s32 unk78; f32 unk7C; s32 unk80; u8 pad84[4]; s32 unk88; } B04B;
typedef struct { VecDst a; VecDst b; } B04C;
void func_00244338(B04A *, B04B *, s32, VecDst *);
void func_002449E0(B04A *, B04B *, s32, VecDst *);
/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline void fill(VecDst *d, B04A *a) {
    Vec3f p = a->pos;
    Vec3f z;
    z.x = 0.0f;
    z.y = 0.0f;
    z.z = 0.0f;
    d->unk0 = 3;
    d->unk4 = 0;
    d->unk8 = 0;
    d->a = p;
    d->b = z;
    d->unk24 = 0;
    z.y = 0.0f;
    p.y = 0.0f;
    d->c = p;
    d->d = z;
    d->unk40 = 0;
}
void func_00246108(B04A *a, B04B *b, B04C *c, s32 f1, s32 f2) {
    f32 t;
    if (b->unk7C > 0.0f) {
        t = b->unk7C - D_800B6D28;
        b->unk7C = t;
        if (t <= 0.0f) {
            b->unk78 = 0;
            b->unk7C = 0.0f;
        }
    }
    if (f1 != 0)
        func_00244338(a, b, b->unk88, &c->b);
    else
        fill(&c->b, a);
    if (f2 != 0) {
        func_002449E0(a, b, b->unk80, &c->a);
        return;
    }
    fill(&c->a, a);
}
