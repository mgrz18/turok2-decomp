/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028FD2C_6.CPP, func_8028FDF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
s32 func_00201818(s32 *table, s32 index) {
    s32 nextIndex = index + 1;
    s32 *next = &table[nextIndex];
    s32 *entry = &table[index];
    s32 size;
    s32 *output = &size;
    *output = next[1] - entry[1];
    return size;
}
