/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_4.c, func_804410AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
struct MenuRules;
typedef struct MenuRules MenuRules;
struct MenuRules;
struct MenuRules {
    char pad0[0x1C];
    s32 locked;
};
struct MenuRules;
struct EntryRecAt18V2;
struct MenuRules;
struct EntryRecAt18V2 {
    char pad0[8];
    s32 flags;
    char pad0C[4];
    s8 back[4];
    char pad14[4];
    struct MenuRules *target;
    char pad1C[0xC];
};
struct EntryRecAt18V2;
struct MenuRecAt10;
struct MenuRecAt10 {
    s16 cursor;
    char pad2[0xA];
    struct EntryRecAt18V2 *entries;
    s32 count;
};
void func_004151B4(struct MenuRecAt10 *menu, u8 dir) {
    struct EntryRecAt18V2 *entry;
    s32 old;
    entry = &menu->entries[menu->cursor];
    do {
        old = menu->cursor;
        menu->cursor += ((s8 *)entry)[dir];
        if (menu->cursor < 0) {
            menu->cursor = menu->count - 1;
        } else if (menu->cursor >= menu->count) {
            menu->cursor = 0;
        }
        entry = &menu->entries[menu->cursor];
        old -= menu->cursor;
        switch (dir) {
        case 16:
            if (entry->flags & 0x80000) {
                entry->back[1] = old;
            }
            break;
        case 17:
            if (entry->flags & 0x40000) {
                entry->back[0] = old;
            }
            break;
        case 18:
            if (entry->flags & 0x200000) {
                entry->back[3] = old;
            }
            break;
        case 19:
            if (entry->flags & 0x100000) {
                entry->back[2] = old;
            }
            break;
        }
    } while ((entry->flags & 0x1800000) != 0x1800000 || entry->target->locked == 0);
}
