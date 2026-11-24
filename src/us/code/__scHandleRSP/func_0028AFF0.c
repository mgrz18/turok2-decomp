#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

void __scAppendList(OSSched *lists, OSScTask *node) {
    if (node->list.t.type == 2) {
        if (lists->audioListTail != NULL) {
            lists->audioListTail->next = node;
        } else {
            lists->audioListHead = node;
        }
        lists->audioListTail = node;
        lists->doAudio = 1;
    } else {
        if (lists->gfxListTail != NULL) {
            lists->gfxListTail->next = node;
        } else {
            lists->gfxListHead = node;
        }
        lists->gfxListTail = node;
    }
    node->next = NULL;
    node->state = node->flags & 3;
}
