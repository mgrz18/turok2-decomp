#include "common.h"
#include "structs/camera.h"
#include "protos/cache.h"

void memset(s32, s32, sptr);           /* extern */
extern u8 D_800A9A08[];

/* Allocates the pool's on-screen texts (0x40 bytes each) and clears them. */
void CCameraPool__AllocTexts(CCameraPool *pool, s32 count) {
    s32 size = count * sizeof(CCameraText);

    pool->m_nTexts = count;
    pool->m_pmeTexts = CCache__Allocate(0, size, 0x23, D_800A9A08);
    pool->m_Texts = *(CCameraText **) pool->m_pmeTexts;
    memset(pool->m_Texts, 0, size);
}
