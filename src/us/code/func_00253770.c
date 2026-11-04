#include "common.h"
#include "structs/player.h"
#include "structs/particle.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_002548FC(s32);

void func_00253770(CPlayer *arg0, s32 arg1, s32 arg2, CParticle *arg3) {
    f32 temp_f0;
    f32 temp_f1;
    f32 var_f0;
    u16 temp_v1;

    if ((arg0->m_Data.m_Health != 0) && (func_002548FC((s32) arg0) == 0)) {
        temp_v1 = arg3->header.m_nObjType;
        switch (temp_v1) {                          /* irregular */
        case 0x128:
            if (arg0->m_Mode != 0x23) {
                temp_f1 = arg0->m_ShockedTimer;
                if (temp_f1 > 0.0f) {
                    temp_f0 = temp_f1 + 30.0f;
                    if (!(temp_f0 <= 150.0f)) {
                        var_f0 = 150.0f;
                        goto block_12;
                    }
                    arg0->m_ShockedTimer = temp_f0;
                } else {
                    var_f0 = 60.0f;
block_12:
                    arg0->m_ShockedTimer = var_f0;
                }
                arg0->m_pHitMeWithChargeDart = (s32) arg3->m_pOwner;
                return;
            }
            break;
        case 0x129:
            if (arg0->m_pHitMeWithCBore == 0) {
                arg0->m_pHitMeWithCBore = (s32) arg3->m_pOwner;
            }
            arg0->m_ShockedTimer = 0.0f;
            func_0024E5F0((s32) arg0, (s32) arg0, 0x23);
            break;
        }
    }
}
