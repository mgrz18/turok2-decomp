/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258F6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef unsigned int u32;
struct SortTable;
typedef struct SortTable SortTable;
struct SortTable;
struct SortTable {
    char unknown00[0xE];
    s16 count;
    u32 entries[1];
};
extern void func_00271B20(SortTable *table);
void func_00271B20(SortTable *table) {
    int i;
    int j;
    u32 right;
    u32 left;
    for (i = 0; i < table->count; i++) {
        for (j = i + 1; j < table->count; j++) {
            right = table->entries[j];
            left = table->entries[i];
            if (right < left) {
                table->entries[i] = right;
                table->entries[j] = left;
            }
        }
    }
}
