#include "common.h"
#include "structs/intelligence.h"

typedef struct Owner {
    /* 0x000 */ u8 pad0[0xBB0];
    /* 0xBB0 */ s32 unkBB0;
} Owner;

typedef struct Obj {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x13];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0xD4 - 0x18];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ Owner *owner;
} Obj;

/* Radius of an object, by the kind of its definition. */
f32 func_002672C8(Obj *obj) {
    CIntelligence *def;
    f32 r;
    f32 m;

    switch (obj->def->m_Type) {
    case 11:
        if (obj->kind == 1 && (obj->flags & 0x300000) && obj->owner->unkBB0 != 0) {
            return 122.88f;
        }
        /* fallthrough */
    case 1:
    case 4:
        return obj->def->m_Enemy.m_CollisionRadius;
    case 0:
    case 5:
    case 8:
    case 10:
    case 12:
        return obj->def->m_Static.m_CollisionRadius;
    case 2:
        def = obj->def;
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
        return obj->def->m_Destructible.m_CollisionRadius;
    default:
        return 0.0f;
    }
}
