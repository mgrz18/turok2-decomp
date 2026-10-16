/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A0294_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern u8 *func_00426DB0(u8 *arg0, u8 *arg1, s32 arg2);
u8 *func_00426DB0(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 var_a2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_a1;
    u8 temp_v0;
    var_a0 = arg0;
    var_a1 = arg1;
    var_a2 = arg2;
    temp_v1 = var_a0;
    if (var_a2 != 0) {
    loop_1:
        temp_v0 = *var_a1;
        var_a1 += 1;
        *var_a0 = temp_v0;
        var_a0 += 1;
        if (temp_v0 & 0xFF) {
            var_a2 -= 1;
            if (var_a2 != 0) {
                goto loop_1;
            }
        }
        if (var_a2 != 0) {
            var_a2 -= 1;
            if (var_a2 != 0) {
                do {
                    *var_a0 = 0;
                    var_a2 -= 1;
                    var_a0 += 1;
                } while (var_a2 != 0);
            }
        }
    }
    return temp_v1;
}
