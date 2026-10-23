/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0023BD24.c, func_0023BD24.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct B09LNode B09LNode;
struct B09LNode {
               B09LNode *next;
               B09LNode *prev;
               u16 slot;
               u8 id;
               u8 tlb;
               s32 unkC;
};
typedef struct B09List {
               B09LNode *head;
               B09LNode *tail;
} B09List;
typedef struct B09SlotA {
            u16 key;
            u8 a;
            u8 b;
} B09SlotA;
extern B09SlotA D_80121318[];
extern B09LNode D_80120FC0[];
extern B09List D_80121290;
extern u8 D_80132000[];
void func_0028EE40(s32 index, s32 pm, void *vaddr, u32 evenpaddr, u32 oddpaddr, s32 asid);
s32 func_0028DAEC(void);
static __inline__ void m03_unlink(B09LNode *n) {
    if (n->prev != ((void *)0)) {
        n->prev->next = n->next;
    } else {
        D_80121290.head = n->next;
        n->next->prev = ((void *)0);
    }
    if (n->next != ((void *)0)) {
        n->next->prev = n->prev;
    } else {
        D_80121290.tail = n->prev;
        n->prev->next = ((void *)0);
    }
}
static __inline__ void m03_push(B09LNode *n) {
    n->next = ((void *)0);
    n->prev = D_80121290.tail;
    if (D_80121290.tail != ((void *)0)) {
        D_80121290.tail->next = n;
    }
    D_80121290.tail = n;
    if (D_80121290.head == ((void *)0)) {
        D_80121290.head = n;
    }
}
void func_0028DBAC(s16 slot, B09LNode *n, s32 unused, u8 *pair) {
    u8 *p = pair;
    s32 idx;
    B09LNode *q;
    if (slot & 1) {
        p = pair - 1;
    }
    if (p[0] != 0xFF && D_80120FC0[p[0]].tlb != 0xFF) {
        idx = D_80120FC0[p[0]].tlb;
        D_80121318[(s16)idx].b = n->id;
    } else if (p[1] != 0xFF && D_80120FC0[p[1]].tlb != 0xFF) {
        idx = D_80120FC0[p[1]].tlb;
        D_80121318[(s16)idx].a = n->id;
    } else {
        idx = func_0028DAEC();
        if (slot & 1) {
            D_80121318[(s16)idx].b = n->id;
            if (p[0] != 0xFF) {
                q = &D_80120FC0[p[0]];
                D_80121318[(s16)idx].a = q->id;
                q->tlb = idx;
                m03_unlink(q);
                m03_push(q);
            }
        } else {
            D_80121318[(s16)idx].a = n->id;
            if (p[1] != 0xFF) {
                q = &D_80120FC0[p[1]];
                D_80121318[(s16)idx].b = q->id;
                q->tlb = idx;
                m03_unlink(q);
                m03_push(q);
            }
        }
        D_80121318[(s16)idx].key = slot & 0xFFFE;
    }
    n->slot = slot;
    n->tlb = idx;
    *pair = n->id;
    func_0028EE40((s16)idx, 0, (void *)(D_80121318[(s16)idx].key << 12),
             D_80121318[(s16)idx].a != 0xFF ? ((u32)D_80132000 & 0x3FFFFFF) + (D_80121318[(s16)idx].a << 12) : -1,
             D_80121318[(s16)idx].b != 0xFF ? ((u32)D_80132000 & 0x3FFFFFF) + (D_80121318[(s16)idx].b << 12) : -1, 7);
}
