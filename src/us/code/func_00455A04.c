/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern s32 D_8011ACE4;
extern char D_800B5C6C[];
extern char D_800B5C60[];
s32 func_00455A04(struct FieldRecAt14 *field) {
    if (D_8011ACE4 != 0) {
        field->text = D_800B5C6C;
    } else {
        field->text = D_800B5C60;
    }
    return 0;
}
extern s32 D_8011ACE4;
