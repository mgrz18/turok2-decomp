/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802798A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct WeightedTable;
struct WeightedTable {
    s16 count;
    s16 entries[25][2];
    s16 reserved;
};
struct WeightedTable;
typedef struct WeightedTable WeightedTable;
extern s32 func_002113A4(void);
extern s32 func_002113A4(void);
s16 func_0026D628(WeightedTable *arg0) {
    s16 *entry;
    s16 *start;
    s32 random;
    s32 total;
    s32 value;
    if (arg0->count == 0) {
        return -1;
    }
    arg0->entries[arg0->count][0] = -1;
    entry = &arg0->entries[0][0];
    total = 0;
    start = entry;
    if (*entry != -1) {
        do {
            total += entry[1];
            entry += 2;
        } while (*entry != -1);
    }
    random = func_002113A4();
    entry = start;
    value = random % total;
    total = 0;
    while (*entry != -1) {
        total += entry[1];
        if (total >= value) {
            break;
        }
        entry += 2;
    }
    return *entry;
}
