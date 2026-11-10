#include "common.h"
#include "structs/camera.h"

s32 CCache__Deallocate(sptr, sptr);                /* extern */
s32 CCache__Flush(s32);                     /* extern */

/* Frees the pool's on-screen texts. */
void CCameraPool__FreeTexts(CCameraPool *pool) {
    CCache__Flush(0);
    if (pool->m_pmeTexts != NULL) {
        CCache__Deallocate(0, pool->m_pmeTexts);
        pool->m_pmeTexts = NULL;
        pool->m_Texts = NULL;
        pool->m_nTexts = 0;
    }
}
