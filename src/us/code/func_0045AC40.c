/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88_7.CPP, func_8040A37C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct Field;
struct Field {
    char pad[0x14];
    char **text;
    char pad18[0x4];
    signed int value;
};
extern char *D_800B48E4[];
extern char *D_800B4888[];
extern char *D_800B48A8[];
extern char *D_800B4870[];
extern char *D_800B487C[];
extern char *D_800B4894[];
extern s32 D_80130994;
extern s32 D_80130984;
extern s32 D_801309B4;
extern s32 D_80130990;
extern char *D_800C140C[];
s32 func_0045AC40(struct Field *field, s16 *arg1) {
    s32 option;
    u32 rel;
    option = *arg1;
    rel = option - 3;
    if (D_80130994 != 0) {
        if (D_801309B4 != 0) {
            field->text = D_800B4870;
        } else {
            field->text = D_800B4888;
        }
        goto done;
    }
    if (D_80130984 != 0) {
        if (rel < 16) {
            field->text = D_800B48A8;
            return 0;
        }
        if (option == 0x15) {
            field->text = D_800C140C;
        } else if (D_801309B4 != 0) {
            field->text = D_800B487C;
        } else {
            field->text = D_800B4894;
        }
        goto done;
    }
    if (D_80130990 != 0) {
        field->text = D_800B48E4;
    }
done:
    return 0;
}
