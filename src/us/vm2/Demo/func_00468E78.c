#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"

/* Typed on CCameraPool (structs/camera.h): m2c's pool view put the free and active lists at
 * their N64 offsets 0xC/0x20, wrong on the host (the pool starts with two pointers). */

void CCamera__Construct_2(CCamera *);

/* Takes the first free camera, resets it and moves it to the active list. */
CCamera *CCameraPool__AllocCamera_2(CCameraPool *arg0) {
    CCamera *temp_s0;

    temp_s0 = (CCamera *) arg0->m_FreeList.m_pHead;
    if (temp_s0 != NULL) {
        CCamera__Construct_2(temp_s0);
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return temp_s0;
}
