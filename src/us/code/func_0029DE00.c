/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BCC90_de_20.c, func_802BCE20_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
struct Func802C1F10Node;
struct Func802C1F10Node {
    s32 field0;
    s32 field4;
    void *field8;
    struct Func802C1F10Node *next;
    u16 field10;
};
struct Func802C1F10Node;
typedef struct Func802C1F10Node Func802C1F10Node;
extern void func_0029C8E0();
extern Func802C1F10Node *D_800B891C;
extern void *D_800B8920;
extern u32 func_0029DFF0(void);
extern void func_0029BC30(void *arg0, Func802C1F10Node *arg1);
extern void func_0029E010(u32);
void func_0029DE00(Func802C1F10Node *arg0) {
    Func802C1F10Node *node;
    Func802C1F10Node *cur;
    Func802C1F10Node **head;
    s32 token;
    node = arg0;
    token = func_0029DFF0();
    if (node == 0) {
        node = D_800B8920;
    } else if (node->field10 != 1) {
        func_0029BC30(node->field8, node);
    }
    head = &D_800B891C;
    if (*head == node) {
        *head = node->next;
    } else {
        cur = *head;
        while (cur->field4 != -1) {
            if (cur->next == node) {
                cur->next = node->next;
                break;
            }
            cur = cur->next;
        }
    }
    if (node == D_800B8920) {
        func_0029C8E0();
    }
    func_0029E010(token);
}
