/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002172D0.c, func_002172D0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct SndSrc {
    u8 type;
    u8 pad1[3];
    Vec3f pos;              /* 0x04 (0x08 in Rage Wars) */
    u8 pad10[0x80 - 0x10];
    void *unk80;            /* 0x80 (0xD0) */
} SndSrc;
typedef struct SndHandle {
    u8 pad0[0xC];
    s32 unkC;
} SndHandle;
typedef struct SndOwner {
    u8 pad0[0xEC];
    SndHandle *sound;       /* 0xEC (0xFC in Rage Wars) */
} SndOwner;
void *func_00275C6C(s32 a, s32 b, Vec3f *c, Vec3f *d, void *e);
void func_00275D34(s32 a, SndHandle *h);
s32 func_00275F7C();
void func_00246860(SndSrc *src, SndOwner *owner, s32 id) {
    void *s0 = (void *)-1;
    if (owner->sound != ((void *)0)) {
        if (owner->sound->unkC == id) {
            return;
        }
        func_00275D34(func_00275F7C(), owner->sound);
    }
    switch (src->type) {
    case 1:
    case 2:
        s0 = src;
        break;
    case 0:
        s0 = src->unk80;
        break;
    }
    owner->sound = func_00275C6C(func_00275F7C(), id, &src->pos, &src->pos, s0);
}
