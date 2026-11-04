#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ f32 unk18;
} WeaponInfo;


typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ f32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ f32 unk118;
} State;

extern WeaponInfo *D_800AF618[];
void func_00253DE0(CPlayer *player, s32 id);
void func_00253E68(CPlayer *player);
void func_00243414(CGameObjectInstance *actor, State *state, s32 mode);

void func_00259878(CGameObjectInstance *actor, State *state) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);

    if (player->m_pLoopingSound_WeaponBackground == 0) {
        func_00253DE0(player, 0x464);
    }
    if (!(player->m_ContState.m_dwButtons & 0x2000)) {
        func_00253E68(player);
        if (state->unk110 < 0.27925270795822144f /* 16 deg */ || state->unk118 > 0.0f) {
            func_00243414(actor, state, 2);
        } else {
            state->unk118 = D_800AF618[22]->unk18 * 15.0f;
            func_00243414(actor, state, 5);
        }
    }
}
