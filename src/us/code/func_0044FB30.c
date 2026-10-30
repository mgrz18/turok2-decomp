/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80442BC8.CPP, func_80442A60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct StateRecAtBCV2;
struct StateRecAtBCV2 {
    char pad0[0xB0];
    s32 a;
    s32 b;
    s32 padB8;
    s32 c;
};
struct StateRecAtBCV2;
struct Handlers;
struct Handlers {
    char pad[0xC];
    void (*callback)(void *, void *);
};
struct Handlers;
struct ItemRecAt20V2;
struct StateRecAtBCV2;
struct ItemRecAt20V2 {
    char pad0[8];
    s32 resource;
    char padC[0x14 - 0xC];
    struct Handlers *handlers;
    char pad18[0x20 - 0x18];
    struct StateRecAtBCV2 *state;
};
struct ItemRecAt20V2;
struct List_func_80442A60_de;
struct List_func_80442A60_de {
    struct ItemRecAt20V2 *head;
};
extern void func_002052D8(void *arg0, void *arg1);
extern void CList__Remove(void *list, void *node);
static __inline__ void remove_item(struct List_func_80442A60_de *list, struct ItemRecAt20V2 *item) {
    struct StateRecAtBCV2 *state;
    if (item->handlers->callback != 0) {
        item->handlers->callback(item, list);
    }
    state = item->state;
    state->a = 0;
    state->b = 0;
    state->c = 0;
    CList__Remove(list, item);
    func_002052D8(0, item->resource);
}
void func_0044FB30(struct List_func_80442A60_de *list) {
    struct ItemRecAt20V2 *item;
loop:
    item = list->head;
    if (item != 0) {
        remove_item(list, item);
        goto loop;
    }
}
