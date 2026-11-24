#include "common.h"
#include "structs/swoosh.h"

/* The port: a DList (structs/swoosh.h SwooshLinks) with host pointers. */
void DList_Init(SwooshLinks *arg0) {
    arg0->head = NULL;
    arg0->tail = NULL;
    arg0->count = 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de_8.c, func_802794C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void DList_AddHead(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    DListLink *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->next = l->head;
        n->prev = NULL;
        ((DListLink *)l->head)->prev = n;
        l->head = n;
    }
    l->count++;
}

s32 DList_AddTail(void *arg0, void *arg1) {
    SwooshLinks *l = arg0;
    DListLink *n = arg1;

    if (l->count == 0) {
        l->head = n;
        l->tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->prev = l->tail;
        n->next = NULL;
        ((DListLink *)l->tail)->next = n;
        l->tail = n;
    }
    return ++l->count;
}

#include "rw/junk_types.h"

/* Unlinks arg1 from the list (DLNode: prev at 0, next at 4; DLList: head at 0, tail at 4). */
void DList_Remove(DLList *arg0, DLNode *arg1) {
    DLNode *temp_v1;
    DLNode *temp_v1_2;

    temp_v1 = arg1->next;
    if (temp_v1 != NULL) {
        temp_v1->prev = arg1->prev;
    }
    temp_v1_2 = arg1->prev;
    if (temp_v1_2 != NULL) {
        temp_v1_2->next = arg1->next;
    }
    if (arg0->head == arg1) {
        arg0->head = arg1->next;
    }
    if (arg0->tail == arg1) {
        arg0->tail = arg1->prev;
    }
    arg0->count = (s32) (arg0->count - 1);
}

void NodePool_Init(NodePool *p, DLNode *mem, s32 stride, u32 n) {
    DLList *u;
    p->freel.head = 0;
    p->freel.tail = 0;
    p->freel.count = 0;
    u = &p->usedl;
    u->head = 0;
    u->tail = 0;
    u->count = 0;
    while (n--) {
        if ((&p->freel)->count == 0) { (&p->freel)->head = (mem); (&p->freel)->tail = (mem); (mem)->prev = 0; (mem)->next = 0; } else { (mem)->prev = (&p->freel)->tail; (mem)->next = 0; (&p->freel)->tail->next = (mem); (&p->freel)->tail = (mem); } (&p->freel)->count++;
        mem = (DLNode *)((u8 *)mem + stride);
    }
}

#ifdef TARGET_PORT

/* Takes the free list's head into the used list's tail. */
s32 *NodePool_Alloc(void *arg0) {
    SwooshPool *p = arg0;
    DListLink *n = p->free.head;

    if (n != NULL) {
        if (n->next != NULL) n->next->prev = n->prev;
        if (n->prev != NULL) n->prev->next = n->next;
        if (p->free.head == n) p->free.head = n->next;
        if (p->free.tail == n) p->free.tail = n->prev;
        p->free.count--;
        if (p->used.count == 0) {
            p->used.head = n;
            p->used.tail = n;
            n->prev = NULL;
            n->next = NULL;
        } else {
            n->prev = p->used.tail;
            n->next = NULL;
            ((DListLink *)p->used.tail)->next = n;
            p->used.tail = n;
        }
        p->used.count++;
    }
    return (s32 *)n;
}
#else
s32 *NodePool_Alloc(void *arg0)
{
  s32 *temp_a1;
  s32 *temp_a1_2;
  s32 *temp_v1;
  void *temp_a1_3;
  s32 *new_var;
  temp_v1 = *((s32 **) (((s8 *) arg0) + 0));
  if (temp_v1 != 0)
  {
    temp_a1 = *((s32 **) (((s8 *) temp_v1) + 4));
    if (temp_a1 != 0)
    {
      *temp_a1 = *((s32 **) (((s8 *) temp_v1) + 0));
    }
    temp_a1_2 = *((s32 **) (((s8 *) temp_v1) + 0));
    if (temp_a1_2 != 0)
    {
      *((s32 **) (((s8 *) temp_a1_2) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 0))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 0)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 4)));
    }
    if ((*((s32 **) (((s8 *) arg0) + 4))) == temp_v1)
    {
      *((s32 **) (((s8 *) arg0) + 4)) = (s32 *) (*((s32 **) (((s8 *) temp_v1) + 0)));
    }
    temp_a1_3 = arg0 + 0xC;
    *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) - 1);
    if ((*((s32 *) (((s8 *) temp_a1_3) + 8))) == 0)
    {
      *((s32 **) (((s8 *) arg0) + 0xC)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_v1) + 0)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
    }
    else
    {
      new_var = *((s32 **) (((s8 *) temp_a1_3) + 4));
      *((s32 **) (((s8 *) temp_v1) + 4)) = 0;
      *((s32 **) (((s8 *) temp_v1) + 0)) = new_var;
      *((s32 **) (((s8 *) (*((s32 **) (((s8 *) temp_a1_3) + 4)))) + 4)) = temp_v1;
      *((s32 **) (((s8 *) temp_a1_3) + 4)) = temp_v1;
    }
    *((s32 *) (((s8 *) temp_a1_3) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a1_3) + 8))) + 1);
  }
  return temp_v1;
}
#endif
#ifdef TARGET_PORT

