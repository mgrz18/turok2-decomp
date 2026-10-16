/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CC08_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct Node_func_8023CC08_de;
typedef struct Node_func_8023CC08_de Node_func_8023CC08_de;
struct Link_func_8023CC08_de;
struct Link_func_8023CC08_de {
    struct Link_func_8023CC08_de *next;
    struct Link_func_8023CC08_de *prev;
    u16 id;
    union { u16 padA; struct { u8 index; u8 slot; }; };
    s32 fieldC;
};
struct Link_func_8023CC08_de;
typedef struct Link_func_8023CC08_de Link_func_8023CC08_de;
struct Node_func_8023CC08_de;
struct Node_func_8023CC08_de {
    struct Node_func_8023CC08_de *next;
    u16 start;
    u16 size;
    char pad8[8];
    u8 *data;
};
extern Link_func_8023CC08_de *D_80121294;
extern Link_func_8023CC08_de *D_80121290;
extern Node_func_8023CC08_de *func_0028EA40(u32 arg0);
Link_func_8023CC08_de *func_0028EA88(void) {
    extern void func_0028DEFC(void *arg0);
    Link_func_8023CC08_de *entry;
    Node_func_8023CC08_de *node;
    entry = D_80121290;
    if (entry->prev != 0) {
        entry->prev->next = *(Link_func_8023CC08_de * volatile *)&entry->next;
    } else {
        D_80121290 = entry->next;
        (*(Link_func_8023CC08_de * volatile *)&entry->next)->prev = 0;
    }
    if (entry->next != 0) {
        entry->next->prev = *(Link_func_8023CC08_de * volatile *)&entry->prev;
    } else {
        D_80121294 = entry->prev;
        (*(Link_func_8023CC08_de * volatile *)&entry->prev)->next = 0;
    }
    func_0028DEFC(entry);
    if (entry->id != 0xFFFF) {
        node = func_0028EA40(entry->id);
        node->data[entry->id - node->start] = 0xFF;
        entry->id = 0xFFFF;
    }
    entry->fieldC = 0;
    return entry;
}
