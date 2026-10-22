/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A3F94.c, func_002A3F94.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct SEnt SEnt;
typedef struct SSlot SSlot;
typedef struct SShape {
               f32 unk0;
               u8 pad4[4];
               f32 unk8;
               s32 unkC;
               u8 pad10[0xC];
               u8 unk1C;
} SShape;
struct SEnt {
               SEnt *prev;
               SEnt *next;
               f32 unk8;
               u8 padC[4];
               u8 m10[0xC];
               u8 m1C[0xC];
               u8 m28[2][0x40];
               f32 unkA8;
               f32 unkAC;
               void *unkB0;
};
struct SSlot {
               u8 pad0[4];
               SSlot *next;
               SShape *shape;
               u8 padC[0x10];
               void *owner;
               u8 pad20[4];
               f32 unk24;
               f32 unk28;
               f32 unk2C;
               f32 unk30;
               s32 unk34;
               u8 pad38[4];
               u32 flags;
               SEnt *list;
               u8 pad44[4];
               s32 unk48;
               f32 unk4C;
               s32 unk50;
};
typedef struct Owner3F94 {
    u8 pad0[0xD4];
    u32 flags; /* Turok 2: 0xD4 */
} Owner3F94;
extern f32 D_800B6D28;
extern f32 D_800B6D2C;
extern f32 D_800B6D30;
void func_00236040(s32 a, SSlot *s, SEnt *e);
void func_002368B0(SSlot *s);
s32 func_00233BD4(s32 a0, SSlot *s) {
    f32 saved, k1, k2;
    SEnt *e, *next;
    Owner3F94 *ow;
    s32 r;
    saved = D_800B6D28;
    if (s->flags & 2) {
        D_800B6D28 = saved * D_800B6D30;
    }
    if (s->flags & 1) {
        D_800B6D28 = D_800B6D28 * D_800B6D2C;
    }
    if (s->flags & 8) {
        s->unk50 = s->unk50 + 1;
        if (s->unk50 >= 3) {
            s->owner = 0;
        }
    }
    if (s->unk2C >= 0.0f) {
        s->unk30 = s->unk30 + D_800B6D28;
    }
    s->unk24 = s->unk24 - D_800B6D28;
    if (s->unk24 < 0.0f) {
        s->unk24 = 0.0f;
    }
    e = s->list;
    if (e != 0) {
        k1 = 1.0f;
        k2 = -3.0f;
        do {
            next = e->next;
            if (s->unk24 <= 0.0f && e->unk8 > 0.0f) {
                e->unk8 = 0.0f;
            }
            if (e->unk8 <= 0.0f) {
                e->unk8 = e->unk8 - k1;
                if (e->unk8 < k2) {
                    func_00236040(a0, s, e);
                }
            } else {
                e->unk8 = e->unk8 - D_800B6D28;
                if (e->unk8 < 0.0f) {
                    e->unk8 = 0.0f;
                }
            }
            e = next;
        } while (e != 0);
    }
    ow = (Owner3F94 *)s->owner;
    if (ow != 0 && (s->flags & 1) && !(ow->flags & 0x200)) {
        func_002368B0(s);
    }
    D_800B6D28 = saved;
    if (s->unk48 != 0 || (s->unk24 != 0.0f && s->owner != 0)) {
        return 1;
    }
    return 0;
}
