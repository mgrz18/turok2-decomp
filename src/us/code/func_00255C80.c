#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

/* Multiplayer/menu state: byte 0x1281 (D_8011AAF1) is the "enabled" flag. */

extern char D_004396DC[];
extern char D_00439748[];
extern char D_0043AB60[];
extern char D_0043AB84[];
extern char D_0043ABA8[];
extern char D_0043ABCC[];

extern void CPlayer__StopLoopingSounds(void);
extern s32 CPlayerPool__GetIndex(void *state, CPlayer *player);
extern void MenuPool_OpenMenu(void *a, char *text, CPlayer *player, s32 d, void *e);

void PlayerState15_Enter(CPlayer *player, CGameObjectInstance *actor) {
    char *text;

    actor->m_vVelocity.x = 0.0f;
    actor->m_vVelocity.y = 0.0f;
    actor->m_vVelocity.z = 0.0f;
    CPlayer__StopLoopingSounds();
    if (D_80119870.m_Settings.multiplayer != 0) {
        switch ((u32) CPlayerPool__GetIndex(&D_80119870.m_PlayerList, player)) {
        default:
        case 0:
            text = D_0043AB60;
            break;
        case 1:
            text = D_0043AB84;
            break;
        case 2:
            text = D_0043ABA8;
            break;
        case 3:
            text = D_0043ABCC;
            break;
        }
        MenuPool_OpenMenu((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    } else {
        text = D_00439748;
        if (P_S16(player, 0x52A) != 0) {
            text = D_004396DC;
        }
        MenuPool_OpenMenu((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    }
}
