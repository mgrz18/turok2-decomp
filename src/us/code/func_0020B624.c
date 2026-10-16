/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026AC38_de_7.c, func_8026D9D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern void func_0020A994();
extern void func_0020A568();
extern void func_0020B624();
extern s32 D_800DD170;
extern void func_002051F4(s32 arg0, s32 arg1);
extern s32 D_800DD178[];
void func_0020B624(void) {
    extern void func_0020565C(s32 arg0, s32 arg1);
    s32 temp_s0;
    s32 var_s1;
    func_0020A568();
    var_s1 = 0;
    func_0020A994();
    if (D_800DD170 > 0) {
        do {
            temp_s0 = D_800DD178[var_s1];
            func_0020565C(0, temp_s0);
            func_002051F4(0, temp_s0);
            var_s1 += 1;
        } while (var_s1 < D_800DD170);
    }
}
