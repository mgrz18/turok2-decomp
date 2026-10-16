/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_17.c, func_802B7B90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
extern int __osDpDeviceBusy();
extern u32 func_0029BF80(void *arg0);
s32 func_00298D50(void *bufPtr, u64 size) {
    if (__osDpDeviceBusy()) {
        return -1;
    }
    ((*(volatile u32 *)(0xA410000CU)) = (u32)(1));
    do {
    } while ((*(volatile u32 *)(0xA410000CU)) & 1);
    ((*(volatile u32 *)(0xA4100000U)) = (u32)(func_0029BF80(bufPtr)));
    ((*(volatile u32 *)(0xA4100004U)) = (u32)(func_0029BF80(bufPtr) + size));
    return 0;
}
