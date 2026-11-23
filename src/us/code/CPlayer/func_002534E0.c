#include "common.h"
#include "structs/player_list.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

s32 Controller_SetRumbleOptionAll(s32);

typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0;

/* 0x1328-byte list entries (see CPlayerPool__GetIndex) linked through 0x1320. */


void CPlayerPool__EndMatch(CPlayerPool *arg0) {
    CPlayer *var_a0;
    CPlayer *var_a0_2;
    CPlayer *var_s0;
    CPlayerPool *var_s1;

    var_s0 = (CPlayer *) &gEngineApp_m_Settings.match;
    var_s0 = (CPlayer *) &gEngineApp_m_Settings.match;
    var_s1 = arg0;
    if ((gEngineApp_m_Settings.match.over == 0) && (gEngineApp_m_Settings.match.done == 0)) {
        Controller_SetRumbleOptionAll(0);
        if (gEngineApp_m_Settings.match.teams != 0) {
            if (gEngineApp_m_Settings.match.teamOver == 0) {
                gEngineApp_m_Settings.match.teamOver = 1;
                var_s0 = var_s1->m_ActiveList.m_pHead;
                var_a0 = var_s0;
                if (var_s0 != NULL) {
                    do {
                        CPlayer__SetState(var_a0, var_s0, 0x12);
                        var_s0 = var_s0->m_pNext;
                        var_a0 = var_s0;
                    } while (var_s0 != NULL);
                }
            }
        } else {
            gEngineApp_m_Settings.match.over = 1;
            var_s0 = var_s1->m_ActiveList.m_pHead;
            var_a0_2 = var_s0;
            if (var_s0 != NULL) {
                do {
                    CPlayer__SetState(var_a0_2, var_s0, 0x12);
                    var_s0 = var_s0->m_pNext;
                    var_a0_2 = var_s0;
                } while (var_s0 != NULL);
            }
        }
    }
}
