#include "common.h"
#include "structs/player.h"

extern f32 D_800B6D28;      /* frame step */
extern u8 D_8011AAF1;
extern u8 D_8011AAFC;
extern u8 D_8011AB01;
extern u8 D_8011AB06;
extern s32 D_8011ACD4;
extern s32 D_8011ACE4;

void func_00279028(s32);
void func_0024A678(void *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size);

/* Draws the player's two target markers. */
void func_0024A86C(CPlayer *player, void *view) {
    s32 sprite;
    u8 show;
    f32 scale;

    player->m_SightIconFrame += D_800B6D28;
    if (player->m_CrossHairMode == 0 || player->m_pOptions->m_TagMode == 1) {
        return;
    }
    func_00279028(0xFF);

    sprite = player->m_SightIcon;
    if (sprite == 0x2DA && D_8011AAF1 != 0) {
        if (D_8011ACE4 == 0) {
            if (D_8011ACD4 != 0) {
                show = D_8011AB06;
            } else {
                show = D_8011AAFC;
            }
        } else {
            show = D_8011AB01;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2E4:
        case 0x320:
        case 0x32A:
        case 0x32B:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            func_0024A678(view, &player->m_vSightPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }

    sprite = player->m_CrossHairIcon;
    if (sprite == 0x2DA && D_8011AAF1 != 0) {
        if (D_8011ACE4 == 0) {
            if (D_8011ACD4 != 0) {
                show = D_8011AB06;
            } else {
                show = D_8011AAFC;
            }
        } else {
            show = D_8011AB01;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2C6:
        case 0x2C7:
        case 0x2E4:
        case 0x320:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            func_0024A678(view, &player->m_vCrossHairPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }
}
