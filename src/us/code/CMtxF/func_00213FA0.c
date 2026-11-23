#include "common.h"
#include "structs/vector.h"
#include "protos/iset.h"
#include "structs/light.h"




s32 Lighting_CollectVolumes(CBox *q, CLightVolume **out, s32 *cnt);

/* Collects up to 64 records of `model` (or of the world grid when there is
 * none) whose boxes overlap `q`; returns whether any of them has bBA set. */
s32 Lighting_CollectVolumesForModel(CBox *q, s32 model, CLightVolume **out, s32 *cnt) {
    CLightVolumeTable *tab;
    CLightVolume *rec;
    s32 n;
    s32 ret;
    s32 j;

    n = 0;
    ret = 0;
    if (model == 0) {
        return Lighting_CollectVolumes(q, out, cnt);
    }
    tab = CISet__GetBlock(model, 2);
    j = tab->count;
    j--;
    rec = tab->volumes;
    for (; j != -1; j--) {
        if (q->m_vMax.x > rec->m_Bounds.m_vMin.x && q->m_vMin.x < rec->m_Bounds.m_vMax.x && q->m_vMax.z > rec->m_Bounds.m_vMin.z &&
            q->m_vMin.z < rec->m_Bounds.m_vMax.z && q->m_vMax.y > rec->m_Bounds.m_vMin.y && q->m_vMin.y < rec->m_Bounds.m_vMax.y) {
            out[n] = rec;
            n++;
            if (rec->unkBA != 0) {
                ret = 1;
            }
            if (n == 64) {
                break;
            }
        }
        rec++;
    }
    *cnt = n;
    return ret;
}
