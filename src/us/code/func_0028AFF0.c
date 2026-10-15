#include "common.h"

typedef struct Node {
    /* 0x00 */ struct Node *m_pNext;
    /* 0x04 */ s32 m_Flags;
    /* 0x08 */ s32 m_InitFlags;
    /* 0x0C */ s32 _pad;
    /* 0x10 */ s32 m_Type;
} Node;

typedef struct {
    /* 0x000 */ u8 _pad0[0x2E4];
    /* 0x2E4 */ Node *m_pHeadA;
    /* 0x2E8 */ Node *m_pHeadB;
    /* 0x2EC */ Node *m_pTailA;
    /* 0x2F0 */ Node *m_pTailB;
    /* 0x2F4 */ u8 _pad1[0xC];
    /* 0x300 */ s32 m_bHasA;
} NodeLists;

/* Appends node to the owner list for its type: type 2 to the first list
 * (and flags that it has one), anything else to the second. */
void func_0028AFF0(NodeLists *lists, Node *node) {
    if (node->m_Type == 2) {
        if (lists->m_pTailA != NULL) {
            lists->m_pTailA->m_pNext = node;
        } else {
            lists->m_pHeadA = node;
        }
        lists->m_pTailA = node;
        lists->m_bHasA = 1;
    } else {
        if (lists->m_pTailB != NULL) {
            lists->m_pTailB->m_pNext = node;
        } else {
            lists->m_pHeadB = node;
        }
        lists->m_pTailB = node;
    }
    node->m_pNext = NULL;
    node->m_Flags = node->m_InitFlags & 3;
}
