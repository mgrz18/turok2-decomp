#include "common.h"
#include "structs/intelligence.h"
#include "structs/level_actor.h"

extern f32 D_800B6D28;

void func_00215868(CLevelActor *arg0) {
    arg0->m_AnimSpeed = (f32) (D_800B6D28 * arg0->header.m_pIntelligence->m_Morpher.m_AnimSpeed);
}
