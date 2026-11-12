#include "common.h"
#include "structs/player.h"
#include "globals/scene.h"
#include "protos/player.h"

s32 PlayerFall_Update(sptr, sptr);

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 speed;
} MoveDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MoveDef *def;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 speed;
} MoveState;


#ifdef TARGET_PORT
/* The port (round 11 z5): MoveState is the N64 layout of the body (the definition at 0x14, the
 * vertical velocity at 0x1C), which missed both host members, and the jump scale (0x20 of the
 * player's intelligence) is CPlayerIntelligence.m_JumpScale (round 12 a5: typed, so the archive
 * swaps it; round 11 read its big-endian bytes). Without it a held jump fell at once (demo 3, gf 72). */
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include <string.h>
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.m_JumpScale) == 0x20, "CIntelligence 0x20");

void PlayerJump_Update(CPlayer *arg0, MoveState *arg1) {
    CGameObjectInstance *body = (CGameObjectInstance *) arg1;
    f32 var_f2;

    var_f2 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->m_ContState.m_dwButtons & 0x10) || (arg0->m_ModeTime >= 3.0f)) {
        CPlayer__SetState(arg0, arg1, 5);
    } else {
        body->m_vVelocity.y = var_f2 * body->header.m_pIntelligence->m_Player.m_JumpScale;
    }
    PlayerFall_Update((sptr) arg0, (sptr) arg1);
}
#else
void PlayerJump_Update(CPlayer *arg0, MoveState *arg1) {
    f32 var_f2;

    var_f2 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->m_ContState.m_dwButtons & 0x10) || (arg0->m_ModeTime >= 3.0f)) {
        CPlayer__SetState(arg0, arg1, 5);
    } else {
        arg1->speed = (f32) (var_f2 * arg1->def->speed);
    }
    PlayerFall_Update((sptr) arg0, (sptr) arg1);
}
#endif
