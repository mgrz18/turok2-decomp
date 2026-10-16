/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_21.c, func_802BBBC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern s32 func_0029E140(void);
s32 func_0029BF80(s32 arg0) {
    if (arg0 < 0 && (u32)arg0 <= 0x9FFFFFFFU) {
        return arg0 & 0x1FFFFFFF;
    }
    if ((u32)(arg0 + 0x60000000) > 0x1FFFFFFFU) {
        return func_0029E140();
    }
    return arg0 & 0x1FFFFFFF;
}
