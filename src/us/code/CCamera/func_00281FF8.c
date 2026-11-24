#include "common.h"
#include "globals/strings.h"
#include "structs/camera.h"

struct CMemEntry *CCache__Allocate(s32, s32, u32, char *);
void *memset(void *, s32, uptr);

/* Allocates the pool's on-screen texts (0x40 bytes each) and clears them. */
void CCameraPool__AllocTexts(CCameraPool *pool, s32 count) {
    s32 size = count * sizeof(CCameraText);

    pool->m_nTexts = count;
    pool->m_pmeTexts = CCache__Allocate(0, size, 0x23, sCCameraPoolCacheDesc_CameraTexts);
    pool->m_Texts = *(CCameraText **) pool->m_pmeTexts;
    memset(pool->m_Texts, 0, size);
}
