/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80265350_de.c, func_80265878_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
extern char CList__Construct;
extern char D_A4694;
extern char func_00400000;
extern char D_29E60;
int func_00267024(u32 arg0) {
    if (arg0 >= (u32)&CList__Construct &&
        arg0 < (u32)&CList__Construct + (u32)&D_A4694) {
        return 1;
    }
    if (arg0 >= (u32)&func_00400000 &&
        arg0 < (u32)&func_00400000 + (u32)&D_29E60) {
        return 1;
    }
    return 0;
}
