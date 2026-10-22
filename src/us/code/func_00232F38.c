/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002848FC.c, func_002848FC.
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
struct B19Obj {
              s8 kind;
              u8 pad1[0x1];
              u16 id; /* Turok 2: id at 0x2 */
              u8 pad4[0x4];
              B19Vec3 pos;
               u8 pad14[0x4];
               void * info;
               B19Vec3 pos2;
               u8 pad28[0x34];
               u32 flags;
               u8 pad60[0xA0];
                u32 flags100;
                u8 pad104[0x14];
                B19Parent * parent;
                u8 pad11C[0x8];
                s32 unk124;
                s32 unk128;
                s32 unk12C;
                s32 * unk130;
                void * unk134;
                s32 unk138;
                u8 pad13C[0x10];
                s16 unk14C;
                u8 pad14E[0x26];
                B19Vec3 vel;
                f32 unk180;
                f32 unk184;
                u8 pad188[0x48];
                s8 unk1D0;
                u8 unk1D1;
                u8 pad1D2[0x7];
                u8 unk1D9;
                u8 pad1DA[0x2];
                B19Handle * unk1DC;
                u8 pad1E0[0x4];
                B19List * slotp;
                u8 pad1E8[0x4];
                B19Obj * next;
                s32 unk1F0;
                B19Obj * next2;
};
s32 func_00232F38(B19Obj *o) {
    switch (o->id) {
    case 5: case 6: case 7: case 9: case 0x11: case 0x13: case 0x1A: case 0x101: case 0x110:
        return 1;
    default:
        return 0;
    }
}
