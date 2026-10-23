/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/IO/80200800_2.CPP, func_80200AD8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;

/* Store byte arg1 at the unaligned address arg0 via a word read-modify-write. */
void func_002A1808(s32 arg0, s32 arg1) {
    s32 shift = (~arg0 & 3) * 8;
    u32 *word = (u32 *)(arg0 & ~3);
    u32 old = *word;
    *word = (old & ~(0xFF << shift)) | ((arg1 & 0xFF) << shift);
}
