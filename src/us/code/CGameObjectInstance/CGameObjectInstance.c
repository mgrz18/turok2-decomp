/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/CAMERA/80243A80_16.CPP, func_802466A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/world.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/render.h"
#include "globals/player.h"
#include "globals/object.h"
#include "protos/scene.h"
#include "protos/game.h"
#include "common.h"
#include "globals/strings.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "structs/cache.h"
#include "structs/vector.h"
/* FAKEMATCH (round 22): 1 register pin (`register f32 t __asm__("$f0")`, written __asm__, which the round-21
 * scan for asm("$ did not match); without it: CGameObjectInstance__AlignToGround 28 of 87 words. */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00246FF4.c, func_00246FF4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "globals/engine.h"
#include "structs/region.h"
#include "globals/collision.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_2.CPP, func_802472F0_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle_event.h"
#include "globals/particle.h"
#include "structs/camera.h"
#include "globals/math.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00247F08.c, func_00247F08, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_7.c, func_802480F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_3.CPP, func_802484B0_de, with Turok 2's layouts.
 * Turok 2 poses the skeleton recursively, one bone per call, keeping its
 * scratch values in file statics (sPoseChildren-sPoseCeilingOverlap). */

#include "common.h"
#include "globals/swoosh.h"
#include "protos/ultra.h"
#include "structs/model.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_6.CPP, func_8024A7A0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/light.h"
/* FAKEMATCH (round 22): 1 register pin (`register f32 py __asm__("$f1")`, missed by the round-21 scan);
 * without it: CGameObjectInstance__DrawShadow 3 of 238 words (the object's y in $f12, not $f1; reading the
 * field twice instead of py gives the same). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0024AA08.c, func_0024AA08, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_8.CPP, func_8024B2D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
/* One source file: CGameObjectInstance__LoadGeometry and
 * CGameObjectInstance__TouchGeometry name the same "anim geometry" literal (GCC
 * shares a string only within a file), the file's whole .rodata. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_13.c, func_8024B3A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_9.CPP, func_8024B53C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "m2c_macros.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_14.c, func_8024B6A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/gbi.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_2.c, func_8024BC94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024BA6C_5.CPP, func_8024C104_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024BA6C_6.CPP, func_8024C664_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_11.c, func_8024C8C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_12.c, func_8024C92C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_13.c, func_8024CA10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_14.c, func_8024CB4C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_15.c, func_8024CC30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024D028_de.c, func_8024D028_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

CQuaternion GameObject_GetGroundRotation(CGameObjectInstance *);
void CAnimationState__Reset(void *state);
void CGameObjectInstance__RequestAnim(CGameObjectInstance *arg0, s32 arg1);
void CMtxF__Identity(CMtxF *);
void CAI__Setup(CGameObjectInstance *, CAI *, void *);
void CPathTrack__StartAtNearestPoint(void *, s32, CVector3 *);
void CGameObjectInstance__SetupAnimation(); /* func_0021A150.c: the N64 and port bodies take their own views */
s32 Instance_NextUniqueId(void);
void CGameObjectInstance__DrawModelLitVertices(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params);

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
        for (i = 0; i < gEngineApp_m_Game.m_PlayerList.m_nPlayers; i++) {
            player = (u8 *)gEngineApp_m_Game_m_PlayerList_m_Players + i * (s32)sizeof(CPlayer);
            if (player == (u8 *) obj) {
                return 0;
            }
            if (player + T2_OFFSETOF(CPlayer, m_WeaponActor) == (u8 *) obj) {
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
        CPathTrack__StartAtNearestPoint(&obj->m_AI.m_PathTrack, sound, &obj->header.m_vPos); /* N64 0x1D4 */
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
        obj->m_Lights[0] = gDefaultLights;
        obj->m_Lights[1] = gDefaultLights;
    }
}

/* Pick, at random, one entry of the actor's animation set (0xBA) whose action
 * is `action`: among the run of equal entries around the one found, leaving
 * out index `skip` (-1: none). */


typedef struct {
    s16 count;      /* 0x00 */
    u8 pad[0x66];
} RandList;         /* 0x68 */
s32 BinarySearch(s32 *, s32, sptr);
void WeightedTable_Clear(void *table);
s32 WeightedTable_Add(void *table, s16 id, s16 weight);
s16 WeightedTable_Pick(void *table); /* ABI: returns s16 (the host leaves the upper bits of the return register undefined) */

sptr CGameObjectInstance__PickRandomAnim(CGameObjectInstance *actor, sptr action, sptr skip) {
    RandList list;
    CMemEntry *res;
    s32 *data;
    s32 *entries;
    s32 count;
    sptr i;
    sptr found;

    if (actor->m_dwFlags & 0x40000) {
        res = CCache__Request(0, ((sptr)actor->m_rpObject), actor->m_rpObjectHeader, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (res != NULL) {
            data = CISet__GetBlock(res->data, 1);
            entries = data + 2;
            count = data[1];
            i = BinarySearch(entries, actor->m_nAnims, action);
            if (i != -1) {
                found = i;
                while (i > 0 && entries[i - 1] == action) {
                    i--;
                }
                WeightedTable_Clear(&list);
                if (skip == -1) {
                    while (i < count && entries[i] == action) {
                        WeightedTable_Add(&list, i++, 10);
                    }
                } else {
                    while (i < count && entries[i] == action) {
                        if (skip != i) {
                            WeightedTable_Add(&list, i, 10);
                        }
                        i++;
                    }
                }
                if (list.count == 0) {
                    WeightedTable_Add(&list, found, 10);
                }
                i = (s16) WeightedTable_Pick(&list);
            }
            CCache__Unlock(0, res);
            return i;
        }
    }
    return -1;
}

#ifndef TARGET_PORT
/* The cache name the animation-object requests read by name (globals/strings.h): in the ROM's .rodata
 * after CGameObjectInstance__Construct's literal, before CGameObjectInstance__SetupAnimation's, so
 * defined here (the port keeps the generated copy). */
const char sCGameObjectInstanceCacheDesc_AnimObjectInfo_def[] __asm__("sCGameObjectInstanceCacheDesc_AnimObjectInfo") = "anim object info";
#endif

#ifdef TARGET_PORT
/* The port: the same through CGameObjectInstance (structs/game_object.h); the SetupActor view below
 * has the N64 offsets (the pointers before 0xA4 and in CAI make the host ones larger). */
#include "structs/game_object.h"
#include "structs/cache.h"
struct CGameObjectInstance;


void CMtxF__TransformBox(void *m, void *in, void *out);
void CAnimationState__Reset(void *state);
sptr CGameObjectInstance__PickRandomAnim(struct CGameObjectInstance *actor, sptr action, sptr skip);

void CGameObjectInstance__SetupAnimation(CGameObjectInstance *actor, s32 *a1, s32 a2, s32 anim) {
    CMemEntry *model;
    CMemEntry *set;
    s32 id;
    void *tree;
    void *setTree;
    s32 size;
    void *data;

    actor->m_dwFlags &= ~0x40000;
    id = CISet__GetROMAddress((sptr)a1, a2, actor->header.m_nObjType);
    model = CCache__Request(0, id, 0x18, 0, 0, 0, "anim object index", 1);
    if (model != NULL) {
        tree = model->data;
        actor->m_rpGeometryHeader = (void *)(sptr)CISet__GetROMAddressAndSize(tree, id, 1, &size);
        actor->m_rpGeometryHeaderSize = size;
        actor->m_rpObject = (void *)(sptr)CISet__GetROMAddressAndSize(tree, id, 0, &size);
        actor->m_rpObjectHeader = size;
        actor->m_rpAnims = (void *)(sptr)CISet__GetROMAddress((sptr)tree, id, 2);
        set = CCache__Request(0, (s32)(sptr)actor->m_rpObject, actor->m_rpObjectHeader, 0, 0, 0, (char *)sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (set != NULL) {
            setTree = set->data;
            data = CISet__GetBlock(setTree, 0);
            ((void (*)(CGameObjectInstance *, void *))actor->m_AI.m_pfCalculateOrientationMatrix)(actor, &actor->m_AI);
            CMtxF__TransformBox(&actor->m_mfOrient, data, &actor->m_vMin);
            CAnimationState__Reset(&actor->m_CurrentAnimation);
            CAnimationState__Reset(&actor->m_BlendAnimation);
            /* block 1 is a CUSet (s32 element size, s32 count): the u16 at +6 is the
             * count's low half on the N64; the archive swaps the words, so on a
             * little-endian host it is the word at +4 */
            actor->m_nAnims = (u16) ((u32 *) CISet__GetBlock((void *)(sptr)setTree, 1))[1];
            actor->m_dwFlags |= 0x40000;
            if (anim > 0) {
                anim = CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) actor, anim, -1);
            } else {
                anim = -anim;
            }
            if (anim == -1) {
                anim = 0;
            }
            actor->m_CurrentAnimation.m_nRequestedAnimIndex = anim;
            actor->m_CurrentAnimation.m_bCycleCompleted = 0;
            if (actor->m_CurrentAnimation.m_nAnimIndex != anim) {
                actor->m_CurrentAnimation.m_bAnimActive = 1;
            }
            actor->m_AI.m_ModeLastFrame = -1;
            actor->m_AI.m_AnimIndex = anim;
            CCache__Unlock(0, set);
        }
        CCache__Unlock(0, model);
    }
}
#else

/* Bind an actor to its model and animation set, then choose its first
 * animation: `anim` > 0 is an action to pick at random (CGameObjectInstance__PickRandomAnim),
 * otherwise -anim is the entry itself. */

typedef struct SetupActor SetupActor;

struct SetupActor {
    u8 pad0[0x2];
    u16 unk2;               /* 0x002 */
    u8 pad4[0x54];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    s32 unkA8;              /* 0x0A8 */
    s32 unkAC;              /* 0x0AC */
    s32 unkB0;              /* 0x0B0 */
    s32 unkB4;              /* 0x0B4 */
    u8 padB8[0x2];
    u16 setId;              /* 0x0BA */
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 unkD8[0x4];          /* 0x0D8 */
    s16 curAnim;            /* 0x0DC */
    s16 nextAnim;           /* 0x0DE */
    u8 padE0[0x2];
    u8 unkE2;               /* 0x0E2 */
    u8 unkE3;               /* 0x0E3 */
    u8 padE4[0x8];
    u8 unkEC[0x54];         /* 0x0EC */
    u8 ai[0x35];            /* 0x140 */
    s8 unk175;              /* 0x175 */
    u8 pad176[0x90];
    s8 unk206;              /* 0x206 */
    u8 pad207[0x3D];
    void (*init)(SetupActor *, void *);  /* 0x244 */
};


void CMtxF__TransformBox(void *m, void *in, void *out);
void CAnimationState__Reset(void *state);
sptr CGameObjectInstance__PickRandomAnim(struct CGameObjectInstance *actor, sptr action, sptr skip);

void CGameObjectInstance__SetupAnimation(SetupActor *actor, sptr a1, s32 a2, sptr anim) {
    s32 *model;
    s32 *set;
    s32 id;
    sptr tree;
    sptr setTree;
    void *data;

    actor->flags &= ~0x40000;
    id = CISet__GetROMAddress(a1, a2, actor->unk2);
    model = CCache__Request(0, id, 0x18, 0, 0, 0, (sptr) "anim object index", 1);
    if (model != NULL) {
        tree = *model;
        actor->unkAC = CISet__GetROMAddressAndSize(tree, id, 1, &actor->unkB4);
        actor->unkA4 = CISet__GetROMAddressAndSize(tree, id, 0, &actor->unkB0);
        actor->unkA8 = CISet__GetROMAddress(tree, id, 2);
        set = CCache__Request(0, actor->unkA4, actor->unkB0, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (set != NULL) {
            setTree = *set;
            data = CISet__GetBlock(setTree, 0);
            actor->init(actor, actor->ai);
            CMtxF__TransformBox(actor->unk58, data, actor->unkBC);
            CAnimationState__Reset(actor->unkD8);
            CAnimationState__Reset(actor->unkEC);
            actor->setId = ((u16 *) CISet__GetBlock(setTree, 1))[3];
            actor->flags |= 0x40000;
            if (anim > 0) {
                anim = CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) actor, anim, -1);
            } else {
                anim = -anim;
            }
            if (anim == -1) {
                anim = 0;
            }
            actor->nextAnim = anim;
            actor->unkE2 = 0;
            if (actor->curAnim != anim) {
                actor->unkE3 = 1;
            }
            actor->unk175 = -1;
            actor->unk206 = anim;
            CCache__Unlock(0, set);
        }
        CCache__Unlock(0, model);
    }
}
#endif
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

/* Advance an actor's animation (CGameObjectInstance__AdvanceAnimation) and move it by the root motion
 * that produced: the new position is taken back out, turned by the actor's
 * rotation, collided along (CGameObjectInstance__MoveWithCollision), then the animation is applied. */

typedef struct MoveActor MoveActor;
#ifdef TARGET_PORT
struct MoveActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Actor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_Actor(header.m_pIntelligence)]; s32 *region; }; /* N64 0x14 */
        struct { u8 _hp_unk58[T2_HOFF_Actor(m_mfOrient)]; u8 unk58[0x40]; }; /* N64 0x58 */
        struct { u8 _hp_unkA4[T2_HOFF_Actor(m_rpObject)]; sptr unkA4; }; /* N64 0xA4: a ROM address */
        struct { u8 _hp_unkB0[T2_HOFF_Actor(m_rpObjectHeader)]; s32 unkB0; }; /* N64 0xB0 */
        struct { u8 _hp_unkBC[T2_HOFF_Actor(m_vMin)]; u8 unkBC[0x18]; }; /* N64 0xBC */
        struct { u8 _hp_flags[T2_HOFF_Actor(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_ai[T2_HOFF_Actor(m_AI)]; u8 ai[0x54]; }; /* N64 0x140 */
        struct { u8 _hp_unk194[T2_HOFF_Actor(m_AI.m_vDesiredPos)]; u8 unk194[0xC]; }; /* N64 0x194 */
        struct { u8 _hp_init[T2_HOFF_Actor(m_AI.m_pfCalculateOrientationMatrix)]; void (*init)(MoveActor *, void *); }; /* N64 0x244 */
    };
};
#else
struct MoveActor {
    u8 pad0[0x4];
    CVector3 pos;               /* 0x004 */
    u8 pad10[0x4];
    s32 *region;            /* 0x014 */
    u8 pad18[0x40];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    u8 padA8[0x8];
    s32 unkB0;              /* 0x0B0 */
    u8 padB4[0x8];
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x68];
    u8 ai[0x54];            /* 0x140 */
    u8 unk194[0xB0];        /* 0x194 */
    void (*init)(MoveActor *, void *);  /* 0x244 */
};
#endif

void CMtxF__TransformBox(void *m, void *in, void *out);
void CVector3__Add(void *out, void *a, void *b);
void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
void CGameObjectInstance__MoveWithCollision(char *arg0, CVector3 *arg1);
typedef struct B10Obj B10Obj;
void CGameObjectInstance__AlignToGround(B10Obj *o);
/* An animation track (0x14 bytes). */
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 current;
    /* 0x06 */ u8 pad6[4];
    /* 0x0A */ s8 blending;
    /* 0x0B */ s8 active;
    /* 0x0C */ u8 padC[8];
} AnimTrack;
#ifdef TARGET_PORT
typedef struct AnimActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AnimActor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_position[T2_HOFF_AnimActor(header.m_vPos)]; CVector3 position; }; /* N64 0x4 */
        struct { u8 _hp_scaleX[T2_HOFF_AnimActor(m_vScale.x)]; f32 scaleX; }; /* N64 0x34 */
        struct { u8 _hp_scaleY[T2_HOFF_AnimActor(m_vScale.y)]; f32 scaleY; }; /* N64 0x38 */
        struct { u8 _hp_scaleZ[T2_HOFF_AnimActor(m_vScale.z)]; f32 scaleZ; }; /* N64 0x3C */
        struct { u8 _hp_heading[T2_HOFF_AnimActor(m_RotY)]; f32 heading; }; /* N64 0x50 */
        struct { u8 _hp_animationKey[T2_HOFF_AnimActor(m_rpAnims)]; sptr animationKey; }; /* N64 0xA8: a ROM address */
        struct { u8 _hp_type[T2_HOFF_AnimActor(m_nTypeFlag)]; u16 type; }; /* N64 0xB8 */
        struct { u8 _hp_animationCount[T2_HOFF_AnimActor(m_nAnims)]; s16 animationCount; }; /* N64 0xBA */
        struct { u8 _hp_flags[T2_HOFF_AnimActor(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_currentTrack[T2_HOFF_AnimActor(m_CurrentAnimation)]; AnimTrack currentTrack; }; /* N64 0xD8 */
        struct { u8 _hp_previousTrack[T2_HOFF_AnimActor(m_BlendAnimation)]; AnimTrack previousTrack; }; /* N64 0xEC */
        struct { u8 _hp_blendFrames[T2_HOFF_AnimActor(m_BlendLength)]; s16 blendFrames; }; /* N64 0x100 */
        struct { u8 _hp_blend[T2_HOFF_AnimActor(m_Blend)]; f32 blend; }; /* N64 0x104 */
        struct { u8 _hp_blendTime[T2_HOFF_AnimActor(m_BlendPercent)]; f32 blendTime; }; /* N64 0x108 */
        struct { u8 _hp_blendStart[T2_HOFF_AnimActor(m_BlendStart)]; u8 blendStart; }; /* N64 0x10C */
        struct { u8 _hp_blendEnd[T2_HOFF_AnimActor(m_BlendFinish)]; u8 blendEnd; }; /* N64 0x10D */
    };
} AnimActor;
#else
typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 position;
    /* 0x010 */ u8 pad10[0x24];
    /* 0x034 */ f32 scaleX;
    /* 0x038 */ f32 scaleY;
    /* 0x03C */ f32 scaleZ;
    /* 0x040 */ u8 pad40[0x10];
    /* 0x050 */ f32 heading;
    /* 0x054 */ u8 pad54[0x54];
    /* 0x0A8 */ s32 animationKey;
    /* 0x0AC */ u8 padAC[0xC];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ s16 animationCount;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ AnimTrack currentTrack;
    /* 0x0EC */ AnimTrack previousTrack;
    /* 0x100 */ s16 blendFrames;
    /* 0x102 */ u8 pad102[2];
    /* 0x104 */ f32 blend;
    /* 0x108 */ f32 blendTime;
    /* 0x10C */ u8 blendStart;
    /* 0x10D */ u8 blendEnd;
} AnimActor;
#endif
void CGameObjectInstance__AdvanceAnimation(AnimActor *actor);

