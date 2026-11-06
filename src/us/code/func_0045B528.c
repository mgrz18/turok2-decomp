/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88.CPP, func_8040A9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B4B94[];
extern char D_800B4B84[];
extern char D_800B4B8C[];
extern int sCharNoteLoad;
extern s32 D_801309A8;
s32 CharNoteMenu_UpdatePrompt_2(struct FieldRecAt14 *field) {
    if (sCharNoteLoad != 0) {
        field->text = D_800B4B84;
    } else if (D_801309A8 != 0) {
        field->text = D_800B4B94;
    } else {
        field->text = D_800B4B8C;
    }
    return 0;
}
