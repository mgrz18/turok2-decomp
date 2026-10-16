/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80446F60_de_10.c, func_80448B84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
extern u16 func_00462CA4(u8 *bytes, s32 count);
u16 func_00462CA4(u8 *bytes, s32 count)
{
    s32 i;
    s32 sum;
    i = 0;
    sum = i;
    for (; i < count; i++) {
        sum += *bytes++;
        sum &= 0xFFFF;
    }
    return sum;
}