void CGameObjectInstance__UpdateAnimation(MoveActor *actor) {
    CVector3 oldPos;
    CVector3 newPos;
    sptr *set;
    void *data;
    s32 *game;
    u32 flags;
    u32 moving;

    flags = actor->flags;
    if (!(flags & 0x40000)) {
        return;
    }
    game = &gScene;
    if (*game != 4 || (flags & 0x2000)) {
        moving = flags & 0x300000;
        oldPos = actor->pos;
        CGameObjectInstance__AdvanceAnimation(actor);
        newPos = actor->pos;
        actor->pos = oldPos;
        if (!moving && *actor->region != 0) {
            CVector3__Subtract(&newPos, &newPos, &oldPos);
            CVector3__Add(&newPos, &newPos, actor->unk194);
            CGameObjectInstance__MoveWithCollision((char *) actor, &newPos);
            if (*actor->region == 1) {
                CScene__CheckAutoActivation(game, actor, &oldPos, &actor->pos);
            }
        }
        CGameObjectInstance__AlignToGround(actor);
        set = CCache__Request(0, actor->unkA4, actor->unkB0, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (set != NULL) {
            data = CISet__GetBlock(*set, 0);
            actor->init(actor, actor->ai);
            CMtxF__TransformBox(actor->unk58, data, actor->unkBC);
            CCache__Unlock(0, set);
        }
    }
}

typedef struct B10H B10H;
#ifdef TARGET_PORT
struct B10Obj {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_B10Obj(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_p14[T2_HOFF_B10Obj(header.m_pCurrentRegion)]; B10H* p14; }; /* N64 0x10 */
        struct { u8 _hp_f5C[T2_HOFF_B10Obj(m_qGround.x)]; f32 f5C; }; /* N64 0x40 */
        struct { u8 _hp_flags100[T2_HOFF_B10Obj(m_dwFlags)]; u32 flags100; }; /* N64 0xD4 */
    };
};
#else
struct B10Obj { /* Turok 2 layout (Rage Wars: p14, f5C, flags100) */
    u8 pad0[0x10];
    B10H* p14;          /* 0x10 */
    u8 pad14[0x2C];
    f32 f5C;            /* 0x40: quaternion */
    u8 pad44[0x90];
    u32 flags100;       /* 0xD4 */
};
#endif

struct B10H {
    u8 pad0[0x2];
                u16 h2;
};
s32 GameObject_IsOnGround(void *actor);
/* The ground rotation through an out pointer: the definition's struct return (its hidden first
 * argument) under its own name on the N64, the port's shim (port/game/t2abi.c, ABI.md struct_scalar). */
#ifdef TARGET_PORT
void GameObject_GetGroundRotation_sret(f32 *loc, B10Obj *o);
#define GameObject_GetGroundRotation_o GameObject_GetGroundRotation_sret
#else
void GameObject_GetGroundRotation_o(f32 *loc, B10Obj *o) __asm__("GameObject_GetGroundRotation");
#endif

s32 GameObject_TiltsToGround(B10Obj *o);
void CQuaternion__TurnToward(f32 *a, f32 *loc);
void CQuaternion__SlerpFast(f32 *a, f32 t, f32 *b, f32 *loc);

void CGameObjectInstance__AlignToGround(B10Obj *o) {
    f32 loc[4];
    u32 fl;
    u32 mask;
    s32 r;
    s32 s;
    register f32 t __asm__("$f0");
    f32 k;
    fl = o->flags100;
    mask = fl & 0x300000;
    if (fl & 1) {
        return;
    }
    r = GameObject_IsOnGround(o); /* ABI: the object in $a0 untouched on MIPS */
    if (o->p14 == 0) {
        return;
    }
    GameObject_GetGroundRotation_o(loc, o);
    s = o->p14->h2 & 1;
    if (GameObject_TiltsToGround(o) == 0) {
        return;
    }
    if (s) {
        loc[0] = loc[1] = loc[2] = 0.0f;
        loc[3] = 1.0f;
    }
    if (mask) {
        if ((!(o->flags100 & 0x1000) && r) || s) {
            CQuaternion__TurnToward(&o->f5C, loc);
        }
    } else {
        if (r && !(o->flags100 & 0x1000)) {
            t = gFrameIncrement;
            t *= 0.125f;
        } else {
            t = gFrameIncrement;
            t *= 0.0375f;
        }
        CQuaternion__SlerpFast(&o->f5C, (sAlignToGroundMax < t) ? sAlignToGroundMax : t, &o->f5C, loc);
    }
}

#ifndef TARGET_PORT
/* CGameObjectInstance__AlignToGround's 1.0f, which it reads as sAlignToGroundMax (and CGameObjectInstance__BuildBoneMatrices's view of
 * the "animations index" after it): defined here, after its own literals (the port keeps the generated copy). */
const f32 sAlignToGroundMax_def __asm__("sAlignToGroundMax") = 1.0f;
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34.CPP, func_8024715C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

union func_80203908_S3_U124;
typedef union func_80203908_S3_U124 func_80203908_S3_U124;
union func_80203908_S3_U124 {
    f32 v0;
    s32 v1;
};
struct CollisionInfo;
struct CollisionInfo {
    s32 w[8];
};
struct ObjectLinks23C;
struct ObjectLinks23C {
    char pad0[0x4];
    CVector3 unk_8;     /* 0x04 (0x08 in Rage Wars) */
    u16 *unk_14;        /* 0x10 (0x14) */
    s32 *unk_18;        /* 0x14 (0x18) */
    char pad18[0xD4 - 0x18];
    s32 unk_100;        /* 0xD4 (0x100) */
    char padD8[0x170 - 0xD8];
    void *unk_1A0;      /* 0x170 (0x1A0) */
    char pad174[0x1C4 - 0x174];
    u16 *unk_1F4;       /* 0x1C4 (0x1F4) */
    char pad1C8[0x204 - 0x1C8];
    u16 unk_238;        /* 0x204 (0x238) */
};
struct CallbackState288 {
    unsigned char padding_0[0x23C];
    void (*callback)(void *, void *, struct CollisionInfo *); /* 0x23C (0x284) */
};
typedef struct ObjectStateC ObjectStateC;
struct ObjectLinks10;
typedef struct ObjectLinks10 ObjectLinks10;
struct CallbackState288;
typedef struct CallbackState288 CallbackState288;
struct ObjectLinks10 {
    char pad0[0xC];
    struct CollisionInfo * unk_C;
};
typedef struct ObjectLinks23C ObjectLinks23C;
struct ObjectStateC;
struct ObjectStateC {
    char pad0[0x8];
    func_80203908_S3_U124 unk_8;
};
extern void CGameObjectInstance__MoveWithCollision(char *, CVector3 *);
typedef struct CollisionInfo CollisionInfo;
extern s32 Collision_MoveInstance(void *arg0, CVector3 arg1, CollisionInfo *arg2);
extern void CGameRegion__FireTriggers(sptr arg0, s32 arg1, void *arg2);
#ifdef TARGET_PORT
/* The host body on CGameObjectInstance: ObjectLinks23C/CallbackState288 are N64 offsets
 * (round 6, u3). */
void CGameObjectInstance__MoveWithCollision(char *arg0, CVector3 *arg1) {
    CGameObjectInstance *o = (CGameObjectInstance *) arg0;
    CollisionInfo *collision;
    CGameRegion *old_contact;
    s32 collided;
    f32 saved;
    void (*callback)(void *, void *, CollisionInfo *);

    if ((o->m_dwFlags & 0x10000) && o->m_AI.m_pModeCurrent != NULL) {
        collision = (CollisionInfo *) o->m_AI.m_pModeCurrent->m_pData;
        if (collision == NULL) {
            collision = (CollisionInfo *) &gCollisionInfoModeDefault;
        }
        saved = ((ObjectStateC *) collision)->unk_8.v0;
        if ((u32) ((u16) o->m_AI.m_AnimType - 0x2B0C) < 8) {
            ((ObjectStateC *) collision)->unk_8.v1 = 0;
        }
        old_contact = o->header.m_pCurrentRegion;
        collided = Collision_MoveInstance(o, *arg1, collision);
        if (old_contact != o->header.m_pCurrentRegion) {
            o->m_AI.m_pPathAvoidRegion = old_contact;
        }
        ((ObjectStateC *) collision)->unk_8.v0 = saved;
        if (old_contact != NULL && o->header.m_pCurrentRegion != NULL
            && old_contact->m_nInfo != o->header.m_pCurrentRegion->m_nInfo && o->header.m_pIntelligence->m_Type == 1) {
            CGameRegion__FireTriggers((sptr) old_contact, 0x80, o);
            CGameRegion__FireTriggers((sptr) o->header.m_pCurrentRegion, 0x40, o);
        }
        if (collided != 0 && !(o->m_dwFlags & 0x300000) && o->header.m_pIntelligence->m_Type == 1
            && gCollisionResults.m_pInstance != NULL && *(u8 *) gCollisionResults.m_pInstance == 1) {
            GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x20, arg0);
        }
        callback = (void (*)(void *, void *, CollisionInfo *)) o->m_AI.m_pfCollision;
        if (callback != NULL) {
            callback(o, &o->m_AI, collision);
        }
    } else {
        o->header.m_vPos = *arg1;
    }
}
#else
void CGameObjectInstance__MoveWithCollision(char *arg0, CVector3 *arg1) {
    CollisionInfo *collision;
    u16 *old_contact;
    s32 collided;
    f32 saved;
    void (*callback)(void *, void *, CollisionInfo *);
    if ((((ObjectLinks23C *)(arg0))->unk_100 & 0x10000) && ((ObjectLinks23C *)(arg0))->unk_1A0 != 0) {
        collision = ((ObjectLinks10 *)(((ObjectLinks23C *)(arg0))->unk_1A0))->unk_C;
        if (collision == 0) {
            collision = (CollisionInfo *) &gCollisionInfoModeDefault;
        }
        saved = ((ObjectStateC *)(collision))->unk_8.v0;
        if ((u32)(((ObjectLinks23C *)(arg0))->unk_238 - 0x2B0C) < 8) {
            ((ObjectStateC *)(collision))->unk_8.v1 = 0;
        }
        old_contact = ((ObjectLinks23C *)(arg0))->unk_14;
        collided = Collision_MoveInstance(arg0, *arg1, collision);
        if (old_contact != ((ObjectLinks23C *)(arg0))->unk_14) {
            ((ObjectLinks23C *)(arg0))->unk_1F4 = old_contact;
        }
        ((ObjectStateC *)(collision))->unk_8.v0 = saved;
        if (old_contact != 0 && ((ObjectLinks23C *)(arg0))->unk_14 != 0 &&
                *old_contact != *((ObjectLinks23C *)(arg0))->unk_14 &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1) {
            CGameRegion__FireTriggers((sptr)old_contact, 0x80, arg0);
            CGameRegion__FireTriggers((sptr)((ObjectLinks23C *)(arg0))->unk_14, 0x40, arg0);
        }
        if (collided != 0 && !(((ObjectLinks23C *)(arg0))->unk_100 & 0x300000) &&
                *((ObjectLinks23C *)(arg0))->unk_18 == 1 && gCollisionResults.m_pInstance != NULL &&
                *(u8 *)gCollisionResults.m_pInstance == 1) {
            GameObject_FireTriggers(gCollisionResults.m_pInstance, 0x20, arg0);
        }
        callback = ((struct CallbackState288 *) ((char *) arg0))->callback;
        if (callback != 0) {
            callback(arg0, arg0 + 0x140, collision);
        }
    } else {
        ((ObjectLinks23C *)(arg0))->unk_8 = *arg1;
    }
}
#endif

typedef struct {
    /* 0x0 */ u16 frames;
    /* 0x2 */ u8 pad2[3];
    /* 0x5 */ u8 start;
    /* 0x6 */ u8 pad6;
    /* 0x7 */ u8 end;
} AnimBlendEntry;
void CAnimationState__SwitchToQueued(void *channel, sptr anims, s32 file, s32 primary, void *active, void *other);
s32 CAnimationState__IsLoaded(void *channel);
AnimBlendEntry *CAnimationState__GetKeyHeader(AnimTrack *track);
void CAnimationState__Release(void *channel);
void CAnimationState__GetRootMotion(AnimTrack *track, CVector3 *delta, f32 *turn, AnimActor *actor);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
f32 Angle_Lerp(f32 t, f32 a, f32 b);
void Angle_Wrap(f32 *angle);
void CMtxF__SetRotateY(void *m, f32 angle);
void CMtxF__TransformPoint(void *m, void *in, void *out);

/* Advances an actor's animation and applies its root motion: the turn and,
 * with flag 0x20000, the move, blended over the transition from the previous
 * track while flag 0x400 is set. */
void CGameObjectInstance__AdvanceAnimation(AnimActor *actor) {
    CVector3 first;
    CVector3 second;
    CVector3 saved;
    CVector3 moved;
    CMtxF matrix;
    s32 active;
    f32 first_value;
    f32 second_value;
    void **resource;
    AnimTrack *track0;
    AnimTrack *track1;
    f32 value;

    if (gScene.m_Mode == 4) {
        if (actor->currentTrack.active == 0) {
            actor->currentTrack.blending = 1;
        }
        if (actor->previousTrack.active == 0) {
            actor->previousTrack.blending = 1;
        }
        if (actor->currentTrack.active == 0 && !(actor->flags & 0x400)) {
            return;
        }
        if (!(actor->flags & 0x2000)) {
            return;
        }
    }

    resource = CCache__Request(0, actor->animationKey, ((actor->animationCount * 4) + 0xF) & ~7, 4, 0, 0,
                             "animations index", 0);
    if (resource == NULL) {
        return;
    }

    track0 = &actor->currentTrack;
    track1 = &actor->previousTrack;
    CAnimationState__SwitchToQueued(track0, (sptr) *resource, actor->animationKey, 1, &active, track1);
    if (CAnimationState__IsLoaded(track0) != 0) {
        AnimBlendEntry *entry;

        CAnimationState__GetRootMotion(track0, &first, &first_value, actor);
        value = first_value;
        saved = first;
        if (active != 0 && track1->current != -1) {
            entry = CAnimationState__GetKeyHeader(track0);
            if (entry->frames != 0 && (actor->flags & 0x200)) {
                u16 count;

                actor->flags |= 0x400;
                count = entry->frames;
                actor->blendTime = 0.0f;
                actor->blendFrames = count;
                actor->blendStart = entry->start;
                actor->blendEnd = entry->end;
            } else {
                actor->flags &= ~0x400;
            }
        }
        if (actor->flags & 0x400) {
            CAnimationState__SwitchToQueued(track1, (sptr) *resource, actor->animationKey, 0, 0, 0);
            if (CAnimationState__IsLoaded(track1) != 0) {
                f32 time;
                f32 end;

                time = actor->blendTime + gFrameIncrement;
                end = (f32) actor->blendFrames;
                actor->blendTime = time;
                if (end < time) {
                    actor->flags &= ~0x400;
                } else {
                    f32 a = (f32) actor->blendEnd;
                    f32 b = (f32) actor->blendStart;
                    f32 t = time / (end + gFrameIncrement);

                    actor->blend = ((a + -2.0f + b) * t * t * t) + (((3.0f - a) - (2.0f * b)) * t * t) + (b * t);
                    CAnimationState__GetRootMotion(&actor->previousTrack, &second, &second_value, actor);
                    CVector3__Lerp(&saved, actor->blend, &second, &first);
                    value = Angle_Lerp(actor->blend, second_value, first_value);
                }
                CAnimationState__Release(track1);
            } else {
                actor->flags &= ~0x400;
            }
        }
        CAnimationState__Release(track0);
        if (actor->flags & 0x20000) {
            actor->heading -= value;
            Angle_Wrap(&actor->heading);
            saved.z = saved.y;
            saved.y = 0.0f;
            CMtxF__SetRotateY(&matrix, actor->heading);
            CMtxF__TransformPoint(&matrix, &saved, &moved);
            actor->position.x += moved.x * actor->scaleX;
            actor->position.z += moved.z * actor->scaleZ;
        } else if (!(actor->flags & 0x300000) && actor->type == 0x137) {
            actor->heading -= value;
            Angle_Wrap(&actor->heading);
        }
    }
    CCache__Unlock(0, resource);
}

#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
int T2Port_AnimEventsEnabled(void); /* port/game/t2actors.c */
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_2.CPP, func_802476C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct AnimEventPair {
    s32 a;
    s32 b;
} AnimEventPair; /* an animation event's value (structs/particle.h CEventValue's two words) */
typedef struct CROMEventEntry {
    u16 frame;
    u16 effect;
    s16 bone;
    s16 pad6;
    CVector3 local;
    AnimEventPair params;
} CROMEventEntry;
/* Turok 2 offsets (Rage Wars in parentheses): pos 0x4 (0x8), transform 0x58
 * (0x74), bone matrices 0x98 (0xB8), type id 0xB8 (0xE4), flags 0xD4 (0x100),
 * animation 0xD8 (0x104), bone mask 0x14C (0x17C). */
