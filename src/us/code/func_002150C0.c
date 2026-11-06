/* WRONG CUT: splat named this D_002150B8 (0x2150B8, 500 bytes): its first 8 bytes are junk fill
 * ("ICKUP_SA" text), the real function starts at the 16-aligned 0x2150C0 (ROM symbol CLevelActor__Construct),
 * 492 bytes; CScene__DecompressGridSection calls it for the level file's third table.
 * Sets up a placed level actor from its 0x20-byte record: position, bounds (position +- radius),
 * model, segment, angle (s8 in 1/254 turns), flags (always with bit 1), default 24-byte state. */

#include "common.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "structs/light.h"
#include "globals/engine.h"

/* A placed actor's record in the level data. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ f32 radius;
    /* 0x14 */ u16 id;
    /* 0x16 */ u16 segment; /* 0xFFFF = none */
    /* 0x18 */ u16 flags;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 model;   /* 0xFFFF = none */
    /* 0x1E */ s8 angle;
} PlacedActorRecord;

extern u8 gLevelActorIntelligence[];         /* the empty model */
extern u8 gScene[];         /* the model cache */

void *CScene__GetIntelligence(void *cache, s32 model);
f32 GameObject_GetCollisionHeight(CLevelActor *actor);

void CLevelActor__Construct(CLevelActor *actor, PlacedActorRecord *record, s32 owner, u8 *segments) {
    CVector3 top;

    actor->header.m_Type = 3;
    actor->header.m_nObjType = record->id;
    actor->m_ViewMask = record->value;
    actor->header.m_vPos = record->pos;
    actor->m_RotY = record->angle * 0.02473695f;
    actor->m_ModelScale = 0.35f;
    actor->m_Height = 0;
    actor->m_Bounds.m_vMin.x = record->pos.x - record->radius;
    actor->m_Bounds.m_vMin.y = record->pos.y - record->radius;
    actor->m_Bounds.m_vMin.z = record->pos.z - record->radius;
    actor->m_Bounds.m_vMax.x = record->pos.x + record->radius;
    actor->m_Bounds.m_vMax.y = record->pos.y + record->radius;
    actor->m_Bounds.m_vMax.z = record->pos.z + record->radius;
    actor->m_Resource = owner;
    if (record->model == 0xFFFF) {
        actor->header.m_pIntelligence = (CIntelligence *) gLevelActorIntelligence;
    } else {
        actor->header.m_pIntelligence = CScene__GetIntelligence(gScene, record->model);
    }
    if (record->segment == 0xFFFF) {
        actor->header.m_pCurrentRegion = NULL;
    } else {
        actor->header.m_pCurrentRegion = (struct CGameRegion *) (segments + record->segment * 32);
    }
    actor->m_wFlags = record->flags | 2;
    actor->m_Timer = 0;
    actor->m_AnimSpeed = 0;
    actor->m_nKilledIndex = record->unk1A;
    top.x = actor->header.m_vPos.x;
    top.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    top.z = actor->header.m_vPos.z;
    *(CLights1 *) actor->m_Lights = gSceneLights;
    actor->m_DrawnFrame = actor->m_UpdatedFrame = (u8)gFrameCount - 1;
}
