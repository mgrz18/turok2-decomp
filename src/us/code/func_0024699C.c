/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021740C.c, func_0021740C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
f32 func_002672C8(void *);
void func_002688C8(void *, f32, f32, f32, s32, s32, s32, s32);
void func_0020EEF8(Vec3f *, Vec3f *, Vec3f *);
void func_0020EF2C(Vec3f *, Vec3f *, Vec3f *);
void func_0020EF60(Vec3f *, Vec3f *, f32);
void func_0020F040(Vec3f *);
extern f32 D_800A6990;
typedef struct { u8 pad0[4]; Vec3f pos; } B04Ob; /* pos at 0x4 in Turok 2 */
void func_0024699C(B04Ob *a, s32 unused, B04Ob *b, B04Ob *c, s32 e, s32 f) {
    Vec3f d;
    Vec3f r;
    if (c != ((void *)0)) {
        func_0020EF2C(&d, &b->pos, &c->pos);
        d.y = 0;
        func_0020F040(&d);
    } else {
        d.x = 0;
        d.y = 0;
        d.z = 0;
    }
    func_0020EF60(&d, &d, func_002672C8(b) + func_002672C8(a) + D_800A6990);
    func_0020EEF8(&r, &b->pos, &d);
    func_002688C8(b, r.x, r.y, r.z, e, f, 0, 0);
}
