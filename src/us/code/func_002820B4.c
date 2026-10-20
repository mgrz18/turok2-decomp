#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

M2C_UNK func_002052D8(M2C_UNK, s32);                /* extern */
M2C_UNK func_002053A8(M2C_UNK);                     /* extern */

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
