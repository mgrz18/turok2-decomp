#include "common.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 unk8;
} Triple;

#ifdef TARGET_PORT
#include "structs/swoosh.h"

/* The port: a DList (structs/swoosh.h SwooshLinks) with host pointers. */
void DList_Init(SwooshLinks *arg0) {
    arg0->head = NULL;
    arg0->tail = NULL;
    arg0->count = 0;
}
#else
void DList_Init(Triple *arg0) {
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
}
#endif
