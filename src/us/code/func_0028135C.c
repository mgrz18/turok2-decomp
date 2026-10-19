#include "common.h"
#include "structs/camera.h"

void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);

/* Starts a camera shake: a free shake source if there is one, else the
 * newest active one is reused. It shakes cameras within `radius` of `pos`,
 * each axis by its amplitude (a tenth of the value given) with the same
 * wave type. */
void func_0028135C(CCameraPool *pool, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos) {
    CShake *shake = pool->m_ShakeFreeList.m_pHead;
    u8 pad[0x18];       /* stack the original keeps here, unused (an inlined helper's, most likely) */
    CVector3 at;

    if (shake != NULL) {
        CList__Remove(&pool->m_ShakeFreeList, shake);
        CList__AddHead(&pool->m_ShakeActiveList, shake);
    } else {
        shake = pool->m_ShakeActiveList.m_pTail;
    }
    at = pos;
    shake->m_vPos = at;
    shake->m_Radius = radius;
    shake->m_XAxis.m_Type = type;
    shake->m_YAxis.m_Type = type;
    shake->m_ZAxis.m_Type = type;
    shake->m_XAxis.m_Shake = ampX * 0.1f;
    shake->m_YAxis.m_Shake = ampY * 0.1f;
    shake->m_ZAxis.m_Shake = ampZ * 0.1f;
}
