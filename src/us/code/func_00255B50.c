/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894_2.CPP, func_8022D040_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

extern char gPlayerState12LookTuning;
extern char gPlayerState12MoveTuning;
extern void CPlayer__TurnAndLook(void *, void *, void *);
extern void CPlayer__MoveInWater(void *, void *, void *);
extern s32 CPlayer__SetState(void *, void *, s32);

/* Landing state: pose 0xC or 0xD, movement tables, animation 0x8A2, 0x8A7
 * or 0x14 by the stick (unless animation 0x1144 is still running), then
 * state 3 once the pose timer passes 2 without body bit 0x800. */
void PlayerState12_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 busy;
    f32 x;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 0xC;
    } else {
        player->m_Mode = 0xD;
    }
    CPlayer__TurnAndLook(player, body, &gPlayerState12LookTuning);
    CPlayer__MoveInWater(player, body, &gPlayerState12MoveTuning);
    busy = 0;
    if (player->m_CharAnim == 0x1144) {
        busy = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!busy) {
        x = player->m_Speed;
        if (1.024f <= x) {
            player->m_CharAnim = 0x8A2;
        } else if (x <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    if (!(body->m_AI.m_dwFlags & 0x800) && player->m_ModeTime > 2.0f) {
        CPlayer__SetState(player, body, 3);
    }
}
