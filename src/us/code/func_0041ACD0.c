/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CD64_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
s32 func_0029E4C0(u8 *);
extern u8 D_800C1F8C[0x64];
s32 func_0041ACD0(u8 *arg0) {
    u8 *input = arg0;
    s32 index = 0;
    if (func_0029E4C0(input) != func_0029E4C0(D_800C1F8C)) {
        return 0;
    }
    for (; index < func_0029E4C0(input); index++) {
        if ((input[index] ^ index) != D_800C1F8C[index]) {
            return 0;
        }
    }
    return 1;
}
