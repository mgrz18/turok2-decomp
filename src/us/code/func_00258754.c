#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ s32 model;
    /* 0x04 */ u8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 unk_48;
    /* 0x4C */ s32 unk_4C;
} WeaponDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk_2C;
    /* 0x030 */ u8 pad30[0x108 - 0x30];
    /* 0x108 */ s32 unk_108;
    /* 0x10C */ s32 unk_10C;
    /* 0x110 */ s32 unk_110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ s32 unk_118;
    /* 0x11C */ u8 pad11C[0x128 - 0x11C];
    /* 0x128 */ s32 unk_128;
} WeaponState;

extern WeaponDef *D_800AF618[];
extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];

extern void func_0027FC2C(void *arg0, void *camera, s32 model);
extern void func_00243414(CGameObjectInstance *weapon, WeaponState *state, s32 arg2);

/* func_00258668, inlined: switches the weapon actor to its owner's
 * requested weapon. */
static __inline__ void SwitchWeapon(CGameObjectInstance *weapon, WeaponState *state) {
    CPlayer *player = *(CPlayer **)((u8 *)weapon + 0x1A8);
    s16 requested = *(s16 *)((u8 *)player + 0xB14);

    if (player->m_Data.m_CurrentWeapon != requested || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = *(s16 *)((u8 *)player + 0xB14);
        state->unk_2C = D_800AF618[requested]->unk_48;
        state->unk_108 = D_800AF618[player->m_Data.m_CurrentWeapon]->unk_4C;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->model);
        }
        state->unk_10C = 0;
        state->unk_110 = 0;
        state->unk_128 = 0;
        state->unk_118 = 0;
    }
}

void func_00258754(CGameObjectInstance *weapon, WeaponState *state) {
    SwitchWeapon(weapon, state);
    func_00243414(weapon, state, 0);
}
