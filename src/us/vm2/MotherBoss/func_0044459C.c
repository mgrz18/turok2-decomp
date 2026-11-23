#include "common.h"
#include "structs/game_object.h"
#include "protos/ai.h"


typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s8 state;
} ModeData;

void MotherBoss_Mode14_ChargeStep_2(s32 arg0, CAI *ai) {
    ((ModeData *) ai->m_pModeCurrent->m_pData)->state = 7;
    if (ai->m_AnimRepeatedCount != 0) {
        CAI__SetMode(arg0, ai, 0xB);
    }
}
