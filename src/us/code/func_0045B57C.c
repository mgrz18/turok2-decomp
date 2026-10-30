/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88.CPP, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern int D_80130960;
struct ItemRecAt24;
typedef struct ItemRecAt24 ItemRecAt24;
struct ItemRecAt24;
struct ItemRecAt24 {
    char pad[0x1C];
    int x;
    int y;
    char *label;
};
extern int func_0045B57C(int unused, ItemRecAt24 *item);
extern char D_80119E2C[];
extern char D_0043B5C8[];
extern void func_00416644(char *, char *, int, int, int);
int func_0045B57C(int unused, ItemRecAt24 *item) {
    if (D_80130960 != 0) {
        func_00416644(D_80119E2C, D_0043B5C8, item->x, item->y, 0);
    } else {
        func_00416644(D_80119E2C, item->label, item->x, item->y, 0);
    }
    return 1;
}
