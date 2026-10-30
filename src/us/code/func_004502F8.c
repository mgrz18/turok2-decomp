/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8_11.CPP, func_8044322C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
struct func_8025E5B0_S1;
struct func_8025E5B0_S1 {
    char pad0[0x14];
    short unk14;
};
struct Entry_func_80441EB0_de;
struct Entry_func_80441EB0_de {
    s16 type;
    char pad[0x22];
};
struct Entry_func_80441EB0_de;
struct Dialog;
struct Dialog {
    struct Entry_func_80441EB0_de *entries;
    s16 count;
    char pad6[0x1A];
    signed int value;
};
struct func_8025E5B0_S1;
typedef struct func_8025E5B0_S1 func_8025E5B0_S1;
struct Entry_func_80441EB0_de;
extern char D_800C0C50[];
extern void func_00414EA8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *);
void CList__AddHead(void *list, void *node);
extern void **func_00204EDC(s32, s32, s32, char *);
void *func_004502F8(struct func_8025E5B0_S1 *owner, struct Dialog *list, s32 a, s32 b, s32 c, s32 d) {
    s32 size = list->count * 40 + 0x1D8;
    s32 i;
    void **block;
    void *first;
    for (i = 0; i < list->count; i++) {
        s32 extra = 0;
        if (list->entries[i].type == 3) {
            extra = 0x330;
        }
        size += extra;
    }
    block = func_00204EDC(0, size, 0x3B, D_800C0C50 + 4);
    if (block == 0) {
        return 0;
    }
    first = *block;
    if (first == 0) {
        return 0;
    }
    func_00414EA8(first, block, list, a, b, c, d, owner);
    CList__AddHead(owner, first);
    if (++owner->unk14 >= 4) {
        owner->unk14 = 0;
    }
    return first;
}
