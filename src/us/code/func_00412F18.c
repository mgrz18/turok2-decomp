/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80400000_de_11.c, func_80403B70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
s32 func_00412F18(void *object) {
    extern struct IntPair *func_002017D4(void *, s32);
    struct IntPair *first = func_002017D4(object, 0);
    struct IntPair *second = func_002017D4(object, 2);
    s32 size = first->field_4 * 36;
    return size + second->field_4 * 36 + 0x30;
}
