/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAC50_de_30.c, func_802BADC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
extern u32 func_0029DFF0(void);
extern u32 func_0029DFC0(void);
extern void func_0029E010(u32);
extern s32 D_80122A20;
extern u64 D_80122A28;
u64 func_0029B170(void) {
    u32 saved;
    u32 count;
    u32 elapsed;
    u64 time;
    saved = func_0029DFF0();
    count = func_0029DFC0();
    elapsed = count - D_80122A20;
    time = D_80122A28;
    func_0029E010(saved);
    return time + elapsed;
}
