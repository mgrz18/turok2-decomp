/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RESMGR/resmgr_string_memory.CPP, func_802A0238_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
s32 func_00426D8C(u8 *arg0) {
    u8 *var_v1;
    u8 temp;
    var_v1 = arg0 + 1;
    if (*arg0 != 0) {
        do {
            temp = *var_v1;
            var_v1 += 1;
        } while (temp != 0);
    }
    return (var_v1 - arg0) - 1;
}
