/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024E1A4.c, func_0024E1A4.
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
typedef struct B11Obj1A0 {
    u8 pad0[0x1C];
    u32 flags1C;
} B11Obj1A0;
typedef struct Obj {
    u8 type;
    u8 pad1[0xD3];
    u32 flags;            /* 0xD4 */
    u8 padD8[0x98];
    B11Obj1A0 *unk170;    /* 0x170 */
} Obj;
s32 func_002681BC(Obj *a) {
    B11Obj1A0 *p;
    if (a->type != 1) {
        return 0;
    }
    if (!(a->flags & 0x2000)) {
        return 0;
    }
    p = a->unk170;
    if (p != 0) {
        if (p->flags1C & 0x10000) {
            return 0;
        }
    }
    return 1;
}
