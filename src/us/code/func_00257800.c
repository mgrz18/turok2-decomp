#include "common.h"
#include "structs/game_object.h"

/* The player: its body (a game object) first; past 0xA00 include/structs/player.h does
 * not line up with the code, so those fields stay unnamed here. */
typedef struct {
    /* 0x0000 */ CGameObjectInstance actor;
    /* 0x0288 */ u8 pad288[0x996 - 0x288];
    /* 0x0996 */ s16 unk996;
    /* 0x0998 */ u8 pad998[0xA64 - 0x998];
    /* 0x0A64 */ u32 unkA64;
    /* 0x0A68 */ u32 unkA68;
    /* 0x0A6C */ u8 padA6C[0xC04 - 0xA6C];
    /* 0x0C04 */ s32 unkC04;
    /* 0x0C08 */ u8 padC08[0x1298 - 0xC08];
    /* 0x1298 */ s32 unk1298;
} Player;

void func_00257800(Player *arg0) {
    arg0->actor.m_vVelocity.x = 0.0f;
    arg0->actor.m_vVelocity.y = 0.0f;
    arg0->actor.m_vVelocity.z = 0.0f;
    arg0->unk1298 = 0;
    arg0->unkC04 = 1;
    arg0->actor.m_dwFlags &= 0xFF7FFFFF;
}
