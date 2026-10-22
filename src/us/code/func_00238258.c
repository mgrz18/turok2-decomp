/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00267540.c, func_00267540.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { s32 kind; } P3Kind;
typedef struct {
    u8 pad0[0x14];
    P3Kind *info;           /* 0x14 (0x18 in Rage Wars) */
    u8 pad18[0xB8 - 0x18];
    u16 id;                 /* 0xB8 (0xE4) */
    u8 padBA[0xD4 - 0xBA];
    u32 flags;              /* 0xD4 (0x100) */
} P3Actor;
extern u8 D_8011AAF1;
extern s32 D_8011001C;
s32 func_00238258(P3Actor *a, P3Actor *b) {
    if (D_8011AAF1 != 0) {
        return 1;
    }
    if (a->id == b->id || !(b->flags & 0x310000)) {
        return 0;
    }
    if (D_8011001C != 0 && !(a->flags & 0x300000) && !(b->flags & 0x300000)) {
        return 0;
    }
    switch (b->info->kind) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        return 1;
    }
    return 0;
}
