#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} WeaponDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 unk_3E;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0xC7];
    /* 0x0C7 */ s8 unk_C7;
    /* 0x0C8 */ u8 padC8[0x10C - 0xC8];
    /* 0x10C */ s32 unk_10C;
    /* 0x110 */ s32 unk_110;
} WeaponState;

extern WeaponDef *D_800AF618[];

extern s32 func_00257EA0(CGameObjectInstance *weapon, WeaponState *state);
extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern void func_00243414(CGameObjectInstance *weapon, WeaponState *state, s32 arg2);

void func_00258A98(CGameObjectInstance *weapon, WeaponState *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);

    P_S32(player, 0xB2C) = 1;
    P_S32(player, 0xB38) = 0;
    P_S32(player, 0xB30) = 0;
    state->unk_10C = 0;
    state->unk_110 = 0;
    if (state->unk_C7 != 0) {
        if ((D_800AF618[player->m_Data.m_CurrentWeapon]->flags & 4) && func_00257EA0(weapon, state) &&
            func_0024E700(player, player->m_Data.m_CurrentWeapon) && (P_S32(player, 0xA64) & 0x2000) &&
            ((PlayerOptions *)player->m_pOptions)->unk_3E == 0) {
            func_00243414(weapon, state, 4);
        } else {
            func_00243414(weapon, state, 2);
        }
    }
}
