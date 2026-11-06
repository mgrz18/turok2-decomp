#include "common.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"
#include "globals/engine.h"


void CLevelActor__UpdateAnimSpeed(CLevelActor *arg0) {
    arg0->m_AnimSpeed = (f32) (gFrameIncrement * arg0->header.m_pIntelligence->m_Morpher.m_AnimSpeed);
}
