/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_24.c, func_8023EE60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
struct Dst;
typedef struct Dst Dst;
struct Dst;
struct Dst {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
};
struct Reusable;
typedef struct Reusable Reusable;
struct EntryRecAt24;
typedef struct EntryRecAt24 EntryRecAt24;
struct Dst;
struct EntryRecAt24;
struct EntryRecAt24 {
    struct Dst *dst;
    s32 val0;
    u8 pad1[3];
    u8 flag0;
    u8 pad2[3];
    u8 flag1;
    u8 pad3[3];
    u8 flag2;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 extra;
};
struct Reusable;
struct Reusable {
    s32 unk0;
    char pad4[0x10];
    s32 unk14;
    char pad18[0x70];
    s32 unk88;
    char pad8C[0x10];
    s32 unk9C;
    char padA0[0x10];
    s32 unkB0;
    s32 unkB4;
    char padB8[0xC];
    s32 unkC4;
};
extern Reusable *D_800F1ADC;
void func_002418C0(EntryRecAt24 *arg0)
{
    EntryRecAt24 *e = arg0;
    s32 counter = -1;
    if (e->dst != 0) {
        do {
            Dst *dst = e->dst;
            D_800F1ADC->unk0 = 0;
            D_800F1ADC->unk14 = counter;
            D_800F1ADC->unk88 = 0;
            D_800F1ADC->unk9C = 0;
            D_800F1ADC->unkB0 = 0;
            D_800F1ADC->unkB4 = 0;
            D_800F1ADC->unkC4 = 0;
            dst->unk0 = e->val0;
            dst->unk4 = e->flag0;
            dst->unk5 = e->flag1;
            dst->unk6 = e->flag2;
            dst->unk8 = e->x;
            dst->unkC = e->y;
            dst->unk10 = e->z;
            dst->unk14 = e->w;
            dst->unk18 = e->extra;
            e += 1;
        } while (e->dst != 0);
    }
}
