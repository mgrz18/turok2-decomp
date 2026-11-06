#include "common.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/engine.h"

void Hud_SetupTextureMode(s32);
void Hud_DrawWorldSprite(void *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size);

/* Draws the player's two target markers. */
void CPlayer__DrawTargetMarkers(CPlayer *player, void *view) {
    s32 sprite;
    u8 show;
    f32 scale;

    player->m_SightIconFrame += gFrameIncrement;
    if (player->m_CrossHairMode == 0 || player->m_pOptions->m_TagMode == 1) {
        return;
    }
    Hud_SetupTextureMode(0xFF);

    sprite = player->m_SightIcon;
    if (sprite == 0x2DA && gEngineApp_m_Settings_multiplayer != 0) {
        if (gEngineApp_m_Settings_match_tag == 0) {
            if (gEngineApp_m_Settings_match_teams != 0) {
                show = gEngineApp_m_Settings.teamCrosshairs;
            } else {
                show = gEngineApp_m_Settings.dmCrosshairs;
            }
        } else {
            show = gEngineApp_m_Settings.tagCrosshairs;
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
            Hud_DrawWorldSprite(view, &player->m_vSightPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }

    sprite = player->m_CrossHairIcon;
    if (sprite == 0x2DA && gEngineApp_m_Settings_multiplayer != 0) {
        if (gEngineApp_m_Settings_match_tag == 0) {
            if (gEngineApp_m_Settings_match_teams != 0) {
                show = gEngineApp_m_Settings.teamCrosshairs;
            } else {
                show = gEngineApp_m_Settings.dmCrosshairs;
            }
        } else {
            show = gEngineApp_m_Settings.tagCrosshairs;
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
            Hud_DrawWorldSprite(view, &player->m_vCrossHairPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }
}
