#include "common.h"
#include "structs/player.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

typedef struct {
#ifdef TARGET_PORT /* the body: its vertical velocity at the host offset (round 11 z5; at the N64 0x1C the
                    * dodge's hop wrote into another member on the host) */
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_vVelocity.y)];
#else
    /* 0x00 */ u8 pad0[0x1C];
#endif
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
