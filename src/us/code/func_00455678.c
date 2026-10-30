/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00444D58.c, func_00444D58.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef struct { u8 pad[0x31]; u8 unk31; } O06C; /* player options */
typedef struct { u8 pad[0x518]; O06C *unk518; } O06B;
typedef struct { u8 pad[0x14]; void *unk14; u8 pad2[4]; O06B *unk1C; } O06A;
extern u8 D_8011AB18;
extern u8 D_800B40B0[];
extern u8 D_800B40C8[];
s32 func_00455678(O06A *a, O06A *b) {
 O06B *q = b->unk1C;
 O06C *p;
 if (q) p = q->unk518; else p = (O06C *)&D_8011AB18;
 if (p->unk31 == 1) a->unk14 = D_800B40B0; else a->unk14 = D_800B40C8;
 return 0;
}
