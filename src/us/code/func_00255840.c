#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ f32 unk1C;
} Arg1;


void PlayerDodge_Enter(CPlayer *arg0, Arg1 *arg1) {
    if (arg0->m_ContState.m_FinalMoveX < 0.0f) {
        arg0->m_SideSpeed = (f32) -30.72f;
    } else {
        arg0->m_SideSpeed = (f32) 30.72f;
    }
    arg1->unk1C = (f32) 307.19998f;
}
