#include "common.h"

typedef struct Node {
    /* 0x000 */ u8 pad0[0x1A0];
    /* 0x1A0 */ s32 flags;
    /* 0x1A4 */ s32 *ownerCount;
    /* 0x1A8 */ struct Node *prev;
    /* 0x1AC */ struct Node *next;
} Node;

extern Node *D_8010AB90; /* free list */
extern Node *D_8010AB94; /* active head */
extern Node *D_8010AB98; /* active tail */

/* Unlinks an active node and pushes it on the free list. */
void func_00227780(Node *arg0) {
    s32 *temp_v1;
    s32 temp_a1;
    Node *temp_v1_2;
    Node *temp_v1_3;
    Node **nextp;

    temp_a1 = arg0->flags;
    if (temp_a1 & 1) {
        temp_v1 = arg0->ownerCount;
        arg0->flags = (s32) (temp_a1 & ~1);
        if (temp_v1 != NULL) {
            *temp_v1 -= 1;
        }
        temp_v1_2 = arg0->prev;
        if (temp_v1_2 != NULL) {
            temp_v1_2->next = arg0->next;
        }
        temp_v1_3 = arg0->next;
        if (temp_v1_3 != NULL) {
            temp_v1_3->prev = arg0->prev;
        }
        if (D_8010AB94 == arg0) {
            D_8010AB94 = arg0->next;
        }
        if (D_8010AB98 == arg0) {
            D_8010AB98 = arg0->prev;
        }
        nextp = &arg0->next;
        *nextp = D_8010AB90;
        arg0->prev = NULL;
        D_8010AB90 = arg0;
    }
}
