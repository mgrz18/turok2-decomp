#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

/* Radius of an object, by the kind of its definition. */
f32 func_002672C8(CGameObjectInstance *obj) {
    CIntelligence *def;
    f32 r;
    f32 m;

    switch (obj->header.m_pIntelligence->m_Type) {
    case 11:
        if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000) && ((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy != NULL) {
            return 122.88f;
        }
        /* fallthrough */
    case 1:
    case 4:
        return obj->header.m_pIntelligence->m_Enemy.m_CollisionRadius;
    case 0:
    case 5:
    case 8:
    case 10:
    case 12:
        return obj->header.m_pIntelligence->m_Static.m_CollisionRadius;
    case 2:
        def = obj->header.m_pIntelligence;
        if (def->m_Platform.m_CollisionType == 2) {
            m = def->m_Platform.m_CollisionWidth;
            if (!(def->m_Platform.m_CollisionRadius <= m)) {
                m = def->m_Platform.m_CollisionRadius;
            }
            r = def->m_Platform.m_CollisionLength;
            if (!(m <= r)) {
                r = m;
            }
            return r * 1.42f;
        }
        return def->m_Platform.m_CollisionRadius;
    case 7:
        return obj->header.m_pIntelligence->m_Destructible.m_CollisionRadius;
    default:
        return 0.0f;
    }
}
