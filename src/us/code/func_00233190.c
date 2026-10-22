/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284D98.c, func_00284D98.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct B19Vec3 { f32 x, y, z; } B19Vec3;
/* Turok 2 layout: id at 0x2, pos at 0x4 (Rage Wars: 0x4 / 0x8). */
typedef struct B19Obj {
    u8 pad0[0x2];
    u16 id;             /* 0x002 */
    B19Vec3 pos;        /* 0x004 */
    u8 pad10[0x118];
    s16 unk128;         /* 0x128 */
} B19Obj;
void func_0020EF2C(B19Vec3 *, B19Vec3 *, B19Vec3 *);
extern const f32 D_800A642C;
void func_00233190(void *owner, B19Obj *ref, B19Obj **arr, s32 n) {
    s32 i;
    B19Vec3 d;
    for (i = 0; i < n; i++) {
        B19Obj *o = arr[i];
        if (o->id != 0x57) {
            func_0020EF2C(&d, &o->pos, &ref->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= D_800A642C) o->unk128 = 1;
        }
    }
}
