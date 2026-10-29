/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802B3B80_5.CPP, func_802AFB6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
extern void func_00290D5C(void *arg0, f32 arg1);
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
struct ListLink;
typedef struct ListLink ListLink;
struct ObjectState10_2;
typedef struct ObjectState10_2 ObjectState10_2;
struct ObjectState4C;
typedef struct ObjectState4C ObjectState4C;
struct ObjectLinks4_5;
typedef struct ObjectLinks4_5 ObjectLinks4_5;
struct Node_func_802AFD6C_de;
struct Node_func_802AFD6C_de {
    struct Node_func_802AFD6C_de *prev;
    struct Node_func_802AFD6C_de *next;
    s32 value;
    short type;
};
struct Node_func_802AFD6C_de;
struct Node_func_802AFD6C_de;
typedef struct Node_func_802AFD6C_de Node_func_802AFD6C_de;
struct ObjectLinks54_2;
struct ObjectLinks54_2 {
    unsigned char padding[80];
    Node_func_802AFD6C_de * link;
};
struct ObjectLinks54_2;
typedef struct ObjectLinks54_2 ObjectLinks54_2;
struct ObjectLinks4_5;
struct ObjectLinks4_5 {
    Node_func_802AFD6C_de * link;
};
struct ObjectState10_2;
struct ObjectState10_2 {
    s16 type;
    u8 pad2[6];
    u8 unk_8;
    u8 unk_9;
    u8 padA;
    u8 unk_B;
    u8 unk_C;
    u8 unk_D;
    u8 padE[2];
};
struct ObjectState4C;
struct ObjectState4C {
    char pad0[0x24];
    s32 unk_24;
    char pad24[0x48 - 0x24 - sizeof(s32)];
    char unk_48;
};
struct ListLink;
extern void func_002933B0(struct ListLink *arg0);
extern void func_002933E0(void *arg0, void **arg1);
extern void func_00290CCC(void *arg0, Node_func_802AFD6C_de *arg1);
void __CSPHandleMetaMsg(void *arg0, ObjectState10_2 *arg1) {
    s32 padding[4];
    s32 total = 0;
    Node_func_802AFD6C_de *head = 0;
    s32 scale;
    Node_func_802AFD6C_de *node;
    Node_func_802AFD6C_de *next;
    if (arg1->unk_8 == 0xFF) {
        if (arg1->unk_9 != 0x51) {
            return;
        }
        scale = ((ObjectState4C *)(arg0))->unk_24;
        func_00290D5C(arg0,
                      (f32)((arg1->unk_B << 16) | (arg1->unk_C << 8) | arg1->unk_D));
        node = ((struct ObjectLinks54_2 *) ((char *) arg0))->link;
        while (node != 0) {
            next = ((struct ObjectLinks4_5 *) ((char *) node))->link;
            total += node->value;
            if (node->type == 0x15) {
                s32 node_total;
                func_002933B0((ListLink *)node);
                if (head != 0) {
                    func_002933E0(node, head);
                } else {
                    head = node;
                    node->prev = 0;
                    node->next = 0;
                }
                node_total = total;
                if (next != 0) {
                    total -= node->value;
                    next->value += node->value;
                }
                node->value = node_total;
            }
            node = next;
        }
        node = head;
        while (node != 0) {
            next = node->prev;
            node->value = (node->value / scale) * ((ObjectState4C *)(arg0))->unk_24;
            func_00290CCC(&((ObjectState4C *)(arg0))->unk_48, node);
            node = next;
        }
    }
}
void func_00290CCC(void *arg0, Node_func_802AFD6C_de *arg1);
void func_00290D5C(void *arg0, f32 arg1);
