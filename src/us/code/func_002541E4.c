#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "globals/player.h"


extern u8 gScene[];
extern void CScene__SetInstanceObjectType(void *, CPlayer *, s32, s32);

void CPlayer__LoadCharacterModel(CPlayer *player) {
    GameSettings *state = &gEngineApp_m_Settings;
    s32 id;

    if (state->multiplayer == 0) {
        id = 0x66;
    } else if (state->match.tag != 0
               && ((CPlayerOptions *)player->m_pOptions)->m_TagMode != 0) {
        id = gTagModeModel;
    } else {
        id = gCharacterModels[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(gScene, player, id, player->m_CharAnim);
}
