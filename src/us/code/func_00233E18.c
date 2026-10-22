/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A41D8.c, func_002A41D8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct V16 {
    f32 x, y, z, w;
} V16;
typedef struct Ent41D8 {
    u8 pad0[0x10];
    Vec3f pos;
    f32 unk1C;
    f32 unk20;
    f32 unk24;
} Ent41D8;
typedef struct Out41D8 {
    V16 a;
    V16 b;
    V16 c;
    V16 d;
} Out41D8;
extern f32 D_800A646C;
void func_0020EF2C(void *out, void *a, void *b);
void func_0020EF60(void *out, void *a, f32 s);
void func_0020EFDC(void *out, void *a, void *b);
void func_0020F040(void *v);
void func_0020F79C(void *m);
void func_0020FAFC(u8 *p, void *a, void *b);
void func_00233E18(Ent41D8 *e0, Ent41D8 *e1, u8 *arg2, Out41D8 *out) {
    V16 v10, v20, v30, v40, v50;
    f32 ty, tz;
    func_0020EF2C(&v20, &e0->pos, &e1->pos);
    func_0020F040(&v20);
    *(s32 *)&v10.x = 0;
    *(s32 *)&v10.y = 0;
    v10.z = D_800A646C;
    func_0020FAFC(arg2 + 0x18C, &v10, &v30);
    ty = -v30.y;
    tz = -v30.z;
    v30.y = ty;
    v30.z = tz;
    func_0020EF60(&v40, &v30, v30.x * v20.x + ty * v20.y + tz * v20.z);
    func_0020EF2C(&v40, &v20, &v40);
    func_0020F040(&v40);
    func_0020EFDC(&v50, &v30, &v40);
    func_0020F040(&v50);
    func_0020EF60(&v50, &v50, e0->unk1C);
    func_0020EF60(&v40, &v40, e0->unk20);
    func_0020EF60(&v30, &v30, e0->unk24);
    func_0020F79C(out);
    out->a.x = v50.x;
    out->a.y = v50.y;
    out->a.z = v50.z;
    out->b.x = v40.x;
    out->b.y = v40.y;
    out->b.z = v40.z;
    out->c.x = v30.x;
    out->c.y = v30.y;
    out->c.z = v30.z;
    out->d.x = e0->pos.x;
    out->d.y = e0->pos.y;
    out->d.z = e0->pos.z;
}
