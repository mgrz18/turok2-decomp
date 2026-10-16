/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_22.c, func_802BB550_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
extern struct IntPair D_801229A0[];
void func_0029B950(s32 arg0, s32 arg1, s32 arg2) {
    extern void func_0029E010(u32);
    extern u32 func_0029DFF0(void);
    u32 temp_v0;
    struct IntPair *e;
    temp_v0 = func_0029DFF0();
    ;
    (&D_801229A0[arg0])->field_0 = arg1;
    (*(&D_801229A0[arg0])).field_4 = arg2;
    func_0029E010(temp_v0);
}
