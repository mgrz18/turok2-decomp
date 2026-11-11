#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Arg1;

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 9 x1): arg1 is the player's body; the N64 words 0x18/0x1C/0x20 are its
 * m_vVelocity (zeroed as 0 words = 0.0f). Through the N64 view the host cleared the header's
 * m_pIntelligence and m_vVelocity.x instead: PlayerClimb_Update's jump test then read
 * m_pIntelligence->m_dwTypeFlags through NULL (the first ladder/climbable wall crashed). */
void PlayerClimb_Enter(CPlayer *arg0, Arg1 *arg1_) {
    CGameObjectInstance *arg1 = (CGameObjectInstance *) arg1_;

    arg0->m_Speed = 0;
    arg0->m_SideSpeed = 0;
    arg0->m_HandSide = 0.024348905f;
    arg1->m_vVelocity.x = 0.0f;
    arg1->m_vVelocity.y = 0.0f;
    arg1->m_vVelocity.z = 0.0f;
}
#else
void PlayerClimb_Enter(CPlayer *arg0, Arg1 *arg1) {
    arg0->m_Speed = 0;
    arg0->m_SideSpeed = 0;
    arg0->m_HandSide = 0.024348905f;
    arg1->unk18 = 0;
    arg1->unk1C = 0;
    arg1->unk20 = 0;
}
#endif
