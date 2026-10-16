/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80213ED4_de_10.c, func_802166F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct WeightedTable;
struct WeightedTable {
    s16 count;
    s16 entries[25][2];
    s16 reserved;
};
struct WeightedTable;
typedef struct WeightedTable WeightedTable;
extern void func_0026D5DC(WeightedTable *);
extern void func_0026D5E4(WeightedTable *, s32, s32);
extern s16 func_0026D628(WeightedTable *);
extern s32 func_00219F74(s32, s32, s32);
s32 func_00245A98(s32 owner, s32 unused, struct IntPair *table, s32 flags) {
    WeightedTable picker;
    s32 first;
    s16 pick;
    func_0026D5DC(&picker);
    first = table->field_4;
    while (table->field_0 != -1) {
        if (table->field_0 == 0 && picker.count != 0) {
            goto choose;
        }
        if ((flags & table->field_0) && func_00219F74(owner, table->field_4, -1) != -1) {
            func_0026D5E4(&picker, (s16) table->field_4, 10);
        }
        table++;
    }
    if (picker.count == 0) {
        func_0026D5E4(&picker, (s16) first, 10);
    }
choose:
    pick = func_0026D628(&picker);
    if (func_00219F74(owner, pick, -1) == -1) {
        return first;
    }
    return pick;
}