/* Unlinks arg1 from the used list, appends it to the free list. */
void NodePool_Free(void *arg0, void *arg1) {
    SwooshPool *p = arg0;
    DListLink *n = arg1;

    if (n->prev != NULL) n->prev->next = n->next;
    if (n->next != NULL) n->next->prev = n->prev;
    if (p->used.head == n) p->used.head = n->next;
    if (p->used.tail == n) p->used.tail = n->prev;
    p->used.count--;
    if (p->free.count == 0) {
        p->free.head = n;
        p->free.tail = n;
        n->prev = NULL;
        n->next = NULL;
    } else {
        n->prev = p->free.tail;
        n->next = NULL;
        ((DListLink *)p->free.tail)->next = n;
        p->free.tail = n;
    }
    p->free.count++;
}
#else
void NodePool_Free(void *arg0, void *arg1)
{
  void **temp_v1;
  void *temp_a2;
  s8 *new_var;
  void *temp_v1_2;
  void *new_var2;
  temp_v1 = *((void ***) (((s8 *) arg1) + 4));
  temp_a2 = arg0 + 0xC;
  if (temp_v1 != 0)
  {
    *temp_v1 = *((void **) (((s8 *) arg1) + 0));
  }
  temp_v1_2 = *((void **) (((s8 *) arg1) + 0));
  if (temp_v1_2 != 0)
  {
    *((void ***) (((s8 *) temp_v1_2) + 4)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void ***) (((s8 *) arg0) + 0xC))) == arg1)
  {
    *((void ***) (((s8 *) arg0) + 0xC)) = (void **) (*((void ***) (((s8 *) arg1) + 4)));
  }
  if ((*((void **) (((s8 *) temp_a2) + 4))) == arg1)
  {
    *((void **) (((s8 *) temp_a2) + 4)) = (void *) (*((void **) (((s8 *) arg1) + 0)));
  }
  *((s32 *) (((s8 *) temp_a2) + 8)) = (s32) ((*((s32 *) (((s8 *) temp_a2) + 8))) - 1);
  if ((*((s32 *) (((s8 *) arg0) + 8))) == 0)
  {
    *((void **) (((s8 *) arg0) + 0)) = arg1;
    *((void **) (((s8 *) arg0) + 4)) = arg1;
    *((void **) (((s8 *) arg1) + 0)) = 0;
    *((void ***) (((s8 *) arg1) + 4)) = 0;
  }
  else
  {
    new_var2 = *((void **) (((s8 *) arg0) + 4));
    *((void ***) (((s8 *) arg1) + 4)) = 0;
    *((void **) (((s8 *) arg1) + 0)) = (void *) new_var2;
    new_var = ((s8 *) arg0) + 4;
    *((void **) (((s8 *) (*((void **) new_var))) + 4)) = arg1;
    *((void **) new_var) = arg1;
  }
  *((s32 *) (((s8 *) arg0) + 8)) = (s32) ((*((s32 *) (((s8 *) arg0) + 8))) + 1);
}
#endif

s16 WeightedTable_PickStatic(s16 *arg0);
extern s32 Random_Next(void);

