/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80444030_6.CPP, func_8044472C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B3FC0[];
extern u8 D_8011AAEF;
extern char D_800B3FA8[];
extern char D_800B3F90[];
s32 func_004551D4(struct FieldRecAt14 *field) {
    switch (D_8011AAEF) {
    case 0:
        field->text = D_800B3F90;
        break;
    case 1:
        field->text = D_800B3FA8;
        break;
    case 2:
        field->text = D_800B3FC0;
        break;
    default:
        return 0;
    }
    return 0;
}
