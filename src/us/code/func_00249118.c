#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"


typedef struct WeaponDef {
    /* 0x00 */ u8 pad0[0xA];
    /* 0x0A */ u16 pickupId;
} WeaponDef;

extern u8 D_8011AAF1;           /* multiplayer */
extern WeaponDef *D_800AF618[];

void func_00285BD0(s32 arg0, s32 arg1);
void func_002553BC(CPlayer *player, s32 arg1);
void func_0026E110(s32 arg0, s32 arg1, s32 arg2);
void func_0042EB04(CPlayer *player);
void func_00268C48(CPlayer *player, CVector3 pos, s32 id, f32 arg);
void func_00275624(s32 sound);
void func_0024E5F0(CPlayer *player, s32 arg1, s32 anim);

static __inline__ s32 fallingFast(CPlayer *player) {
    return player->m_DuckOffset > 5.12f;
}

static __inline__ s32 inWater(CPlayer *player) {
    return player->m_Mode == 14;
}

/* Kills the player: drops the weapon in multiplayer and plays a death
   animation picked from the cause (`flags`) unless `anim` is given. */
void func_00249118(CPlayer *player, s32 arg1, s32 flags, s32 anim) {
    s16 weapon;
    s16 mode;

    func_00285BD0(player->m_ContState.m_pContState, 0);
    func_002553BC(player, arg1);
    func_0026E110(arg1, 1, arg1);
    func_0042EB04(player);
    if (D_8011AAF1 != 0) {
        weapon = player->m_Data.m_CurrentWeapon;
        if (weapon != 32 && weapon != 24 && weapon != 34 && player->m_pOptions->m_TagMode == 0) {
            func_00268C48(player, player->m_CharacterActor.header.m_vPos, D_800AF618[weapon]->pickupId, 150.0f);
        }
        player->m_ShockedTimer = 0.0f;
        player->m_CharacterActor.m_dwFlags &= ~0x800000;
        if (player->m_pOptions->m_TagMode == 1) {
            func_00275624(0x27D9);
        }
    }
    if (anim != -1) {
        func_0024E5F0(player, arg1, anim);
    } else if (fallingFast(player)) {
        func_0024E5F0(player, arg1, 0x19);
    } else {
        mode = player->m_Mode;
        if (inWater(player)) {
            func_0024E5F0(player, arg1, 0x1E);
        } else if (mode == 8) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 9) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 10) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (mode == 11) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (flags & 0x2000000) {
            func_0024E5F0(player, arg1, 0x1C);
        } else if (flags & 0x1000000) {
            func_0024E5F0(player, arg1, 0x18);
        } else if (flags & 0x400000) {
            func_0024E5F0(player, arg1, 0x16);
        } else {
            func_0024E5F0(player, arg1, 0x13);
        }
    }
}
