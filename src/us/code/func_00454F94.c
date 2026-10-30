/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044465C.c, func_0044465C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 pad[0x2C]; u8 f; } O06C;
typedef struct { u8 pad[0x518]; O06C *opts; } O06B; /* player options */
typedef struct { u8 pad[0x14]; void *unk14; u8 pad2[4]; O06B *unk1C; } O06A;
extern u8 D_8011AB18;
extern u8 D_800B3F18[];
extern u8 D_800B3F00[];
s32 func_00454F94(O06A *a, O06A *b) {
O06C *p = (O06C *)&D_8011AB18;
if (b->unk1C) { O06C *q = b->unk1C->opts; if (q) p = q; }
 if (p->f != 0) { if (p->f == 1) a->unk14 = D_800B3F00; } else a->unk14 = D_800B3F18;
 return 0; }
