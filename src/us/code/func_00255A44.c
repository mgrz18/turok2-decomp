#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern f32 GameObject_GetWaterHeight(CGameObjectInstance *actor);
extern void CCamera__ResetWaterWobble(void *camera);
extern void Sound__Play(s16 sound, CVector3 pos, s32, s32);

/* Tracks the lowest height reached, damps the player's motion and the actor's
 * vertical speed (clamped at -51.2), resets state and plays sound 2000 there. */
void PlayerSwim_Enter(CPlayer *player, CGameObjectInstance *actor) {
    f32 h;
    CBarrel *s;

    h = GameObject_GetWaterHeight(actor) - player->m_vEyeOffset.y;
    if (h < player->m_vDesiredPos.y) {
        player->m_vDesiredPos.y = h;
    }
    player->m_Speed *= 0.5f;
    player->m_SideSpeed *= 0.5f;
    actor->m_vVelocity.y *= 0.75f;
    if (actor->m_vVelocity.y < -51.199997f) {
        actor->m_vVelocity.y = -51.199997f;
    }
    player->m_bOxygenOut = 0;
    CCamera__ResetWaterWobble(player->m_pCamera);
    s = &player->m_Barrel;
    s->m_State = 0;
    s->m_Turn = 0;
    s->m_nCursor = -1;
    s->m_Open = 0;
    Sound__Play(2000, actor->header.m_vPos, 0, -1);
}
