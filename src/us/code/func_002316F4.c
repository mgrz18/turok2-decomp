#include "common.h"

typedef struct Node {
    u8 pad0[0x1C8];
    struct Node *next;    /* 0x1C8 */
} Node;

typedef struct Bucket {
    Node *head;
    u8 pad4[0x10];
} Bucket;

extern void func_0022F1C8(u8 *arg0, Node *node);

void func_002316F4(u8 *arg0) {
    s32 i;
    Node *node;
    Node *cur;

    for (i = 0; i < 3; i++) {
        node = ((Bucket *)(arg0 + 0xEC28))[i].head;
        while (node != NULL) {
            cur = node;
            node = node->next;
            func_0022F1C8(arg0, cur);
        }
    }
}
