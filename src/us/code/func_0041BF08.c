/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444530.c, func_00444530.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
typedef struct { u8 pad[0x2B]; s8 f; } O06C; /* player options */
typedef struct { u8 pad[0x518]; O06C *options; } O06B; /* options ptr at 0x518 (0x5D8 in Rage Wars) */
typedef struct { u8 pad[0x1C]; O06B *unk1C; } O06A;
extern s32 func_0041648C(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
extern u8 D_8011AB18;
s32 func_0041BF08(s32 a, O06A *b) {
    O06C *p = (O06C *)&D_8011AB18;
    if (b->unk1C) { O06C *q = b->unk1C->options; if (q) p = q; }
    p->f = func_0041648C((s32)b, p->f, 1, 0, 1, 1);
    return 0;
}