#ifdef TARGET_PORT
typedef struct ObjectLinks {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ObjectLinks(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_ObjectLinks(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_transform[T2_HOFF_ObjectLinks(m_mfOrient)]; char transform[0x40]; }; /* N64 0x58 */
        struct { u8 _hp_boneMatrices[T2_HOFF_ObjectLinks(m_pmtDrawMtxs)]; char *boneMatrices; }; /* N64 0x98 */
        struct { u8 _hp_type[T2_HOFF_ObjectLinks(m_nTypeFlag)]; u16 type; }; /* N64 0xB8 */
        struct { u8 _hp_flags[T2_HOFF_ObjectLinks(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_anim[T2_HOFF_ObjectLinks(m_CurrentAnimation)]; char anim[sizeof(CAnimationState)]; }; /* N64 0xD8 */
        struct { u8 _hp_boneMask[T2_HOFF_ObjectLinks(m_AI.m_VisBits)]; s32 boneMask; }; /* N64 0x14C */
    };
} ObjectLinks;
#else
typedef struct ObjectLinks {
    char pad0[4];
    CVector3 pos;
    char pad10[0x48];
    char transform[0x40];
    char *boneMatrices;
    char pad9C[0x1C];
    u16 type;
    char padBA[0x1A];
    s32 flags;
    char anim[0x74];
    s32 boneMask;
} ObjectLinks;
#endif

/* Bone record: the mask bits sit at 0x6C. */
typedef struct BoneInfo {
    char pad0[0x6C];
    s32 mask;
} BoneInfo;
void *CAnimationState__GetEvents(void *channel);
s32 CAnimationState__GetEventCount(void *channel);
void * CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0);
void CMtxF__FromMtxAffine(void *out, void *in);
void ImpactEvent_Send(void *src, void *dst, u16 effect, s32 id, CVector3 pos, AnimEventPair value);

/* Fires the animation events whose frame lies in [from, to). */
void CGameObjectInstance__FireAnimEvents(ObjectLinks *obj, void *anim, f32 from, f32 to) {
    CVector3 pos;
    CVector3 local;
    f32 matrix[16];
    CROMEventEntry *events;
    s32 count;
    s32 i;
    s32 checkMask;
    s32 fire;
    s32 *resource;
    void *table;
    if (!(obj->flags & 0x4000000) && anim != obj->anim) {
        return;
    }
    switch (obj->type) {
    case 0x44E:
    case 0x451:
    case 0x453:
        checkMask = 1;
        break;
    default:
        checkMask = 0;
        break;
    }
    table = 0;
    events = CAnimationState__GetEvents(anim);
    count = CAnimationState__GetEventCount(anim);
    resource = 0;
    for (i = 0; i < count; i++) {
        if (from <= (f32)events[i].frame && (f32)events[i].frame < to) {
            fire = 1;
            if (events[i].bone == -1) {
                local.x = events[i].local.x;
                local.y = events[i].local.z;
                local.z = events[i].local.y;
                CMtxF__TransformPoint(obj->transform, &local, &pos);
            } else {
                if (checkMask && (gImpactEventHandlers[events[i].effect].flags & 4)) {
                    if (resource == 0) {
                        resource = CGameObjectInstance__LoadGeometry(obj);
                        if (resource == 0) {
                            goto masked;
                        }
                        table = CISet__GetBlock(*(void **)resource, 3); /* the data pointer, pointer-wide */
                    }
                    fire = ((BoneInfo *)CISet__GetBlock(CISet__GetBlock(table, events[i].bone), 1))->mask & obj->boneMask;
                }
            masked:
                if (fire == 0) {
                    continue;
                }
                if (obj->boneMatrices != 0) {
                    CMtxF__FromMtxAffine(matrix, obj->boneMatrices + (events[i].bone << 6));
                    CMtxF__TransformPoint(matrix, &events[i].local, &pos);
                } else {
                    pos = obj->pos;
                }
            }
#ifdef TARGET_PORT
            /* The events end in CParticleSystem__Spawn (func_0022D2FC.c), on host types since
             * round 7: on by default, T2_ANIM_EVENTS=0 turns them off (docs/port/RENDER.md 6). */
            if (fire != 0 && !T2Port_AnimEventsEnabled()) {
                fire = 0;
            }
            if (fire != 0) {
                void T2Port_AnimEventLog(void *obj, void *anim, int i, int frame, float from, float to, int effect);
                T2Port_AnimEventLog(obj, anim, i, events[i].frame, from, to, events[i].effect); /* DEBUG-ONLY (T2_ANIM_LOG) */
            }
#endif
            if (fire != 0) {
                ImpactEvent_Send(obj, obj, events[i].effect, -1, pos, events[i].params);
                gAnimEventCount++;
            }
        }
    }
    if (resource != 0) {
        CCache__Unlock(0, resource);
    }
}

/* An actor's facing as a quaternion: turned half a circle from its owner's
 * player (flags 0x300000), its heading plus pitch (0x24C) in a region of
 * type 4, or else its heading applied to its own rotation (0x40). */


f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void CQuaternion__Multiply(void *out, void *a, void *b);

CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *actor) {
    CQuaternion result;
    CQuaternion pitch;
    CQuaternion yaw;
    CCamera *camera;
    f32 s;

    if (actor->m_dwFlags & 0x300000) {
        camera = ((CPlayer *) actor->m_AI.m_pParent)->m_pCamera;
        if (camera == NULL) {
            goto own;
        }
        s = func_002982F0(1.5707964f);
        gQuaternionSin = s;
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        yaw.w = func_002974C0(1.5707964f);
        CQuaternion__Multiply(&result, &yaw, &camera->m_qViewAlign);
    } else if (actor->header.m_pIntelligence->m_Type == 4) {
        s = func_002982F0(actor->m_AI.m_Turret.m_Pitch * 0.5f);
        pitch.x = s;
        pitch.y = 0.0f;
        pitch.z = 0.0f;
        gQuaternionSin = s;
        pitch.w = func_002974C0(actor->m_AI.m_Turret.m_Pitch * 0.5f);
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        gQuaternionSin = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        CQuaternion__Multiply(&result, &pitch, &yaw);
    } else {
    own:
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        gQuaternionSin = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        CQuaternion__Multiply(&result, &yaw, &actor->m_qGround);
    }
    return result;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00247BA4.c, func_00247BA4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B10Q4 {
    f32 x, y, z, w;
} B10Q4;
typedef struct B10Mt {
    f32 m[16];
} B10Mt;
/* Turok 2 layout (Rage Wars: pos 0x8, f70 0x70, flags 0x100, f174 0x174). */
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
/* The rotation through an out pointer: the N64 definition's struct return (its hidden first
 * argument) under its own name, the port's shim (port/game/t2abi.c, ABI.md struct_scalar). */
#ifdef TARGET_PORT
void CGameObjectInstance__GetRotation_sret(B10Q4 *out, CGameObjectInstance *o);
#define CGameObjectInstance__GetRotation_o CGameObjectInstance__GetRotation_sret
#else
void CGameObjectInstance__GetRotation_o(B10Q4 *out, CGameObjectInstance *o) __asm__("CGameObjectInstance__GetRotation");
#endif

f32 GameObject_GetCollisionHeight(void *obj);
void CVector3__Subtract(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Normalize(CVector3 *v);
void CQuaternion__ToMatrix(void *q, void *m);
f32 Math_AcosLookup(f32 x);
f32 func_00298470(f32 x); /* sqrtf */

B10Q4 *CGameObjectInstance__GetAutoAimRotation(B10Q4 *out, CGameObjectInstance *self, CVector3 dir, B10Q4 *outq, f32 angle) {
    CVector3 v10;
    CVector3 v20;
    CVector3 v30;
    CVector3 v40;
    CVector3 v50;
    CVector3 v60;
    B10Q4 q70;
    B10Q4 q80;
    B10Q4 q90;
    B10Mt mA0;
    CGameObjectInstance *best;
    CGameObjectInstance *obj;
    s32 i;
    s32 n;
    f32 thr;
    f32 d;
    f32 t;
    f32 s;
    f32 new_var;
    f32 new_var2;
    new_var2 = angle;
    v20.x = 0;
    v20.y = 0;
    v20.z = 1.0f;
    CGameObjectInstance__GetRotation_o(&q70, self);
    if (new_var2 == 0.0f) {
        if (outq) {
            outq->x = outq->y = outq->z = 0;
            outq->w = 1.0f;
        }
        *out = q70;
        return out;
    }
    CQuaternion__ToMatrix(&q70, &mA0);
    CMtxF__TransformPoint(&mA0, &v20, &v30);
    best = 0;
    thr = func_002974C0(new_var2 * 0.008726647f);
    n = gScene.m_nRegistered;
    for (i = 0; i < n; i++) {
        obj = gScene.m_pRegistered[i];
        if (obj == self) {
            continue;
        }
        if (obj->m_dwFlags & 1) {
            continue;
        }
        if (obj->m_AI.m_Health == 0) {
            continue;
        }
        v60 = obj->header.m_vPos;
        v60.y = v60.y + obj->m_CollisionYOffset;
        v60.y = v60.y + GameObject_GetCollisionHeight(obj) * 0.5f;
        CVector3__Subtract(&v10, &v60, &dir);
        CVector3__Normalize(&v10);
        d = v30.x * v10.x + v30.y * v10.y + v30.z * v10.z;
        if (thr < d) {
            thr = d;
            v50 = v10;
            best = obj;
        }
    }
    if (best) {
        CVector3__Cross(&v40, &v30, &v50);
        CVector3__Normalize(&v40);
        t = Math_AcosLookup(thr);
        t *= func_00298470(func_00298470(1.0f - t / (new_var2 * 0.008726647f)));
        t *= 0.5f;
        s = func_002982F0(t);
        gQuaternionSin = s;
        q80.x = v40.x * s;
        q80.y = v40.y * s;
        q80.z = v40.z * s;
        new_var = t;
        q80.w = func_002974C0(new_var);
        CQuaternion__Multiply(&q90, &q70, &q80);
        if (outq) {
            *outq = q80;
        }
        *out = q90;
        return out;
    }
    if (outq) {
        outq->x = outq->y = outq->z = 0.0f;
        outq->w = 1.0f;
    }
    *out = q70;
    return out;
}

/* Start a 4-step chain on `b` from entry `idx` (following each entry's next
 * link), unless the entry is flagged 3, and give `b` a direction: v1 relative
 * to c's position, made perpendicular to v2. */

typedef struct {
    u8 pad0[0x12];
    s8 b12;             /* 0x12 */
    u8 pad13;
    s8 b14[4];          /* 0x14 */
    CVector3 v18;          /* 0x18 */
    u8 pad24[0x10];
    s8 b34;             /* 0x34 */
} B10Pb;
typedef struct {
    u8 pad0[0x30];
    f32 f30;            /* 0x30 */
    f32 f34;            /* 0x34 */
    f32 f38;            /* 0x38 */
} B10Pc;
typedef struct {
    u8 pad0[0x64];
    s8 b64;             /* 0x64: next entry */
    u8 b65;             /* 0x65 */
} B10Ent2;
void CVector3__Scale(CVector3 *o, CVector3 *a, f32 s);

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__LoadGeometry receives func_0021B39C's own argument
 * registers (pass-through); the host passes them. */
void func_0021B39C(sptr a, B10Pb *b, B10Pc *c, CVector3 v1, CVector3 v2, s32 idx) {
    CVector3 r;
    CVector3 t;
    CVector3 x;
    s32 **n;
    void *node;
    B10Ent2 *e;
    s32 i;
    s32 cur;

    if (b->b34 != 0x3C && b->b12 < 0x17 && (n = CGameObjectInstance__LoadGeometry(a)) != 0) {
        node = CISet__GetBlock(*n, 3);
        e = CISet__GetBlock(CISet__GetBlock(node, idx), 1);
        cur = idx;
        if ((e->b65 & 3) == 3) {
            CCache__Unlock(0, n);
            return;
        }
        for (i = 0; i < 4; i++) {
            b->b14[i] = cur;
            cur = e->b64;
            if (cur != -1) {
                e = CISet__GetBlock(CISet__GetBlock(node, cur), 1);
            }
        }
        CCache__Unlock(0, n);
        b->b12 = 0x19;
        t.x = c->f30;
        t.y = c->f34;
        t.z = c->f38;
        CVector3__Subtract(&v1, &v1, &t);
        CVector3__Cross(&r, &v1, &v2);
        CVector3__Normalize(&r);
        CVector3__Normalize(&v1);
        CVector3__Normalize(&v2);
        CVector3__Cross(&x, &r, &v1);
        CVector3__Scale(&r, &r, x.x * v2.x + x.y * v2.y + x.z * v2.z);
        CVector3__Normalize(&r);
        b->v18 = r;
    }
}

extern void CVector3__Normalize(CVector3 *);
void CMtxF__Copy(void *dst, void *src);
extern void CVector3__Scale(CVector3 *, CVector3 *, f32);
void CMtxF__Translate(void *m, f32 x, f32 y, f32 z);
void CMtxF__SetAxisAngle(void *m, f32 angle, f32 x, f32 y, f32 z);
void CMtxF__MultiplyAffine(void *out, void *a, void *b);

void CGameObjectInstance__WobbleBone(s32 time, s32 flags, f32 *matrix) {
    f32 rotation[16];
    f32 inverse[16];
    CVector3 origin;
    CVector3 axis;
    u32 t;
    f32 angle;
    f32 flip;
    s32 mode;
    t = gFrameCount;
    t += (flags & 0x38) * 50;
    t += (u32)(time & 0xFFFF) / 113;
    if (!(flags & 4)) {
        t += 40;
    }
    axis.x = 0.0f;
    axis.z = 0.0f;
    axis.y = 1.0f;
    mode = flags & 0xC3;
    if (mode == 2) {
        axis.z = 1.0f;
        axis.x = 0.0f;
        angle = sWobbleSineTable.value[t % 120] - 0.5f;
        axis.y = sWobbleSineTable.value[(t + 12) % 120];
    } else if (mode == 0x82) {
        axis.x = 1.0f;
        axis.z = 0.0f;
        angle = sWobbleSineTable.value[t % 120] - 0.5f;
        axis.y = sWobbleSineTable.value[(t + 12) % 120];
        angle *= 2.0f;
    } else if (mode == 0x40) {
        angle = sWobbleSineTable.value[(t + 30) % 120] - 0.5f;
        axis.x = sWobbleSineTable.value[(t + 12) % 120];
        axis.y = sWobbleSineTable.value[(t + 42) % 120];
        axis.z = sWobbleSineTable.value[(t + 110) % 120];
        angle *= 2.0f;
    } else if (mode == 1) {
        angle = sWobbleSineTable.value[(t + 70) % 120] - 0.5f;
        angle *= 0.7f;
    } else {
        return;
    }
    CVector3__Normalize(&axis);
    origin.x = matrix[12];
    origin.y = matrix[13];
    origin.z = matrix[14];
    CMtxF__Copy(inverse, matrix);
    flip = -1.0f;
    CVector3__Scale(&origin, &origin, flip);
    CMtxF__Translate(inverse, origin.x, origin.y, origin.z);
    CMtxF__SetAxisAngle(rotation, angle, axis.x, axis.y, axis.z);
    CMtxF__MultiplyAffine(matrix, inverse, rotation);
    CVector3__Scale(&origin, &origin, flip);
    CMtxF__Translate(matrix, origin.x, origin.y, origin.z);
}

/* float -> u32 as the R4300 does it (cvt.w.s: a negative value wraps). arm64's fcvtzu saturates
 * negatives to 0 (MACOS.md section 9, tools/port/f2ucheck.py); x86-64 happens to wrap. */
#ifdef TARGET_PORT
#define T2_F2U(x) ((u32)(s32)(x))
#else
#define T2_F2U(x) ((u32)(x))
#endif

/* A bone's track references: -1 means "use the default frame". */
typedef struct JointRef {
    /* 0x0 */ s16 pos;
    /* 0x2 */ s16 rot;
} JointRef;
typedef struct DefaultFrame {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s16 rot[4];
} DefaultFrame;
typedef struct KeyLookup {
    /* 0x00 */ JointRef *joints;
    /* 0x04 */ DefaultFrame *frames;
    /* 0x08 */ void *posTable;
    /* 0x0C */ void *rotTable;
    /* 0x10 */ s32 rotFrame0;
    /* 0x14 */ s32 rotFrame1;
    /* 0x18 */ s32 posFrame0;
    /* 0x1C */ s32 posFrame1;
    /* 0x20 */ f32 blend;
} KeyLookup;
typedef struct TwistPart {
    /* 0x00 */ u8 pad0[0x12];
    /* 0x12 */ s8 count;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s8 bones[4];
    /* 0x18 */ CVector3 axis;
} TwistPart;
/* A bone's children. */
typedef struct BoneList {
    /* 0x0 */ s32 pad0;
    /* 0x4 */ u32 count;
    /* 0x8 */ s32 items[1];
} BoneList;
typedef struct PoseObject PoseObject;
typedef struct PoseContext PoseContext;
typedef void (*PoseCallback)(f32 (*)[4], PoseContext *);
#ifdef TARGET_PORT
#include "structs/game_object.h"
struct PoseObject {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PoseObject(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_PoseObject(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_PoseObject(header.m_pCurrentRegion)]; CGameRegion *region; }; /* N64 0x10 */
        struct { u8 _hp_def[T2_HOFF_PoseObject(header.m_pIntelligence)]; s32 *def; }; /* N64 0x14 */
        struct { u8 _hp_height[T2_HOFF_PoseObject(m_CollisionYOffset)]; f32 height; }; /* N64 0x54 */
        struct { u8 _hp_flags[T2_HOFF_PoseObject(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_blend[T2_HOFF_PoseObject(m_Blend)]; f32 blend; }; /* N64 0x104 */
        struct { u8 _hp_part[T2_HOFF_PoseObject(m_AI)]; TwistPart part; }; /* N64 0x140 */
        struct { u8 _hp_callback[T2_HOFF_PoseObject(m_AI.m_pfEffects)]; PoseCallback callback; }; /* N64 0x248 */
    };
};
#else
struct PoseObject {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ CGameRegion *region;
    /* 0x014 */ s32 *def; /* type id first */
    /* 0x018 */ u8 pad18[0x3C];
    /* 0x054 */ f32 height;
    /* 0x058 */ u8 pad58[0x7C];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x2C];
    /* 0x104 */ f32 blend;
    /* 0x108 */ u8 pad108[0x38];
    /* 0x140 */ TwistPart part;
    /* 0x164 */ u8 pad164[0xE4];
    /* 0x248 */ PoseCallback callback;
};
#endif

struct PoseContext {
    /* 0x00 */ PoseObject *obj;
    /* 0x04 */ u32 *out;
    /* 0x08 */ f32 (*bones)[4][4];
    /* 0x0C */ void *anims;
    /* 0x10 */ void *file;
    /* 0x14 */ sptr arg2;
    /* 0x18 */ void *channel;
    /* 0x1C */ void *blendChannel;
    /* 0x20 */ KeyLookup keys;
    /* 0x44 */ KeyLookup blendKeys;
    /* 0x68 */ s32 visible;
    /* 0x6C */ s32 blend;
    /* 0x70 */ s32 moving;
    /* 0x74 */ s32 f74;
    /* 0x78 */ s32 f78;
    /* 0x7C */ s32 f7C;
    /* 0x80 */ s32 f80;
    /* 0x84 */ s32 f84;
    /* 0x88 */ s32 f88;
    /* 0x8C */ u8 *boneData;
    /* 0x90 */ s32 bone;
};
#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* the callbacks read the context through T2PoseContext (structs/pose_host.h) */
_Static_assert(__builtin_offsetof(PoseContext, obj) == __builtin_offsetof(T2PoseContext, obj), "pose ctx");
_Static_assert(__builtin_offsetof(PoseContext, arg2) == __builtin_offsetof(T2PoseContext, info), "pose ctx");
_Static_assert(__builtin_offsetof(PoseContext, keys) == __builtin_offsetof(T2PoseContext, keys), "pose ctx");
_Static_assert(__builtin_offsetof(PoseContext, f88) == __builtin_offsetof(T2PoseContext, f88), "pose ctx");
_Static_assert(__builtin_offsetof(PoseContext, boneData) == __builtin_offsetof(T2PoseContext, boneData), "pose ctx");
_Static_assert(__builtin_offsetof(PoseContext, bone) == __builtin_offsetof(T2PoseContext, bone), "pose ctx");
_Static_assert(sizeof(PoseContext) == sizeof(T2PoseContext), "pose ctx");
#endif

void CQuaternion__Slerp(CQuaternion *out, f32 amount, CQuaternion *a, CQuaternion *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CMtxF__ScaleAxes(void *m, f32 x, f32 y, f32 z);
u32 T2Port_N64Address(const void *p);
f32 CGameRegion__GetCeilingHeight(CGameRegion *region, f32 x, f32 z); /* the ceiling height */

static __inline__ void trackPos(KeyLookup *t, s32 i, CVector3 *out) {
    if (t->joints[i].pos == -1) {
        *out = t->frames[i].pos;
    } else {
        f32 *data = CISet__GetBlock(t->posTable, t->joints[i].pos);
        f32 *a = &data[t->posFrame0];
        f32 *b = &data[t->posFrame1];
        f32 blend = t->blend;

        out->x = a[0] + blend * (b[0] - a[0]);
        out->y = a[1] + blend * (b[1] - a[1]);
        out->z = a[2] + blend * (b[2] - a[2]);
    }
}

static __inline__ void defaultRot(KeyLookup *t, s32 i, CQuaternion *out) {
    DefaultFrame *frame = &t->frames[i];

    out->x = frame->rot[0] * (1.0f / 32767.0f);
    out->y = frame->rot[1] * (1.0f / 32767.0f);
    out->z = frame->rot[2] * (1.0f / 32767.0f);
    out->w = frame->rot[3] * (1.0f / 32767.0f);
}

/* Spherical interpolation between the two keys. */
static __inline__ void slerpRot(KeyLookup *t, s32 i, CQuaternion *out) {
    if (t->joints[i].rot != -1) {
        f32 *data = CISet__GetBlock(t->rotTable, t->joints[i].rot);

        CQuaternion__Slerp(out, t->blend, (CQuaternion *) &data[t->rotFrame0], (CQuaternion *) &data[t->rotFrame1]);
    } else {
        defaultRot(t, i, out);
    }
}

/* Linear interpolation between the two keys. */
static __inline__ void lerpRot(KeyLookup *t, s32 i, CQuaternion *out) {
    if (t->joints[i].rot == -1) {
        defaultRot(t, i, out);
    } else {
        f32 *data = CISet__GetBlock(t->rotTable, t->joints[i].rot);
        f32 *a = &data[t->rotFrame0];
        f32 *b = &data[t->rotFrame1];
        f32 blend = t->blend;

        out->x = a[0] + blend * (b[0] - a[0]);
        out->y = a[1] + blend * (b[1] - a[1]);
        out->z = a[2] + blend * (b[2] - a[2]);
        out->w = a[3] + blend * (b[3] - a[3]);
    }
}

static __inline__ void lerpVec(CVector3 *out, f32 t, CVector3 *a, CVector3 *b) {
    out->x = a->x + t * (b->x - a->x);
    out->y = a->y + t * (b->y - a->y);
    out->z = a->z + t * (b->z - a->z);
}

static __inline__ void quatToMtx(f32 (*m)[4], CQuaternion *q, CVector3 *t) {
    f32 xx = q->x * q->x;
    f32 yy = q->y * q->y;
    f32 zz = q->z * q->z;
    f32 ww = q->w * q->w;
    f32 xy = 2.0f * q->x * q->y;
    f32 wz = 2.0f * q->w * q->z;
    f32 xz = 2.0f * q->x * q->z;
    f32 wy = 2.0f * q->w * q->y;
    f32 yz = 2.0f * q->y * q->z;
    f32 wx = 2.0f * q->w * q->x;

    m[0][0] = ww + xx - yy - zz;
    m[0][1] = xy + wz;
    m[0][2] = xz - wy;
    m[0][3] = 0.0f;
    m[1][0] = xy - wz;
    m[1][1] = ww - xx + yy - zz;
    m[1][2] = yz + wx;
    m[1][3] = 0.0f;
    m[2][0] = xz + wy;
    m[2][1] = yz - wx;
    m[2][2] = ww - xx - yy + zz;
    m[2][3] = 0.0f;
    m[3][0] = t->x;
    m[3][1] = t->y;
    m[3][2] = t->z;
    m[3][3] = 1.0f;
}

static __inline__ void twistPart(TwistPart *part, f32 (*cur)[4], s32 n, s32 i) {
    CVector3 turn;
    CVector3 axis;
    f32 rotm[4][4];
    f32 inv[4][4];
    f32 angle;

    if (i != 0) {
        turn.x = cur[3][0];
        turn.y = cur[3][1];
        turn.z = cur[3][2];
        axis = part->axis;
        angle = gSwingAmplitudes[0x19 - part->count];
        angle *= 0.3f;
        angle *= (n == 5 ? -2.0f : (f32) n);
        CMtxF__Copy(inv, cur);
        CVector3__Scale(&turn, &turn, -1.0f);
        CMtxF__Translate(inv, turn.x, turn.y, turn.z);
        CMtxF__SetAxisAngle(rotm, angle, axis.x, axis.y, axis.z);
        CMtxF__MultiplyAffine(cur, inv, rotm);
        CVector3__Scale(&turn, &turn, -1.0f);
        CMtxF__Translate(cur, turn.x, turn.y, turn.z);
        if (part->bones[0] == i) {
            part->count--;
        }
    }
}

/* guMtxF2L: 16.16 fixed point, integer halves first. */
static __inline__ void packMtx(u32 *out, f32 (*m)[4]) {
    u32 *hi = out;
    u32 *lo = out + 8;
    u32 e1;
    u32 e2;

    e1 = T2_F2U(m[0][0]);
    e2 = T2_F2U(m[0][1]);
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[0][2]);
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[1][0]);
    e2 = T2_F2U(m[1][1]);
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[1][2]);
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[2][0]);
    e2 = T2_F2U(m[2][1]);
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[2][2]);
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[3][0]);
    e2 = T2_F2U(m[3][1]);
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = T2_F2U(m[3][2]);
    e2 = 65536;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
}

