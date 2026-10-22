/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00283D24.c, func_00283D24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
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
struct B19Obj { /* Turok 2 layout */
    u8 pad0[0x4];
    B19Vec3 pos;            /* 0x04 */
    u8 pad10[0xE8];
    B19Parent *parent;      /* 0xF8 (Rage Wars: 0x118) */
    u8 padFC[0xB0];
    s8 unk1AC;              /* Rage Wars: 0x1D0 */
};
s32 func_002325F0(B19Obj *);
void func_00236EA0(B19Obj *, B19Obj *, s32, s32, B19Vec3, B19Rec8);
void func_00229340(B19Obj *, s32, s32, s32);
void func_00275544(s16, f32, f32, f32, s32, s32);
void func_002322D8(B19Obj *o) {
    B19Parent *par = o->parent;
    s32 k = func_002325F0(o) ? 12 : 10;
    s32 s2 = par->tbl->w70[k];
    s32 a2 = par->tbl->w8C[k];
    B19Rec8 r = par->tbl->rec[k];
    s32 s3 = par->tbl->wA8[k];
    if (a2 != 0xFFFF) func_00236EA0(o, o, a2, -1, o->pos, r);
    if (s2 != 0xFFFF) func_00229340(o, s2, o->unk1AC, 1);
    if (s3 != 0xFFFF) func_00275544(s3, o->pos.x, o->pos.y, o->pos.z, 0, -1);
}
