/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_31.c, func_8040ACA0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
extern char D_800B5010[];
extern char D_800B4FD4[];
extern char D_800B4F68[];
extern char D_800B4BFC[];
extern char D_800B4F98[];
extern char D_800B4D84[];
extern char D_800B4CB4[];
extern char D_800B4E38[];
extern char D_800B4E90[];
extern char D_800B4EF8[];
extern char D_800B4F1C[];
extern char D_800B4D1C[];
extern char D_800B4DEC[];
extern char D_800B4BB8[];
extern char D_800B4E70[];
extern char D_800B4C48[];
extern u32 D_801309C0;
s32 func_004228B0(struct FieldRecAt14 *field) {
    u32 state = D_801309C0;
    if (state >= 16) {
        goto state_other;
    }
    switch (state) {
        case 0: goto state_other;
        case 1: goto case_1;
        case 2: goto case_2;
        case 3: goto case_3;
        case 4: goto case_4;
        case 5: goto case_5;
        case 6: goto case_6;
        case 7: goto case_7;
        case 8: goto case_8;
        case 9: goto case_9;
        case 10: goto case_10;
        case 11: goto case_11;
        case 12: goto case_12;
        case 13: goto case_13;
        case 14: goto case_14;
        case 15: goto case_15;
        }
state_other:
    field->text = D_800B4BB8;
    goto done;
case_1:
    field->text = D_800B4BFC;
    goto done;
case_2:
    field->text = D_800B4C48;
    goto done;
case_3:
    field->text = D_800B4D1C;
    goto done;
case_4:
    field->text = D_800B4CB4;
    goto done;
case_5:
    field->text = D_800B4D84;
    goto done;
case_6:
    field->text = D_800B4DEC;
    goto done;
case_7:
    field->text = D_800B4E38;
    goto done;
case_8:
    field->text = D_800B4E70;
    goto done;
case_9:
    field->text = D_800B4E90;
    goto done;
case_10:
    field->text = D_800B4EF8;
    goto done;
case_11:
    field->text = D_800B4F1C;
    goto done;
case_12:
    field->text = D_800B4F68;
    goto done;
case_13:
    field->text = D_800B4F98;
    goto done;
case_14:
    field->text = D_800B4FD4;
    goto done;
case_15:
    field->text = D_800B5010;
done:
    return 0;
}
