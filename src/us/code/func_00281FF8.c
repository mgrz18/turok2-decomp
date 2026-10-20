#include "common.h"
#include "m2c_macros.h"
#include "structs/camera.h"

s32 *func_00204EDC(M2C_UNK, s32, M2C_UNK, M2C_UNK *); /* extern */
M2C_UNK func_00266C5C(s32, M2C_UNK, s32);           /* extern */
extern M2C_UNK D_800A9A08;

/* Allocates the pool's on-screen texts (0x40 bytes each) and clears them. */
void func_00281FF8(CCameraPool *pool, s32 count) {
    s32 size = count * sizeof(CCameraText);

    pool->m_nTexts = count;
    pool->m_pmeTexts = func_00204EDC(0, size, 0x23, &D_800A9A08);
    pool->m_Texts = *(CCameraText **) pool->m_pmeTexts;
    func_00266C5C(pool->m_Texts, 0, size);
}
