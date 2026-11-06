/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM/802624A0_10.CPP, func_80262CE0_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/list.h"
#include "structs/game_object.h"

/* The animation header: what this reads of it. */
typedef struct AnimInfo {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u16 intelligence;
    /* 0x1E */ u16 angle;
} AnimInfo;

extern CWorld gScene;
extern s32 D_800B6D54;                  /* effects enabled */
extern s32 D_800B6D00;
extern s32 D_8011001C;
extern char D_800A6070[];               /* "anim object index" */
extern char D_800A6084[];               /* "anim object info" */

s32 CScene__FindObjectType(CWorld *world, s32 id);
s32 CISet__GetROMAddress(s32 *table, s32 base, s32 index);
s32 *CCache__Request(s32 heap, s32 src, s32 size, s32 type, s32 arg4, s32 callback, char *name, s32 arg7);
s32 CISet__GetROMAddressAndSize(s32 table, s32 base, s32 index, s32 *size);
void *CISet__GetBlock(s32 table, s32 index);
s32 CScene__GetIntelligence(CWorld *world, s32 index);
f32 HalfToFloat(s32 angle);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 angle);
void CCache__Unlock(s32 heap, s32 *block);
void CList__Remove(CList *list, void *node);
void CList__AddHead(CList *list, void *node);
void CGameObjectInstance__Construct(CListedInstance *effect, s32 id, s32 variant, s32 unused, s32 count, s32 sound, s32 owner,
                   f32 scale, CVector3 position, s32 flags, CVector3 color, CVector3 direction, s32 arg12);
void func_0021E5D4(CListedInstance *effect);

/* Takes a node from the free list when allowed (always with D_8011001C, otherwise while fewer than
 * three are active), moves it to the active list and attaches the counted reference. */
static __inline__ CListedInstance *take_effect(CInstancePool *scene, s32 *ref) {
    CListedInstance *effect;

    if (D_8011001C == 0 && (u32)scene->m_ActiveList.m_nCount >= 3) {
        return NULL;
    }
    effect = scene->m_FreeList.m_pHead;
    if (effect != NULL) {
        CList__Remove(&scene->m_FreeList, effect);
        CList__AddHead(&scene->m_ActiveList, effect);
        effect->m_pOwnerCount = ref;
        if (ref != NULL) {
            *ref += 1;
        }
    }
    return effect;
}

static __inline__ CListedInstance *spawn_effect(CInstancePool *scene, s32 id, s32 variant, s32 count, s32 owner,
                                            CVector3 position, f32 scale, CVector3 direction, CVector3 color, s32 *ref) {
    CListedInstance *effect;

    if (D_800B6D54 == 0 || count == 0) {
        return NULL;
    }
    effect = take_effect(scene, ref);
    if (effect == NULL) {
        return NULL;
    }
    CGameObjectInstance__Construct(effect, id, variant, D_800B6D00, count, -1, owner, scale, position, 0x80000, color, direction, 0);
    func_0021E5D4(effect);
    return effect;
}

/* Spawns an effect of the given type in a scene: loads the type's animation index and header
 * ("anim object index", "anim object info"), takes its intelligence from the header and turns
 * color by the header's angle, then spawns it (see spawn_effect). Returns the effect or NULL. */
CListedInstance *CInstancePool__SpawnAnimInstance(CInstancePool *scene, s32 variant, s32 owner, CVector3 position, f32 scale,
                           CVector3 direction, CVector3 color, s32 *ref) {
    CListedInstance *effect;
    AnimInfo *info;
    s32 *index;
    s32 *block;
    s32 id;
    s32 model;
    s32 src;
    s32 count;
    s32 size;

    effect = NULL;
    id = CScene__FindObjectType(&gScene, variant);
    if (id != -1) {
        src = CISet__GetROMAddress(gScene.m_pObjects, gScene.m_ObjectsKey, id);
        index = CCache__Request(0, src, 0x18, 0, 0, 0, D_800A6070, 1);
        if (index != NULL) {
            block = CCache__Request(0, CISet__GetROMAddressAndSize(*index, src, 0, &size), size, 0, 0, 0, D_800A6084, 1);
            if (block != NULL) {
                info = CISet__GetBlock(*block, 0);
                count = CScene__GetIntelligence(&gScene, info->intelligence);
                CVector3__Scale(&color, &color, HalfToFloat(info->angle));
                model = id;     /* a copy: the ROM allocates it apart from id */
                effect = spawn_effect(scene, model, variant, count, owner, position, scale, direction, color, ref);
                CCache__Unlock(0, block);
            }
            CCache__Unlock(0, index);
        }
    }
    return effect;
}
