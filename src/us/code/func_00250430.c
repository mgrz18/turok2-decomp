/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_15.CPP, func_80224408_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

extern char D_800ADE70;     /* standing look tuning */
extern char D_800ADE88;     /* crouching look tuning */

void func_0024FC08(CPlayer *player, CGameObjectInstance *body);
void func_0024EF70(CPlayer *player, CGameObjectInstance *body, void *tuning);
s32 func_0024E5F0(CPlayer *player, CGameObjectInstance *body, s32 mode);

static __inline__ s32 try_jump(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Standing and walking: bob the view, turn it with the crouching or standing
 * tuning, hand off to a jump or to moving, then pick the turning animation. */
void func_00250430(CPlayer *player, CGameObjectInstance *body) {
    f32 heading;
    s32 crouching;
    s32 animate;

    func_0024FC08(player, body);
    heading = body->m_RotY;
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        func_0024EF70(player, body, &D_800ADE88);
    } else {
        func_0024EF70(player, body, &D_800ADE70);
    }
    heading = body->m_RotY - heading;
    if (try_jump(player, body)) {
        return;
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f || (player->m_SniperMode != 2 && player->m_ContState.m_FinalMoveY != 0.0f)) {
        func_0024E5F0(player, body, 3);
    }
    animate = 1;
    if ((player->m_CharAnim == 0x1130 || player->m_CharAnim == 0x1135) && player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0) {
        animate = 0;
    }
    if (animate) {
        if (body->m_AI.m_dwFlags & 0x6000) {
            player->m_CharAnim = 0xA28;
        } else if (0.017453294f < heading) {
            player->m_CharAnim = 0xB5E;
        } else if (heading < -0.017453294f) {
            player->m_CharAnim = 0xB54;
        } else {
            player->m_CharAnim = 1;
        }
    }
}
