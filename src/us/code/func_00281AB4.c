#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

M2C_UNK CList__AddTail(s32, s32);                    /* extern */
M2C_UNK CList__Remove(void *, s32);                 /* extern */
M2C_UNK func_00281B58(s32);                         /* extern */

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
