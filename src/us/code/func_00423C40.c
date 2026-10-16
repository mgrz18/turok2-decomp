/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8040B868_de_4.c, func_8040B868_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B5168[];
extern char D_800B51C4[];
extern char D_800B5200[];
extern char D_800B5250[];
extern char D_800B52B0[];
s32 func_00423C40(struct FieldRecAt14 *field) {
    extern u32 D_801309B0;
    u32 state = D_801309B0;
    if (state >= 5) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto state_1;
        case 2: goto state_2;
        case 3: goto state_3;
        case 4: goto state_4;
        }
state_other:
    field->text = D_800B5168;
    goto done;
state_1:
    field->text = D_800B51C4;
    goto done;
state_2:
    field->text = D_800B5200;
    goto done;
state_3:
    field->text = D_800B5250;
    goto done;
state_4:
    field->text = D_800B52B0;
done:
    return 0;
}