s16 WeightedTable_PickStatic(s16 *arg0) {
    s16 *entry;
    s16 *start;
    s32 random;
    s32 total;
    s32 value;
    entry = arg0;
    total = 0;
    start = entry;
    if (*entry != -1) {
        do {
            total += entry[1];
            entry += 2;
        } while (*entry != -1);
    }
    random = Random_Next();
    entry = start;
    value = random % total;
    total = 0;
    while (*entry != -1) {
        total += entry[1];
        if (total >= value) {
            break;
        }
        entry += 2;
    }
    return *entry;
}

#include "m2c_macros.h"

void WeightedTable_Clear(s16 *arg0) {
    *arg0 = 0;
}

typedef struct Pair16 {
                s16 a;
                s16 b;
} Pair16;
typedef struct PBuf {
    s16 n;
    Pair16 e[0x18];
} PBuf;

s32 WeightedTable_Add(PBuf *b, s16 x, s16 y) {
    if (b->n == 0x18) {
        return 0;
    }
    b->e[b->n].a = x;
    b->e[b->n].b = y;
    b->n++;
    return 1;
}

s16 WeightedTable_Pick(WeightedTable *arg0) {
    s16 *entry;
    s16 *start;
    s32 random;
    s32 total;
    s32 value;
    if (arg0->count == 0) {
        return -1;
    }
    arg0->entries[arg0->count][0] = -1;
    entry = &arg0->entries[0][0];
    total = 0;
    start = entry;
    if (*entry != -1) {
        do {
            total += entry[1];
            entry += 2;
        } while (*entry != -1);
    }
    random = Random_Next();
    entry = start;
    value = random % total;
    total = 0;
    while (*entry != -1) {
        total += entry[1];
        if (total >= value) {
            break;
        }
        entry += 2;
    }
    return *entry;
}

#include "structs/vector.h"
#include "structs/player.h"
#include "globals/scene.h"
#include "protos/game.h"
#ifdef TARGET_PORT
int T2Port_AnimEventsEnabled(void); /* port/game/t2actors.c */
#endif

/* Animation attachment events: for each attachment whose event bit is enabled
 * on the object, moves its offset through the bone matrix to world space and
 * plays the event: a footstep/impact sound (louder when the bone's surface is
 * water), the splash effects, or a dust puff of 5/10/15/20 units. */

typedef struct {
    /* 0x00 */ CVector3 offset;
    /* 0x0C */ u32 event;
    /* 0x10 */ u16 bone;
    /* 0x12 */ u8 pad12[0x14 - 0x12];
} Attachment;
typedef struct {
    s16 lifeBase;
    s16 lifeRange;
    s8 chance;
    s8 modifier;
    s8 countBase;
    s8 countRange;
} __attribute__((aligned(4))) PuffArgs;
extern u8 *CISet__GetBlock(void *list, s32 index);
extern void CMtxF__TransformPoint(CMtxF *mtx, CVector3 *in, CVector3 *out);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
extern void Actor_SpawnParticle(CGameObjectInstance *obj, s32 sound, CVector3 pos, s32 flags);
extern void GameObject_TraceMove(CGameObjectInstance *obj, CVector3 pos, CVector3 *out, s32 *region, s32 a, s32 b);
extern f32 CGameObjectInstance__GetObjectScale(CGameObjectInstance *obj);
extern void ImpactEvent_Send(CGameObjectInstance *obj, CGameObjectInstance *owner, s32 type, s32 id, CVector3 pos, PuffArgs args);

