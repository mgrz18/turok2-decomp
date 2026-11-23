#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
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


void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CMtxF__Identity(CMtxF *m);
void CMtxF__RotateVector(CMtxF *m, CVector3 *in, CVector3 *out);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *m);

/* The blast's push from `src` turned into a world vector, in the frame of `target`. */
void Blast_GetPush(CGameObjectInstance *src, CGameObjectInstance *target, Blast blast, CVector3 *out, u32 flags) {
    CVector3 v;
    CVector3 dir;
    CQuaternion q;
    CMtxF m;
    CGameObjectInstance *obj;

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
    switch (target->header.m_Type) {
    case 1:
        if (target->m_AI.m_ModeFlags & 0x2000) {
            v.x = 0.0f;
            v.z = 0.0f;
            v.y = 133.12f;
        }
        CMtxF__SetRotateY(&m, target->m_RotY + 3.1415927f);
        break;
    case 2:
        obj = target;
        switch (obj->header.m_nObjType) {
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
            dir = obj->m_vVelocity;
            dir.y = 0.0f;
            break;
        default:
            if (flags & 0x400000) {
                CVector3__Subtract(&dir, &src->header.m_vPos, &obj->header.m_vPos);
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
    if (src->m_dwFlags & 0x300000) {
        if (((CPlayer *)src->m_AI.m_pParent)->m_pOnEnemy != 0) {
            CVector3__Scale(&v, &v, 0.1f);
        }
    }
    CMtxF__RotateVector(&m, &v, out);
}
