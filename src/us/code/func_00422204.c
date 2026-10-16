/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_11.c, func_8040A9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B4AF8[];
extern char D_800B4AE0[];
extern char D_800B4AEC[];
extern int D_80130960;
extern s32 D_801309A8;
s32 func_00422204(struct FieldRecAt14 *field) {
    if (D_80130960 != 0) {
        field->text = D_800B4AE0;
    } else if (D_801309A8 != 0) {
        field->text = D_800B4AF8;
    } else {
        field->text = D_800B4AEC;
    }
    return 0;
}
