/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_9.CPP, func_8022A170_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

typedef struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
} IntrusiveList;

struct ObjectPool {
    void *allocation;
    char *objects;
    s32 count;
    IntrusiveList active_queue; /* 0x0C */
    IntrusiveList object_queue; /* 0x20 */
};

/* Minimal Turok 2 view of a pooled actor (offsets differ from Rage Wars). */
struct Shared_ActorView {
    u8 pad0[0x288];
    u8 weapon[0x1320 - 0x288]; /* 0x288 */
    struct Shared_ActorView *next; /* 0x1320 */
};

extern u8 D_8011AAD8[]; /* Shared_Game */

extern void func_002542B4(void *arg0, s32 arg1);
extern s32 func_0021EAE8(void *arg0, s32 arg1);
extern void func_0020565C(s32 arg0, void *arg1);
extern void func_0021EBC0(void *arg0);

void func_00253018(struct ObjectPool *arg0) {
    void *node;
    if (*(void **)arg0 != 0) {
        func_0020565C(0, *(void **)arg0);
    }
    node = arg0->object_queue.head;
    if (node != 0) {
        u8 *base = D_8011AAD8;
        do {
            func_0021EBC0(node);
            func_0021EBC0(&((struct Shared_ActorView *)(node))->weapon);
            func_002542B4(node, 0);
            if (base[0x19] != 0) {
                func_0021EAE8(node, 1);
            }
            node = ((struct Shared_ActorView *)(node))->next;
        } while (node != 0);
    }
}
