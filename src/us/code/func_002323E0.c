/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00283D24.c, func_00283D24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct B19Rec8 { s32 a; s32 b; } B19Rec8;
typedef struct B19Tbl {
    B19Rec8 rec[14];
    u16 w70[14];
    u16 w8C[14];
    u16 wA8[14];
} B19Tbl;
typedef struct B19Parent {
    char pad0[0x18];
    B19Tbl *tbl;
} B19Parent;
typedef struct B19Vec3 { f32 x, y, z; } B19Vec3;
/* Turok 2: pos 0x4, parent 0xF8, unk1AC 0x1AC (Rage Wars 0x8 / 0x118 / 0x1D0). */
typedef struct B19Obj {
    char pad0[4];
    B19Vec3 pos;
    char pad10[0xE8];
    B19Parent *parent;
    char padFC[0xB0];
    s8 unk1AC;
} B19Obj;
s32 func_002325F0(B19Obj *);
void func_00236EA0(B19Obj *, B19Obj *, s32, s32, B19Vec3, B19Rec8);
void func_00229340(B19Obj *, s32, s32, s32);
void func_00275544(s16, f32, f32, f32, s32, s32);
void func_002323E0(B19Obj *o) {
    B19Parent *par = o->parent;
    /* Turok 2 uses slots 13 / 11 (Rage Wars 12 / 10). */
    s32 k = func_002325F0(o) ? 13 : 11;
    s32 s2 = par->tbl->w70[k];
    s32 a2 = par->tbl->w8C[k];
    B19Rec8 r = par->tbl->rec[k];
    s32 s3 = par->tbl->wA8[k];
    if (a2 != 0xFFFF) func_00236EA0(o, o, a2, -1, o->pos, r);
    if (s2 != 0xFFFF) func_00229340(o, s2, o->unk1AC, 1);
    if (s3 != 0xFFFF) func_00275544(s3, o->pos.x, o->pos.y, o->pos.z, 0, -1);
}
