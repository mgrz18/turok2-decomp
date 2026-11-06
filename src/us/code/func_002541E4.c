#include "common.h"
#include "structs/player.h"
#include "structs/game.h"


extern s32 D_800ADB5C;
extern s32 D_800ADB30[];
extern u8 D_800F7078[];
extern void CScene__SetInstanceObjectType(void *, CPlayer *, s32, s32);

void CPlayer__LoadCharacterModel(CPlayer *player) {
    GameSettings *state = &D_8011AAD8;
    s32 id;

    if (state->multiplayer == 0) {
        id = 0x66;
    } else if (state->match.tag != 0
               && ((CPlayerOptions *)player->m_pOptions)->m_TagMode != 0) {
        id = D_800ADB5C;
    } else {
        id = D_800ADB30[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(D_800F7078, player, id, player->m_CharAnim);
}
