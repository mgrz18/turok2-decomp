/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802607B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
u32 func_00217C58(u32 *stream, u32 width) {
    u32 bitAddress;
    u32 shift;
    u32 *word;
    u32 value;
    u32 next;
    bitAddress = *stream;
    word = (u32 *) ((bitAddress & 0xF0000000) |
                    ((bitAddress & 0x0FFFFFE0) >> 3));
    shift = bitAddress & 0x1F;
    value = word[0];
    next = word[1];
    if (shift != 0) {
        value >>= shift;
        next <<= 0x20 - shift;
        value |= next;
    }
    if (width < 0x20U) {
        value &= (1 << width) - 1;
    }
    *stream = bitAddress + width;
    return value;
}
