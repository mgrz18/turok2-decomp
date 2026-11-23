#include "structs/vector.h"
#include "globals/game.h"
#include "globals/scene.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00281A70.c, func_00281A70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
typedef struct B18Ent B18Ent;
/* Turok 2 layout (Rage Wars offsets in parentheses). */
struct B18Ent {
    u8 pad0[2];
    u16 type;           /* 0x002 (0x004) */
    CVector3 pos;          /* 0x004 (0x008) */
    u8 pad10[0xFC];
    B18Ent *owner;      /* 0x10C (0x12C) */
    u8 pad110[0x18];
    s16 unk128;         /* 0x128 (0x14C) */
    u8 pad12A[0x1A];
    s32 health;         /* 0x144 (0x174) */
    u8 pad148[0x3DC];
    s32 unk524;         /* 0x524 (0x5E4) */
    u8 pad528[0xDF8];
    B18Ent *next;       /* 0x1320 (0x16E0) */
};
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
#ifdef TARGET_PORT
#include "structs/particle.h"
#include "structs/player.h"
/* the host structs by member: kind-1 instances (m_AI.m_Health), the players (m_Data.m_Health,
 * m_pNext), the particles (m_pOwner, m_nFrames, the type) */
void CParticle__CheckProximity(sptr unused, B18Ent *ov, B18Ent **arrv, s32 n) {
    CParticle *o = (CParticle *) ov;
    CParticle **arr = (CParticle **) arrv;
    CVector3 d;
    s32 i;
    s32 cnt;
    CScene *scene = &gScene;
    CPlayer *p;

    cnt = scene->m_nKind1;
    for (i = 0; i < cnt; i++) {
        CGameObjectInstance *e = scene->m_pKind1[i];
        if (e->m_AI.m_Health > 0) {
            CVector3__Subtract(&d, &e->header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
    for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if ((void *) o->m_pOwner != (void *) p && p->m_Data.m_Health > 0) {
            CVector3__Subtract(&d, &p->m_CharacterActor.header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
    for (i = 0; i < n; i++) {
        CParticle *e = arr[i];
        if (e != o && (e->header.m_nObjType == 0xC || e->header.m_nObjType == 0x22)) {
            CVector3__Subtract(&d, &e->header.m_vPos, &o->header.m_vPos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
                o->m_nFrames = 1;
                return;
            }
        }
    }
}
#else
void CParticle__CheckProximity(sptr unused, B18Ent *o, B18Ent **arr, s32 n) {
    CVector3 d;
    s32 i;
    s32 cnt;
    CScene *scene = &gScene;
    cnt = scene->m_nKind1;
    for (i = 0; i < cnt; i++) {
        B18Ent *e = (B18Ent *)scene->m_pKind1[i];
        if (e->health > 0) {
            CVector3__Subtract(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
    {
        B18Ent *p;
        for (p = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; p != ((void *)0); p = p->next) {
            if (o->owner != p && p->unk524 > 0) {
                CVector3__Subtract(&d, &p->pos, &o->pos);
                if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                    o->unk128 = 1;
                    return;
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        B18Ent *e = arr[i];
        if (e != o && (e->type == 0xC || e->type == 0x22)) {
            CVector3__Subtract(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
}
#endif
