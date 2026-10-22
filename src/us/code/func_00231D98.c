/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002838F4.c, func_002838F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct B19Def B19Def;
typedef struct B19PF B19PF;
typedef struct B19Lim B19Lim;
typedef struct B19Rec8 B19Rec8;
typedef struct B19Tbl B19Tbl;
typedef struct B19Info38 B19Info38;
typedef struct B19List B19List;
typedef struct B19Handle B19Handle;
typedef struct B19Vec3 B19Vec3;
typedef struct B19Parent B19Parent;
typedef struct B19Obj B19Obj;
struct B19Def {
              u8 pad0[0x6];
              s8 b6;
              s8 b7;
              u8 pad8[0x4];
              s16 sC;
};
struct B19PF {
              u8 pad0[0x1C];
               f32 a;
               f32 b;
};
struct B19Lim {
              u8 pad0[0x10];
               u16 a;
               u16 b;
};
struct B19Rec8 {
              s32 a;
              s32 b;
};
struct B19Tbl {
              B19Rec8 rec[14];
               u16 w70[14];
               u16 w8C[14];
               u16 wA8[14];
};
struct B19Info38 {
              u8 pad0[0x8];
              s8 idx;
              u8 pad9[0x3];
              s8 n;
};
struct B19List {
              B19Obj * head;
              B19Obj * tail;
              u8 pad8[0xC];
};
struct B19Handle {
              u8 pad0[0x8];
              s32 f8;
              s32 fC;
};
struct B19Vec3 {
              f32 x;
              f32 y;
              f32 z;
};
struct B19Parent {
              u32 flags;
              u8 pad4[0xC];
               s16 s10;
               u8 pad12[0x2];
               s32 unk14;
               B19Tbl * tbl;
               u8 pad1C[0x8];
               B19PF * pf;
               u8 pad28[0x8];
               B19Lim * lim;
               u8 pad34[0x4];
               B19Info38 * info38;
};
struct B19Obj {
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    B19Vec3 pos;     /* 0x4 */
    u8 pad10[0x30];
    u32 flags;       /* 0x40 */
    u8 pad44[0xD4];
    s32 unk118;      /* 0x118 */
};
extern u8 D_801102B8[];
extern u8 D_8010FF34[];
void func_0022D09C(B19Obj *, s32, s32);
void func_002350EC(void *, B19Obj *, s32);
s32 func_00228AB0(void *, s32);
void func_00232AC4(B19Obj *, s32);
void func_00275544(s16, f32, f32, f32, s32, s32);
void func_00231D98(B19Obj *o, B19Def *def) {
    s32 t;
    func_0022D09C(o, 0, 0);
    if (def->b6 != -1) func_002350EC(D_801102B8, o, def->b6);
    if (def->b7 != -1) o->unk118 = func_00228AB0(D_8010FF34, def->b7);
    else o->unk118 = 0;
    if (!(o->flags & 0x200000) && (t = def->sC) != 0) {
        switch (o->id) {
        case 0x22:
        case 0x5F:
        case 0x60:
            func_00232AC4(o, t);
            break;
        default:
            func_00275544(t, o->pos.x, o->pos.y, o->pos.z, 0, -1);
            break;
        }
    }
}
