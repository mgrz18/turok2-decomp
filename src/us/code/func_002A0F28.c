/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_14.c, func_802BFE68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern void func_002A0F28(s32 *arg0, s32 arg1);
void func_002A0F28(s32 *arg0, s32 arg1) {
    s32 *ptr;
    s32 i;
    s32 j;
    s32 k;
    s32 tmp;
    s32 n2;
    i = 0;
    n2 = arg1 * 2;
    if (arg1 > 0) {
        ptr = arg0;
        do {
            *ptr = i;
            i++;
            ptr++;
        } while (i < arg1);
    }
    j = 1;
    i = 1;
    if (n2 > 0) {
        do {
            if (j > i) {
                tmp = arg0[(j - 1) / 2];
                arg0[(j - 1) / 2] = arg0[(i - 1) / 2];
                arg0[(i - 1) / 2] = tmp;
            }
            k = arg1;
            while (k >= 2 && j > k) {
                j -= k;
                k >>= 1;
            }
            i += 2;
            j += k;
        } while (n2 >= i);
    }
}
