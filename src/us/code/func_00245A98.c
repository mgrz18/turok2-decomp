/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80213ED4_de_10.c, func_802166F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/ai.h"

extern void WeightedTable_Clear(WeightedTable *);
extern void WeightedTable_Add(WeightedTable *, s32, s32);
extern s16 WeightedTable_Pick(WeightedTable *);
extern s32 CGameObjectInstance__PickRandomAnim(s32, s32, s32);
s32 AI_PickAnimFromTable(s32 owner, s32 unused, struct IntPair *table, s32 flags) {
    WeightedTable picker;
    s32 first;
    s16 pick;
    WeightedTable_Clear(&picker);
    first = table->field_4;
    while (table->field_0 != -1) {
        if (table->field_0 == 0 && picker.count != 0) {
            goto choose;
        }
        if ((flags & table->field_0) && CGameObjectInstance__PickRandomAnim(owner, table->field_4, -1) != -1) {
            WeightedTable_Add(&picker, (s16) table->field_4, 10);
        }
        table++;
    }
    if (picker.count == 0) {
        WeightedTable_Add(&picker, (s16) first, 10);
    }
choose:
    pick = WeightedTable_Pick(&picker);
    if (CGameObjectInstance__PickRandomAnim(owner, pick, -1) == -1) {
        return first;
    }
    return pick;
}
