/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/FRONTEND/8040B45C_18.CPP, func_8040B3DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern s32 D_801309B8;
extern char D_800B4B84[];
extern char D_800B4B8C[];
s32 func_0045B010(struct FieldRecAt14 *field) {
    if (D_801309B8 != 0) {
        field->text = D_800B4B84;
    } else {
        field->text = D_800B4B8C;
    }
    return 0;
}
extern s32 D_801309B8;
