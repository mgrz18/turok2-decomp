#include "common.h"
#include "structs/game_object.h"

s32 CAI__SetMode(s32, s32, s32);

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 state;
} ModeData;

void MotherBoss_Mode14_ChargeStep_2(s32 arg0, CAI *ai) {
    ((ModeData *) ai->m_pModeCurrent->m_pData)->state = 7;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, (s32) ai, 0xB);
    }
}
