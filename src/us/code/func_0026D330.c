/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00279620.c, func_00279620.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct DLNode {
                struct DLNode *prev;
                struct DLNode *next;
} DLNode;
typedef struct DLList {
                DLNode *head;
                DLNode *tail;
                s32 count;
} DLList;
typedef struct NodePool {
                DLList freel;
                DLList usedl;
} NodePool;
void func_0026D330(NodePool *p, DLNode *mem, s32 stride, u32 n) {
    DLList *u;
    p->freel.head = 0;
    p->freel.tail = 0;
    p->freel.count = 0;
    u = &p->usedl;
    u->head = 0;
    u->tail = 0;
    u->count = 0;
    while (n--) {
        if ((&p->freel)->count == 0) { (&p->freel)->head = (mem); (&p->freel)->tail = (mem); (mem)->prev = 0; (mem)->next = 0; } else { (mem)->prev = (&p->freel)->tail; (mem)->next = 0; (&p->freel)->tail->next = (mem); (&p->freel)->tail = (mem); } (&p->freel)->count++;
        mem = (DLNode *)((u8 *)mem + stride);
    }
}
