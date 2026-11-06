#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Arg1;

void PlayerClimb_Enter(CPlayer *arg0, Arg1 *arg1) {
    arg0->m_Speed = 0;
    arg0->m_SideSpeed = 0;
    arg0->m_HandSide = 0.024348905f;
    arg1->unk18 = 0;
    arg1->unk1C = 0;
    arg1->unk20 = 0;
}
