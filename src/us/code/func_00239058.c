/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002683F0.c, func_002683F0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B15Blk8 {
    s16 a, b;
    s32 c;
} B15Blk8;
typedef struct B15Def {
               s32 type;
               u8 pad4[0xA];
               s8 unkE;
} B15Def;
/* Turok 2: pos at 0x4 (Rage Wars 0x8). */
typedef struct B15Src {
    u8 type;
    u8 pad1[3];
    Vec3f pos;          /* 0x04 */
} B15Src;
extern f32 D_800A65E8;
extern f32 D_800A65EC;
void func_00237AFC(B15Src *a, s32 b, s32 c, Vec3f pos, B15Blk8 blk, s32 e, s32 f, s32 g);
/* Static inline in the Rage Wars source file (dropped by the importer);
 * Turok 2 passes 0x400000 where Rage Wars passes 0x200. */
static inline void w(B15Src *a, s32 b, s32 c, Vec3f pos, B15Blk8 blk) {
    func_00237AFC(a, b, c, pos, blk, 1, 0, 0x400000);
}
void func_00239058(B15Src *a, s32 b, s32 c, Vec3f pos, B15Blk8 blk) {
    f32 d;
    d = a->pos.y - pos.y;
    if (d < 0.0f ? -d <= D_800A65E8 : d <= D_800A65EC) {
        w(a, b, c, pos, blk);
    }
}
