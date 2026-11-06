/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225D10.c, func_00225D10, with Turok 2's layouts (names from AbuN64/ragewars-decomp, WEAPONS/80225D10.CPP). */
/* A player travelling to a target: turn toward it and follow a parabola. */

#include "common.h"
#include "structs/player.h"

typedef CVector3 Position;

typedef struct TravelTarget {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ Position pos;
    /* 0x40 */ f32 height;
    /* 0x44 */ u8 pad44[0x1A];
    /* 0x5E */ u8 duration;
} TravelTarget;

extern f32 gFrameIncrement;      /* frame time */
extern char gStandLookTuning;
extern void CPlayer__TurnAndLook(void *, void *, void *);
extern s32 CPlayer__SetState(void *, void *, s32);
extern void Angle_Wrap(f32 *value);

void PlayerJumpPad_Update(CPlayer *player, void *body) {
    f32 yaw, targetYaw, alternate, duration, t;
    Position pos;

    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    player->m_JumpPadCurrentTime += gFrameIncrement;
    yaw = player->m_CharacterActor.m_RotY;
    targetYaw = player->m_JumpPadAngleDirection;
    if (targetYaw != yaw) {
        Angle_Wrap(&yaw);
        Angle_Wrap(&targetYaw);
        if (yaw > targetYaw) {
            alternate = targetYaw + 6.2831855f;
            if (alternate - yaw < yaw - targetYaw) {
                targetYaw = alternate;
            }
        } else {
            alternate = targetYaw - 6.2831855f;
            if (yaw - alternate < targetYaw - yaw) {
                targetYaw = alternate;
            }
        }
        if (targetYaw > yaw) {
            yaw += (targetYaw - yaw) * 0.2f;
            if (targetYaw < yaw) {
                yaw = targetYaw;
            }
        } else {
            yaw -= (yaw - targetYaw) * 0.2f;
            if (yaw < targetYaw) {
                yaw = targetYaw;
            }
        }
        Angle_Wrap(&yaw);
    }
    player->m_WeaponActor.m_RotY = yaw;
    player->m_CharacterActor.m_RotY = yaw;
    duration = (f32)(((TravelTarget *)player->m_pJumpPadRegionSet)->duration * 15);
    if (player->m_JumpPadCurrentTime < duration) {
        t = player->m_JumpPadCurrentTime / duration;
        pos.x = player->m_vJumpPadStart.x + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.x - player->m_vJumpPadStart.x) * t;
        pos.z = player->m_vJumpPadStart.z + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.z - player->m_vJumpPadStart.z) * t;
        pos.y = player->m_vJumpPadStart.y + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.y - player->m_vJumpPadStart.y) * t
              + ((TravelTarget *)player->m_pJumpPadRegionSet)->height * ((t - t * t) * 4.0f);
        player->m_vDesiredPos = pos;
        return;
    }
    CPlayer__SetState(player, body, 2);
}
