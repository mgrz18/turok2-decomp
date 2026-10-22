/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160_3.CPP, func_80268864_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct IntPair;
struct IntPair;
struct IntPair;
struct IntPair;
extern s32 D_8011ACA4;
extern void func_00246860(u8 *, u8 *, s32);
void func_00239350(void *unused, u8 *object, s32 third, struct IntPair pair, s32 fifth, s32 sixth) {
    if (D_8011ACA4 == 0 && object != 0 && object[0] == 1) {
        do {
            func_00246860(object, object + 0x140, sixth);
        } while (0);
    }
}
