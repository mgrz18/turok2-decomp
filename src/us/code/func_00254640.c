#include "common.h"
#include "globals/game.h"

s32 GameObject_IsOnGround(sptr);


#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/intelligence.h"
#include "structs/region.h"
/* The host body on the members (STABILITY.md section 7): the N64 offsets (CPlayer 0xA08 m_Mode,
 * 0xB28 m_CameraMovementScaler; the actor's CAI flags at 0x140) wrote a float into the host
 * CPlayer every frame. The intelligence (0x1C: the player body's speed) and the region info
 * (0x30, 0x52) are ROM records at the same offsets on both sides (offsetof checks them). */
void CPlayer__UpdateSpeedScale(void *arg0, void *arg1, void *arg2) {
    CPlayer *player = arg0;
    CGameObjectInstance *actor = arg1;
    CRegionInfo *info = arg2;
    f32 s;

    if ((u16) player->m_Mode < 2U) {
        player->m_CameraMovementScaler = 1.0f;
        return;
    }
    {
        /* CPlayerIntelligence types 0x1C as pad08 (u8[]): the archive's swap plan leaves those
         * bytes big-endian (1.0f reads as 4.6e-41 little-endian). Read them as the N64 does.
         * The real fix: name the field in structs/intelligence.h + swap_spec (archive rebuild). */
        const u8 *b = (const u8 *) actor->header.m_pIntelligence + __builtin_offsetof(CIntelligence, m_Player.pad08);
        u32 w = ((u32) b[0] << 24) | ((u32) b[1] << 16) | ((u32) b[2] << 8) | b[3];
        s = *(f32 *) &w;
    }
    player->m_CameraMovementScaler = s;
    if (gEngineApp_m_Settings.multiplayer != 0) {
        player->m_CameraMovementScaler = s * gEngineApp_m_Settings.speed;
    }
    if (!(actor->m_AI.m_dwFlags & 0xC0000) && info != NULL
        && (GameObject_IsOnGround((sptr) actor) != 0 || (info->m_wFlags52 & 0x800))) {
        player->m_CameraMovementScaler *= info->unk_30;
    }
}
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.pad08) == 0x1C, "CIntelligence 0x1C");
_Static_assert(__builtin_offsetof(CRegionInfo, unk_30) == 0x30, "CRegionInfo 0x30");
_Static_assert(__builtin_offsetof(CRegionInfo, m_wFlags52) == 0x52, "CRegionInfo 0x52");
#else
void CPlayer__UpdateSpeedScale(void *arg0, void *arg1, void *arg2)
{
  s32 *new_var;
  f32 temp_f1;
  if (((u16) (*((u16 *) (((s8 *) arg0) + 0xA08)))) < 2U)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = 1.0f;
    return;
  }
  temp_f1 = *((f32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x1C));
  *((f32 *) (((s8 *) arg0) + 0xB28)) = temp_f1;
  new_var = &gEngineApp_m_Settings;
  if ((*((u8 *) (((s8 *) new_var) + 0x19))) != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) (temp_f1 * (*((f32 *) (((s8 *) new_var) + 0x1C))));
  }
  if (((!((*((s32 *) (((s8 *) arg1) + 0x140))) & 0xC0000)) && (arg2 != 0)) && ((GameObject_IsOnGround((sptr) arg1) != 0) || ((*((u16 *) (((s8 *) arg2) + 0x52))) & 0x800)))
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) ((*((f32 *) (((s8 *) arg0) + 0xB28))) * (*((f32 *) (((s8 *) arg2) + 0x30))));
  }
}
#endif
