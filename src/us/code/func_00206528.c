/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_14.c, func_80254D00_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
extern void func_0020656C(s32, s32);
extern int D_800D8218;
extern int D_800D8208[];
extern void func_00206528(void);
void func_00206528(void) {
    s32 *p = &D_800D8218;
    func_0020656C(0, *p);
    D_800D8208[*p] = 0;
}
