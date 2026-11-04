#include "common.h"
#include "structs/camera.h"

s32 func_002052D8(s32, s32);                /* extern */
s32 func_002053A8(s32);                     /* extern */

/* Frees the pool's on-screen texts. */
void func_002820B4(CCameraPool *pool) {
    func_002053A8(0);
    if (pool->m_pmeTexts != NULL) {
        func_002052D8(0, pool->m_pmeTexts);
        pool->m_pmeTexts = NULL;
        pool->m_Texts = NULL;
        pool->m_nTexts = 0;
    }
}
