/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B274_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "structs/game_object.h"
#include "protos/scene.h"

extern void CGameObjectInstance__SetupAnimation(CGameObjectInstance *, s32 *, s32, sptr);

/* CScene__FindObjectType, inlined: the index of id in the level's object type table. */
static __inline__ s32 find(IdTable *table, u16 id) {
    s32 index = 0, count;
    u16 *entry;
    entry = table->ids;
    count = table->count;
    if (count > 0) {
        do {
            if (*entry == id) return index;
            index++;
            entry++;
        } while (index < count);
    }
    return -1;
}

void CScene__SetInstanceObjectType(CScene *scene, CGameObjectInstance *obj, s32 type, sptr anim) {
    s32 index = 0;
    if (obj->m_nTypeFlag != type) {
        index = find(scene->m_pObjectTypes, type);
        if (index != -1) {
            obj->header.m_nObjType = index;
            obj->m_nTypeFlag = type;
            CGameObjectInstance__SetupAnimation(obj, scene->m_pObjects, scene->m_ObjectsKey, anim);
        }
    }
}
