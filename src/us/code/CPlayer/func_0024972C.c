#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "gfx.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/player.h"
#include "protos/scene.h"

void CGameObjectInstance__Draw(CPlayer *player, sptr arg1);

/* Draws the player's model: picks it (single player, tag mode or the
   character), then sets up the render state before CGameObjectInstance__Draw. */
void CPlayer__DrawCharacter(CPlayer *player, sptr arg1) {
    s32 model;
    Gfx *g;

    if (gEngineApp_m_Settings.multiplayer == 0) {
        model = 0x66;
    } else if (gEngineApp_m_Settings_match_tag != 0 && player->m_pOptions->m_TagMode != 0) {
        model = gTagModeModel;
    } else {
        model = gCharacterModels[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(&gScene, player, model, player->m_CharAnim);
    g = gDList++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x100000;
    g = gDList++;
    g->w0 = 0xD9FFFFFF;
    g->w1 = 0x10000;
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 2;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 2;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFE;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFE;
    CGameObjectInstance__Draw(player, arg1);
}
