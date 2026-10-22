/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284E70.c, func_00284E70.
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
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    B19Vec3 pos;     /* 0x4 */
    u8 pad10[0x118];
    s16 unk128;      /* 0x128 */
};
extern const f32 D_800A6430;
void func_0020EF2C(B19Vec3 *, B19Vec3 *, B19Vec3 *);
void func_00230AB0(void *, B19Obj *, B19Obj **, s32);
void func_00233268(void *owner, B19Obj **arr, s32 n) {
    s32 i, j;
    B19Vec3 d;
    B19Obj *o;
    B19Obj *e;
    for (i = 0; i < n; i++) {
        o = arr[i];
        switch (o->id) {
        case 0xC:
            break;
        case 0x22:
            func_00230AB0(owner, o, arr, n);
            break;
        case 0x57:
            for (j = 0; j < n; j++) {
                e = arr[j];
                if (e->id != 0x57) {
                    func_0020EF2C(&d, &e->pos, &o->pos);
                    if (d.x * d.x + d.y * d.y + d.z * d.z <= D_800A6430) e->unk128 = 1;
                }
            }
            break;
        }
    }
}
