#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the same on CGameObjectInstance/CAI (the N64 code reads both at raw offsets:
 * def 0x14, flags 0xD4; the AI's m_VarS32[0..1] 0x10C/0x110, m_ConditionTimer 0x64,
 * m_Action 0x37). The intelligence is asset data (the same bytes on the host): its words at
 * 0x14 (the type flags) and 0x7C stay byte offsets. */
void Turret_Advance(u8 *actor_, u8 *anim_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CAI *anim = (CAI *) anim_;
    u8 *def;

    def = (u8 *) actor->header.m_pIntelligence;
    anim->m_VarS32[0] = 0;
    anim->m_VarS32[1] = 0;
    anim->m_ConditionTimer = *(f32 *) (def + 0x7C);
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            actor->m_dwFlags &= ~0x2000;
            actor->m_dwFlags &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            actor->m_dwFlags &= ~0x100;
            CAI__SetMode(actor, anim, 0);
        } else {
            CAI__SetMode(actor, anim, 1);
        }
    } else {
        CAI__SetMode(actor, anim, 0x40);
    }
    anim->m_Action = 0;
}
#else
void Turret_Advance(u8 *actor, u8 *anim) {
    u8 *def;

    def = *(u8 **) (actor + 0x14);
    *(s32 *) (anim + 0x10C) = 0;
    *(s32 *) (anim + 0x110) = 0;
    *(f32 *) (anim + 0x64) = *(f32 *) (def + 0x7C);
    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        if (*(s32 *) (def + 0x14) & 0x1000) {
            *(s32 *) (actor + 0xD4) &= ~0x2000;
            *(s32 *) (actor + 0xD4) &= ~0x100;
        }
        if (*(s32 *) (def + 0x14) & 0x800) {
            *(s32 *) (actor + 0xD4) &= ~0x100;
            CAI__SetMode(actor, anim, 0);
        } else {
            CAI__SetMode(actor, anim, 1);
        }
    } else {
        CAI__SetMode(actor, anim, 0x40);
    }
    anim[0x37] = 0;
}
#endif
