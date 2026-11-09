#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/engine.h"

s32 CPlayer__Hurt(s32, s32, s32, s32, s32);

s32 Sound__PlaySpeech(s32);                     /* extern */

void CPlayer__UpdateOxygen(CPlayer *arg0, s32 arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s1;

    temp_f1 = arg0->m_Oxygen;
    temp_f0 = temp_f1 + gFrameIncrement;
    arg0->m_Oxygen = temp_f0;
    if ((temp_f1 < 300.0f) && (temp_f0 > 300.0f) && (gEngineApp_m_Settings.multiplayer == 0)) {
        Sound__PlaySpeech(0x163);
    }
    if (arg0->m_Oxygen > 600.0f) {
        temp_v0 = (s32) arg0->m_Data.m_Health >> 8;
        var_s1 = 1;
        if ((0x96 - temp_v0) > 0) {
            temp_v1 = (0x96 - temp_v0) / 25;
            if (temp_v1 > 0) {
                var_s1 = temp_v1;
            }
        }
        if (arg0->m_bOxygenOut == 0) {
            if (gEngineApp_m_Settings.multiplayer == 0) {
                Sound__PlaySpeech(0x164);
            }
        }
        arg0->m_bOxygenOut = 1;
        CPlayer__Hurt((sptr) arg0, arg1, var_s1 << 8, 0, 0x1C);
        arg0->m_Oxygen = (f32) (arg0->m_Oxygen - 37.5f);
    }
}
