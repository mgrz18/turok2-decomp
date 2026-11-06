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
extern int CharNoteMenu_SelectNo_2(int unused, ItemRecAt24 *item);
extern char D_80119E2C[];
extern char D_0043B5C8[];
extern void MenuPool_OpenMenu_2(char *, char *, int, int, int);
int CharNoteMenu_SelectNo_2(int unused, ItemRecAt24 *item) {
    if (D_80130960 != 0) {
        MenuPool_OpenMenu_2(D_80119E2C, D_0043B5C8, item->x, item->y, 0);
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, item->label, item->x, item->y, 0);
    }
    return 1;
}
