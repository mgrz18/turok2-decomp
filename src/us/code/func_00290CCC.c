/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_2.c, __CSPRepostEvent.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct ObjRecAt8V3;
typedef struct ObjRecAt8V3 ObjRecAt8V3;
struct Node_func_802AFD6C_de;
struct Node_func_802AFD6C_de {
    struct Node_func_802AFD6C_de *prev;
    struct Node_func_802AFD6C_de *next;
    s32 value;
    short type;
};
struct Node_func_802AFD6C_de;
struct ObjRecAt8V3;
struct ObjRecAt8V3 {
    char pad0[0x8];
    struct Node_func_802AFD6C_de * unk8;
};
struct Node_func_802AFD6C_de;
typedef struct Node_func_802AFD6C_de Node_func_802AFD6C_de;
extern void func_00290CCC(void *arg0, Node_func_802AFD6C_de *arg1);
extern s32 func_0029E230(s32);
void func_00290CCC(void *arg0, Node_func_802AFD6C_de *arg1) {
    extern void func_002933E0(void *, void * *);
    ObjRecAt8V3 *object = arg0;
    s32 saved;
    Node_func_802AFD6C_de **link;
    Node_func_802AFD6C_de *node;
    s32 value;
    s32 node_value;
    saved = func_0029E230(1);
    link = &object->unk8;
    if (link != 0) {
loop:
        node = *link;
        if (node == 0) {
            goto insert;
        }
        value = arg1->value;
        node_value = node->value;
        if (value < node_value) {
            node->value = node_value - value;
insert:
            func_002933E0(arg1, link);
            goto done;
        }
        arg1->value = value - node_value;
        link = (Node_func_802AFD6C_de **)*link;
        if (link != 0) {
            goto loop;
        }
    }
done:
    func_0029E230(saved);
}
