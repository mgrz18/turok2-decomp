/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004465B4.c, func_004465B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct { s32 unk0; f32 unk4; s32 unk8; u8 pad[12]; } O06R;
typedef struct { s16 unk0; u8 pad[0x1E]; void *unk20; } O06A2;
extern s32 func_0025340C(void *a, s32 b);
extern s32 func_00285628(void *a);
extern s32 func_00285670(void *a);
extern u8 D_80119870[];
extern O06R D_800C1FB0[];
extern f32 D_800B6D28;
extern f32 D_800C1348;
s32 func_00456214(s32 a, O06A2 *b) {
    s32 n = func_0025340C(D_80119870, *(s32 *)((u8 *)b + 0x1C));
    f32 v = D_800C1FB0[n].unk4 + D_800B6D28;
    D_800C1FB0[n].unk4 = v;
    if (D_800C1348 <= v) D_800C1FB0[n].unk4 = v - D_800C1348;
    if (b->unk0 == 1) {
        if (func_00285628(b->unk20) && D_800C1FB0[n].unk0 > 0) {
            D_800C1FB0[n].unk0--;
            D_800C1FB0[n].unk4 = 0;
        } else if (func_00285670(b->unk20) && D_800C1FB0[n].unk0 < D_800C1FB0[n].unk8) {
            D_800C1FB0[n].unk0++;
            D_800C1FB0[n].unk4 = 0;
        }
    }
    return 0;
}
