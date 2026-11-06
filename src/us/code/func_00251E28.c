#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/collision.h"
#include "structs/camera.h"
#include "globals/engine.h"

extern char gEngineApp_m_Game_m_CameraPool[];

f32 Random_Float(f32 scale);
void CCamera__StartFlash(CCamera *cam, s32 maxOpacity, s32 red, s32 green, s32 blue, s32 attack, s32 sustain, s32 decay);
void CCameraPool__StartShake(void *pool, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos);
s32 CPlayer__SetState(CPlayer *player, CPlayer *body, s32 mode);
void DamageEvent_Set(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
void Player_OnMultiplayerKill(CPlayer *actor, void *ai, DamageEvent *hit);

/* Being shocked: flash and shake the view at random; when the time runs out
 * (or the player dies) drop back to standing and take the shock's damage. */
void PlayerShock_Update(CPlayer *player) {
    DamageEvent event;
    CCamera *cam;

    if (Random_Float(100.0f) > 80.0f) {
        cam = player->m_pCamera;
        CCamera__StartFlash(cam, 0xFF, 0xFF, 0, 0, (u8) (u32) Random_Float(7.5f), 3, 7);
        CCameraPool__StartShake(gEngineApp_m_Game_m_CameraPool, 0.0f, 204.79999f, 0.0f, 512.0f, 0, cam->m_vPos);
    }
    player->m_CharacterActor.m_AI.m_ConditionTimer -= gFrameIncrement;
    if (player->m_CharacterActor.m_AI.m_ConditionTimer <= 0.0f || player->m_Data.m_Health == 0) {
        player->m_CharacterActor.m_dwFlags &= ~0x800000;
        CPlayer__SetState(player, player, 2);
        DamageEvent_Set(&event, player->m_pHitMeWithCBore, 0x3200, 25.599998f, 0x100000, 0);
        player->m_pHitMeWithCBore = NULL;
        Player_OnMultiplayerKill(player, (u8 *) player + 0x140, &event);
    }
}
