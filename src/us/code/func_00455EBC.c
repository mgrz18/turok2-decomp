/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443F40.c, func_00443F40.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
/* Turok 2 layout: the field Rage Wars keeps at 0x58 is at 0x1E8 here. */
typedef struct {
    u8 pad0[0x120];
    u32 unk120;
    u8 pad124[0x1C0 - 0x124];
    u32 unk1C0;
    u8 pad1C4[0x1E8 - 0x1C4];
    u32 unk1E8;
    u8 pad1EC[0x210 - 0x1EC];
    u32 unk210;
    u8 pad214[0x238 - 0x214];
    u32 unk238;
    u8 pad23C[0x260 - 0x23C];
    u32 unk260;
} O06B;
typedef struct { u8 pad[0xC]; O06B *unkC; u8 pad2[0xC]; void *unk1C; } O06A;
extern s32 func_0025340C(void *a, void *b);
extern void func_00285A68(s32 a);
extern s32 D_800C2020;
extern u8 D_80119870[];
extern s32 D_8011ACE4[];
void func_00455EBC(O06A *a) {
    s32 n;
    D_800C2020 = 0;
    n = 0;
    if (a->unk1C) n = func_0025340C(D_80119870, a->unk1C);
    func_00285A68(n);
    a->unkC->unk238 |= 0x1800000;
    a->unkC->unk260 |= 0x1800000;
    a->unkC->unk1C0 &= ~0x1800000;
    a->unkC->unk1E8 &= ~0x1800000;
    a->unkC->unk210 &= ~0x1800000;
    if (D_8011ACE4[0]) a->unkC->unk120 |= 0x1000000; else a->unkC->unk120 &= ~0x1000000;
}
