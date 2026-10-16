/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409144_de_14.c, func_80409744_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern s32 D_800C2028;
extern void func_004211AC(void);
void func_00426480(s32);
s32 func_0042655C(s32);
void func_004211AC(void) {
    s32 var_s0;
    s32 var_s1;
    s32 unavailable;
    if (D_800C2028 == -1) {
        var_s0 = 0;
        var_s1 = 0;
    unavailable = -2;
loop_2:
        func_00426480(var_s0);
        if (func_0042655C(var_s0) == unavailable) {
            var_s0 += 1;
            if (var_s0 >= 4) {
                var_s0 = 0;
            }
            var_s1 += 1;
            if (var_s1 < 4) {
                goto loop_2;
            }
        }
        if (var_s1 == 4) {
            var_s0 = -1;
        }
        D_800C2028 = var_s0;
    }
}
