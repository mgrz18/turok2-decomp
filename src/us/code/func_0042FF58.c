#include "common.h"
#include "structs/list.h"
#include "structs/camera.h"
#include "protos/list.h"

void CCamera__Construct(CCamera *);              /* extern */

/* Takes the first free camera, resets it and moves it to the active list (the pool's lists
 * through CCameraPool: its head pointers make the host offsets larger than the N64's). */
sptr CCameraPool__AllocCamera(CCameraPool *arg0) {
    CCamera *temp_s0;

    temp_s0 = arg0->m_FreeList.m_pHead;
    if (temp_s0 != 0) {
        CCamera__Construct(temp_s0);
        CList__Remove(&arg0->m_FreeList, temp_s0);
        CList__AddTail(&arg0->m_ActiveList, temp_s0);
    }
    return (sptr) temp_s0;
}
