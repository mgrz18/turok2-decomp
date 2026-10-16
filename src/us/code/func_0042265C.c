/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80409A5C_de_12.c, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ItemRecAt24;
typedef struct ItemRecAt24 ItemRecAt24;
struct ItemRecAt24;
struct ItemRecAt24 {
    char pad[0x1C];
    int x;
    int y;
    char *label;
};
extern int func_0042265C(int unused, ItemRecAt24 *item);
extern int D_80130960;
extern char D_80119E2C[];
extern char D_0043B6A8[];
extern void func_00416644(char *, char *, int, int, int);
int func_0042265C(int unused, ItemRecAt24 *item) {
    if (D_80130960 != 0) {
        func_00416644(D_80119E2C, D_0043B6A8, item->x, item->y, 0);
    } else {
        func_00416644(D_80119E2C, item->label, item->x, item->y, 0);
    }
    return 1;
}
