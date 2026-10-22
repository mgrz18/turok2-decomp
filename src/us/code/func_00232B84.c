/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/CODE_8028414C.CPP, Particle_AllocPoolNode.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
void CList__AddHead(void *list, void *node);
extern void CList__Remove(void *list, void *node);
struct PoolNode84478;
typedef struct PoolBucket84478 {
    struct PoolNode84478 *head;
    struct PoolNode84478 *tail;
    s32 data[3];
} PoolBucket84478;
typedef struct PoolOwner84478 {
    char pad0[0xEC00]; /* Turok 2 offsets (Rage Wars: 0xFC00) */
    PoolBucket84478 free;
    char padEC14[0x14];
    PoolBucket84478 slots[256];
} PoolOwner84478;
typedef struct PoolNode84478 {
    char pad0[0x40]; /* Turok 2 layout (Rage Wars: state 0x5C, owner 0x1E4) */
    s32 state;
    char pad44[0x1C0 - 0x44];
    PoolBucket84478 *owner;
    char pad1C4[8];
    s32 counter;
    s32 flags;
} PoolNode84478;
extern void func_0022F1C8(PoolOwner84478 *, PoolNode84478 *);
PoolNode84478 *func_00232B84(PoolOwner84478 *owner, s32 index) {
    PoolNode84478 *node;
    PoolBucket84478 *slot;
    s32 i;
    PoolNode84478 *tail;
    if (owner->free.head == 0) {
        for (i = 0; i <= (index & 0xFF); i++) {
            tail = (&owner->slots[i])->tail;
            if (tail != 0) {
                func_0022F1C8(owner, tail);
                break;
            }
        }
    }
    node = owner->free.head;
    if (node == 0) {
        return 0;
    }
    CList__Remove(&owner->free, node);
    slot = &owner->slots[index & 0xFF];
    CList__AddHead(slot, node);
    node->owner = slot;
    node->flags = 0;
    node->counter = 0;
    node->state = 0x100;
    return node;
}
