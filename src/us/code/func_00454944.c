/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0043DC50.c, func_0043DC50.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad[8]; u32 unk8; u8 pad2[8]; void *unk14; } O06A;
extern s32 D_8011AADC;
extern s32 D_8011AAD8;
extern u8 D_800B5AF4[];
extern u8 D_800B5B10[];
s32 func_00454944(O06A *a) {
    if (D_8011AADC & 0x8000) a->unk8 |= 0x1000000; else a->unk8 &= ~0x1000000;
    if (D_8011AAD8 & 0x8000) a->unk14 = D_800B5AF4; else a->unk14 = D_800B5B10;
    return 0;
}
