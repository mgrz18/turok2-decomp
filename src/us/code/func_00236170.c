#include "common.h"
#include "structs/game_object.h"
#include "structs/particle.h"

s32 CMtxF__FromMtxAffine(s32, s32);
s32 CMtxF__ClampEntries(s32);
s32 Swoosh_NewNode(s32, s32, s32);

s32 CMtxF__ScaleAxes(s32 *, f32, f32, f32);    /* extern */

typedef struct Node00236170 {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Node00236170 *next;
    /* 0x08 */ u8 pad8[0x1C - 0x8];
    /* 0x1C */ CParticle *inst;
    /* 0x20 */ u8 pad20[0x24 - 0x20];
    /* 0x24 */ f32 strength;
} Node00236170;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Node00236170 *head;
} World00236170;

void Swoosh_AddParticleNode(World00236170 *arg0, CParticle *arg1, s32 arg2) {
    u8 sp10[0x40];
    f32 temp_f0;
    f32 var_f0;
    Node00236170 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            if (var_s0->inst == arg1) {
                temp_f0 = var_s0->strength;
                if (temp_f0 > 0.0f) {
                    if (temp_f0 > 0.0f) {
                        CMtxF__FromMtxAffine((s32) sp10, arg2);
                        var_f0 = 0.01f;
                        if (arg1->m_pEffect->m_pModel != 0) {
                            var_f0 = 0.29257143f;
                        }
                        CMtxF__ScaleAxes(sp10, var_f0, var_f0, var_f0);
                        CMtxF__ClampEntries((s32) sp10);
                        Swoosh_NewNode((s32) arg0, (s32) var_s0, (s32) sp10);
                    }
                }
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
