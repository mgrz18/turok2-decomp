/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00247BA4.c, func_00247BA4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef double f64;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B10Q4 {
    f32 x, y, z, w;
} B10Q4;
typedef struct B10Mt {
    f32 m[16];
} B10Mt;
/* Turok 2 layout (Rage Wars: pos 0x8, f70 0x70, flags 0x100, f174 0x174). */
typedef struct B10Obj {
    u8 pad0[0x4];
    Vec3f pos;              /* 0x004 */
    u8 pad10[0x54 - 0x10];
    f32 f54;                /* 0x054 */
    u8 pad58[0xD4 - 0x58];
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x144 - 0xD8];
    s32 f144;               /* 0x144 */
} B10Obj;
extern const f32 D_800A5B2C;
extern const f32 D_800A5B30;
extern const f32 D_800A5B34;
extern const f32 D_800A5B38;
extern const f32 D_800A5B3C;
extern const f32 D_800A5B40;
extern const f32 D_800A5B44;
extern f32 D_800DD78C;
extern B10Obj *D_800F7C70[];
extern s32 D_800F7E70;
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void func_0021AE6C(B10Q4 *out, B10Obj *o);
f32 func_002671B4(B10Obj *o);
void func_0020EF2C(Vec3f *o, Vec3f *a, Vec3f *b);
void func_0020EFDC(Vec3f *o, Vec3f *a, Vec3f *b);
void func_0020F040(Vec3f *v);
void func_0020F85C(B10Mt *m, Vec3f *a, Vec3f *b);
void func_00210F68(B10Q4 *o, B10Q4 *a, B10Q4 *b);
void func_00211114(B10Q4 *q, B10Mt *m);
f32 func_0021153C(f32 x);
f32 func_00298470(f32 x); /* sqrtf */
B10Q4 *func_0021B038(B10Q4 *out, B10Obj *self, Vec3f dir, B10Q4 *outq, f32 angle) {
    Vec3f v10;
    Vec3f v20;
    Vec3f v30;
    Vec3f v40;
    Vec3f v50;
    Vec3f v60;
    B10Q4 q70;
    B10Q4 q80;
    B10Q4 q90;
    B10Mt mA0;
    B10Obj *best;
    B10Obj *obj;
    s32 i;
    s32 n;
    f32 thr;
    f32 d;
    f32 t;
    f32 s;
    f32 new_var;
    f32 new_var2;
    new_var2 = angle;
    v20.x = 0;
    v20.y = 0;
    v20.z = D_800A5B2C;
    func_0021AE6C(&q70, self);
    if (new_var2 == 0.0f) {
        if (outq) {
            outq->x = outq->y = outq->z = 0;
            outq->w = D_800A5B2C;
        }
        *out = q70;
        return out;
    }
    func_00211114(&q70, &mA0);
    func_0020F85C(&mA0, &v20, &v30);
    best = 0;
    thr = func_002974C0(new_var2 * D_800A5B30);
    n = D_800F7E70;
    for (i = 0; i < n; i++) {
        obj = D_800F7C70[i];
        if (obj == self) {
            continue;
        }
        if (obj->flags & 1) {
            continue;
        }
        if (obj->f144 == 0) {
            continue;
        }
        v60 = obj->pos;
        v60.y = v60.y + obj->f54;
        v60.y = v60.y + func_002671B4(obj) * D_800A5B34;
        func_0020EF2C(&v10, &v60, &dir);
        func_0020F040(&v10);
        d = v30.x * v10.x + v30.y * v10.y + v30.z * v10.z;
        if (thr < d) {
            thr = d;
            v50 = v10;
            best = obj;
        }
    }
    if (best) {
        func_0020EFDC(&v40, &v30, &v50);
        func_0020F040(&v40);
        t = func_0021153C(thr);
        t *= func_00298470(func_00298470(D_800A5B3C - t / (new_var2 * D_800A5B38)));
        t *= D_800A5B40;
        s = func_002982F0(t);
        D_800DD78C = s;
        q80.x = v40.x * s;
        q80.y = v40.y * s;
        q80.z = v40.z * s;
        new_var = t;
        q80.w = func_002974C0(new_var);
        func_00210F68(&q90, &q70, &q80);
        if (outq) {
            *outq = q80;
        }
        *out = q90;
        return out;
    }
    if (outq) {
        outq->x = outq->y = outq->z = 0.0f;
        outq->w = D_800A5B44;
    }
    *out = q70;
    return out;
}
