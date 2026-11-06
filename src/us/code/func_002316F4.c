#include "common.h"

typedef struct Node {
    u8 pad0[0x1C8];
    struct Node *next;    /* 0x1C8 */
} Node;

typedef struct Bucket {
    Node *head;
    u8 pad4[0x10];
} Bucket;

extern void CParticleSystem__Free(u8 *arg0, Node *node);

void CParticleSystem__FreeAll(u8 *arg0) {
    s32 i;
    Node *node;
    Node *cur;

    for (i = 0; i < 3; i++) {
        node = ((Bucket *)(arg0 + 0xEC28))[i].head;
        while (node != NULL) {
            cur = node;
            node = node->next;
            CParticleSystem__Free(arg0, cur);
        }
    }
}
