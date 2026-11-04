#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"

typedef struct {
    /* 0x00 */ s32 model;
    /* 0x04 */ u8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} WeaponInfo;


typedef struct {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *owner;
} Object;

typedef struct {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ s32 unk2C;
    /* 0x030 */ u8 pad30[0xC7 - 0x30];
    /* 0x0C7 */ s8 visible;
    /* 0x0C8 */ u8 padC8[0x108 - 0xC8];
    /* 0x108 */ s32 unk108;
    /* 0x10C */ s32 unk10C;
    /* 0x110 */ s32 unk110;
    /* 0x114 */ u8 pad114[4];
    /* 0x118 */ s32 unk118;
    /* 0x11C */ u8 pad11C[0x120 - 0x11C];
    /* 0x120 */ s32 unk120;
    /* 0x124 */ u8 pad124[0x128 - 0x124];
    /* 0x128 */ s32 unk128;
} AI;

typedef struct {
    /* 0x00 */ s16 mode;
    /* 0x02 */ u8 pad2[0x16];
} PoseInfo;   /* 0x18 bytes */

extern PoseInfo D_800ADF68[];
extern WeaponInfo *D_800AF618[];   /* weapon table */
extern u32 D_800B6D20;
extern s32 D_800F7078;
extern u8 D_801198B0[];
extern struct {
    s32 enabled;
} D_8011ACE4;
extern s32 func_0024E700(CPlayer *player, s32 weapon);
extern s32 func_00257BA0(CPlayer *player);
extern void func_00243414(Object *obj, AI *ai, s32 mode);
extern void func_00275544(s32 sound, CVector3 pos, CVector3 *source, s32 arg);
extern void func_0027FC2C(void *buf, s32 camera, s32 model);

/* Switches the weapon model to the player's next weapon (see func_002596B4)
 * and puts the weapon AI back in mode 0. */
static __inline__ void switchWeapon(Object *obj, AI *ai) {
    CPlayer *player = obj->owner;

    if (player->m_Data.m_CurrentWeapon != player->m_RequestedWeapon || D_800F7078 != 4) {
        player->m_Data.m_CurrentWeapon = player->m_RequestedWeapon;
        ai->unk2C = D_800AF618[player->m_Data.m_CurrentWeapon]->unk48;
        ai->unk108 = D_800AF618[player->m_Data.m_CurrentWeapon]->unk4C;
        if (D_800B6D20 >= 5) {
            func_0027FC2C(D_801198B0, player->m_pCamera, D_800AF618[player->m_Data.m_CurrentWeapon]->model);
        }
        ai->unk10C = 0;
        ai->unk110 = 0;
        ai->unk128 = 0;
        ai->unk118 = 0;
    }
    func_00243414(obj, ai, 0);
}

/* First-person weapon, idle: picks another weapon when the current one runs
 * out, plays sound 0x27D8 on fire in tag mode, otherwise switches to the
 * queued weapon unless the pose (modes 1 and 7) or visibility forbids it. */
void func_00258000(Object *obj, AI *ai) {
    CPlayer *player;
    s32 mode;

    player = obj->owner;
    mode = D_800ADF68[player->m_Mode].mode;
    if (ai->visible && !func_0024E700(player, player->m_Data.m_CurrentWeapon)) {
        player->m_RequestedWeapon = func_00257BA0(player);
    }
    player->m_ContState.m_pContState->rumbleBase = 0;
    ai->unk120 = 0;
    if (D_8011ACE4.enabled && player->m_pOptions->m_TagMode) {
        if (player->m_ContState.m_dwButtonsSingle & 0x2000) {
            func_00275544(0x27D8, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
        }
        return;
    }
    if (mode == 1 || mode == 7) {
        return;
    }
    if (!ai->visible) {
        return;
    }
    switchWeapon(obj, ai);
}