void CGameObjectInstance__PoseBone(PoseObject *obj, PoseContext *ctx, s32 bone, f32 (*parent)[4]) {
    void *node;
    u8 *boneData;
    u8 flags;
    f32 (*cur)[4];
    BoneList *children;
    s32 *items;
    u32 count;
    u32 i;
    s32 n;

    node = CISet__GetBlock(ctx->anims, bone);
    boneData = CISet__GetBlock(node, 1);
    sPoseBoneData = boneData;
    flags = boneData[0x65];
    ctx->boneData = boneData;
    ctx->bone = bone;
    sPoseFlags = flags;

    trackPos(&ctx->keys, bone, &sPosePosA);
    if (ctx->moving) {
        slerpRot(&ctx->keys, bone, &sPoseRotA);
    } else {
        lerpRot(&ctx->keys, bone, &sPoseRotA);
    }
    if (ctx->blend) {
        trackPos(&ctx->blendKeys, bone, &sPosePosB);
        if (ctx->moving) {
            slerpRot(&ctx->blendKeys, bone, &sPoseRotB);
        } else {
            lerpRot(&ctx->blendKeys, bone, &sPoseRotB);
        }
        lerpVec(&sPosePos, obj->blend, &sPosePosB, &sPosePosA);
        CQuaternion__Slerp(&sPoseRot, obj->blend, &sPoseRotB, &sPoseRotA);
    } else {
        sPosePos = sPosePosA;
        sPoseRot = sPoseRotA;
    }
    if (bone == 0 && ctx->visible) {
        sPosePos.x = sPosePos.y = 0.0f;
    }
    quatToMtx(sPoseMtx, &sPoseRot, &sPosePos);
    if (obj->callback != NULL) {
        obj->callback(sPoseMtx, ctx);
    }
    cur = ctx->bones[bone];
    CMtxF__MultiplyAffine(cur, sPoseMtx, parent);

    if (bone == 0) {
        switch (sPoseBoneType = *obj->def) {
            case 4:
                obj->height = ((f32 *) obj->def)[0xE];
                break;
            case 1:
            case 11:
                sPoseCollisionHeight = GameObject_GetCollisionHeight(obj);
                obj->height = cur[3][1] * (1.0f / 65536.0f) - obj->pos.y - sPoseCollisionHeight * 0.5f;
                if (obj->height < 0.0f) {
                    obj->height = 0.0f;
                }
                if ((obj->flags & 0x20000000) && (sPoseRegion = obj->region) != NULL
                    && (sPoseRegion->m_wFlags & REGION_FLAG_CEILING)) {
                    sPoseCeilingHeight = CGameRegion__GetCeilingHeight(sPoseRegion, obj->pos.x, obj->pos.z);
                    sPoseCeilingOverlap = obj->pos.y + obj->height + sPoseCollisionHeight - sPoseCeilingHeight;
                    if (sPoseCeilingOverlap > 0.0f) {
                        cur[3][1] -= sPoseCeilingOverlap * 65536.0f;
                        obj->height -= sPoseCeilingOverlap;
                    }
                }
                break;
            default:
                obj->height = 0.0f;
                break;
        }
    }

    if (obj->part.count) {
        n = 0;
        if (obj->part.bones[0] == bone) {
            n = 4;
        }
        if (obj->part.bones[1] == bone) {
            n = 3;
        }
        if (obj->part.bones[2] == bone) {
            n = 2;
        }
        if (obj->part.bones[3] == bone) {
            n = 1;
        }
        if (n != 0 && obj->callback != NULL) {
            twistPart(&obj->part, cur, n, bone);
        }
    }
    if (obj->flags & 0x800000) {
#ifdef TARGET_PORT
        CGameObjectInstance__WobbleBone((s32) T2Port_N64Address(obj), sPoseFlags, cur);
#else
        CGameObjectInstance__WobbleBone(obj, sPoseFlags, cur);
#endif
    }

    if (ctx->f78 && (sPoseFlags & 0xC7) == 0xC3) {
        ctx->f78 = 0;
        CMtxF__ScaleAxes(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f7C && (sPoseFlags & 0xC7) == 0xC7) {
        ctx->f7C = 0;
        CMtxF__ScaleAxes(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f80 && (sPoseFlags & 0xC7) == 0xC2) {
        ctx->f80 = 0;
        CMtxF__ScaleAxes(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f84 && (sPoseFlags & 0xC7) == 0xC6) {
        ctx->f84 = 0;
        CMtxF__ScaleAxes(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f74 && (sPoseFlags & 3) == 1) {
        ctx->f74 = 0;
        CMtxF__ScaleAxes(cur, 3.0f, 3.0f, 3.0f);
    }

    if (ctx->f88) {
        CMtxF__Copy(sPoseMtx, cur);
        CMtxF__ScaleAxes(sPoseMtx, 0.5f, 0.5f, 0.5f);
        packMtx(ctx->out + bone * 16, sPoseMtx);
    } else {
        packMtx(ctx->out + bone * 16, cur);
    }

    children = CISet__GetBlock(node, 0);
    count = children->count;
    sPoseChildren = children;
    items = children->items;
    for (i = 0; i < count; i++) {
        CGameObjectInstance__PoseBone(obj, ctx, items[i], cur);
    }
}

/* Builds an animated object's bone matrices: binds its animation index,
 * loads the current (and, when blending, the next) animation into the two
 * channels, finds their keyframes, fills a draw context and has
 * CGameObjectInstance__PoseBone pose the skeleton from the object's matrix (scaled to the
 * fixed-point 65536 units), timing that call in a profile slot. Then draws
 * the object's attachments. Returns whether the matrices were built. */

typedef struct AnimChannel {
    /* 0x00 */ f32 time;
    /* 0x04 */ s16 current;
    /* 0x06 */ s16 next;
    /* 0x08 */ s16 frames;
    /* 0x0A */ s8 active;
    /* 0x0B */ s8 blending;
    /* 0x0C */ u32 loopStart;
    /* 0x10 */ void *resource; /* CAnimationState.pmeAnimData */
} AnimChannel;
typedef struct AnimKeyLookup {
    /* 0x00 */ void *posKeys;
    /* 0x04 */ void *rotKeys;
    /* 0x08 */ void *posTracks;
    /* 0x0C */ void *rotTracks;
    /* 0x10 */ s32 offset0;
    /* 0x14 */ s32 offset1;
    /* 0x18 */ s32 rootOffset0;
    /* 0x1C */ s32 rootOffset1;
    /* 0x20 */ f32 frac;
} AnimKeyLookup;
#ifdef TARGET_PORT
typedef struct AnimObject {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AnimObject(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_def[T2_HOFF_AnimObject(header.m_pIntelligence)]; s32 *def; }; /* N64 0x14 */
        struct { u8 _hp_mtx[T2_HOFF_AnimObject(m_mfOrient)]; f32 mtx[4][4]; }; /* N64 0x58 */
        struct { u8 _hp_animIndex[T2_HOFF_AnimObject(m_pmtDrawMtxs)]; sptr animIndex; }; /* N64 0x98: the MtxPool block, a pointer */
        struct { u8 _hp_animFile[T2_HOFF_AnimObject(m_rpAnims)]; sptr animFile; }; /* N64 0xA8: a ROM address */
        struct { u8 _hp_animCount[T2_HOFF_AnimObject(m_nAnims)]; s16 animCount; }; /* N64 0xBA */
        struct { u8 _hp_flags[T2_HOFF_AnimObject(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_channel[T2_HOFF_AnimObject(m_CurrentAnimation)]; AnimChannel channel; }; /* N64 0xD8 */
        struct { u8 _hp_blendChannel[T2_HOFF_AnimObject(m_BlendAnimation)]; AnimChannel blendChannel; }; /* N64 0xEC */
        struct { u8 _hp_hasEffects[T2_HOFF_AnimObject(m_ActiveSwooshes)]; u8 hasEffects; }; /* N64 0x10F */
        struct { u8 _hp_attachMask[T2_HOFF_AnimObject(m_AI.m_VisBits)]; u32 attachMask; }; /* N64 0x14C */
        struct { u8 _hp_owner[T2_HOFF_AnimObject(m_AI.m_pParent)]; struct AnimObject *owner; }; /* N64 0x1A8 */
        struct { u8 _hp_hasAttachments[T2_HOFF_AnimObject(m_dwHotPointFlags)]; s32 hasAttachments; }; /* N64 0x27C */
    };
} AnimObject;
#else
typedef struct AnimObject {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ s32 *def;           /* type id first */
    /* 0x018 */ u8 pad18[0x40];
    /* 0x058 */ f32 mtx[4][4];
    /* 0x098 */ s32 animIndex;
    /* 0x09C */ u8 pad9C[0xC];
    /* 0x0A8 */ s32 animFile;
    /* 0x0AC */ u8 padAC[0xE];
    /* 0x0BA */ s16 animCount;
    /* 0x0BC */ u8 padBC[0x18];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ AnimChannel channel;
    /* 0x0EC */ AnimChannel blendChannel;
    /* 0x100 */ u8 pad100[0xF];
    /* 0x10F */ u8 hasEffects;
    /* 0x110 */ u8 pad110[0x3C];
    /* 0x14C */ u32 attachMask;
    /* 0x150 */ u8 pad150[0x58];
    /* 0x1A8 */ struct AnimObject *owner;
    /* 0x1AC */ u8 pad1AC[0xD0];
    /* 0x27C */ s32 hasAttachments;
} AnimObject;
#endif

/* What CGameObjectInstance__PoseBone reads to pose the skeleton. */
typedef struct AnimPoseContext {
    /* 0x00 */ AnimObject *obj;
    /* the bone matrices (PoseBone's `out`): pointer-wide on the host */
    /* 0x04 */ sptr animIndex;
    /* 0x08 */ void *bones;
    /* 0x0C */ s32 *anims;
    /* 0x10 */ s32 *file;
    /* 0x14 */ sptr arg2;
    /* 0x18 */ AnimChannel *channel;
    /* 0x1C */ AnimChannel *blendChannel;
    /* 0x20 */ AnimKeyLookup keys;
    /* 0x44 */ AnimKeyLookup blendKeys;
    /* 0x68 */ s32 visible;
    /* 0x6C */ s32 blend;
    /* 0x70 */ s32 moving;
    /* 0x74 */ s32 f74;
    /* 0x78 */ s32 f78;
    /* 0x7C */ s32 f7C;
    /* 0x80 */ s32 f80;
    /* 0x84 */ s32 f84;
    /* 0x88 */ s32 f88;
    /* as CGameObjectInstance__PoseBone's view (func_0021B95C.c) */
    /* 0x8C */ u8 *boneData;
    /* 0x90 */ s32 bone;
    /* 0x94 */ u8 pad94[4];
} AnimPoseContext;
/* A profile slot (osGetCount timings). */
typedef struct Profile {
    /* 0x00 */ u64 start;
    /* 0x08 */ u8 pad8[8];
    /* 0x10 */ s32 frame;
    /* 0x14 */ s32 lastCalls;
    /* 0x18 */ s32 calls;
    /* 0x1C */ u8 pad1C[4];
    /* 0x20 */ f32 last;
    /* 0x24 */ f32 usec;
    /* 0x28 */ f32 average;
} Profile;
/* "animations index", CGameObjectInstance__AdvanceAnimation's cache name (func_0021A7EC.c), 4 bytes past
 * sAlignToGroundMax (CGameObjectInstance__AlignToGround's 1.0f, a float elsewhere). */
extern char sAnimIndexName_view[] __asm__("sAlignToGroundMax");
extern Profile sBoneMatrixProfile;
extern s32 sBoneMatrixCount[]; /* &sBoneMatrixProfile.calls */
s32 Cinema_PathPlaying(void);
void *MtxPool_Alloc(void *pool, s32 count);
void CAnimationState__FindKeys(AnimChannel *channel, AnimKeyLookup *keys);
void SwooshSystem_AddBonePoints(void *effects, AnimObject *obj, void *bones);
void GameObject_EmitHotPoints(void *list, s32 count, void *bones, void *obj, void *anims);

/* OS_CYCLES_TO_USEC */
static __inline__ void addMicroseconds(f32 *usec, u32 cycles) {
    f32 *dst = usec;
    *dst = *usec + (f32) ((u64) cycles * 64 / 3000);
}

s32 CGameObjectInstance__BuildBoneMatrices(AnimObject *obj, s32 *file, sptr arg2) {
    AnimPoseContext ctx;
    u8 bones[0x2000];
    f32 mtx[4][4];
    s32 *anims;
    sptr *handle; /* *handle: the animation table's data, a pointer */
    s32 ok;
    s32 hidden;
    s32 isType9;
    s32 inCinema;
    s32 type;
    AnimChannel *channel;
    AnimChannel *blendChannel;
    Profile *prof;
    u32 elapsed;
    s32 *lists;
    s32 *count;
    s32 n;
    s32 i;
    s32 *entry;
    s32 len;
    s32 *data;

    anims = CISet__GetBlock(file, 3);
    obj->animIndex = (sptr) MtxPool_Alloc(gScene.m_Heap, *anims);
    if (obj->animIndex == 0) {
        return 0;
    }
    ok = 0;
    hidden = obj->flags & 1;
    isType9 = *obj->def == 9;
    inCinema = 0;
    handle = (void *) CCache__Request(0, obj->animFile, ((obj->animCount * 4) + 0xF) & ~7, 4, 0, 0, sAnimIndexName_view + 4, 0);
    if (handle != NULL) {
        channel = &obj->channel;
        blendChannel = &obj->blendChannel;
        CAnimationState__SwitchToQueued(channel, *handle, obj->animFile, 0, 0, 0);
        if (CAnimationState__IsLoaded(channel)) {
            if ((obj->flags & 0x400)
                && (CAnimationState__SwitchToQueued(blendChannel, *handle, obj->animFile, 0, 0, 0), CAnimationState__IsLoaded(blendChannel))) {
                ctx.blend = 1;
            } else {
                ctx.blend = 0;
            }
            ctx.obj = obj;
            ctx.animIndex = obj->animIndex;
            ctx.bones = bones;
            ctx.anims = anims;
            ctx.arg2 = arg2;
            ctx.channel = channel;
            ctx.blendChannel = blendChannel;
            ctx.file = file;
            CAnimationState__FindKeys(&obj->channel, &ctx.keys);
            CAnimationState__FindKeys(&obj->blendChannel, &ctx.blendKeys);
            ctx.visible = (!hidden || isType9) && !inCinema;
            type = *obj->def;
            if (obj->flags & 0x300000) {
                if (obj == obj->owner) {
                    ctx.visible = 1;
                } else {
                    ctx.visible = 0;
                }
                if (Cinema_PathPlaying()) {
                    ctx.visible = 0;
                }
            }
            if (!Cinema_PathPlaying()) {
                if ((channel->blending || blendChannel->blending) && type != 1) {
                    ctx.moving = 1;
                } else {
                    ctx.moving = 0;
                }
            } else {
                ctx.moving = 1;
            }
            if (!(obj->flags & 0x200000) && (type == 1 || type == 11)) {
                if (gEngineApp_m_Settings.cheatFlags & 0x100) {
                    ctx.f78 = 1;
                    ctx.f7C = 1;
                    ctx.f80 = 1;
                    ctx.f84 = 1;
                } else {
                    ctx.f78 = 0;
                    ctx.f7C = 0;
                    ctx.f80 = 0;
                    ctx.f84 = 0;
                }
                if (gEngineApp_m_Settings.cheatFlags & 0x20) {
                    ctx.f74 = 1;
                } else {
                    ctx.f74 = 0;
                }
                if (gEngineApp_m_Settings.cheatFlags & 0x80) {
                    ctx.f88 = 1;
                } else {
                    ctx.f88 = 0;
                }
            } else {
                ctx.f78 = 0;
                ctx.f7C = 0;
                ctx.f80 = 0;
                ctx.f84 = 0;
                ctx.f74 = 0;
                ctx.f88 = 0;
            }
            mtx[0][0] = obj->mtx[0][0] * 65536.0f;
            mtx[1][0] = obj->mtx[1][0] * 65536.0f;
            mtx[2][0] = obj->mtx[2][0] * 65536.0f;
            mtx[3][0] = obj->mtx[3][0] * 65536.0f;
            mtx[0][1] = obj->mtx[0][1] * 65536.0f;
            mtx[1][1] = obj->mtx[1][1] * 65536.0f;
            mtx[2][1] = obj->mtx[2][1] * 65536.0f;
            mtx[3][1] = obj->mtx[3][1] * 65536.0f;
            mtx[0][2] = obj->mtx[0][2] * 65536.0f;
            mtx[1][2] = obj->mtx[1][2] * 65536.0f;
            mtx[2][2] = obj->mtx[2][2] * 65536.0f;
            mtx[3][2] = obj->mtx[3][2] * 65536.0f;
            mtx[0][3] = mtx[1][3] = mtx[2][3] = 0.0f;
            mtx[3][3] = 65536.0f;

            prof = &sBoneMatrixProfile;
            prof->start = osGetCount();
            CGameObjectInstance__PoseBone(obj, &ctx, 0, mtx);
            elapsed = osGetCount() - (u32) prof->start;
            if (gFrameCount != prof->frame) {
                prof->frame = gFrameCount;
                prof->average = prof->average * 0.97f + prof->usec * 0.03f;
                prof->last = prof->usec;
                prof->lastCalls = prof->calls;
                prof->usec = 0.0f;
                prof->calls = 0;
            }
            do { /* a profiling macro: its own block */
                addMicroseconds(&prof->usec, elapsed);
                sBoneMatrixCount[0]++;
            } while (0);

            if (obj->hasAttachments) {
                count = CISet__GetBlock(file, 2);
                lists = CISet__GetBlock(file, 4);
                n = *count;
                for (i = 0; i < n; i++) {
                    if (obj->attachMask & (1 << i)) {
                        entry = CISet__GetBlock(lists, i);
                        len = entry[1];
                        if (len != 0) {
                            data = entry + 2;
                            GameObject_EmitHotPoints(data, len, bones, obj, CISet__GetBlock(file, 3));
                        }
                    }
                }
            }
            if (obj->hasEffects) {
                SwooshSystem_AddBonePoints(&gEngineApp_m_SwooshSystem, obj, bones);
            }
            if (ctx.blend) {
                CAnimationState__Release(blendChannel);
            }
            CAnimationState__Release(channel);
            ok = 1;
        }
        CCache__Unlock(0, handle);
    }
    if (!ok) {
        obj->animIndex = 0;
    }
    return ok;
}

/* Draw pass for one actor in one view: if the actor is drawable (flags
 * 0x40000 and a draw callback) and active (0x2000), load its "anim object
 * info", decide whether it is in the view (bounding box against the view
 * frustum, or a box grown by 512 units for actors casting shadows,
 * flag 0x10000000), make sure its model is loaded (CGameObjectInstance__BuildBoneMatrices, playing its
 * spawn effect once, flag 0x200) and call its draw callback.
 *
 * The do { } while (0) blocks (macro bodies in the original, most likely)
 * count as loops for GCC's register priorities (refs weighted by loop
 * depth): they put anim before inView and model before view, as in the ROM. */

typedef struct DrawActor DrawActor;
#ifdef TARGET_PORT
#include "structs/game_object.h"
struct DrawActor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Actor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_Actor(header.m_pCurrentRegion)]; CGameRegion *region; }; /* N64 0x10 */
        struct { u8 _hp_height[T2_HOFF_Actor(m_CollisionYOffset)]; f32 height; }; /* N64 0x54 */
        struct { u8 _hp_matrix[T2_HOFF_Actor(m_mfOrient)]; u8 matrix[0x40]; }; /* N64 0x58 */
        struct { u8 _hp_animFile[T2_HOFF_Actor(m_rpObject)]; sptr animFile; }; /* a ROM address, pointer-wide */ /* N64 0xA4 */
        struct { u8 _hp_animIndex[T2_HOFF_Actor(m_rpObjectHeader)]; s32 animIndex; }; /* N64 0xB0 */
        struct { u8 _hp_boxMin[T2_HOFF_Actor(m_vMin)]; f32 boxMin[3]; }; /* N64 0xBC */
        struct { u8 _hp_boxMax[T2_HOFF_Actor(m_vMax)]; f32 boxMax[3]; }; /* N64 0xC8 */
        struct { u8 _hp_flags[T2_HOFF_Actor(m_dwFlags)]; s32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_lights[T2_HOFF_Actor(m_Lights)]; u8 lights[2][0x18]; }; /* N64 0x110 (CLights1[2]) */
        struct { u8 _hp_ai[T2_HOFF_Actor(m_AI)]; u8 ai[0xF4]; }; /* N64 0x140 */
        struct { u8 _hp_draw[T2_HOFF_Actor(m_AI.m_pfDraw)]; void (*draw)(DrawActor *, void *, CDrawArgs *); }; /* N64 0x234 */
        struct { u8 _hp_update[T2_HOFF_Actor(m_AI.m_pfCalculateOrientationMatrix)]; void (*update)(DrawActor *, void *); }; /* N64 0x244 */
    };
};
#else
struct DrawActor {
    u8 pad0[0x4];
    CVector3 pos;               /* 0x04 */
    CGameRegion *region;         /* 0x10 */
    u8 pad14[0x40];
    f32 height;             /* 0x54 */
    u8 matrix[0x4C];        /* 0x58 */
    s32 animFile;           /* 0xA4 */
    u8 padA8[0x8];
    s32 animIndex;          /* 0xB0 */
    u8 padB4[0x8];
    f32 boxMin[3];          /* 0xBC */
    f32 boxMax[3];          /* 0xC8 */
    s32 flags;              /* 0xD4 */
    u8 padD8[0x38];
    u8 lights[2][0x18];     /* 0x110 */
    u8 ai[0xF4];            /* 0x140 */
    void (*draw)(DrawActor *, void *, CDrawArgs *);  /* 0x234 */
    u8 pad238[0xC];
    void (*update)(DrawActor *, void *);            /* 0x244 */
};
#endif

s32 Frustum_TestBox(void *, f32 *);

void CGameObjectInstance__Draw(DrawActor *actor, CCamera *view) {
    f32 offset[3];
    f32 box[6];
    CVector3 pos;
    CDrawArgs args;
    s32 flags;
    void **anim;
    void **model;
    sptr info;
    s32 inView;
    s32 near;
    f32 y;

    flags = actor->flags;
    if (!(flags & 0x40000) || actor->draw == NULL) {
        return;
    }
    if (!(flags & 0x2000)) {
        actor->flags = flags & ~0x200;
        return;
    }
    do {
        anim = CCache__Request(0, actor->animFile, actor->animIndex, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (anim == NULL) {
            return;
        }
    } while (0);
    info = CISet__GetBlock(*anim, 0);
    if (gEngineApp_m_Settings.paused != 0) {
        actor->update(actor, actor->ai);
        CMtxF__TransformBox(actor->matrix, (void *) info, actor->boxMin);
    }
    if (!(actor->flags & 0x300000)) {
        inView = 0;
        near = 0;
        if (actor->region == NULL || (view->m_ViewParams.m_VisBits & actor->region->m_VisBits)) {
            if (view->m_ViewMax.x > actor->boxMin[0] && view->m_ViewMin.x < actor->boxMax[0]
                && view->m_ViewMax.z > actor->boxMin[2] && view->m_ViewMin.z < actor->boxMax[2]
                && view->m_ViewMax.y > actor->boxMin[1] && view->m_ViewMin.y < actor->boxMax[1]
                && Frustum_TestBox(view->m_ViewVolume, actor->boxMin)) {
                inView = 1;
                if (actor->region != NULL) {
                    near = 1;
                }
            }
            if (!near && actor->region != NULL && (actor->flags & 0x10000000)) {
                y = GameObject_GetRegionFloorHeight(actor, 0);
                box[0] = actor->boxMin[0] - 512.0f;
                box[1] = y - 0.5f;
                box[2] = actor->boxMin[2] - 512.0f;
                box[3] = actor->boxMax[0] + 512.0f;
                box[5] = actor->boxMax[2] + 512.0f;
                box[4] = y + 0.5f;
                if (box[0] < view->m_ViewMax.x && view->m_ViewMin.x < box[3]
                    && box[2] < view->m_ViewMax.z && view->m_ViewMin.z < box[5]
                    && box[1] < view->m_ViewMax.y && view->m_ViewMin.y < box[4]
                    && Frustum_TestBox(view->m_ViewVolume, box)) {
                set_near:
                    near = 1;
                }
            }
        }
    } else {
        inView = 1;
        goto set_near;
    }
    if (inView || near) {
        do {
            model = CGameObjectInstance__LoadGeometry(actor);
        } while (0);
        if (model != NULL) {
            if (!(actor->flags & 0x200)) {
                if (!CGameObjectInstance__BuildBoneMatrices(actor, *model, info)) {
                    goto free_model;
                }
                if (!(actor->flags & 8)) {
                    pos = actor->pos;
                    pos.y += actor->height;
                    pos.y += GameObject_GetCollisionHeight(actor) * 0.5f;
                    CScene__GetLightsAt(&gScene, &pos, actor->lights[gEvenOdd]);
                }
                actor->flags |= 0x200;
            }
            args.m_bDraw = inView;
            args.m_bShadow = near;
            args.m_pCamera = view;
            args.m_hModel = (sptr)model;
            args.m_pOffset = offset;
            args.m_nVariant = 0;
            args.m_nModel = 0;
            args.m_nShadowModel = 0;
            offset[0] = offset[1] = offset[2] = 0.0f;
            actor->draw(actor, actor->ai, &args);
        free_model:
            CCache__Unlock(0, model);
        }
    }
    do {
        CCache__Unlock(0, anim);
    } while (0);
}

void Render_QueueModelInstance(sptr model, sptr matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass);

static __inline__ void pickSets(CGameObjectInstance *actor, s32 index, s32 *set0, s32 *set1) {
    CIntelligence *intel = actor->header.m_pIntelligence;

    if (intel->m_Type == 1) {
        s32 state;

        if (actor->m_AI.m_Mode == 0x3C) {
            state = actor->m_AI.m_ModeBefore;
        } else {
            state = actor->m_AI.m_Mode;
        }
        switch (state) {
            case 0x36:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[0];
                break;
            case 0x37:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[1];
                break;
            case 0x38:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[2];
                break;
            case 0x39:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[3];
                break;
            case 0x3a:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[4];
                break;
            case 0x3b:
                *set0 = *set1 = intel->m_Enemy.m_DeathModels[5];
                break;
            default:
                *set0 = (&intel->m_nModel)[index];
                *set1 = (&intel->m_nShadowModel)[index];
                break;
        }
    } else {
        *set0 = (&intel->m_nModel)[index];
        *set1 = (&intel->m_nShadowModel)[index];
    }
}
/* Draw an actor's shadow. Without flag 0x10000000 it gets the blob shadow
 * (Shadow_DrawBlob). Otherwise its projected shadow model is built once, while
 * above the ground: the current view's light direction (signed bytes) and
 * strength (colour) are sampled at the actor (DLights_GetAtPoint), the alpha is
 * 128 + 56 per unit of light (faded by gBlendFade during a fade), the
 * direction is kept at least 0.15 from horizontal and the model is projected
 * along it onto a plane just under the actor (0.9 of its height, up to 512),
 * grown by 0.003 per unit of height, turned like the actor and set on the
 * ground; then it is submitted (CGameObjectInstance__SubmitShadowModel). */

#ifdef TARGET_PORT
#include "structs/model.h"
/* CDrawArgs.m_pOffset at its host offset */
typedef struct {
    u8 pad0[__builtin_offsetof(CDrawArgs, m_pOffset)];
    sptr blobType;
} ShadowParams;
#else
typedef struct {
    u8 pad0[0x10];
    s32 blobType;           /* 0x10 */
} ShadowParams;
#endif
void CGameObjectInstance__DrawShadow(CGameObjectInstance *a, sptr unused, ShadowParams *s);

void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, sptr arg1, CDrawArgs *params) {
    pickSets(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw != 0) {
        Render_QueueModelInstance(params->m_hModel, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, params->m_nModel, actor->m_AI.m_nTexture);
    }
}

void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, sptr arg1, CDrawArgs *params) {
    pickSets(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw != 0) {
        Render_DrawModel(params->m_hModel, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, params->m_nModel, actor->m_AI.m_nTexture);
    }
    gBlendState = 0;
}

