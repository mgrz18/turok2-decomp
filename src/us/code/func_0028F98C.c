/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_10.c, __readVarLen.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
extern s32 func_0028F460(s32, s32);
s32 func_0028F98C(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 var_s0;
    var_s0 = (u8) func_0028F460(arg0, arg1);
    if (var_s0 & 0x80) {
        var_s0 &= 0x7F;
        do {
            temp_v0 = (u8) func_0028F460(arg0, arg1);
            var_s0 = (var_s0 << 7) + (temp_v0 & 0x7F);
        } while (temp_v0 & 0x80);
    }
    return var_s0;
}
