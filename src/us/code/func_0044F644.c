/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/8043F69C_6.CPP, func_80442574_de.
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
extern void *func_0044F644(void *owner, struct Dialog *list, s32 b, s32 c, s32 d);
void CList__AddHead(void *list, void *node);
extern char D_800C0C50[];
extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_00414EA8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *);
void *func_0044F644(void *arg0, struct Dialog *list, s32 b, s32 c, s32 d) {
    struct func_8025E5B0_S1 *owner;
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
    func_00414EA8(first, block, list, 0, b, c, d, arg0);
    CList__AddHead(arg0, first);
    owner = arg0;
    if (++owner->unk14 >= 4) {
        owner->unk14 = 0;
    }
    return first;
}
extern char D_800C0C50[];
extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_00414EA8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *);
extern char D_800C0C50[];
extern void **func_00204EDC(s32, s32, s32, char *);
extern void func_00414EA8(void *, void **, struct Dialog *, s32, s32, s32, s32, struct func_8025E5B0_S1 *);
