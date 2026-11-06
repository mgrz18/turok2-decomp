/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894.CPP, func_8022CC34_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

extern char gStandLookTuning;
extern char gStandMoveTuning;
extern void Math_EaseToward(f32 *value, f32 target, f32 rate);
extern void CPlayer__TurnAndLook(void *, void *, void *);
extern void CPlayer__MoveOnFoot(void *, void *, void *);
extern s32 CPlayer__SetState(void *, void *, s32);
extern s32 GameObject_IsOnGround(void *body);
extern f32 GameObject_GetGroundHeight(void *body, s32 arg);

/* Eases the lean back to zero, runs the movement tables, and on the ground
 * (falling) picks animation 0x7F8 when the slope speed is under 61.44. */
void PlayerFall_Update(CPlayer *player, CGameObjectInstance *body) {
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    if (!(player->m_ModeBeforeFlags & 0x8000)) {
        CPlayer__MoveOnFoot(player, body, &gStandMoveTuning);
    }
    if (body->m_vVelocity.y <= 0.0f) {
        if (GameObject_IsOnGround(body) != 0) {
            CPlayer__SetState(player, body, 2);
        }
    }
    if (body->m_vVelocity.y <= 0.0f) {
        if (GameObject_GetGroundHeight(body, 0) < 0.0f) {
            if (-GameObject_GetGroundHeight(body, 0) < 61.44f) {
                goto set_value;
            }
        } else if (GameObject_GetGroundHeight(body, 0) < 61.44f) {
set_value:
            player->m_CharAnim = 0x7F8;
        }
    }
}
