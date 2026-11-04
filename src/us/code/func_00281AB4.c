#include "common.h"
#include "structs/camera.h"

s32 CList__AddTail(s32, s32);                    /* extern */
s32 CList__Remove(void *, s32);                 /* extern */
s32 func_00281B58(s32);                         /* extern */

/* Takes a free text from the pool and puts it on the camera's active texts. */
CCameraText *func_00281AB4(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        func_00281B58(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}