void Model_PulseVertexColors(char *, char *);
void CGameObjectInstance__TouchGeometry(CGameObjectInstance *arg0);
void func_0021F418(void);
void func_0021F420(void);
typedef struct RootChannel {
    f32 time;
    s16 current,next,frames;
    u8 active,blending;
    char padc[4]; void *resource;
} RootChannel;
typedef struct RootTriple {
    s32 x;
    s32 y;
    s32 z;
} RootTriple;
void *CGameObjectInstance__GetRootOffset(RootTriple *arg0, void *arg1, RootChannel *arg2);
void CGameObjectInstance__SendAnimImpactEvents(CGameObjectInstance *a, s32 b);
s32 func_0021F5B8(CGameObjectInstance *obj, s32 arg1);
s32 func_0021F660(void);
void CGameObjectInstance__ApplyRootMotion(CGameObjectInstance *a, f32 *p);
void *CGameObjectInstance__GetBodySetup(char *arg0);
typedef struct Swing {
    u8 pad0[0x12];
    s8 step;                    /* 0x12: counts down */
    u8 pad13;
    s8 lastStep;                /* 0x14 */
    u8 pad15[0x18 - 0x15];
    CVector3 axis;                  /* 0x18 */
} Swing;
void CGameObjectInstance__SwingBone(s32 arg0, Swing *swing, f32 *matrix, s32 active, s32 dir);
void Anim_LerpVector(void *arg0, f32 arg1, void *arg2, void *arg3);
void Anim_LerpQuaternion(void *arg0, f32 t, void *a, void *b);
void Anim_GetBoneTranslationLerp(void *arg0, s32 arg1, void *arg2);
void Anim_GetBoneRotationLerp(void *arg0, s32 arg1, Vector4f *arg2);
void Anim_GetBoneRotationSlerp(ObjectLinks24_2 *arg0, s32 arg1, Vector4f *arg2);
void CMtxF__ToMtxAffineUnscaled(f32 *src, PackedMatrixWords *dst);
void Quaternion_ToMatrix(f32 *m, f32 *q, f32 *t);
void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1);

