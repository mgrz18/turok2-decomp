#ifndef T2_PLAYER_OFFSETS_H
#define T2_PLAYER_OFFSETS_H

/* Port only (round 8, w1): the player-state files that address CPlayer by raw N64 byte offsets
 * (`P_S32(player, 0xAB8)`, func_002559E0.c ... func_002568CC.c) keep their text; under TARGET_PORT
 * their P_* macros pass the offset through T2PlayerOff, which returns the host offset of the
 * member the N64 offset names (the host CPlayer is larger: its pointers are 8 bytes, so every
 * offset past m_CharacterActor.header moves). An offset not in the table traps. */

#include <stddef.h>
#include "structs/player.h"

static __inline__ size_t T2PlayerOff(unsigned int n64) {
    switch (n64) {
    case 0x018: return offsetof(CPlayer, m_CharacterActor.m_vVelocity.x);
    case 0x01C: return offsetof(CPlayer, m_CharacterActor.m_vVelocity.y);
    case 0x020: return offsetof(CPlayer, m_CharacterActor.m_vVelocity.z);
    case 0x0D4: return offsetof(CPlayer, m_CharacterActor.m_dwFlags);
    case 0x520: return offsetof(CPlayer, m_Data.m_nDeathmatchCharacter);
    case 0x524: return offsetof(CPlayer, m_Data.m_Health);
    case 0x52A: return offsetof(CPlayer, m_Data.m_Lives);
    case 0xA50: return offsetof(CPlayer, m_ContState.m_pContState);
    case 0xA78: return offsetof(CPlayer, m_Speed);
    case 0xA7C: return offsetof(CPlayer, m_SideSpeed);
    case 0xA80: return offsetof(CPlayer, m_TotalSpeed);
    case 0xA84: return offsetof(CPlayer, m_YVelLastFrame);
    case 0xA88: return offsetof(CPlayer, m_bOnGround);
    case 0xA8C: return offsetof(CPlayer, m_BurstSpeed);
    case 0xA90: return offsetof(CPlayer, m_BurstSideSpeed);
    case 0xA94: return offsetof(CPlayer, m_BurstTimer);
    case 0xA9C: return offsetof(CPlayer, m_TimeOffGround);
    case 0xAA0: return offsetof(CPlayer, m_vDesiredPos.x);
    case 0xAA4: return offsetof(CPlayer, m_vDesiredPos.y);
    case 0xAB0: return offsetof(CPlayer, m_JumpAngle);
    case 0xAB4: return offsetof(CPlayer, m_StartJumpAngle);
    case 0xAB8: return offsetof(CPlayer, m_DesiredJumpAngle);
    case 0xABC: return offsetof(CPlayer, m_JumpU);
    case 0xAC8: return offsetof(CPlayer, m_vHeadRotOffset.x);
    case 0xACC: return offsetof(CPlayer, m_vHeadRotOffset.y);
    case 0xAD0: return offsetof(CPlayer, m_vHeadRotOffset.z);
    case 0xAD4: return offsetof(CPlayer, m_vHeadWobbleRotOffset.x);
    case 0xAD8: return offsetof(CPlayer, m_vHeadWobbleRotOffset.y);
    case 0xADC: return offsetof(CPlayer, m_vHeadWobbleRotOffset.z);
    case 0xAE0: return offsetof(CPlayer, m_vEyeOffset.x);
    case 0xAE4: return offsetof(CPlayer, m_vEyeOffset.y);
    case 0xAE8: return offsetof(CPlayer, m_vEyeOffset.z);
    case 0xAEC: return offsetof(CPlayer, m_qGround);
    case 0xAF8: return offsetof(CPlayer, m_qGround.w);
    case 0xAFC: return offsetof(CPlayer, m_BreathAmp);
    case 0xB00: return offsetof(CPlayer, m_BreathSpeed);
    case 0xB24: return offsetof(CPlayer, m_ActualCameraVertEyeOffset);
    case 0xB28: return offsetof(CPlayer, m_CameraMovementScaler);
    case 0xB98: return offsetof(CPlayer, m_JumpPadCurrentTime);
    case 0xB9C: return offsetof(CPlayer, m_JumpPadAngleDirection);
    case 0xBA0: return offsetof(CPlayer, m_pJumpPadRegionSet);
    case 0xBA4: return offsetof(CPlayer, m_vJumpPadStart.x);
    case 0xBA8: return offsetof(CPlayer, m_vJumpPadStart.y);
    case 0xBAC: return offsetof(CPlayer, m_vJumpPadStart.z);
    case 0xBFC: return offsetof(CPlayer, m_WaitDeathmatchTimer);
    case 0xC04: return offsetof(CPlayer, m_CharAnim);
    case 0x1284: return offsetof(CPlayer, m_bHeadMovementDisabled);
    case 0x1298: return offsetof(CPlayer, m_ShockedTimer);
    }
    __builtin_trap();
}

#endif
