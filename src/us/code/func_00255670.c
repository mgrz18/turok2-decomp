#include "common.h"
#include "structs/player.h"
#include "globals/scene.h"

s32 CPlayer__SetState(s32, s32, s32);
s32 PlayerFall_Update(s32, s32);

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


void PlayerJump_Update(CPlayer *arg0, MoveState *arg1) {
    f32 var_f2;

    var_f2 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->m_ContState.m_dwButtons & 0x10) || (arg0->m_ModeTime >= 3.0f)) {
        CPlayer__SetState((s32) arg0, (s32) arg1, 5);
    } else {
        arg1->speed = (f32) (var_f2 * arg1->def->speed);
    }
    PlayerFall_Update((s32) arg0, (s32) arg1);
}