void CGameObjectInstance__DrawModelLitVertices(CGameObjectInstance *actor, s32 arg1, CDrawArgs *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    pickSets(actor, params->m_nVariant, &params->m_nModel, &params->m_nShadowModel);
    actor->m_AI.m_VisBits = 1 << params->m_nModel;
    if (params->m_bShadow != 0) {
        CGameObjectInstance__DrawShadow(actor, arg1, params);
    }
    if (params->m_bDraw == 0) {
        return;
    }
    resource = CCache__RequestDecompression(0, actor->m_dwUniqueID | gLitVerticesKey, params->m_hModel, 0, 0, actor,
                             CGameObjectInstance__ConstructLitVertices, "lit vertices", 1);
    if (resource == NULL) {
        return;
    }
    CCache__LockForDisplayList(0, resource);
    header = *resource;
    if (header[0] == 1) {
        object = (char *) header + header[1];
        Model_PulseVertexColors(object, (char *) header + header[2]);
        data = object;
    } else {
        data = (char *) (header + 2);
    }
    Render_QueueModelInstance(params->m_hModel, ((sptr)actor->m_pmtDrawMtxs), 1, 0, data, params->m_nModel, actor->m_AI.m_nTexture);
    CCache__Unlock(0, resource);
}

/* on MIPS the instance reaches it in $a0 (the caller's first argument, untouched) */
s32 GameObject_CastsShadow(CGameObjectInstance *);
#define GameObject_CastsShadow() GameObject_CastsShadow(a)

f32 func_00298470(f32);     /* sqrtf */
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define DLights_GetAtPoint DLights_GetAtPoint_f3
#endif

void DLights_GetAtPoint(void *, f32, f32, f32, CVector3 *, f32 *);
void CVector3__Normalize(CVector3 *);
void CMtxF__SetShadowProjection(void *, CVector3 *, f32);
void CMtxF__ScaleColumns(void *, f32, f32, f32);
void Shadow_DrawBlob(); /* (ShadowActor *, CVector3 *offset, s32), func_0021E03C.c: the blob type travels as the offset */
#ifdef TARGET_PORT
#include "structs/camera.h"
#include "structs/model.h"
/* Model is the view's CCamera (m_mfProjection, m_mProjection[2]) and ModelRef the
 * CDrawArgs: their host members (the N64 offsets put the shadow's projection, and the
 * one restored after it, on the wrong bytes: every actor drawn after a shadow got it) */
typedef struct Model {
    union {
        struct { u8 _hp_unk1CC[__builtin_offsetof(CCamera, m_mfProjection)]; u8 unk1CC[0x40]; };
        struct { u8 _hp_mtx[__builtin_offsetof(CCamera, m_mProjection)]; Mtx mtx[2]; };
    };
} Model;

typedef struct ModelRef {
    union {
        struct { u8 _hp_model[__builtin_offsetof(CDrawArgs, m_pCamera)]; Model *model; };
        struct { u8 _hp_unkC[__builtin_offsetof(CDrawArgs, m_hModel)]; sptr unkC; };
        struct { u8 _hp_unk1C[__builtin_offsetof(CDrawArgs, m_nShadowModel)]; s32 unk1C; };
    };
} ModelRef;
#else
typedef struct Model {
    u8 pad0[0x1CC];
    u8 unk1CC[0x368 - 0x1CC];   /* 0x1CC */
    Mtx mtx[2];                 /* 0x368: one per frame buffer */
} Model;

typedef struct ModelRef {
    u8 pad0[0x8];
    Model *model;               /* 0x08 */
    s32 unkC;                   /* 0x0C */
    u8 pad10[0x1C - 0x10];
    s32 unk1C;                  /* 0x1C */
} ModelRef;
#endif
void CGameObjectInstance__SubmitShadowModel(CGameObjectInstance *owner, ModelRef *ref, void *arg2, s32 arg3);

void CGameObjectInstance__DrawShadow(CGameObjectInstance *a, sptr unused, ShadowParams *s) {
    CVector3 dir;
    f32 r;
    u8 m[0x40];
    u8 q[0x40];
    f32 dy;
    f32 h;
    f32 ty;
    f32 t;
    f32 u2;
    s32 v;
    CLights1 *light;
    register f32 py __asm__("$f1");

    if (a->header.m_pCurrentRegion != 0 && GameObject_CastsShadow() != 0 && !(a->m_dwFlags & 0x200000)) {
        if (a->m_dwFlags & 0x10000000) {
            if (a->m_pmfShadow == NULL) {
                h = GameObject_GetRegionFloorHeight(a, 0);
                py = a->header.m_vPos.y;
                if (py < h) {
                    goto end;
                }
                t = py - h;
                dy = t;
                if (512.0f < t) {
                    dy = 512.0f;
                }
                a->m_pmfShadow = MtxPool_Alloc(gScene.m_Heap, 1);
                if (a->m_pmfShadow == NULL) {
                    goto end;
                }
                light = &a->m_Lights[gEvenOdd];
                dir.x = light->m_Dir[0] * 0.007874016f;
                dir.y = light->m_Dir[1] * 0.007874016f;
                dir.z = light->m_Dir[2] * 0.007874016f;
                t = (f32) light->m_Col[0] * (f32) light->m_Col[0];
                u2 = (f32) light->m_Col[1] * (f32) light->m_Col[1];
                t = t + u2;
                r = func_00298470(t + u2) * -0.00090497744f;
                DLights_GetAtPoint((u8 *) &gScene.m_DLightSet, a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z, &dir, &r);
                if (r < -1.0f) {
                    r = -1.0f;
                }
                v = (s32) (r * 56.0f + 128.0f);
                if (gBlendState == 1) {
                    v = (s32) ((f32) v * gBlendState_m_Color[3] * 0.003921569f);
                }
                a->m_ShadowAlpha = v;
                dir.y = 1.0f;
                CVector3__Normalize(&dir);
                if (dir.y < 0.0f) {
                    if (-dir.y < 0.15f) {
                        goto clampy;
                    }
                } else if (dir.y < 0.15f) {
clampy:
                    if (0.0f <= dir.y) {
                        dir.y = 0.15f;
                    } else {
                        dir.y = -0.15f;
                    }
                    CVector3__Normalize(&dir);
                }
                ty = h + dy * 0.9f;
                CMtxF__SetShadowProjection(m, &dir, ty);
                CMtxF__Translate(m, -a->header.m_vPos.x, -ty, -a->header.m_vPos.z);
                t = dy * 0.003f + 1.0f;
                CMtxF__ScaleColumns(m, t, t, t);
                CQuaternion__ToMatrix(&a->m_qGround, q);
                CMtxF__MultiplyAffine(a->m_pmfShadow, m, q);
                CMtxF__Translate(a->m_pmfShadow, a->header.m_vPos.x, h, a->header.m_vPos.z);
                if (a->m_pmfShadow == NULL) {
                    goto end;
                }
            }
            CGameObjectInstance__SubmitShadowModel(a, s, a->m_pmfShadow, a->m_ShadowAlpha);
        } else {
            Shadow_DrawBlob(a, s->blobType, 0);
        }
    }
end:
    return;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

typedef struct ObjDef {
    u16 pad0;
    u16 flags;                  /* 0x02 */
} ObjDef;
typedef struct ShadowActor {
    u8 pad0[0x4];
    CVector3 pos;                   /* 0x04 */
    ObjDef *def;                /* 0x10 */
    u8 pad14[0x40 - 0x14];
    u8 rot[0x54 - 0x40];        /* 0x40 */
    f32 unk54;                  /* 0x54 */
    u8 pad58[0x10E - 0x58];
    u8 shadowAlpha;             /* 0x10E */
} ShadowActor;
s32 CGameRegion__IsSteep(ObjDef *def);
f32 GameObject_GetGroundHeight(ShadowActor *actor, s32 arg1);    /* ground height */
f32 GameObject_GetWallRadius(ShadowActor *actor);              /* radius */
void CVector3__ClampLength(void *v, f32 length);
void CMtxF__ToMtxAffine(CMtxF *m, void *dst);
void Render_DrawShadowQuad(void *mtx, s32 arg1, s32 alpha);
/* Draws the blob shadow under an actor. */
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 z1): the actor by member of CGameObjectInstance (the N64 view: 0x04 m_vPos,
 * 0x10 the region (its flags at 0x02), 0x40 m_qGround, 0x54 m_CollisionYOffset, 0x10E
 * m_ShadowAlpha). Through the N64 offsets the alpha's read-modify-write landed in byte 2 of
 * m_CurrentAnimation.pmeAnimData: the first-person weapon's blob shadow (CPlayerPool__ReloadAnimInfo)
 * turned a released animation (NULL) into 0x630000 and the next weapon animation switch copied it
 * into the blend channel: CAnimationState__FindKeys crashed (level 135 of the River of Souls). */
void Shadow_DrawBlob(ShadowActor *actor_, CVector3 *offset, s32 arg2) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    ObjDef *def = (ObjDef *) actor->header.m_pCurrentRegion;
    CMtxF mtx;
    CVector3 pos;
    void *gmtx;
    s32 visible;
    f32 ground;
    f32 height;
    f32 size;
    f32 lift;
    f32 a;
    u8 alpha;

    visible = !CGameRegion__IsSteep(def);
    if (def != NULL && (def->flags & 0x4000)) {
        visible = 0;
    }
    if (!visible) {
        return;
    }
    gmtx = MtxPool_Alloc(gScene.m_Heap, 1);
    if (gmtx == NULL) {
        return;
    }
    ground = GameObject_GetGroundHeight(actor_, 0);
    height = actor->header.m_vPos.y + actor->m_CollisionYOffset - ground;
    size = MAX(MIN(GameObject_GetWallRadius(actor_), GameObject_GetWallRadius(actor_) - height * 0.1f), 0.0f);
    if (arg2) {
        lift = 15.36f;
        size *= 0.5f;
    } else {
        lift = 0.0f;
        size *= 0.7f;
    }
    pos.x = actor->header.m_vPos.x + offset->x;
    pos.z = actor->header.m_vPos.z + offset->z;
    pos.y = ground + lift + offset->y;
    CVector3__ClampLength(&pos, 20000.0f);
    CQuaternion__ToMatrix(&actor->m_qGround, &mtx);
    CMtxF__ScaleAxes(&mtx, size, 1.0f, size);
    CMtxF__Translate(&mtx, pos.x, pos.y, pos.z);
    CMtxF__ToMtxAffine(&mtx, gmtx);
    a = 200.0f - height * 1.2207031f;
    alpha = a > 255.0f ? 255 : a < 0.0f ? 0 : a > 255.0f ? 255 : (u8)a;
    actor->m_ShadowAlpha = actor->m_ShadowAlpha + (alpha - actor->m_ShadowAlpha) * 0.5f;
    Render_DrawShadowQuad(gmtx, arg2 == 0, actor->m_ShadowAlpha);
}
#else
void Shadow_DrawBlob(ShadowActor *actor, CVector3 *offset, s32 arg2) {
    CMtxF mtx;
    CVector3 pos;
    void *gmtx;
    s32 visible;
    f32 ground;
    f32 height;
    f32 size;
    f32 lift;
    f32 a;
    u8 alpha;

    visible = !CGameRegion__IsSteep(actor->def);
    if (actor->def != NULL && (actor->def->flags & 0x4000)) {
        visible = 0;
    }
    if (!visible) {
        return;
    }
    gmtx = MtxPool_Alloc(gScene.m_Heap, 1);
    if (gmtx == NULL) {
        return;
    }
    ground = GameObject_GetGroundHeight(actor, 0);
    height = actor->pos.y + actor->unk54 - ground;
    size = MAX(MIN(GameObject_GetWallRadius(actor), GameObject_GetWallRadius(actor) - height * 0.1f), 0.0f);
    if (arg2) {
        lift = 15.36f;
        size *= 0.5f;
    } else {
        lift = 0.0f;
        size *= 0.7f;
    }
    pos.x = actor->pos.x + offset->x;
    pos.z = actor->pos.z + offset->z;
    pos.y = ground + lift + offset->y;
    CVector3__ClampLength(&pos, 20000.0f);
    CQuaternion__ToMatrix(actor->rot, &mtx);
    CMtxF__ScaleAxes(&mtx, size, 1.0f, size);
    CMtxF__Translate(&mtx, pos.x, pos.y, pos.z);
    CMtxF__ToMtxAffine(&mtx, gmtx);
    a = 200.0f - height * 1.2207031f;
    alpha = a > 255.0f ? 255 : a < 0.0f ? 0 : a > 255.0f ? 255 : (u8)a;
    actor->shadowAlpha = actor->shadowAlpha + (alpha - actor->shadowAlpha) * 0.5f;
    Render_DrawShadowQuad(gmtx, arg2 == 0, actor->shadowAlpha);
}
#endif

typedef struct Box {
    CVector3 corners[8];            /* 0x00 */
    u8 pad60[0x68 - 0x60];
    f32 unk68;                  /* 0x68 */
    s32 mask;                   /* 0x6C: parts it belongs to */
} Box;                          /* 0x70 bytes */
void CVector3__Scale(CVector3 *dst, CVector3 *src, f32 scale);
void CVector3__Subtract(CVector3 *dst, CVector3 *a, CVector3 *b);    /* subtract */

/* Copies the actor's bounding box whose center is nearest to pos into out. */
s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *actor, CVector3 pos, Box *out) {
    CVector3 center;
    CVector3 d;
    CMtxF mtx;
    void **handle;
    void *boxes;
    CMtxF *matrices;
    Box *box;
    Box *best;
    s32 count;
    s32 i;
    s32 j;
    f32 dist;
    f32 bestDist;

    matrices = ((CMtxF *)actor->m_pmtDrawMtxs);
    if (matrices == NULL) {
        matrices = ((CMtxF *)actor->m_pmtLastFrameDrawMtxs);
        if (matrices == NULL) {
            return 0;
        }
    }
    handle = CGameObjectInstance__LoadGeometry(actor);
    if (handle == NULL) {
        return 0;
    }
    boxes = CISet__GetBlock(*handle, 3);
    best = NULL;
    count = *(s32 *)boxes;
    bestDist = 3.4028235e38f;
    for (i = 0; i < count; i++) {
        box = CISet__GetBlock(CISet__GetBlock(boxes, i), 1);
        if (!(box->mask & actor->m_AI.m_VisBits)) {
            continue;
        }
        center = box->corners[0];
        for (j = 1; j < 8; j++) {
            CVector3__Add(&center, &center, &box->corners[j]);
        }
        CVector3__Scale(&center, &center, 0.125f);
        CMtxF__FromMtxAffine(&mtx, &matrices[i]);
        CMtxF__TransformPoint(&mtx, &center, &d);
        CVector3__Subtract(&d, &d, &pos);
        dist = d.x * d.x + d.y * d.y + d.z * d.z;
        if (dist < bestDist) {
            best = box;
            bestDist = dist;
        }
    }
    if (best != NULL) {
        *out = *best;
        if ((actor->m_dwFlags & 0x300000) && gEngineApp_m_Settings.multiplayer == 0) {
            out->unk68 = 1.0f;
        }
    }
    CCache__Unlock(0, handle);
    return best != NULL;
}

struct LevelActor;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct TypeEntry;
typedef struct TypeEntry TypeEntry;
struct TypeEntry {
    void *unk0;
    void (*callback)(void *, void *);
};
typedef struct LevelActor LevelActor;
struct LevelActor {
    u8 pad0[0x14];
    CIntelligence *definition; /* 0x14 */
    u8 pad18[0xD4 - 0x18];
    s32 flags;                         /* 0xD4 */
    u8 padD8[0x207 - 0xD8];
    u8 updateCount;                    /* 0x207 */
};
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/ai_type.h"

/* The port: the same on CGameObjectInstance/CPlayer (the LevelActor view and the 0x288/0x1328
 * strides are the N64 layouts): runs the type's init on the actor and its AI, and counts the
 * update for actors with flag 0x08000000. A player's own actor has no type block, its weapon
 * actor gets gWeaponActorTypeInfo. */
void CGameObjectInstance__InitAIType(char *arg0)
{
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
    CIntelligenceTypeInfo *entry;
    s32 type = obj->header.m_pIntelligence->m_Type;
    s32 i;

    entry = NULL;
    if ((u32) type < 13) {
        entry = gIntelligenceTypeInfo[type];
        if (type == 11) {
            for (i = 0; i < gEngineApp_m_Game_m_PlayerList_m_nPlayers; i++) {
                CPlayer *player = &gEngineApp_m_Game_m_PlayerList_m_Players[i];
                if (obj == &player->m_CharacterActor) {
                    entry = NULL;
                    break;
                }
                if (obj == &player->m_WeaponActor) {
                    entry = &gWeaponActorTypeInfo;
                    break;
                }
            }
        }
    }
    if (entry != NULL && entry->init != NULL) {
        entry->init(obj, &obj->m_AI);
    }
    if (obj->m_dwFlags & 0x08000000) {
        obj->m_AI.m_AnimRepeatedCount++;
    }
}
#else
void CGameObjectInstance__InitAIType(char *arg0)
{
    s32 type;
    s32 i;
    s32 count;
    s32 limit;
    char *object;
    TypeEntry *entry;
    type = ((struct LevelActor *)(arg0))->definition->m_Type;
    if ((u32)type < 13) {
        goto valid_type;
    }
zero_entry:
    entry = 0;
    goto selected;
special_entry:
    entry = (TypeEntry *) &gWeaponActorTypeInfo;
    goto selected;
valid_type:
    if (type == 11) {
        count = gEngineApp_m_Game_m_PlayerList_m_nPlayers;
        i = 0;
        if (count > 0) {
            limit = count;
            object = gEngineApp_m_Game_m_PlayerList_m_Players;
            do {
                if (object == arg0) {
                    goto zero_entry;
                }
                if (object + 0x288 == arg0) {
                    goto special_entry;
                }
                i++;
                object += 0x1328;
            } while (i < limit);
        }
    }
    entry = (TypeEntry *) gIntelligenceTypeInfo[type];
selected:
    if (entry != 0 && entry->callback != 0) {
        entry->callback(arg0, (char *)arg0 + 0x140);
    }
    if ((((struct LevelActor *)(arg0))->flags & 0x08000000) != 0) {
        (((struct LevelActor *)(arg0))->updateCount)++;
    }
}
#endif

void Model_TouchTextures(sptr);
extern void Model_DecompressCallbackExtra(void);

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
DEFERRED void * CGameObjectInstance__LoadGeometry(CGameObjectInstance *arg0) {
    return CCache__Request(0, (sptr) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra, "anim geometry", 0);
}

DEFERRED void CGameObjectInstance__TouchGeometry(CGameObjectInstance *arg0) {
    sptr temp_v0;

    temp_v0 = CCache__Request(0, (sptr) arg0->m_rpGeometryHeader, arg0->m_rpGeometryHeaderSize, 0, 0, Model_DecompressCallbackExtra,
                            "anim geometry", 0);
    if (temp_v0 != 0) {
        Model_TouchTextures(temp_v0);
        CCache__Unlock(0, temp_v0);
    }
}

