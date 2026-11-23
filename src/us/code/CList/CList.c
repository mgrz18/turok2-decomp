#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"

void CList__Construct(CList *arg0, s32 arg1, s32 arg2) {
    arg0->m_LastOffset = arg1;
    arg0->m_NextOffset = arg2;
    arg0->m_pTail = 0;
    arg0->m_pHead = 0;
    arg0->m_nCount = 0;
}

/* Links element arg1 in front of the list head; the links live at the list offsets inside it. */
void CList__AddHead(CList *arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = arg0->m_pHead;
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pHead + arg0->m_LastOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
    arg0->m_pHead = arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}

/* Appends arg1 to the list; m_LastOffset and m_NextOffset say where in an
 * element its links are. */
void CList__AddTail(CList *arg0, void *arg1) {
    void *temp_v1;

    temp_v1 = arg0->m_pTail;
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pTail + arg0->m_NextOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
    arg0->m_pTail = arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}

/* CList: inserts element arg2 before element arg1 (at the head when arg1 has no previous). */
void CList__InsertBefore(CList *arg0, void *arg1, void *arg2) {
    s32 temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    s32 var_v0;

    temp_v1 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_v1 != 0) {
        *(void **)((u8 *)temp_v1 + arg0->m_NextOffset) = arg2;
        temp_v0 = arg0->m_LastOffset;
        *(void **)((u8 *)arg2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = arg2;
        *(void **)((u8 *)arg2 + arg0->m_NextOffset) = arg1;
        var_v0 = arg0->m_nCount + 1;
    } else {
        temp_v1_2 = arg0->m_pHead;
        if (temp_v1_2 != 0) {
            *(void **)((u8 *)arg2 + arg0->m_NextOffset) = temp_v1_2;
            *(void **)((u8 *)arg0->m_pHead + arg0->m_LastOffset) = arg2;
        } else {
            *(void **)((u8 *)arg2 + arg0->m_NextOffset) = 0;
            arg0->m_pTail = arg2;
        }
        *(void **)((u8 *)arg2 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = arg2;
        var_v0 = arg0->m_nCount + 1;
    }
    arg0->m_nCount = var_v0;
}

/* CList: insert arg2 after arg1, or at the tail when arg1 has no next. */
void CList__InsertAfter(CList *arg0, void *arg1, void *arg2) {
    s32 temp_v0;
    void *temp_v1;
    void *temp_v1_2;
    s32 var_v0;

    temp_v1 = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    if (temp_v1 != 0) {
        *(void **)((u8 *)temp_v1 + arg0->m_LastOffset) = arg2;
        temp_v0 = arg0->m_NextOffset;
        *(void **)((u8 *)arg2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = arg2;
        *(void **)((u8 *)arg2 + arg0->m_LastOffset) = arg1;
        var_v0 = arg0->m_nCount + 1;
    } else {
        temp_v1_2 = arg0->m_pTail;
        if (temp_v1_2 != 0) {
            *(void **)((u8 *)arg2 + arg0->m_LastOffset) = temp_v1_2;
            *(void **)((u8 *)arg0->m_pTail + arg0->m_NextOffset) = arg2;
        } else {
            *(void **)((u8 *)arg2 + arg0->m_LastOffset) = 0;
            arg0->m_pHead = arg2;
        }
        *(void **)((u8 *)arg2 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = arg2;
        var_v0 = arg0->m_nCount + 1;
    }
    arg0->m_nCount = var_v0;
}

void CList__Remove(CList *arg0, void *arg1) {
    void *temp_a2;
    void *temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;

    temp_a2 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_a2 != 0) {
        temp_v0 = arg0->m_NextOffset;
        *(void **)((u8 *)temp_a2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
    }
    temp_a2_2 = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    if (temp_a2_2 != 0) {
        temp_v0_2 = arg0->m_LastOffset;
        *(void **)((u8 *)temp_a2_2 + temp_v0_2) = *(void **)((u8 *)arg1 + temp_v0_2);
    }
    if (arg0->m_pHead == arg1) {
        arg0->m_pHead = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    }
    if (arg0->m_pTail == arg1) {
        arg0->m_pTail = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    }
    arg0->m_nCount = arg0->m_nCount - 1;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80255F70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CList__RemoveHead(CList *arg0) {
    char *cursor = arg0->m_pHead;
    int stride = arg0->m_NextOffset;
    void *value = *(void **)(cursor + stride);
    int count = arg0->m_nCount - 1;
    arg0->m_nCount = count;
    arg0->m_pHead = value;
}

void CList__RemoveTail(CList *arg0) {
    char *cursor = arg0->m_pTail;
    int stride = arg0->m_LastOffset;
    void *value = *(void **)(cursor + stride);
    int count = arg0->m_nCount - 1;
    arg0->m_nCount = count;
    arg0->m_pTail = value;
}

/* Moves element arg1 to the head of the list; m_LastOffset and
 * m_NextOffset say where in an element its links are. */
void CList__MoveToHead(CList *arg0, void *arg1) {
    void *temp_a2;
    void *temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;
    void *temp_v1;

    temp_a2 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_a2 != 0) {
        temp_v0 = arg0->m_NextOffset;
        *(void **)((u8 *)temp_a2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
    }
    temp_a2_2 = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    if (temp_a2_2 != 0) {
        temp_v0_2 = arg0->m_LastOffset;
        *(void **)((u8 *)temp_a2_2 + temp_v0_2) = *(void **)((u8 *)arg1 + temp_v0_2);
    }
    if (arg0->m_pHead == arg1) {
        arg0->m_pHead = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    }
    if (arg0->m_pTail == arg1) {
        arg0->m_pTail = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    }
    temp_v1 = arg0->m_pHead;
    arg0->m_nCount = (s32) (arg0->m_nCount - 1);
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pHead + arg0->m_LastOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
        arg0->m_pTail = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
    arg0->m_pHead = arg1;
    arg0->m_nCount = (s32) (arg0->m_nCount + 1);
}

/* CList: unlinks element arg1 and appends it at the tail. */
void CList__MoveToTail(CList *arg0, void *arg1) {
    void *temp_a2;
    void *temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;
    void *temp_v1;

    temp_a2 = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    if (temp_a2 != 0) {
        temp_v0 = arg0->m_NextOffset;
        *(void **)((u8 *)temp_a2 + temp_v0) = *(void **)((u8 *)arg1 + temp_v0);
    }
    temp_a2_2 = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    if (temp_a2_2 != 0) {
        temp_v0_2 = arg0->m_LastOffset;
        *(void **)((u8 *)temp_a2_2 + temp_v0_2) = *(void **)((u8 *)arg1 + temp_v0_2);
    }
    if (arg0->m_pHead == arg1) {
        arg0->m_pHead = *(void **)((u8 *)arg1 + arg0->m_NextOffset);
    }
    if (arg0->m_pTail == arg1) {
        arg0->m_pTail = *(void **)((u8 *)arg1 + arg0->m_LastOffset);
    }
    temp_v1 = arg0->m_pTail;
    arg0->m_nCount = arg0->m_nCount - 1;
    if (temp_v1 != 0) {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = temp_v1;
        *(void **)((u8 *)arg0->m_pTail + arg0->m_NextOffset) = arg1;
    } else {
        *(void **)((u8 *)arg1 + arg0->m_LastOffset) = 0;
        arg0->m_pHead = arg1;
    }
    *(void **)((u8 *)arg1 + arg0->m_NextOffset) = 0;
    arg0->m_pTail = arg1;
    arg0->m_nCount = arg0->m_nCount + 1;
}

extern s32 Random_Next(void);

void *CList__GetRandom(void *arg0) {
    s32 index;
    s32 offset;
    void *node;
    node = ((CList *)arg0)->m_pHead;
    if (node == 0) {
        return 0;
    }
    index = Random_Next() % (u32)((CList *)arg0)->m_nCount;
    index--;
    if (index != -1) {
        offset = ((CList *)arg0)->m_NextOffset;
        do {
            { u8 *cursor = (u8 *)node; cursor += offset; node = *(void **)cursor; }
            index--;
        } while (index != -1);
    }
    return node;
}

/* Walks a chain of offsets relative to the base at 0xC until a zero link. */
void func_00200A74(s32 *arg0) {
    sptr next;
    s32 base;

    next = arg0[0];
    if (next != 0) {
        base = arg0[3];
        do {
            next = *(s32 *)(next + base);
        } while (next != 0);
    }
}

int CList__Contains(CList *arg0, void *arg1) {
    void *node = arg0->m_pHead;
    if (node != 0) {
        do {
            if (node == arg1) {
                return 1;
            }
            node = *(void **)((u8 *)node + arg0->m_NextOffset);
        } while (node != 0);
    }
    return 0;
}
