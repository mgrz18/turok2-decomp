#include "common.h"
#include "structs/camera.h"

/* Four 0x228-byte elements and their free/active lists. */
typedef struct {
    /* 0x000 */ u8 data[0x228];
} PoolElement;

typedef struct {
    /* 0x000 */ PoolElement elements[4];
    /* 0x8A0 */ CList m_FreeList;
    /* 0x8B4 */ CList m_ActiveList;
    /* 0x8C8 */ s16 m_nActive;
} ElementPool4;

void CList__Construct(CList *, s32, s32);
s32 CList__AddTail(CList *, PoolElement *);

void func_0042FFF0(ElementPool4 *arg0) {
    PoolElement *var_s0;
    s32 var_s1;

    CList__Construct(&arg0->m_ActiveList, 0, 4);
    CList__Construct(&arg0->m_FreeList, 0, 4);
    var_s1 = 0;
    var_s0 = arg0->elements;
    do {
        CList__AddTail(&arg0->m_FreeList, var_s0);
        var_s1 += 1;
        var_s0++;
    } while (var_s1 < 4);
    arg0->m_nActive = 0;
}
