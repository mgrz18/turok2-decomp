#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

extern CListedInstance *D_8010FEB4;

void func_0040BD10(void) {
    CListedInstance *var_s0;

    var_s0 = D_8010FEB4;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_Instance.m_AI.m_Health > 0) {
                var_s0->m_Instance.m_AI.m_Health = 0;
                func_00243414((s32) var_s0, (s32) &var_s0->m_Instance.m_AI, 0x34);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}
