/* CPlayer code: CPlayer__SetCharacter, 0x0042E430-0x0042E5AC (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/player.h"

extern CIntelligence *CScene__FindIntelligence(void *world, s32 kind, s32 character);
extern s32 Random_Next(void);
extern void CPlayer__GiveStartingWeapons(CPlayer *p);
extern void CScene__SetInstanceObjectType(void *world, CPlayer *p, s32, s32);

static __inline__ CIntelligence *findDef(s32 kind, s32 character) {
    return CScene__FindIntelligence(&gScene, kind, character);
}

void CPlayer__SetCharacter(CPlayer *p, s32 character) {
    CIntelligence *def;
    s32 modelId;

    p->m_Data.m_nDeathmatchCharacter = character;
    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    def = findDef(11, character);
    if (def == NULL) {
        def = findDef(11, -1);
        if (def == NULL) {
            def = findDef(-1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = def;
    p->m_CharacterActor.m_vScale.x = def->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = def->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = def->m_Player.m_ScaleZ;
    p->m_Data.m_Health = def->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    if (p->m_Data.m_nDeathmatchCharacter == 7) {
        p->m_CharacterActor.m_AI.m_nTexture = Random_Next() % 18;
    } else {
        p->m_CharacterActor.m_AI.m_nTexture = -1;
    }
    CPlayer__GiveStartingWeapons(p);
    modelId = 0x66;
    if (gEngineApp_m_Settings.multiplayer) {
        if (gEngineApp_m_Settings_match_tag != 0 && p->m_pOptions->m_TagMode != 0) {
            modelId = gTagModeModel;
        } else {
            modelId = gCharacterModels[p->m_CharacterActor.header.m_pIntelligence->m_Id];
        }
    }
    CScene__SetInstanceObjectType(&gScene, p, modelId, p->m_CharAnim);
}
