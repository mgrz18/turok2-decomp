/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044121C.c, func_0044121C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct { u8 pad[0x1C]; s32 unk1C; } O06Q;
typedef struct { u8 pad0[8]; u32 unk8; u8 pad[4]; u8 b[4]; u8 pad14[4]; O06Q *unk18; } O06E;
typedef struct { s16 unk0; u8 pad2[0xA]; u8 *unkC; s32 unk10; } O06A;
void func_0044E1B4(O06A *a, u8 idx) {
    O06E *e;
    s32 d;
    e = (O06E *)(a->unkC + a->unk0 * 40);
    do {
        d = a->unk0;
        a->unk0 += (s8)((u8 *)e)[idx];
        if (a->unk0 < 0) a->unk0 = a->unk10 - 1; else if (a->unk0 >= a->unk10) a->unk0 = 0;
        e = (O06E *)(a->unkC + a->unk0 * 40);
        d = d - a->unk0;
        switch (idx) {
        case 0x10: if (e->unk8 & 0x80000) e->b[1] = d; break;
        case 0x11: if (e->unk8 & 0x40000) e->b[0] = d; break;
        case 0x12: if (e->unk8 & 0x200000) e->b[3] = d; break;
        case 0x13: if (e->unk8 & 0x100000) e->b[2] = d; break;
        }
    } while ((e->unk8 & 0x1800000) != 0x1800000 || e->unk18->unk1C == 0);
}
