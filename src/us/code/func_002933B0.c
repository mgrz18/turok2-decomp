#include "common.h"

typedef struct Link {
    /* 0x00 */ struct Link *next;
    /* 0x04 */ struct Link **prevNext;
} Link;

/* Unlinks a node from its list. */
void func_002933B0(Link *arg0) {
    Link **temp_v1_2;
    Link *temp_v1;

    temp_v1 = arg0->next;
    if (temp_v1 != NULL) {
        temp_v1->prevNext = arg0->prevNext;
    }
    temp_v1_2 = arg0->prevNext;
    if (temp_v1_2 != NULL) {
        *temp_v1_2 = arg0->next;
    }
}
