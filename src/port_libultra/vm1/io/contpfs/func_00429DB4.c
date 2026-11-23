/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/__osRepairPackId.c, __osIdCheckSum.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
inline s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data;
    u32 j;
    *csum = *icsum = 0;
    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u8 *)ptr + j);
        *csum += data;
        *icsum += ~data;
    }
    return 0;
}
