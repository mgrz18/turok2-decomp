/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00268768.c, func_00268768.
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
typedef struct B15Src {
               u8 type;
               u8 pad1[3];
               Vec3f pos;       /* 0x4 */
               u8 pad10[4];
               B15Def *def;     /* 0x14 */
               u8 pad18[0x68];
               void *unk80;     /* 0x80 (0xD0 in Rage Wars) */
} B15Src;
typedef struct B15Globals {
                u32 flags0;
                u8 pad4[0x1C8];
                s32 unk1CC;
} B15Globals;
void func_002755B0(s16 id, Vec3f pos, s32 a, void *src, f32 vol);
s32 func_00268E60(void *a);
extern B15Globals D_8011AAD8;
extern f32 D_800A65F0;
extern f32 D_800A65F4;
extern f32 D_800A65F8;
void func_00239254(B15Src *act, B15Src *src, s32 c, Vec3f pos, B15Blk8 k) {
    f32 vol = D_800A65F0;
    void *s = (void *)-1;
    if (D_8011AAD8.unk1CC == 0 && func_00268E60(act) == k.c) {
        switch (src->type) {
        case 0:
            s = src->unk80;
            break;
        case 1:
            if (act->def->type == 1) {
                if (D_8011AAD8.flags0 & 0x40) {
                    vol = D_800A65F4;
                }
                if (D_8011AAD8.flags0 & 0x20) {
                    vol = D_800A65F8;
                }
            }
            break;
        }
        func_002755B0(k.b, pos, 0, s, vol);
    }
}
