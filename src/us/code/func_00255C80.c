#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + (off)))

/* Multiplayer/menu state: byte 0x1281 (D_8011AAF1) is the "enabled" flag. */
extern u8 D_80119870[];

extern char D_004396DC[];
extern char D_00439748[];
extern char D_0043AB60[];
extern char D_0043AB84[];
extern char D_0043ABA8[];
extern char D_0043ABCC[];

extern void func_0042EB04(void);
extern s32 func_0025340C(void *state, CPlayer *player);
extern void func_00416644(void *a, char *text, CPlayer *player, s32 d, void *e);

void func_00255C80(CPlayer *player, CGameObjectInstance *actor) {
    char *text;

    actor->m_vVelocity.x = 0.0f;
    actor->m_vVelocity.y = 0.0f;
    actor->m_vVelocity.z = 0.0f;
    func_0042EB04();
    if (D_80119870[0x1281] != 0) {
        switch ((u32) func_0025340C(D_80119870, player)) {
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
        func_00416644((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    } else {
        text = D_00439748;
        if (P_S16(player, 0x52A) != 0) {
            text = D_004396DC;
        }
        func_00416644((u8 *)P_S32(player, 0x51C) + 0x53C, text, player, P_S32(player, 0xA50), NULL);
    }
}
