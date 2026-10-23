#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + (off)))

typedef struct {
    u8 data[0x224];
} ControllerProfile;

extern ControllerProfile D_800F5EC0[4];

extern void func_002470E8(void *arg0, ControllerProfile *profile);
extern f32 func_00253CFC(CPlayer *player);

void func_002568CC(CPlayer *player) {
    CVector3 *v;
    f32 zero;

    func_002470E8((u8 *)player + 0xA40, &D_800F5EC0[player->m_nController]);
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
    P_F32(player, 0xAE4) = func_00253CFC(player);
    P_F32(player, 0xAE8) = 0.0f;
    v = &P_VEC(player, 0xAEC);
    zero = P_F32(player, 0xAE8);
    v->x = v->y = v->z = zero;
    P_F32(player, 0xAF8) = 1.0f;
    P_F32(player, 0xB24) = zero;
    P_F32(player, 0xB28) = 1.0f;
    P_VEC(player, 0xAA0) = player->m_CharacterActor.header.m_vPos;
}
