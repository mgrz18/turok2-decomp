#include "structs/vector.h"
#include "structs/particle_event.h"
#include "globals/object.h"

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
/* Bone record: the mask bits sit at 0x6C. */
typedef struct BoneInfo {
    char pad0[0x6C];
    s32 mask;
} BoneInfo;
extern void CMtxF__TransformPoint(void *arg0, void *arg1, void *arg2);
extern void *CISet__GetBlock(void *, s32);
extern void CCache__Unlock(void *heap, s32 *resource);
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
                if (checkMask && (gImpactEventHandlers_m_Flags[events[i].effect].m_Flags & 4)) {
                    if (resource == 0) {
                        resource = CGameObjectInstance__LoadGeometry(obj);
                        if (resource == 0) {
                            goto masked;
                        }
                        table = CISet__GetBlock((void *)*resource, 3);
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
