#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))


extern u8 D_8011AAF1;
extern f32 D_800ADD88[];
extern f32 D_800B6D28; /* the frame's time step */

void CPlayer__RegenerateHealth(CPlayer *player) {
    f32 rate;
    f32 cur;
    f32 next = 0.0f; /* doubles as the zero for the tests below */
    f32 max;

    if (D_8011AAF1 != 0) {
        rate = D_800ADD88[P_S32(player, 0x520)];
        if (rate != next) {
            cur = P_S32(player, 0x524);
            if (cur != next && ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
                next = cur + (s32)(rate * 256.0f) * (D_800B6D28 * (1.0f / 15));
                max = (u32)(player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8);
                if (!(max <= next)) {
                    max = next;
                }
                P_S32(player, 0x524) = max;
            }
        }
    }
}
