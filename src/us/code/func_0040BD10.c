#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);

typedef struct ActorNode {
    /* 0x000 */ CGameObjectInstance obj;
    /* 0x288 */ u8 pad288[4];
    /* 0x28C */ struct ActorNode *next;
} ActorNode;

extern ActorNode *D_8010FEB4;

void func_0040BD10(void) {
    ActorNode *var_s0;

    var_s0 = D_8010FEB4;
    if (var_s0 != NULL) {
        do {
            if (var_s0->obj.m_AI.m_Health > 0) {
                var_s0->obj.m_AI.m_Health = 0;
                func_00243414((s32) var_s0, (s32) &var_s0->obj.m_AI, 0x34);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
