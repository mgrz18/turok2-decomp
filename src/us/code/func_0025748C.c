#include "common.h"
#include "structs/player.h"

#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 unk_3E;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk_18;
} Intelligence;

extern u8 D_8011AAF1;
extern f32 D_800ADD88[];
extern f32 D_800B6D28; /* the frame's time step */

void func_0025748C(CPlayer *player) {
    f32 rate;
    f32 cur;
    f32 next = 0.0f; /* doubles as the zero for the tests below */
    f32 max;

    if (D_8011AAF1 != 0) {
        rate = D_800ADD88[P_S32(player, 0x520)];
        if (rate != next) {
            cur = P_S32(player, 0x524);
            if (cur != next && ((PlayerOptions *)player->m_pOptions)->unk_3E == 0) {
                next = cur + (s32)(rate * 256.0f) * (D_800B6D28 * (1.0f / 15));
                max = (u32)(((Intelligence *)player->m_CharacterActor.header.m_pIntelligence)->unk_18 << 8);
                if (!(max <= next)) {
                    max = next;
                }
                P_S32(player, 0x524) = max;
            }
        }
    }
}
