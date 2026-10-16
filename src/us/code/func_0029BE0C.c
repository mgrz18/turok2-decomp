/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_18.c, func_802BBA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef unsigned long long u64;
extern u32 D_80122A30;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
extern u32 func_0029DFC0(void);
extern void func_0029E210(u32 compare);
extern void func_0029BE0C(u64 tim);
void func_0029BE0C(u64 interval) {
    u64 compare;
    u32 saved = func_0029DFF0();
    D_80122A30 = func_0029DFC0();
    compare = D_80122A30 + interval;
    func_0029E210(compare);
    func_0029E010(saved);
}
