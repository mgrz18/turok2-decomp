/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_10.c, func_80448BB4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
s32 func_00462CD4(u16 *block, u16 *sum, u16 *inverse) {
    u32 offset;
    *inverse = 0;
    *sum = 0;
    for (offset = 0; offset < 0x1C; offset += 2) {
        u16 value = *(u16 *) ((char *) block + offset);
        *sum += value;
        *inverse += (u16) ~value;
    }
    return 0;
}
