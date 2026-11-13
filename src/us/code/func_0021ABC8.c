#include "structs/vector.h"
#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"
int T2Port_AnimEventsEnabled(void); /* port/game/t2actors.c */
#endif
#include "structs/particle_event.h"
#include "globals/object.h"
#include "globals/particle.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_2.CPP, func_802476C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct IntPair {
    int field_0;
    int field_4;
};
typedef struct CROMEventEntry {
    u16 frame;
    u16 effect;
    s16 bone;
    s16 pad6;
    CVector3 local;
    struct IntPair params;
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
extern void CMtxF__TransformPoint(void *arg0, void *arg1, void *arg2);
extern CROMEventEntry *CAnimationState__GetEvents(void *);
extern s32 CAnimationState__GetEventCount(void *);
extern s32 *CGameObjectInstance__LoadGeometry(ObjectLinks *);
extern void CMtxF__FromMtxAffine(f32 *, char *);
extern void ImpactEvent_Send(ObjectLinks *, ObjectLinks *, u16, s32, CVector3, struct IntPair);
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
#ifdef TARGET_PORT
                        table = CISet__GetBlock(*(void **)resource, 3); /* the data pointer, pointer-wide */
#else
                        table = CISet__GetBlock((void *)*resource, 3);
#endif
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
