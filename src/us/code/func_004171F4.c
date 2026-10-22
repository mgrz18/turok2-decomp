/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443298.c, func_00443298.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct { u32 w[6]; } O06Seg;
typedef struct { u8 pad[0x110]; O06Seg seg[1]; } O06A; /* seg at 0x110 in Turok 2 (0x140 in Rage Wars) */
typedef struct { f32 f0; s32 unk4; s32 unk8; f32 fC; s32 unk10; } O06Cam;
extern s32 D_800AB8B0;
extern O06Cam D_800AB8B4[];
extern O06Seg D_800AB0E8;
extern s32 D_800B6D1C;
extern s32 D_801308E0;
extern void func_0021D928(void *a, s32 b, s32 c);
void func_004171F4(O06A *a, s32 b, s32 c) {
 D_800AB8B0 = 3;
 a->seg[D_800B6D1C] = D_800AB0E8;
 D_800AB8B4->f0 = 255.0f;
 D_800AB8B4->unk4 = 0;
 D_800AB8B4->unk8 = 0;
 D_800AB8B4->unk10 = 0;
 D_800AB8B4->fC = (f32)(D_801308E0 / 2);
 func_0021D928(a, b, c);
 D_800AB8B0 = 0;
}
