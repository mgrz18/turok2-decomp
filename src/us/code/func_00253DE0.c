#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/game.h"

extern void LoopSounds_Stop(void *pool, struct CLoopingSoundData *sound);
extern struct CLoopingSoundData *LoopSounds_Start(void *pool, s32 id, CVector3 *pos,
                                               CVector3 *pos2, s32 arg4);

/* Restarts the player's weapon background looping sound with sound id. */
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id) {
    if (gScene.m_WarpState == 0 && gEngineApp_m_Settings.paused == 0) {
        LoopSounds_Stop(LoopSounds_Get(), player->m_pLoopingSound_WeaponBackground);
        player->m_pLoopingSound_WeaponBackground =
            LoopSounds_Start(LoopSounds_Get(), id, &player->m_pCamera->m_vPos,
                          &player->m_pCamera->m_vPos, -1);
    }
}
