/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80254D44_de.c, func_80255048_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
struct EntryRecAt18V3;
struct EntryRecAt18V3 {
    char pad0[0xC];
    s32 flags;
    s32 stamp;
    s32 pad14;
    struct EntryRecAt18V3 *next;
};
struct EntryList;
struct EntryRecAt18V3;
struct EntryList {
    s32 pad0;
    struct EntryRecAt18V3 *first;
};
struct EntryList;
typedef struct EntryList EntryList;
struct EntryRecAt18V3;
typedef struct EntryRecAt18V3 EntryRecAt18V3;
EntryRecAt18V3 *func_00206870(s32 unused, EntryList *list, EntryRecAt18V3 **recent)
{
    extern s32 D_800D8DF0[];
    EntryRecAt18V3 *e = list->first;
    if (*recent == 0) {
        for (; e != 0; e = e->next) {
            if (!(e->flags & 0x702)) {
                if ((u32)(D_800D8DF0[0] - e->stamp) >= 5) {
                    return e;
                }
                *recent = e;
                break;
            }
        }
    }
    for (; e != 0; e = e->next) {
        if (!(e->flags & 0x702) && (u32)(D_800D8DF0[0] - e->stamp) >= 5) {
            return e;
        }
    }
    return 0;
}
