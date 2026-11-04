#include "common.h"
#include "structs/game_object.h"

/* An object on the list at D_8010FEB4, linked through 0x28C. */
typedef struct ListedObject {
    /* 0x000 */ CGameObjectInstance obj;
    /* 0x288 */ s32 unk288;
    /* 0x28C */ struct ListedObject *next;
} ListedObject;

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern ListedObject *D_8010FEB4;

void func_00444D10(void) {
    ListedObject *var_s0;

    var_s0 = D_8010FEB4;
    if (var_s0 != NULL) {
        do {
            if (var_s0->obj.m_AI.m_Health > 0) {
                var_s0->obj.m_AI.m_Health = 0;
                func_00243414(&var_s0->obj, &var_s0->obj.m_AI, 0x34);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
