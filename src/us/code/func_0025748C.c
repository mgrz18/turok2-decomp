#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/engine.h"
#include "globals/player.h"

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#endif

void CPlayer__RegenerateHealth(CPlayer *player) {
    f32 rate;
    f32 cur;
    f32 next = 0.0f; /* doubles as the zero for the tests below */
    f32 max;

    if (gEngineApp_m_Settings.multiplayer != 0) {
        rate = gCharacterHealthRegen[P_S32(player, 0x520)];
        if (rate != next) {
            cur = P_S32(player, 0x524);
            if (cur != next && ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
                next = cur + (s32)(rate * 256.0f) * (gFrameIncrement * (1.0f / 15));
                max = (u32)(player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8);
                if (!(max <= next)) {
                    max = next;
                }
                P_S32(player, 0x524) = max;
            }
        }
    }
}
