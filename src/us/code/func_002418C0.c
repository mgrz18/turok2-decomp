/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_24.c, func_8023EE60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
