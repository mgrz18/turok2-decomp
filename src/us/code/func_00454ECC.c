/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444594.c, func_00444594.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed int s32;
typedef struct { u8 pad[0x2B]; s8 f; } O06C; /* player options (u8 fields in Turok 2) */
typedef struct { u8 pad[0x518]; O06C *options; } O06B; /* options ptr at 0x518 (0x5D8 in Rage Wars) */
typedef struct { u8 pad[0x14]; void *unk14; u8 pad2[4]; O06B *unk1C; } O06A;
extern u8 D_8011AB18;
extern u8 D_800B3EB8[];
extern u8 D_800B3ED0[];
s32 func_00454ECC(O06A *a, O06A *b) {
O06C *p = (O06C *)&D_8011AB18;
if (b->unk1C) { O06C *q = b->unk1C->options; if (q) p = q; }
 switch (p->f) { case 0: a->unk14 = D_800B3EB8; break; case 1: a->unk14 = D_800B3ED0; break; }
 return 0; }
