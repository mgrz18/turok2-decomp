/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284064.c, func_00284064.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef float f32;
typedef struct B19PF {
    u8 pad0[0x1C];
    f32 a;
    f32 b;
} B19PF;
typedef struct B19Q2 {
    u8 pad0[0x6C];
    f32 f6C;
} B19Q2;
typedef struct B19Parent {
    u32 flags;
    u8 pad4[0x20];
    B19PF *pf;          /* 0x24 */
} B19Parent;
/* Turok 2: parent at 0xF8 (Rage Wars 0x118). */
typedef struct B19Obj {
    u8 pad0[0xF8];
    B19Parent *parent;  /* 0xF8 */
} B19Obj;
extern f32 D_800A6404;
extern f32 D_800A6408;
extern f32 D_800A640C;
extern f32 D_800A6410;
extern f32 D_800A6414;
extern f32 D_800A6418;
extern f32 D_800A641C;
extern f32 D_800A6420;
extern f32 D_800A6424;
/* Static inline in the Rage Wars source file (dropped by the importer). */
static inline f32 Ramp2(B19Parent *par, f32 xin) {
    register f32 x asm("$f4") = xin;
    f32 d;
    f32 k;
    d = x - D_800A6404;
    k = !(d < 0.0f) ? (!(D_800A640C < d * D_800A6408) ? (!(d < 0.0f) ? d * D_800A6410 : 0.0f) : D_800A640C) : 0.0f;
    {
        register f32 fb asm("$f2") = par->pf->b;
        register f32 fa asm("$f3") = par->pf->a;
        x = x * (fa + (fb - fa) * k) * D_800A6414;
    }
    return x;
}
f32 func_0023270C(B19Obj *o, f32 px, B19Q2 *q) {
    B19Parent *par = o->parent;
    register f32 x asm("$f5") = px;
    f32 t;
    if (par->flags & 4) {
        t = Ramp2(par, x);
    } else {
        t = D_800A6418;
    }
    {
        f32 m = q->f6C;
        return t * D_800A6424 * (m + (D_800A641C - m) * D_800A6420);
    }
}
