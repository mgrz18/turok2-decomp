#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern f32 func_00268A5C(CGameObjectInstance *actor);
extern void func_00280EAC(void *camera);
extern void func_00275544(s16 sound, CVector3 pos, s32, s32);

/* Tracks the lowest height reached, damps the player's motion and the actor's
 * vertical speed (clamped at -51.2), resets state and plays sound 2000 there. */
void func_00255A44(CPlayer *player, CGameObjectInstance *actor) {
    f32 h;
    CBarrel *s;

    h = func_00268A5C(actor) - player->m_vEyeOffset.y;
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
    func_00280EAC(player->m_pCamera);
    s = &player->m_Barrel;
    s->m_nItems = 0;
    s->unk_0x4 = 0;
    s->m_nLastItem = -1;
    s->unk_0x8 = 0;
    func_00275544(2000, actor->header.m_vPos, 0, -1);
}
