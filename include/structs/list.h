#ifndef STRUCTS_LIST_H
#define STRUCTS_LIST_H

/* Acclaim's intrusive list (CList__*): the head and tail element, and where
 * in an element its links are (the IntrusiveList of the Rage Wars decomp).
 * CList__RemoveHead and its neighbours are the list functions.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"

typedef struct CList {
    /* 0x00 */ void *m_pHead;
    /* 0x04 */ void *m_pTail;
    /* 0x08 */ s32 m_LastOffset;    /* where an element keeps its previous link */
    /* 0x0C */ s32 m_NextOffset;    /* where an element keeps its next link */
    /* 0x10 */ s32 m_nCount;
} CList; /* size 0x14 */

#endif
