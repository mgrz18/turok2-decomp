/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_16.CPP, func_802466A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x0 */ u32 type;
} CIntelligence;

typedef struct {
    s32 words[6];
} Lights1; /* libultra Lights1, copied as words */

typedef struct {
    /* 0x00 */ u8 pad00[0x14];
    /* 0x14 */ s32 bank;
    /* 0x18 */ u8 pad18[0x28];
    /* 0x40 */ s32 directory;
} World;

s32 func_00224180(World *, s32);
CQuaternion func_00267944(CGameObjectInstance *);
void func_00219910(CAnimationState *);
void func_0021E960(CGameObjectInstance *, s32);
void func_0020F79C(CMtxF *);
f32 func_0026872C(CGameObjectInstance *, s32);
void func_002429BC(CGameObjectInstance *, CAI *, void *);
void func_00284760(void *, s32, CVector3 *);
void func_0021A150(CGameObjectInstance *, s32, s32, s32);
s32 func_00214ED8(void);
void func_0021DA8C(void);

extern World D_800F7078;
extern s32 D_80119878;      /* player count */
extern u8 *D_80119874;      /* players, 0x1328 bytes each */
extern u8 D_800AE994[];
extern void *D_800ACB60[];
extern Lights1 D_800AB0E8;

/* The body (collision) setup for the object's intelligence type; a player's
 * second object (its body at +0x288) gets its own. */
static __inline__ void *body_setup(CGameObjectInstance *obj) {
    u32 type;
    s32 i;
    u8 *player;

    type = ((CIntelligence *) obj->header.m_pIntelligence)->type;
    if (type >= 13) {
        return 0;
    }
    if (type == 11) {
        for (i = 0; i < D_80119878; i++) {
            player = D_80119874 + i * 0x1328;
            if (player == (u8 *) obj) {
                return 0;
            }
            if (player + 0x288 == (u8 *) obj) {
                return D_800AE994;
            }
        }
    }
    return D_800ACB60[type];
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

void func_00219BC0(CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence, s32 sound,
                   void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale, CVector3 velocity) {
    s32 animIndex;
    s32 frame;
    World *world;
    f32 ground;

    obj->m_dwFlags &= ~0x40000;
    if (model == -1) {
        model = func_00224180(&D_800F7078, type);
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
    obj->m_qGround = func_00267944(obj);
    obj->m_CollisionYOffset = 0;
    obj->m_nAnims = 0;
    obj->m_BlendFinish = 0;
    obj->m_BlendStart = 0;
    obj->m_dwHotPointFlags = 0x7800074;
    func_00219910(&obj->m_CurrentAnimation);
    func_00219910(&obj->m_BlendAnimation);
    func_0021E960(obj, 0);
    obj->m_dwFlags = (obj->m_dwFlags & ~0x400) | 0x2100;
    func_0020F79C(&obj->m_mfOrient);
    obj->m_pmtDrawMtxs = 0;
    obj->m_pmtLastFrameDrawMtxs = 0;
    obj->m_pmfShadow = 0;
    obj->m_ActiveSwooshes = 0;
    if (!(obj->m_dwFlags & 1) && obj->header.m_pCurrentRegion != 0) {
        ground = func_0026872C(obj, 0);
        if (obj->header.m_vPos.y - ground < 51.199997f) {
            obj->header.m_vPos.y = ground;
        }
    }
    obj->m_dwFlags |= 0x10000;
    obj->m_dwFlags |= 0x20000;
    if (((CIntelligence *) obj->header.m_pIntelligence)->type == 0 || (obj->m_dwFlags & 1)) {
        obj->m_dwFlags &= ~0x20000;
    }
    animIndex = 0;
    frame = 0;
    func_002429BC(obj, &obj->m_AI, body_setup(obj));
    if (obj->m_AI.m_pModeCurrent != 0) {
        animIndex = -obj->m_AI.m_AnimIndex;
    }
    obj->m_AI.m_pParent = 0;
    if (sound != -1) {
        func_00284760((u8 *) obj + 0x1D4, sound, &obj->header.m_vPos);
    }
    world = &D_800F7078;
    func_0021A150(obj, world->directory, world->bank, animIndex);
    if (frame != 0) {
        obj->m_CurrentAnimation.m_AnimationFrame = frame;
        obj->m_dwFlags &= ~0x400;
    }
    obj->m_dwUniqueID = func_00214ED8() << 10;
    if (obj->m_dwFlags & 8) {
        obj->m_AI.m_pfDraw = func_0021DA8C;
        *(Lights1 *) obj->m_Lights[0] = D_800AB0E8;
        *(Lights1 *) obj->m_Lights[1] = D_800AB0E8;
    }
}
