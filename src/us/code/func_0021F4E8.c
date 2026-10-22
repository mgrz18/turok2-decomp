/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C1B4.c, func_0024C1B4.
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
typedef struct Obj {
    u8 pad0[0x58];
    u8 mtx58[0x40]; /* 0x58: transform (0x74 in Rage Wars) */
} Obj;
typedef struct B11Pair {
    s32 a;
    s32 b;
} B11Pair;
typedef struct B11Rec {
    u8 pad0[0x2];
    u16 type;
    u8 pad4[0x4];
    Vec3f pos;
    B11Pair unk14;
} B11Rec;
B11Rec *func_002199C8(s32 a);
s32 func_002199F4(s32 a);
void func_0020F85C(void *m, void *v, Vec3f *out);
void func_00236EA0(Obj *a, Obj *b, u16 c, s32 d, Vec3f e, B11Pair f);
void func_0021F4E8(Obj *a, s32 b) {
    B11Rec *list = func_002199C8(b);
    s32 n = func_002199F4(b);
    s32 i;
    s32 t;
    Vec3f v;
    for (i = 0; i < n; i++) {
        t = list[i].type;
        if (t < 8) {
            if (t >= 6) {
                func_0020F85C(a->mtx58, &list[i].pos, &v);
                func_00236EA0(a, a, list[i].type, -1, v, list[i].unk14);
            }
        }
    }
}
