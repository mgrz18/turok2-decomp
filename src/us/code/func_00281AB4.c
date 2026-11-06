#include "common.h"
#include "structs/camera.h"

s32 CList__AddTail(s32, s32);                    /* extern */
s32 CList__Remove(void *, s32);                 /* extern */
s32 CCameraText__Reset(s32);                         /* extern */

/* Takes a free text from the pool and puts it on the camera's active texts. */
CCameraText *CCameraPool__TakeText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        CCameraText__Reset(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}