void GameObject_EmitHotPoints(Attachment *ents, s32 n, CMtxF *mtxs, CGameObjectInstance *obj, void *bones) {
    CVector3 pos;
    CVector3 mapped;
    CVector3 zero;
    CQuaternion rot;
    PuffArgs puff;
    sptr region;
    f32 len;
    s32 kind;
    s32 flag;
    s32 size;

#ifdef TARGET_PORT
    /* the hot-point effects end in CParticleSystem__Spawn: on by default, T2_ANIM_EVENTS=0
     * turns them off (see CGameObjectInstance__FireAnimEvents) */
    if (!T2Port_AnimEventsEnabled()) {
        return;
    }
#endif
    for (n = n - 1; n != -1; n--) {
        if (ents->event & obj->m_dwHotPointFlags) {
            flag = (CISet__GetBlock(CISet__GetBlock(bones, ents->bone), 1)[0x66] == 5) * 2;
            size = 0;
            CMtxF__TransformPoint(&mtxs[ents->bone], &ents->offset, &pos);
            CVector3__Scale(&pos, &pos, 1.0f / 65536.0f);
            switch (ents->event) {
            case 0x2:
                Actor_SpawnParticle(obj, 0x51, pos, flag);
                break;
            case 0x8:
                Actor_SpawnParticle(obj, 0x5C, pos, flag);
                break;
            case 0x1:
            case 0x4:
            case 0x10:
            case 0x20:
                break;
            case 0x40:
                obj->m_AI.m_vBoreTarget = pos;
                kind = obj->header.m_pIntelligence->m_Type;
                if ((kind == 1 && obj->m_AI.m_Mode == 0x21) || (kind == 0xB && ((CPlayer *) obj->m_AI.m_pParent)->m_Mode == 0x23)) {
                    GameObject_TraceMove(obj, pos, &mapped, &region, 0, 1);
                    CQuaternion__FromDirection(&rot, &obj->m_AI.m_vBoreCollNormal);
                    zero.x = 0;
                    zero.y = 0;
                    zero.z = 0;
                    if (obj->m_AI.m_ModeTime < 15.0f) {
                        CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, 0, 0, 0xE6, zero, rot, mapped, region, -1, flag | 1);
                    }
                    CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, 0, 0, 0x8A, zero, rot, mapped, region, -1, flag | 1);
                }
                break;
            case 0x80:
                Actor_SpawnParticle(obj, 0xAC, pos, flag);
                break;
            case 0x100:
                Actor_SpawnParticle(obj, 0xAD, pos, flag);
                break;
            case 0x200:
                Actor_SpawnParticle(obj, 0xAE, pos, flag);
                break;
            case 0x400:
                Actor_SpawnParticle(obj, 0xAF, pos, flag);
                break;
            case 0x800:
                Actor_SpawnParticle(obj, 0xB0, pos, flag);
                break;
            case 0x1000:
                Actor_SpawnParticle(obj, 0xB1, pos, flag);
                break;
            case 0x2000:
                Actor_SpawnParticle(obj, 0xB2, pos, flag);
                break;
            case 0x4000:
                Actor_SpawnParticle(obj, 0xB3, pos, flag);
                break;
            case 0x8000:
                Actor_SpawnParticle(obj, 0xB4, pos, flag);
                break;
            case 0x10000:
                Actor_SpawnParticle(obj, 0xB5, pos, flag);
                break;
            case 0x20000:
                Actor_SpawnParticle(obj, 0xB6, pos, flag);
                break;
            case 0x40000:
                Actor_SpawnParticle(obj, 0xB7, pos, flag);
                break;
            case 0x80000:
                Actor_SpawnParticle(obj, 0xB8, pos, flag);
                break;
            case 0x100000:
                Actor_SpawnParticle(obj, 0xB9, pos, flag);
                break;
            case 0x200000:
                Actor_SpawnParticle(obj, 0xBA, pos, flag);
                break;
            case 0x400000:
                Actor_SpawnParticle(obj, 0xBB, pos, flag);
                break;
            case 0x20000000:
                Actor_SpawnParticle(obj, 0x1E9, pos, flag);
                break;
            case 0x40000000:
                Actor_SpawnParticle(obj, 0x1EA, pos, flag);
                break;
            case 0x80000000:
                Actor_SpawnParticle(obj, 0x1EB, pos, flag);
                break;
            case 0x8000000:
            case 0x10000000:
                Actor_SpawnParticle(obj, 0x11C, pos, flag);
                break;
            case 0x800000:
                size = 5;
                break;
            case 0x2000000:
                size = 0xF;
                break;
            case 0x1000000:
                size = 0xA;
                break;
            case 0x4000000:
                size = 0x14;
                break;
            }
            if (size != 0) {
                len = CGameObjectInstance__GetObjectScale(obj);
                puff.lifeRange = size << 8;
                puff.chance = puff.modifier = puff.countBase = puff.countRange = 0;
                {
                    f32 t = 5.0f;
                    t = t / len;
                    puff.lifeBase = t * 256.0f;
                }
                ImpactEvent_Send(obj, obj, 3, -1, pos, puff);
            }
        }
        ents++;
    }
}
