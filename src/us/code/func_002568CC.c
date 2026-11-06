#include "common.h"
#include "structs/player.h"
#include "structs/controller.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + (off)))



extern void CPlayerContState__Init(void *arg0, ControllerProfile *profile);
extern f32 Actor_GetEyeHeight(CPlayer *player);

void CPlayer__ResetControlState(CPlayer *player) {
    CVector3 *v;
    f32 zero;

    CPlayerContState__Init((u8 *)player + 0xA40, &gControllers[player->m_nController]);
    P_S32(player, 0xA78) = 0;
    P_S32(player, 0xA7C) = 0;
    P_S32(player, 0xA80) = 0;
    P_S32(player, 0xA84) = 0;
    P_S32(player, 0xA88) = 1;
    P_S32(player, 0xA8C) = 0;
    P_S32(player, 0xA90) = 0;
    P_S32(player, 0xA94) = 0;
    P_S32(player, 0xA9C) = 0;
    P_S32(player, 0xAFC) = 0;
    P_S32(player, 0xB00) = 0;
    P_S32(player, 0x1284) = 0;
    P_S32(player, 0xAB0) = 0;
    P_S32(player, 0xAB4) = 0;
    P_S32(player, 0xAB8) = 0;
    P_S32(player, 0xABC) = 0;
    P_S32(player, 0xAC8) = 0;
    P_S32(player, 0xACC) = 0;
    P_S32(player, 0xAD0) = 0;
    P_S32(player, 0xAD4) = 0;
    P_S32(player, 0xAD8) = 0;
    P_S32(player, 0xADC) = 0;
    P_S32(player, 0xAE0) = 0;
    P_F32(player, 0xAE4) = Actor_GetEyeHeight(player);
    P_F32(player, 0xAE8) = 0.0f;
    v = &P_VEC(player, 0xAEC);
    zero = P_F32(player, 0xAE8);
    v->x = v->y = v->z = zero;
    P_F32(player, 0xAF8) = 1.0f;
    P_F32(player, 0xB24) = zero;
    P_F32(player, 0xB28) = 1.0f;
    P_VEC(player, 0xAA0) = player->m_CharacterActor.header.m_vPos;
}
