#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"

/* A blast: radius, damage, then the push (strength, x, y, z). */
#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
typedef struct Blast {
    u16 damage;
    u16 radius;
    s8 z;
    s8 y;
    s8 x;
    s8 strength;
} __attribute__((aligned(4))) Blast;
#else
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;
#endif

#ifdef TARGET_PORT
/* The owner is a player (CPlayer starts with its actor): 0xBB0 is CPlayer.m_pOnEnemy, a pointer
 * (round 12 a5, tools/port/n64viewcheck.py). */
#include "structs/player.h"
typedef struct PushOwner {
    union {
        struct { u8 _hp_reduced[__builtin_offsetof(CPlayer, m_pOnEnemy)]; void *reduced; };
    };
} PushOwner;
#else
typedef struct PushOwner {
    /* 0x000 */ u8 pad0[0xBB0];
    /* 0xBB0 */ s32 reduced;
} PushOwner;
#endif

#ifdef TARGET_PORT
typedef struct PushSource {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PushSource(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_PushSource(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_flags[T2_HOFF_PushSource(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_owner[T2_HOFF_PushSource(m_AI.m_pParent)]; PushOwner *owner; }; /* N64 0x1A8 */
    };
} PushSource;
#else
typedef struct PushSource {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0xD4 - 0x10];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ PushOwner *owner;
} PushSource;
#endif

#ifdef TARGET_PORT
typedef struct PushTarget {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PushTarget(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_PushTarget(header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_type[T2_HOFF_PushTarget(header.m_nObjType)]; u16 type; }; /* N64 0x2 */
        struct { u8 _hp_pos[T2_HOFF_PushTarget(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_dir[T2_HOFF_PushTarget(m_vVelocity)]; CVector3 dir; }; /* N64 0x18 */
        struct { u8 _hp_angle[T2_HOFF_PushTarget(m_RotY)]; f32 angle; }; /* N64 0x50 */
        struct { u8 _hp_playerFlags[T2_HOFF_PushTarget(m_AI.m_ModeFlags)]; u32 playerFlags; }; /* N64 0x17C */
    };
} PushTarget;
#else
typedef struct PushTarget {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x8];
    /* 0x018 */ CVector3 dir;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 angle;
    /* 0x054 */ u8 pad54[0x17C - 0x54];
    /* 0x17C */ u32 playerFlags;
} PushTarget;
#endif


void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CMtxF__Identity(CMtxF *m);
void CMtxF__RotateVector(CMtxF *m, CVector3 *in, CVector3 *out);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *m);

/* The blast's push from `src` turned into a world vector, in the frame of `target`. */
void Blast_GetPush(PushSource *src, PushTarget *target, Blast blast, CVector3 *out, u32 flags) {
    CVector3 v;
    CVector3 dir;
    CQuaternion q;
    CMtxF m;
    PushTarget *obj;

    if (gScene.m_bStreamAll != 0) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        return;
    }
    v.x = blast.x;
    v.y = blast.y;
    v.z = blast.z;
    CVector3__Scale(&v, &v, blast.strength * 0.80629915f);
    switch (target->kind) {
    case 1:
        if (target->playerFlags & 0x2000) {
            v.x = 0.0f;
            v.z = 0.0f;
            v.y = 133.12f;
        }
        CMtxF__SetRotateY(&m, target->angle + 3.1415927f);
        break;
    case 2:
        obj = target;
        switch (obj->type) {
        case 0xE:
            dir.x = 0.0f;
            dir.z = 0.0f;
            v.x = 0.0f;
            v.y = 0.0f;
            dir.y = 1.0f;
            v.z = 512.0f;
            break;
        case 0x68:
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 409.59998f;
            dir = obj->dir;
            dir.y = 0.0f;
            break;
        default:
            if (flags & 0x400000) {
                CVector3__Subtract(&dir, &src->pos, &obj->pos);
                dir.y = 7.68f;
            } else {
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
            }
            break;
        }
        CQuaternion__FromDirection(&q, &dir);
        CQuaternion__ToMatrix(&q, &m);
        break;
    default:
        CMtxF__Identity(&m);
        break;
    }
    if (src->flags & 0x300000) {
        if (src->owner->reduced != 0) {
            CVector3__Scale(&v, &v, 0.1f);
        }
    }
    CMtxF__RotateVector(&m, &v, out);
}
