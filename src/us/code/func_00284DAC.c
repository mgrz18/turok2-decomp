/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_7.c, func_80263CF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct ObjectState30;
typedef struct ObjectState30 ObjectState30;
extern void func_00284DAC(void *arg0);
struct ObjectState224;
typedef struct ObjectState224 ObjectState224;
struct ObjectState30;
struct ObjectState30 {
    unsigned char padding_0[44];
    s8 unk_2C;
    s8 unk_2D;
    s8 unk_2E;
    s8 unk_2F;
};
struct ObjectState224;
struct ObjectState224 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s8 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s8)];
    s32 unk_8;
    char pad8[0xC - 0x8 - sizeof(s32)];
    s32 unk_C;
    char padC[0x10 - 0xC - sizeof(s32)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    char pad14[0x18 - 0x14 - sizeof(s32)];
    s32 unk_18;
    char pad18[0x1C - 0x18 - sizeof(s32)];
    s32 unk_1C;
    char pad1C[0x20 - 0x1C - sizeof(s32)];
    s32 unk_20;
    char pad20[0x24 - 0x20 - sizeof(s32)];
    s32 unk_24;
    char pad24[0x28 - 0x24 - sizeof(s32)];
    s32 unk_28;
    char pad28[0xC4 - 0x28 - sizeof(s32)];
    s8 unk_C4;
    char padC4[0xC5 - 0xC4 - sizeof(s8)];
    s8 unk_C5;
    char padC5[0xC6 - 0xC5 - sizeof(s8)];
    s8 unk_C6;
    char padC6[0xC7 - 0xC6 - sizeof(s8)];
    s8 unk_C7;
    char padC7[0xC8 - 0xC7 - sizeof(s8)];
    s32 unk_C8;
    char padC8[0xCC - 0xC8 - sizeof(s32)];
    s32 unk_CC;
    char padCC[0xD0 - 0xCC - sizeof(s32)];
    s32 unk_D0;
    char padD0[0xD4 - 0xD0 - sizeof(s32)];
    s32 unk_D4;
    char padD4[0x220 - 0xD4 - sizeof(s32)];
    s32 unk_220;
};
extern u8 D_800F677B[];
extern s32 func_0026EB3C(void *arg0, void *arg1, s32 arg2);
void func_00284DAC(void *arg0) {
    char *o = (char *) arg0;
    s32 type;
    s32 bit;
    s32 bit2;
    s32 i;
    type = ((ObjectState224 *)(o))->unk_4;
    bit = ((D_800F677B[type * 4] >> 3) ^ 1) & 1;
    if (bit != 0 && ((ObjectState224 *)(o))->unk_0 == 0) {
        i = 0;
        ((ObjectState224 *)(o))->unk_4 = type;
        ((ObjectState224 *)(o))->unk_0 = 0;
        ((ObjectState224 *)(o))->unk_C8 = 0;
        ((ObjectState224 *)(o))->unk_CC = 0;
        ((ObjectState224 *)(o))->unk_220 = 0;
        ((ObjectState224 *)(o))->unk_8 = 0;
        ((ObjectState224 *)(o))->unk_C = 0;
        ((ObjectState224 *)(o))->unk_10 = 0;
        ((ObjectState224 *)(o))->unk_C4 = 0;
        ((ObjectState224 *)(o))->unk_C5 = 0;
        ((ObjectState224 *)(o))->unk_C6 = 0;
        ((ObjectState224 *)(o))->unk_C7 = 0;
        ((ObjectState224 *)(o))->unk_14 = 0;
        ((ObjectState224 *)(o))->unk_18 = 0;
        ((ObjectState224 *)(o))->unk_1C = 0;
        ((ObjectState224 *)(o))->unk_20 = 0;
        ((ObjectState224 *)(o))->unk_24 = 0;
        ((ObjectState224 *)(o))->unk_28 = 0;
        do {
            ((struct ObjectState30 *) (o + (i * 4)))->unk_2C = 0;
            ((struct ObjectState30 *) (o + (i * 4)))->unk_2D = 0;
            ((struct ObjectState30 *) (o + (i * 4)))->unk_2E = 0x30;
            ((struct ObjectState30 *) (o + (i * 4)))->unk_2F = 0;
            i += 1;
        } while (i < 0x20);
        bit2 = ((D_800F677B[type * 4] >> 3) ^ 1) & 1;
        ((ObjectState224 *)(o))->unk_D0 = 0;
        ((ObjectState224 *)(o))->unk_D4 = 0;
        ((ObjectState224 *)(o))->unk_0 = bit2;
        func_0026EB3C(o + 0x140, o + 0x16C, 3);
    }
    ((ObjectState224 *)(o))->unk_0 = bit;
}