struct CGameObjectInstance;
void CGameObjectInstance__ConstructFromRecord(void *arg0, InputRecAt27 *arg1, s32 mode, s32 index);
/* The call as the ROM makes it (u16 ids, a u8 flag byte, one argument more than the definition,
 * func_00219BC0.c, takes): the definition under its own name on the N64, cast on the port. */
#ifdef TARGET_PORT
void CGameObjectInstance__Construct(struct CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                   s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale, CVector3 velocity);
#define CGameObjectInstance__Construct_rec ((void (*)(void *, u16, u16, s32, void *, s32, sptr, f32, CVector3, u8, \
                                                      CVector3, CVector3, s32)) CGameObjectInstance__Construct)
#else
extern void CGameObjectInstance__Construct_rec(void *, u16, u16, s32, void *, s32, sptr, f32,
                          CVector3, u8, CVector3, CVector3, s32) __asm__("CGameObjectInstance__Construct");
#endif

void CGameObjectInstance__ConstructFromRecord(void *arg0, InputRecAt27 *arg1, s32 mode, s32 index) {
    void *resource0;
    sptr resource1;
    s32 lookup;
    s32 amountRaw;
    f32 fzero;
    f32 amount;
    CVector3 zero;
    resource0 = CScene__GetIntelligence(&gScene, arg1->resource22);
    if (arg1->resource20 == 0xFFFF) {
        resource1 = 0;
    } else {
        resource1 = CScene__GetRegion(&gScene, arg1->resource20);
    }
    amountRaw = arg1->amount24;
    fzero = 0.0f;
    amount = amountRaw * 9.587673e-5f;
    zero.x = zero.y = zero.z = fzero;
    lookup = CScene__GetInstanceSavedBit(&gScene, arg0);
    CGameObjectInstance__Construct_rec(arg0, arg1->unk1C, arg1->unk1E, arg1->unk0,
                  resource0, arg1->unk27 == 0xFF ? -1 : arg1->unk27,
                  resource1, amount, *(CVector3 *)arg1->vec4, arg1->unk26,
                  *(CVector3 *)arg1->vec10, zero, lookup);
}

void CGameObjectInstance__UpdateBounds(CGameObjectInstance *obj, sptr arg1) {
    ((void (*)(void *, void *)) obj->m_AI.m_pfCalculateOrientationMatrix)(obj, &obj->m_AI);
    CMtxF__TransformBox(obj->m_mfOrient, (void *) arg1, &obj->m_vMin);
}

struct OrientActor;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
typedef struct OrientActor OrientActor;
struct OrientActor {               /* Turok 2 layout (Rage Wars offsets in comments) */
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    CVector3 position0;           /* 0x04 (0x08) */
    s32 model;                    /* 0x10 (0x14) */
    CIntelligence *definition; /* 0x14 (0x18) */
    CVector3 position1;           /* 0x18 (0x1C) */
    u8 pad24[0x10];
    CVector3 scale;               /* 0x34 (0x50) */
    CQuatern rotation;            /* 0x40 (0x5C) */
    f32 yaw;                      /* 0x50 (0x6C) */
    f32 eyeHeight;                /* 0x54 (0x70) */
    f32 transform[16];            /* 0x58 (0x74) */
};
extern s32 Cinema_PathPlaying(void);
#ifndef TARGET_PORT
extern void GameObject_GetFloorTilt(CQuatern *arg0, void *arg1);
#endif

extern void CMtxF__SwapYZRows(void *);
#ifdef TARGET_PORT
#include "structs/game_object.h"
CQuaternion GameObject_GetFloorTilt(CGameObjectInstance *obj);

/* Host body on CGameObjectInstance: the OrientActor view has the N64 header size (a 4-byte
 * intelligence pointer), and GameObject_GetFloorTilt returns its quaternion by value (MIPS
 * passes a hidden result pointer, x86-64 returns it in registers). */
void CGameObjectInstance__CalculateOrientationMatrix(void *arg0) {
    CGameObjectInstance *obj = (CGameObjectInstance *)arg0;
    CQuatern trig;
    CQuatern product;
    CQuatern copy;
    f32 *transform;
    f32 sine;

    copy = *(CQuatern *)&obj->m_qGround;
    if (Cinema_PathPlaying() != 0) {
        CQuaternion tilt = GameObject_GetFloorTilt(obj);
        copy = *(CQuatern *)&tilt;
    }
    sine = func_002982F0(obj->m_RotY * 0.5f);
    trig.x = 0.0f;
    trig.y = sine;
    trig.z = 0.0f;
    gQuaternionSin = sine;
    trig.w = func_002974C0(obj->m_RotY * 0.5f);
    CQuaternion__Multiply(&product, &trig, &copy);
    transform = obj->m_mfOrient[0];
    CQuaternion__ToMatrix(&product, transform);
    CMtxF__ScaleAxes(transform, obj->m_vScale.x, obj->m_vScale.y, obj->m_vScale.z);
    CVector3__ClampLength(&obj->header.m_vPos.x, 20000.0f);
    CMtxF__Translate((char *)transform, obj->header.m_vPos.x, obj->header.m_vPos.y, obj->header.m_vPos.z);
    CMtxF__SwapYZRows(transform);
}
#else
void CGameObjectInstance__CalculateOrientationMatrix(void *arg0) {
    CQuatern trig;
    CQuatern product;
    CQuatern copy;
    CQuatern *copy_ptr;
    CQuatern *product_ptr;
    char *transform;
    f32 scale;
    f32 sine;
    f32 angle;
    copy = ((struct OrientActor *)(arg0))->rotation;
    copy_ptr = &copy;
    if (Cinema_PathPlaying() != 0) {
        GameObject_GetFloorTilt(copy_ptr, arg0);
    }
    angle = ((struct OrientActor *)(arg0))->yaw;
    scale = (0.5f);
    sine = func_002982F0(angle * scale);
    trig.x = 0.0f;
    trig.y = sine;
    trig.z = 0.0f;
    angle = ((struct OrientActor *)(arg0))->yaw * scale;
    gQuaternionSin = sine;
    trig.w = func_002974C0(angle);
    product_ptr = &product;
    CQuaternion__Multiply(product_ptr, &trig, copy_ptr);
    transform = &((struct OrientActor *)(arg0))->transform[0];
    CQuaternion__ToMatrix(product_ptr, transform);
    CMtxF__ScaleAxes(transform, ((struct OrientActor *)(arg0))->scale.x,
                    ((struct OrientActor *)(arg0))->scale.y,
                    ((struct OrientActor *)(arg0))->scale.z);
    CVector3__ClampLength(&((struct OrientActor *)(arg0))->position0.x, 20000.0f);
    CMtxF__Translate(transform, ((struct OrientActor *)(arg0))->position0.x,
                   ((struct OrientActor *)(arg0))->position0.y,
                   ((struct OrientActor *)(arg0))->position0.z);
    CMtxF__SwapYZRows(transform);
}
#endif

void func_0021E958(void) {

}

/* Requests animation arg1; marks the animation as changed if it is not the current one. */
void CGameObjectInstance__RequestAnim(CGameObjectInstance *arg0, s32 arg1) {
    arg0->m_CurrentAnimation.m_nRequestedAnimIndex = arg1;
    arg0->m_CurrentAnimation.m_bCycleCompleted = 0;
    if (arg0->m_CurrentAnimation.m_nAnimIndex != arg1) {
        arg0->m_CurrentAnimation.m_bAnimActive = 1;
    }
}

/* Restarts the current animation. */
void CGameObjectInstance__RestartAnim(CGameObjectInstance *arg0) {
    arg0->m_CurrentAnimation.m_bAnimActive = 1;
    arg0->m_CurrentAnimation.m_AnimationFrame = 0;
    arg0->m_CurrentAnimation.m_bCycleCompleted = 0;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x400);
}
s32 CGameObjectInstance__SetAnim(CGameObjectInstance *arg0, sptr arg1, s32 arg2);

s32 CGameObjectInstance__SetAnimByAction(void *a, s32 c, s32 flag) {
    sptr temp_v0 = CGameObjectInstance__PickRandomAnim(a, c, -1);
    if (temp_v0 != -1) {
        return CGameObjectInstance__SetAnim(a, temp_v0, flag);
    }
    return 0;
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__RequestAnim receives CGameObjectInstance__SetAnim's own argument
 * registers (pass-through); the host passes them. */
s32 CGameObjectInstance__SetAnim(CGameObjectInstance *arg0, sptr arg1, s32 arg2) {
    if ((arg1 < 0) || ((arg2 == 0) && (arg0->m_dwFlags & 0x400))) {
        return 0;
    }
    CGameObjectInstance__RequestAnim(arg0, arg1);
    return 1;
}

/* The entries passed as words (the halves sign-extended already): the definition (code/DList,
 * (table, s16, s16)) under its own name on the N64. */
#ifdef TARGET_PORT
s32 WeightedTable_Add(void *table, s16 id, s16 weight);
#define WeightedTable_Add_w(t, id, w) WeightedTable_Add((void *) (t), id, w)
#else
s32 WeightedTable_Add_w(sptr table, s32 id, s32 weight) __asm__("WeightedTable_Add");
#endif
#ifdef TARGET_PORT
/* Host body (round 7 v2). The table (id, weight) s16 pairs ending in -1 is reached only through
 * a mode row's animation word (CAIModeInfo.unk_10, an sptr: CAI__UpdateModeAnim), so gendata
 * has no type for it and copies it as bytes in ROM order: read the halves big-endian. The
 * caller declares the result as a pointer: return it sign-extended and pointer-wide (an s16
 * return left the high bits of the register undefined). */
static s16 T2_BE16(const s16 *p) {
    const u8 *b = (const u8 *) p;
    return (s16) ((b[0] << 8) | b[1]);
}
sptr CGameObjectInstance__PickAvailableAction(sptr arg0, s16 *arg1, s32 arg2) {
    u8 sp10[0x68];
    s16 *entry;

    WeightedTable_Clear(sp10);
    for (entry = arg1; T2_BE16(entry) != -1; entry += 2) {
        if (CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) arg0, (sptr) T2_BE16(entry), arg2) != -1) {
            WeightedTable_Add_w((sptr) sp10, (s32) T2_BE16(entry), (s32) T2_BE16(entry + 1));
        }
    }
    return (sptr) (s16) WeightedTable_Pick(sp10);
}
#else
s16 CGameObjectInstance__PickAvailableAction(sptr arg0, s16 *arg1, s32 arg2)
{
  s32 new_var2;
  u8 sp10[0x68];
  s16 *var_s0;
  int new_var;
  u16 var_a1;
  WeightedTable_Clear(sp10);
  var_s0 = arg1;
  var_a1 = (u16) (*var_s0);
  if ((*var_s0) != (-1))
  {
    do
    {
      if (CGameObjectInstance__PickRandomAnim((struct CGameObjectInstance *) arg0, (s32) ((s16) var_a1), arg2) != (-1))
      {
        new_var = 2;
        new_var2 = (s32) (*((s16 *) (((s8 *) var_s0) + 0)));
        WeightedTable_Add_w((sptr) (sp10), new_var2, (s32) (*((s16 *) (((s8 *) var_s0) + new_var))));
      }
      var_s0 = (typeof(var_s0)) ((s8 *) ((typeof(var_s0)) (((s8 *) var_s0) + 4)));
      var_a1 = (u16) (*var_s0);
    }
    while ((*var_s0) != (-1));
  }
  return WeightedTable_Pick(sp10);
}
#endif
#ifdef TARGET_PORT
void *CGameObjectInstance__LoadGeometry(struct CGameObjectInstance *arg0);
#define CGameObjectInstance__LoadGeometry_call(obj) ((void **) CGameObjectInstance__LoadGeometry(obj))
#else
void **CGameObjectInstance__LoadGeometry_call(void *obj) __asm__("CGameObjectInstance__LoadGeometry");
#endif

s32 CGameObjectInstance__LoadBoneMatrices(CGameObjectInstance *obj, s32 arg1) {
    s32 result;
    void **res;
    void **sub;
    void *first;

    result = 0;
    if (!(obj->m_dwFlags & 0x40000)) {
        return result;
    }
    res = CCache__Request(0, (sptr)obj->m_rpObject, obj->m_rpObjectHeader, 0, 0, 0, sCGameObjectInstanceCacheDesc_AnimObjectInfo, arg1);
    if (res != NULL) {
        sub = CGameObjectInstance__LoadGeometry_call(obj);
        if (sub != NULL) {
            first = *sub;
            result = CGameObjectInstance__BuildBoneMatrices(obj, first, CISet__GetBlock(*res, 0));
            CCache__Unlock(0, sub);
        }
        CCache__Unlock(0, res);
    }
    return result;
}

void CGameObjectInstance__ClearDrawMatrices(CGameObjectInstance *arg0) {
    struct Mtx *temp_a1;

    temp_a1 = arg0->m_pmtDrawMtxs;
    arg0->m_pmtDrawMtxs = 0;
    arg0->m_pmfShadow = 0;
    arg0->m_dwFlags &= ~0x200;
    arg0->m_pmtLastFrameDrawMtxs = temp_a1;
}

typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad018[0x15C];
    /* 0x174 */ s8 mode;
    /* 0x175 */ u8 pad175;
    /* 0x176 */ s8 prevMode;
} Func0021EBE4Obj;

void CGameObjectInstance__GetDeathModels(Func0021EBE4Obj *obj, s32 offset, s32 *outA, s32 *outB) {
    CIntelligence *def;
    s32 mode;

    def = obj->def;
    if (def->m_Type == 1) {
        if (obj->mode == 60) {
            mode = obj->prevMode;
        } else {
            mode = obj->mode;
        }
        switch ((u32) (mode - 54)) {
            case 0:
                *outA = *outB = def->m_Enemy.m_DeathModels[0];
                return;
            case 1:
                *outA = *outB = def->m_Enemy.m_DeathModels[1];
                return;
            case 2:
                *outA = *outB = def->m_Enemy.m_DeathModels[2];
                return;
            case 3:
                *outA = *outB = def->m_Enemy.m_DeathModels[3];
                return;
            case 4:
                *outA = *outB = def->m_Enemy.m_DeathModels[4];
                return;
            case 5:
                *outA = *outB = def->m_Enemy.m_DeathModels[5];
                return;
        }
    }
    *outA = ((s8 *) def)[offset + 0xE];
    *outB = ((s8 *) def)[offset + 0x10];
}

void CGameObjectInstance__DrawModelWithLights(CGameObjectInstance *arg0, s32 arg1, s32 arg2, CLights1 *src) {
    arg0->m_Lights[gEvenOdd] = *src;
    CGameObjectInstance__DrawModel(arg0, arg1, arg2);
}

typedef struct Slot24 {
    s32 w[6];
} Slot24;
void CMtxF__Multiply(f32 (*mf)[4], void *arg1, void *arg2);
void CMtxF__ClampEntries(f32 (*mf)[4]);
void CMtxF__ToMtx(f32 (*mf)[4], Mtx *m);
void Render_DrawModelFlat(s32, sptr, sptr, s32, Slot24 *, sptr, s32, s32);

void CGameObjectInstance__SubmitShadowModel(CGameObjectInstance *owner, ModelRef *ref, void *arg2, s32 arg3) {
    f32 mf[4][4];
    Model *model;
    Mtx *m;

    if (owner->m_pmtDrawMtxs != NULL) {
        m = MtxPool_Alloc(gScene.m_Heap, 1);
        if (m != NULL) {
            model = ref->model;
            CMtxF__Multiply(mf, arg2, model->unk1CC);
            CMtxF__ClampEntries(mf);
            CMtxF__ToMtx(mf, m);
            {
                Gfx *g = gDList++;
                g->w0 = 0xDA380007;
                g->w1 = GBI_ADDR(m);
            }
            Render_DrawModelFlat((u8)arg3, ref->unkC, (sptr) owner->m_pmtDrawMtxs, 1, &owner->m_Lights[gEvenOdd], 0, ref->unk1C,
                          owner->m_AI.m_nTexture);
            {
                Gfx *g = gDList++;
                g->w0 = 0xDA380007;
                g->w1 = GBI_ADDR(&model->mtx[gEvenOdd]);
            }
        }
    }
}

void CGameObjectInstance__EmitFlaggedHotPoints(u8 *actor, s32 *data, s32 arg2) {
    s32 *countp;
    s32 *table;
    s32 *entry;
    s32 count;
    s32 value;
    s32 i;
    s32 bit;
    s32 *params;

    countp = CISet__GetBlock(data, 2);
    table = CISet__GetBlock(data, 4);
    count = *countp;
    for (i = 0; i < count; i++) {
        bit = 1 << i;
        if (*(s32 *)(actor + 0x14C) & bit) {
            entry = CISet__GetBlock(table, i);
            value = entry[1];
            if (value != 0) {
                params = entry + 2;
                GameObject_EmitHotPoints(params, value, (void *) (sptr) arg2, actor, CISet__GetBlock(data, 3));
            }
        }
    }
}
CVector3 *CGameObjectInstance__LocalToWorld(CVector3 *out, void *actor, CVector3 v);
void GameObject_TraceMove(void *actor, CVector3 pos, void *outPos, void *outRegion, s32 flagA, s32 flagB);

void CGameObjectInstance__TraceMoveLocal(CGameObjectInstance *actor, CVector3 v, s32 arg4, s32 arg5) {
    CVector3 local;
    CVector3 out;

    if (actor->m_dwFlags & 0x300000) {
        v.x = -v.x;
        v.z = -v.z;
        CMtxF__TransformPoint(((u8 *)((CPlayer *)actor->m_AI.m_pParent)->m_pCamera) + 0x14C, &v, &out);
    } else {
        CGameObjectInstance__LocalToWorld(&local, actor, v);
        CVector3__Add(&out, &actor->header.m_vPos, &local);
    }
    GameObject_TraceMove(actor, out, (void *) (sptr) arg4, (void *) (sptr) arg5, 1, 1);
}

CVector3 *CGameObjectInstance__LocalToWorld(CVector3 *out, void *actor, CVector3 v) {
    CQuaternion q;
    f32 m[16];
    CVector3 r;

    q = CGameObjectInstance__GetRotation(actor);
    CQuaternion__ToMatrix(&q.x, m);
    CMtxF__TransformPoint(m, &v, &r);
    *out = r;
    return out;
}

