#include "common.h"
#include "structs/camera.h"

void CCache__Flush(s32);
void CCache__Deallocate(sptr, void *);

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
