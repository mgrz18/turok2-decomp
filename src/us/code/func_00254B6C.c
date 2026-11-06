#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

void CPlayerPool__ClearLastTagMode(CPlayerPool *arg0) {
    struct CPlayerOptions *temp_a0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->m_pOptions;
            if (temp_a0->m_LastTagMode == 1) {
                temp_a0->m_LastTagMode = 0U;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
}
