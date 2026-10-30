/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RESMGR/resmgr_string_memory.CPP, func_802A0324_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern u8 *func_0045FE7C(u8 *destination, u8 *source, s32 count);
u8 *func_0045FE7C(u8 *destination, u8 *source, s32 count) {
    u8 *end = destination + 1;
    u8 byte;
    if (*destination != 0) {
        do { } while (*end++ != 0);
    }
    --end;
    while (count-- != 0) {
        byte = *source++;
        *end++ = byte;
        if (!(byte & 0xFF)) return destination;
    }
    *end = 0;
    return destination;
}
