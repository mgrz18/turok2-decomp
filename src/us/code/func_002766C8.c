/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_7.c, func_80259988_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Node_func_80259918_de;
struct Node_func_80259918_de {
    struct Node_func_80259918_de *prev;
    struct Node_func_80259918_de *next;
    char pad08[0xB0 - 0x8];
    s32 key;
};
struct Node_func_80259918_de;
struct OwnerRecAtD8;
struct OwnerRecAtD8 {
    s32 unk00;
    struct Node_func_80259918_de *activePrev;
    struct Node_func_80259918_de *activeNext;
    char pad0C[0xD8 - 0xC];
    struct Node_func_80259918_de *otherPrev;
};
struct Node_func_80259918_de;
typedef struct Node_func_80259918_de Node_func_80259918_de;
struct OwnerRecAtD8;
typedef struct OwnerRecAtD8 OwnerRecAtD8;
void func_002766C8(void *arg0, s32 arg1)
{
    OwnerRecAtD8 *owner = arg0;
    Node_func_80259918_de *node = owner->activeNext;
    Node_func_80259918_de *head = (Node_func_80259918_de *)&owner->activePrev;
    Node_func_80259918_de *other = (Node_func_80259918_de *)&owner->otherPrev;
    if (node != head) {
        Node_func_80259918_de *sentinel = head;
        do {
            Node_func_80259918_de *next = node->next;
            if (node->key == arg1) {
                Node_func_80259918_de *tail;
                node->prev->next = next;
                node->next->prev = node->prev;
                tail = owner->otherPrev;
                node->next = other;
                node->prev = tail;
                owner->otherPrev->next = node;
                owner->otherPrev = node;
            }
            node = next;
        } while (node != sentinel);
    }
}
