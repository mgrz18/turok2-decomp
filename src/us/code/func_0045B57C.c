/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80409A88.CPP, func_8040AA4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern int sCharNoteLoad;
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
extern char gEngineApp_m_Game_m_MenuPool[];
extern char gPakLoadProfileMenuDesc_2[];
extern void MenuPool_OpenMenu_2(char *, char *, int, int, int);
int CharNoteMenu_SelectNo_2(int unused, ItemRecAt24 *item) {
    if (sCharNoteLoad != 0) {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, gPakLoadProfileMenuDesc_2, item->x, item->y, 0);
    } else {
        MenuPool_OpenMenu_2(gEngineApp_m_Game_m_MenuPool, item->label, item->x, item->y, 0);
    }
    return 1;
}
