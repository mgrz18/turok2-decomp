/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A5588.c, func_002A5588, with Turok 2's layouts. */

#include "common.h"
#include "globals/scene.h"
#include "globals/particle.h"
#include "structs/particle.h"
#include "structs/swoosh.h"

/* The particle a trail follows (CParticle): its effect's flags (bit 31: ..., 0x10000: a trail
 * keyed by the particle's owner and type), its type, owner and trail count. */
typedef CParticle TrailActor;

typedef struct TrailInfo {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ s16 sound;
    /* 0x12 */ u8 pad12[0xD];
    /* 0x1F */ u8 tex0;
    /* 0x20 */ u8 tex1;
} TrailInfo;

typedef Swoosh Trail;
typedef SwooshNode TrailNode;
typedef CSwooshSystem TrailMgr;

void CMtxF__Identity(void *m);
void func_0026D230(void *p);
Trail *NodePool_Alloc(SwooshPool *list);
void func_002316A0(void *a, void *b);
TrailInfo *CScene__GetSwoosh(void *tbl, s32 arg);
void func_00207914(s32 *p, s32 v);
TrailNode *Swoosh_NewNode(void *o, Trail *s, void *m);

/* Adds a node to the actor's trail, starting the trail first when the actor has none. */
void Trail_AddNode(TrailMgr *mgr, TrailActor *a, s32 type) {
    u8 m[0x40];
    Trail *s;
    register Trail *q asm("$5");
    s32 qid;
    f32 start;
    TrailInfo *t;
    TrailNode *node;
    void *tex0;
    void *tex1;
    void *key;
    s32 id;

    if (a->m_pEffect->m_dwFlags & 0x10000) {
        key = a->m_pOwner;
        id = a->header.m_nObjType;
        s = NULL;
        q = mgr->m_SwooshPool.used.head;
        while (q != NULL) {
            if (q->owner == key) {
                qid = q->id;
                s = q;
                if (qid == id) {
                    break;
                }
            }
            q = q->next;
            s = NULL;
        }
    } else {
        s = NULL;
    }
    if (s == NULL) {
        s = NodePool_Alloc(&mgr->m_SwooshPool);
        if (s == NULL) {
            return;
        }
        t = CScene__GetSwoosh(&gScene, type);
        tex0 = sTrailTextures[t->tex0];
        tex1 = sTrailTextures[t->tex1];
        func_0026D230(&s->nodes);
        s->flags = 2;
        if ((s32) a->m_pEffect->m_dwFlags < 0) {
            s->flags = 6;
        }
        if (a->m_pEffect->m_dwFlags & 0x10000) {
            s->flags |= 8;
            s->owner = a->m_pOwner;
            s->id = a->header.m_nObjType;
        } else {
            s->owner = a;
            s->id = -1;
        }
        s->info = t;
        s->tex0 = tex0;
        s->tex1 = tex1;
        start = t->unk0;
        *(s32 *) &s->unk28 = 0;
        s->time = start;
        s->layer = t->sound;
        *(s32 *) &s->length = 0;
        s->unk50 = 0;
        *(s32 *) &s->unk2C = 0;
        *(s32 *) &s->unk30 = 0;
        s->unk38 = -1;
        if (s->layer != -1) {
            func_00207914(s->texture, s->layer);
            if (a != NULL) {
                func_002316A0(a, s->texture);
            }
        }
        if (s == NULL) {
            return;
        }
    }
    a->m_ActiveSwooshes++;
    if (a->m_pEffect->m_dwFlags & 0x10000) {
        CMtxF__Identity(m);
        node = Swoosh_NewNode(mgr, s, m);
        if (node != NULL) {
            node->owner = a;
        }
    }
}
