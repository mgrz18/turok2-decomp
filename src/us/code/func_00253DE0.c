#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern s32 D_80110028;
extern s32 D_8011ACA4;
extern void *func_00275F7C(void);
extern void func_00275D34(void *pool, struct CLoopingSoundData *sound);
extern struct CLoopingSoundData *func_00275C6C(void *pool, s32 id, CVector3 *pos,
                                               CVector3 *pos2, s32 arg4);

/* Restarts the player's weapon background looping sound with sound id. */
void func_00253DE0(CPlayer *player, s32 id) {
    if (D_80110028 == 0 && D_8011ACA4 == 0) {
        func_00275D34(func_00275F7C(), player->m_pLoopingSound_WeaponBackground);
        player->m_pLoopingSound_WeaponBackground =
            func_00275C6C(func_00275F7C(), id, &player->m_pCamera->m_vPos,
                          &player->m_pCamera->m_vPos, -1);
    }
}
