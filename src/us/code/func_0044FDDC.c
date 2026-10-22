/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00442E80.c, func_00442E80.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad[0x328]; s32 unk328; } O06A; /* 0x478 in Rage Wars */
typedef struct { s32 unk0; s16 unk4; s16 pad6; u32 unk8; s16 unkC; s16 unkE; u8 unk10; u8 unk11; u8 unk12; u8 unk13; s32 unk14; s32 unk18; s32 unk1C; O06A *unk20; s32 unk24; } O06E;
typedef struct { s16 unk0; s16 pad2; s32 unk4; s16 unk8; s16 unkA; u8 unkC; u8 unkD; u8 unkE; u8 unkF; s32 unk10; } O06S;
/* Extra block size for an item: kind 3 carries 0x330 bytes (0x480 in Rage Wars); inlined. */
static __inline__ s32 func_00442E48(s16 *a) {
    s32 n = 0;
    if (*a == 3) n = 0x330;
    return n;
}
static __inline__ void func_00442E64(s16 *a, O06A *b) {
    if (*a == 3) b->unk328 = 1;
}
void func_0044FDDC(O06E *a, O06S *b, O06A **c, s32 d, s32 e) {
    s32 sz;
    a->unk0 = d;
    a->unk4 = b->unk0;
    a->unk8 = b->unk4 | 0x1800000;
    a->unkC = b->unk8;
    a->unkE = b->unkA;
    a->unk10 = b->unkC;
    a->unk11 = b->unkD;
    a->unk12 = b->unkE;
    a->unk13 = b->unkF;
    a->unk14 = b->unk10;
    a->unk18 = (s32)b;
    a->unk1C = -1;
    a->unk24 = e;
    sz = func_00442E48((s16 *)b);
    if (sz) {
        a->unk20 = *c;
        func_00442E64((s16 *)b, *c);
        *c = (O06A *)((u8 *)*c + func_00442E48((s16 *)b));
    }
}