typedef struct B11Pt {
    f32 x;
    f32 y;
} B11Pt;
typedef struct B11Box {
    u16 unk0;
    u16 flags2;
    B11Pt *p4;
    B11Pt *p8;
    B11Pt *pC;
} B11Box;
void CGameRegion__GetNormalRaw(f32 *out, B11Box *box);

s32 GameObject_IsFacingRegionNormal(CGameObjectInstance *a) {
    B11Box *b = (B11Box *)a->header.m_pCurrentRegion;
    f32 t;
    f32 hi;
    f32 t2;
    f32 lo;
    f32 s;
    f32 c;
    f32 buf[3];
    if (b == 0) {
        return 0;
    }
    t = b->p8->y;
    if (!(b->p4->y <= t)) {
        t = b->p4->y;
    }
    hi = b->pC->y;
    if (!(t <= hi)) {
        hi = t;
    }
    t2 = b->p8->y;
    if (!(t2 <= b->p4->y)) {
        t2 = b->p4->y;
    }
    lo = b->pC->y;
    if (!(lo <= t2)) {
        lo = t2;
    }
    if (hi < a->header.m_vPos.y) {
        return 0;
    }
    if (a->header.m_vPos.y < lo) {
        return 0;
    }
    s = func_002982F0(a->m_RotY);
    c = func_002974C0(a->m_RotY);
    CGameRegion__GetNormalRaw(buf, b);
    return buf[0] * -s + buf[2] * -c < 0.0f;
}

f32 func_0021F1A4(void) {
    return 1.0f;
}

s32 CAI__Update(CGameObjectInstance *, CAI *);

/* Updates the actor's AI (CGameObjectInstance.m_AI, 0x140). */
void CGameObjectInstance__UpdateAI(CGameObjectInstance *arg0) {
    CAI__Update(arg0, &arg0->m_AI);
}

void CGameObjectInstance__SetHotPointFlag2(CGameObjectInstance *arg0, s32 arg1) {
    if (arg1 != 0) {
        arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags | 2);
        return;
    }
    arg0->m_dwHotPointFlags = (arg0->m_dwHotPointFlags & ~2);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CGameObjectInstance__LoadGeometry receives CGameObjectInstance__GetGeometryKind's own argument
 * registers (pass-through); the host passes them. */
s32 CGameObjectInstance__GetGeometryKind(void * pt0) {
    s32 **res;
    s32 value;

    value = -1;
    res = (s32 **) CGameObjectInstance__LoadGeometry_call(pt0);
    if (res != NULL) {
        value = ((u8 *) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*res, 3), 0), 1))[0x66];
        CCache__Unlock(0, res);
    }
    return value;
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
f32 CGameObjectInstance__GetAverageScale(CGameObjectInstance *arg0) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f1;

    temp_f12 = arg0->m_vScale.x;
    temp_f1 = arg0->m_vScale.y;
    temp_f0 = arg0->m_vScale.z;
    return func_00298470(((temp_f12 * temp_f12) + (temp_f1 * temp_f1) + (temp_f0 * temp_f0)) * 0.33333334f);
}

/* The first section of an object model (CISet__GetBlock(*model, 0)): only its angle at 0x1E is read. */
typedef struct {
    /* 0x00 */ u8 pad0[0x1E];
    /* 0x1E */ u16 angle;
} ModelSection;
f32 HalfToFloat(u16);                             /* extern */

f32 CGameObjectInstance__GetObjectScale(CGameObjectInstance *arg0) {
    f32 var_f0;
    f32 var_f20;
    sptr *temp_v0;

    var_f20 = 1.0f;
    if (arg0->m_dwFlags & 0x40000) {
        temp_v0 = CCache__Request(0, (sptr) arg0->m_rpObject, arg0->m_rpObjectHeader, 4, 0, 0, &sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        var_f0 = var_f20;
        if (temp_v0 != NULL) {
            var_f20 = HalfToFloat(((ModelSection *) CISet__GetBlock(*temp_v0, 0))->angle);
            CCache__Unlock(0, temp_v0);
            goto block_3;
        }
    } else {
block_3:
        var_f0 = var_f20;
    }
    return var_f0;
}

DEFERRED void func_0021F418(void) {

}

DEFERRED void func_0021F420(void) {

}

typedef struct RootBox {
    CVector3 min;
    CVector3 max;
} RootBox;
struct RootActor;
typedef struct RootActor RootActor;
struct RootActor {
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    CVector3 position0; /* 0x04 in Turok 2 */
    s32 model;
    CIntelligence *definition;
    u8 pad18[0x34 - 0x18]; /* Turok 2 layout differs from Rage Wars here */
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    RootBox bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    Shared_ParticleLighting lighting[2];
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};
void CAnimationState__GetBonePosition(void *channel, s32 bone, f32 t, f32 *out);

DEFERRED void *CGameObjectInstance__GetRootOffset(RootTriple *arg0, void *arg1, RootChannel *arg2) {
    char *o1 = (char *) arg1;
    char mtx[0x40];
    f32 spVec[3];
    f32 inVec[3];
    f32 outVec[3];
    CAnimationState__GetBonePosition(arg2, 0, arg2->time, spVec);
    inVec[0] = spVec[0];
    inVec[1] = spVec[2];
    inVec[2] = spVec[1];
    CMtxF__SetRotateY(mtx, ((RootActor *)o1)->yaw);
    CMtxF__TransformPoint(mtx, inVec, outVec);
    outVec[0] *= ((RootActor *)o1)->scale.x;
    outVec[1] *= ((RootActor *)o1)->scale.y;
    outVec[2] *= ((RootActor *)o1)->scale.z;
    *arg0 = *(RootTriple *) outVec;
    return arg0;
}

typedef struct B11Rec {
    u8 pad0[0x2];
    u16 type;
    u8 pad4[0x4];
    CVector3 pos;
    AnimEventPair unk14;
} B11Rec;

DEFERRED void CGameObjectInstance__SendAnimImpactEvents(CGameObjectInstance *a, s32 b) {
    B11Rec *list = CAnimationState__GetEvents((void *) (sptr) b);
    s32 n = CAnimationState__GetEventCount((void *) (sptr) b);
    s32 i;
    s32 t;
    CVector3 v;
    for (i = 0; i < n; i++) {
        t = list[i].type;
        if (t < 8) {
            if (t >= 6) {
                CMtxF__TransformPoint(a->m_mfOrient, &list[i].pos, &v);
                ImpactEvent_Send(a, a, list[i].type, -1, v, list[i].unk14);
            }
        }
    }
}

DEFERRED s32 func_0021F5B8(CGameObjectInstance *obj, s32 arg1) {
    s32 temp_s0;
    sptr temp_v0;

    if (obj->m_dwFlags & 0x40000) {
        temp_v0 = CCache__Request(0, ((sptr) obj->m_rpObject), ((s32) obj->m_rpObjectHeader), 4, 0, 0, &sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (temp_v0 != 0) {
            temp_s0 = ((s32 *)(CISet__GetBlock(*(s32 *)temp_v0, 1) + (arg1 << 2)))[2];
            CCache__Unlock(0, temp_v0);
            return temp_s0;
        }
    }
    return -1;
}

DEFERRED s32 func_0021F660(void) {
    return 0;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C33C.c, func_0024C33C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Region B11Region;
/* The angle passed as its word: the port's shim (port/game/t2abi.c, ABI.md float_int), the N64
 * definition under its own name. */
#ifdef TARGET_PORT
void CMtxF__SetRotateY_i(void *m, s32 a);
#else
void CMtxF__SetRotateY_i(void *m, s32 a) __asm__("CMtxF__SetRotateY");
#endif

DEFERRED void CGameObjectInstance__ApplyRootMotion(CGameObjectInstance *a, f32 *p) {
    f32 t0[3];
    f32 t1[3];
    f32 d[3];
    f32 r[3];
    CVector3 w;
    f32 m[16];
    CAnimationState__GetBonePosition(p, 0, 0.0f, t0);
    CAnimationState__GetBonePosition(p, 0, *p, t1);
    d[0] = t1[0] - t0[0];
    d[1] = 0.0f;
    d[2] = t1[1] - t0[1];
    CMtxF__SetRotateY_i(m, *(s32 *)&a->m_RotY);
    CMtxF__TransformPoint(m, d, r);
    w.x = a->header.m_vPos.x + r[0] * a->m_vScale.x;
    w.y = a->header.m_vPos.y;
    w.z = a->header.m_vPos.z + r[2] * a->m_vScale.z;
    GameObject_TraceMove(a, w, &a->header.m_vPos, (B11Region **)&a->header.m_pCurrentRegion, 0, 0);
}

struct ObjRecAt18V2;
typedef struct ObjRecAt18V2 ObjRecAt18V2;
struct ObjRecAt18V2 {
    char pad0[0x14];
    s32 * unk18; /* at 0x14 in Turok 2 */
};

DEFERRED void *CGameObjectInstance__GetBodySetup(char *arg0)
{
    s32 type;
    s32 i;
    s32 count;
    s32 limit;
    char *object;
    CPlayerPool *list;
    ObjRecAt18V2 *actor = (void *)arg0;
    type = *actor->unk18;
    if ((unsigned int)type >= 13) {
        return 0;
    }
    if (type == 11) {
        list = &gEngineApp_m_Game.m_PlayerList;
        count = list->m_nPlayers;
        i = 0;
        if (count > 0) {
            limit = count;
            object = (char *)list->m_Players;
            do {
                if (object == arg0) {
                    return 0;
                }
                if (object + T2_OFFSETOF(CPlayer, m_WeaponActor) == arg0) {
                    return &gWeaponActorTypeInfo;
                }
                i++;
                object += (s32)sizeof(CPlayer);
            } while (i < limit);
        }
    }
    return gIntelligenceTypeInfo[type];
}

/* Rotates the matrix about its own origin by the swing angle of this step. */
DEFERRED void CGameObjectInstance__SwingBone(s32 arg0, Swing *swing, f32 *matrix, s32 active, s32 dir) {
    CVector3 origin;
    CVector3 axis;
    f32 rotation[16];
    f32 inverse[16];
    f32 angle;
    f32 flip;

    if (active != 0) {
        origin.x = matrix[12];
        origin.y = matrix[13];
        origin.z = matrix[14];
        axis = swing->axis;
        angle = gSwingAmplitudes[25 - swing->step];
        angle *= 0.3f;
        angle *= (dir == 5 ? -2.0f : dir);
        CMtxF__Copy(inverse, matrix);
        flip = -1.0f;
        CVector3__Scale(&origin, &origin, flip);
        CMtxF__Translate(inverse, origin.x, origin.y, origin.z);
        CMtxF__SetAxisAngle(rotation, angle, axis.x, axis.y, axis.z);
        CMtxF__MultiplyAffine(matrix, inverse, rotation);
        CVector3__Scale(&origin, &origin, flip);
        CMtxF__Translate(matrix, origin.x, origin.y, origin.z);
        if (swing->lastStep == active) {
            swing->step--;
        }
    }
}

DEFERRED void Anim_LerpVector(void *arg0, f32 arg1, void *arg2, void *arg3)
{
  f32 temp_f1;
  f32 temp_f1_2;
  f32 temp_f1_3;
  f32 new_var;
  new_var = *((f32 *) (((s8 *) arg3) + 0));
  temp_f1 = *((f32 *) (((s8 *) arg2) + 0));
  *((f32 *) (((s8 *) arg0) + 0)) = (f32) (temp_f1 + (arg1 * (new_var - temp_f1)));
  temp_f1_2 = *((f32 *) (((s8 *) arg2) + 4));
  new_var = *((f32 *) (((s8 *) arg3) + 4));
  *((f32 *) (((s8 *) arg0) + 4)) = (f32) (temp_f1_2 + (arg1 * (new_var - temp_f1_2)));
  ;
  *((f32 *) (((s8 *) arg0) + 8)) = (f32) ((*((f32 *) (((s8 *) arg2) + 8))) + (arg1 * ((*((f32 *) (((s8 *) arg3) + 8))) - (*((f32 *) (((s8 *) arg2) + 8))))));
}

DEFERRED void Anim_LerpQuaternion(void *arg0, f32 t, void *a, void *b) {
    Vector4f *out = arg0;
    Vector4f *left = a;
    Vector4f *right = b;
    out->x = left->x + (t * (right->x - left->x));
    out->y = left->y + (t * (right->y - left->y));
    out->z = left->z + (t * (right->z - left->z));
    out->w = left->w + (t * (right->w - left->w));
}

DEFERRED void Anim_GetBoneTranslationLerp(void *arg0, s32 arg1, void *arg2) {
    ObjectLinks24 *o = arg0;
    CVector3 *out = arg2;
    ObjRecAt2V1 *indices = (void *)o->unk_0;
    s16 idx;
    ObjectState14 *recs;
    f32 *base;
    f32 *a;
    f32 scale;
    idx = indices[arg1].translation;
    if (idx == -1) {
        recs = (void *)o->unk_4;
        *out = recs[arg1].position;
        return;
    }
    base = CISet__GetBlock(o->translationFrames, (s32) idx);
    a = base + o->translationFirst;
    base = base + o->translationNext;
    scale = o->unk_20;
    out->x = a[0] + scale * (base[0] - a[0]);
    out->y = a[1] + scale * (base[1] - a[1]);
    out->z = a[2] + scale * (base[2] - a[2]);
}

DEFERRED void Anim_GetBoneRotationLerp(void *arg0, s32 arg1, Vector4f *arg2) {
    char *o = (char *)arg0;
    s16 idx;
    char *rec;
    void *base;
    char *a;
    idx = ((struct ObjRecAt2V1 *) (((ObjectLinks24 *) o)->unk_0 + (arg1 * 4)))->unk2;
    if (idx == -1) {
        f32 scale;
        rec = ((ObjectLinks24 *)(o))->unk_4 + arg1 * 0x14;
        scale = (3.0518509447574615e-05f);
        arg2->x = (f32)(((ObjectState14 *)(rec))->unk_C) * scale;
        arg2->y = (f32)(((ObjectState14 *)(rec))->unk_E) * scale;
        arg2->z = (f32)(((ObjectState14 *)(rec))->unk_10) * scale;
        arg2->w = (f32)(((ObjectState14 *)(rec))->unk_12) * scale;
        return;
    }
    base = CISet__GetBlock(((ObjectLinks24 *)(o))->unk_C, (s32)idx);
    a = (char *)base + (((ObjectLinks24 *)(o))->unk_10) * 4;
    base = (char *)base + (((ObjectLinks24 *)(o))->unk_14) * 4;
    {
        f32 scale;
        scale = ((ObjectLinks24 *)(o))->unk_20;
        arg2->x = ((Vector4f *)(a))->x + scale * (((Vector4f *)(base))->x - ((Vector4f *)(a))->x);
        arg2->y = ((Vector4f *)(a))->y + scale * (((Vector4f *)(base))->y - ((Vector4f *)(a))->y);
        arg2->z = ((Vector4f *)(a))->z + scale * (((Vector4f *)(base))->z - ((Vector4f *)(a))->z);
        arg2->w = ((Vector4f *)(a))->w + scale * (((Vector4f *)(base))->w - ((Vector4f *)(a))->w);
    }
}

/* The amount passed as its word: the port's shim (port/game/t2abi.c, ABI.md float_int), the N64
 * definition under its own name. */
#ifdef TARGET_PORT
void CQuaternion__Slerp_i(void *out, s32 amount, void *a, void *b);
#else
void CQuaternion__Slerp_i(void *out, s32 amount, void *a, void *b) __asm__("CQuaternion__Slerp");
#endif

DEFERRED void Anim_GetBoneRotationSlerp(ObjectLinks24_2 *arg0, s32 arg1, Vector4f *arg2) {
    ObjectLinks24_2 *o = arg0;
    ObjRecAt2V1 *indices;
    s16 idx;
    ObjectState14 *rec;
    void *base;
    f32 scale;
    indices = (void *)o->unk_0;
    idx = indices[arg1].unk2;
    if (idx == -1) {
        rec = (void *)(o->unk_4 + arg1 * 0x14);
        scale = 3.051851e-5f;
        arg2->x = (f32)rec->unk_C * scale;
        arg2->y = (f32)rec->unk_E * scale;
        arg2->z = (f32)rec->unk_10 * scale;
        arg2->w = (f32)rec->unk_12 * scale;
        return;
    }
    base = CISet__GetBlock(o->unk_C, (s32)idx);
    CQuaternion__Slerp_i(arg2, o->unk_20,
                    (char *)base + o->unk_10 * 4,
                    (char *)base + o->unk_14 * 4);
}

DEFERRED void CMtxF__ToMtxAffineUnscaled(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    s32 a;
    s32 b;
    { f32 value = (src[0]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[1]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[2]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[4]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[5]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[6]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[8]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[9]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[10]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[12]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[13]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[14]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | 1;
    *lower = a << 16;
}

DEFERRED void Quaternion_ToMatrix(f32 *m, f32 *q, f32 *t)
{
    f32 x = q[0];
    f32 xx = x * x;
    f32 y = q[1];
    f32 yy = y * y;
    f32 z = q[2];
    f32 zz = z * z;
    f32 w = q[3];
    f32 ww = w * w;
    f32 x2 = x + x;
    f32 xy = x2 * y;
    f32 w2 = w + w;
    f32 wz = w2 * z;
    f32 xz = x2 * z;
    f32 wy = w2 * y;
    f32 wx = w2 * x;
    f32 yz = (y + y) * z;
    m[0] = ww + xx - yy - zz;
    m[1] = xy + wz;
    m[2] = xz - wy;
    m[4] = xy - wz;
    m[5] = ww - xx + yy - zz;
    m[6] = yz + wx;
    m[8] = xz + wy;
    m[9] = yz - wx;
    m[10] = ww - xx - yy + zz;
    m[12] = t[0];
    m[13] = t[1];
    m[14] = t[2];
    m[3] = 0.0f;
    m[7] = 0.0f;
    m[11] = 0.0f;
    m[15] = 1.0f;
}

sptr Lighting_BuildLitVertices(sptr, sptr, sptr, sptr, s32);

/* host: the box (m_vMin/m_vMax) and the draw matrices at the host offsets; arg1 is the CCacheEntry */
DEFERRED void CGameObjectInstance__ConstructLitVertices(void *arg0, s32 **arg1)
{
  CGameObjectInstance *o = arg0;
  sptr new_var = (sptr) ((CCacheEntry *) arg1)->memEntry->data;
  CCache__ReleaseAndReplaceData(0, arg1, Lighting_BuildLitVertices(new_var, 0, (sptr) &o->m_vMin, (sptr) o->m_pmtDrawMtxs, 1));
}
