#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"


void SceneActor_Init(CGameObjectInstance *arg0) {
    arg0->header.m_pIntelligence = &gLevelActorIntelligence;
    arg0->header.m_Type = 3;
    arg0->m_AI.m_RotYLastFrame = 0.0f;
}
