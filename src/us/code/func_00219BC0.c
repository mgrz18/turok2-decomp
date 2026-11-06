/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_16.CPP, func_802466A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/world.h"
#include "globals/render.h"
#include "globals/player.h"

s32 CScene__FindObjectType(CScene *, s32);
CQuaternion GameObject_GetGroundRotation(CGameObjectInstance *);
void CAnimationState__Reset(CAnimationState *);
void CGameObjectInstance__RequestAnim(CGameObjectInstance *, s32);
void CMtxF__Identity(CMtxF *);
f32 GameObject_GetRegionFloorHeight(CGameObjectInstance *, s32);
void CAI__Setup(CGameObjectInstance *, CAI *, void *);
void CPathTrack__StartAtNearestPoint(void *, s32, CVector3 *);
void CGameObjectInstance__SetupAnimation(CGameObjectInstance *, s32 *, s32, s32);
s32 Instance_NextUniqueId(void);
void CGameObjectInstance__DrawModelLitVertices(void);

extern CScene gScene;
extern s32 gEngineApp_m_Game_m_PlayerList_m_nPlayers;      /* player count */
extern u8 *gEngineApp_m_Game_m_PlayerList_m_Players;      /* players, 0x1328 bytes each */
extern void *gIntelligenceTypeInfo[];

/* The body (collision) setup for the object's intelligence type; a player's
 * second object (its body at +0x288) gets its own. */
static __inline__ void *body_setup(CGameObjectInstance *obj) {
    u32 type;
    s32 i;
    u8 *player;

    type = obj->header.m_pIntelligence->m_Type;
    if (type >= 13) {
        return 0;
    }
    if (type == 11) {
        for (i = 0; i < gEngineApp_m_Game_m_PlayerList_m_nPlayers; i++) {
            player = gEngineApp_m_Game_m_PlayerList_m_Players + i * 0x1328;
            if (player == (u8 *) obj) {
                return 0;
            }
            if (player + 0x288 == (u8 *) obj) {
                return &gWeaponActorTypeInfo;
            }
        }
    }
    return gIntelligenceTypeInfo[type];
}

static __inline__ void clear_motion(CGameObjectInstance *obj) {
    obj->m_vVelocity.x = 0;
    obj->m_vVelocity.y = 0;
    obj->m_vVelocity.z = 0;
    obj->m_vWaterDisplacement.x = 0;
    obj->m_vWaterDisplacement.y = 0;
    obj->m_vWaterDisplacement.z = 0;
    obj->m_pInstanceBelow = 0;
}

void CGameObjectInstance__Construct(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence, s32 sound,
                   void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale, CVector3 velocity) {
    s32 animIndex;
    s32 frame;
    CScene *world;
    f32 ground;

    obj->m_dwFlags &= ~0x40000;
    if (model == -1) {
        model = CScene__FindObjectType(&gScene, type);
        if (model == -1) {
            return;
        }
    }
    clear_motion(obj);
    obj->header.m_Type = 1;
    obj->header.m_nObjType = model;
    obj->header.m_pIntelligence = intelligence;
    obj->header.m_pCurrentRegion = region;
    obj->m_RotY = rotY;
    obj->header.m_vPos = pos;
    obj->m_dwFlags = flags;
    obj->m_vScale = scale;
    obj->m_vVelocity = velocity;
    obj->m_nTypeFlag = type;
    obj->m_qGround = GameObject_GetGroundRotation(obj);
    obj->m_CollisionYOffset = 0;
    obj->m_nAnims = 0;
    obj->m_BlendFinish = 0;
    obj->m_BlendStart = 0;
    obj->m_dwHotPointFlags = 0x7800074;
    CAnimationState__Reset(&obj->m_CurrentAnimation);
    CAnimationState__Reset(&obj->m_BlendAnimation);
    CGameObjectInstance__RequestAnim(obj, 0);
    obj->m_dwFlags = (obj->m_dwFlags & ~0x400) | 0x2100;
    CMtxF__Identity(&obj->m_mfOrient);
    obj->m_pmtDrawMtxs = 0;
    obj->m_pmtLastFrameDrawMtxs = 0;
    obj->m_pmfShadow = 0;
    obj->m_ActiveSwooshes = 0;
    if (!(obj->m_dwFlags & 1) && obj->header.m_pCurrentRegion != 0) {
        ground = GameObject_GetRegionFloorHeight(obj, 0);
        if (obj->header.m_vPos.y - ground < 51.199997f) {
            obj->header.m_vPos.y = ground;
        }
    }
    obj->m_dwFlags |= 0x10000;
    obj->m_dwFlags |= 0x20000;
    if (obj->header.m_pIntelligence->m_Type == 0 || (obj->m_dwFlags & 1)) {
        obj->m_dwFlags &= ~0x20000;
    }
    animIndex = 0;
    frame = 0;
    CAI__Setup(obj, &obj->m_AI, body_setup(obj));
    if (obj->m_AI.m_pModeCurrent != 0) {
        animIndex = -obj->m_AI.m_AnimIndex;
    }
    obj->m_AI.m_pParent = 0;
    if (sound != -1) {
        CPathTrack__StartAtNearestPoint((u8 *) obj + 0x1D4, sound, &obj->header.m_vPos);
    }
    world = &gScene;
    CGameObjectInstance__SetupAnimation(obj, world->m_pObjects, world->m_ObjectsKey, animIndex);
    if (frame != 0) {
        obj->m_CurrentAnimation.m_AnimationFrame = frame;
        obj->m_dwFlags &= ~0x400;
    }
    obj->m_dwUniqueID = Instance_NextUniqueId() << 10;
    if (obj->m_dwFlags & 8) {
        obj->m_AI.m_pfDraw = CGameObjectInstance__DrawModelLitVertices;
        *(CLights1 *) obj->m_Lights[0] = gDefaultLights;
        *(CLights1 *) obj->m_Lights[1] = gDefaultLights;
    }
}
