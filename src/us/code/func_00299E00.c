/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7B80_us_28.c, func_802B8C40_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

int func_00299E00(unsigned int arg0) {
    int var_a1;
    int var_a2;
    int var_a3;
    unsigned int var_a0;
    unsigned int temp_v1;
    int temp_v0;
    var_a0 = arg0;
    var_a1 = 0;
    var_a3 = 0xF;
    do {
        var_a2 = 0;
        if (var_a1 & 0x10) {
            var_a2 = 0x15;
        }
        var_a1 = var_a1 * 2;
        temp_v1 = var_a0 >> 10;
        var_a0 = var_a0 * 2;
        temp_v0 = ((var_a1 & 0xFF) | (temp_v1 & 1)) ^ var_a2;
        var_a1 = temp_v0;
        var_a3 -= 1;
    } while (var_a3 >= 0);
    return temp_v0 & 0x1F;
}
