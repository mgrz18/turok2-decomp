/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024BCF8.c, func_0024BCF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B11Pt {
    f32 x;
    f32 y;
} B11Pt;
typedef struct B11Box {
    u16 unk0;
    u16 flags2;
    B11Pt *p4;
    B11Pt *p8;
    B11Pt *pC;
} B11Box;
/* Turok 2 layout: pos at 0x4, yaw at 0x50 (Rage Wars: 0x8 / 0x6C). */
typedef struct Obj {
    u8 pad0[0x4];
    Vec3f pos;          /* 0x04 */
    B11Box *unk14;      /* 0x10 */
    u8 pad14[0x3C];
    f32 yaw;            /* 0x50 */
} Obj;
void func_0026BC54(f32 *out, B11Box *box);
s32 func_0021F080(Obj *a) {
    B11Box *b = a->unk14;
    f32 t;
    f32 hi;
    f32 t2;
    f32 lo;
    f32 s;
    f32 c;
    f32 buf[3];
    if (b == 0) {
        return 0;
    }
    t = b->p8->y;
    if (!(b->p4->y <= t)) {
        t = b->p4->y;
    }
    hi = b->pC->y;
    if (!(t <= hi)) {
        hi = t;
    }
    t2 = b->p8->y;
    if (!(t2 <= b->p4->y)) {
        t2 = b->p4->y;
    }
    lo = b->pC->y;
    if (!(lo <= t2)) {
        lo = t2;
    }
    if (hi < a->pos.y) {
        return 0;
    }
    if (a->pos.y < lo) {
        return 0;
    }
    s = func_002982F0(a->yaw);
    c = func_002974C0(a->yaw);
    func_0026BC54(buf, b);
    return buf[0] * -s + buf[2] * -c < 0.0f;
}
