#include "common.h"
#include "structs/camera.h"

/* An element of the pool below: a game object and a pointer, at 0x290, to
 * a counter of its users. */
typedef struct {
    /* 0x000 */ u8 pad0[0x290];
    /* 0x290 */ s32 *m_pUseCount;
} PoolObject;

/* The pool: a free and an active list of PoolObjects. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x5300];
    /* 0x5300 */ CList m_FreeList;
    /* 0x5314 */ CList m_ActiveList;
} ObjectPool;

extern s32 CList__AddHead(CList *, PoolObject *);
extern s32 CList__Remove(CList *, PoolObject *);
extern s32 D_8011001C;

/* Takes a free object (when fewer than three are active, or always while
 * D_8011001C is set), makes it active and counts it in *arg1. */
PoolObject *func_002283FC(ObjectPool *arg0, s32 *arg1) {
    PoolObject *node;

    if (D_8011001C != 0 || (u32) arg0->m_ActiveList.m_nCount < 3) {
        node = arg0->m_FreeList.m_pHead;
        if (node != 0) {
            CList__Remove(&arg0->m_FreeList, node);
            CList__AddHead(&arg0->m_ActiveList, node);
            node->m_pUseCount = arg1;
            if (arg1 != NULL) {
                *arg1 += 1;
            }
        }
        return node;
    }
    return 0;
}
