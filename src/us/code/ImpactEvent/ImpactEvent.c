#include "structs/vector.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/particle.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00265E30.c, func_00265E30.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/world.h"

typedef struct M04Part {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) M04Part;
typedef void (*M04Handler)(void *, void *, s32, CVector3, M04Part);
typedef struct M04WorldB {
                u8 pad0[0xBF8];
                void *tab[1];
} M04WorldB;

/* Dispatch one message through the handler table (inlined). */
static __inline__ void send_(void *tgt, void *dst, s32 kind, CVector3 p, M04Part q) {
    if (gImpactEventHandlers[kind].handler != 0) {
        ((M04Handler) gImpactEventHandlers[kind].handler)(tgt, dst, kind, p, q);
    }
}

void ImpactEvent_Send(void *src, void *dst, s32 kind, s32 id, CVector3 pos, M04Part part) {
    s32 i;
    s32 n;
    u8 *p;
    void **pp;
    void *cur;
    u8 *base;
    CPlayer *o;
    if (gScene.m_Mode == 4 || (gImpactEventHandlers[kind].flags & 0x10)) {
        if (!(gImpactEventHandlers[kind].flags & 2)) {
            send_(src, dst, kind, pos, part);
        } else if (gImpactEventHandlers[kind].flags & 8) {
            n = gScene.m_nActors;
            base = (u8 *)gScene.m_pActors;
            i = 0;
            if (n > 0) {
                do {
                    cur = base + i * (s32)sizeof(CGameObjectInstance);
                    if (cur != src) {
                        send_(cur, dst, kind, pos, part);
                    }
                    i++;
                } while (i < n);
            }
            o = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
            if (o != 0) {
                do {
                    if ((void *)o != src && (void *)&o->m_WeaponActor != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->m_pNext;
                } while (o != 0);
            }
        } else {
#ifdef TARGET_PORT
            /* N64 0xDF8 / 0xBF8: the registered instances by member (pointer-wide on the host) */
            n = gScene.m_nRegistered;
            for (i = 0; i < n; i++) {
                cur = (void *) gScene.m_pRegistered[i];
#else
            n = *(s32 *)((u8 *)&gScene + 0xDF8);
            for (i = 0; i < n; i++) {
                cur = ((M04WorldB *)&gScene)->tab[i];
#endif
                if (cur != src) {
                    send_(cur, dst, kind, pos, part);
                }
            }
            o = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
            if (o != 0) {
                do {
                    if ((void *)o != src && (void *)&o->m_WeaponActor != src) {
                        send_(o, dst, kind, pos, part);
                    }
                    o = o->m_pNext;
                } while (o != 0);
            }
        }
    }
}

typedef struct B15Def {
               s32 type;
               u8 pad4[0xA];
               s8 unkE;
} B15Def;
typedef struct B15Obj B15Obj;
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/particle.h"
/* The port: the same fields at their host offsets. The view reads a CGameObjectInstance (type 1)
 * and, for a particle target (type 2), the CParticle's effect (0xF8) and owner (0x10C). */
#define T2_B15_GOI(m) __builtin_offsetof(CGameObjectInstance, m)
#define T2_B15_PART(m) __builtin_offsetof(CParticle, m)
struct B15Obj {
    union {
        struct { u8 type; };
        struct { u8 _hp_unk4[T2_B15_GOI(header.m_nObjType)]; u16 unk4; };
        struct { u8 _hp_def[T2_B15_GOI(header.m_pIntelligence)]; B15Def *def; };
        struct { u8 _hp_unkE4[T2_B15_GOI(m_nTypeFlag)]; u16 unkE4; };
        struct { u8 _hp_flags[T2_B15_GOI(m_dwFlags)]; u32 flags; };
        struct { u8 _hp_unk118[T2_B15_PART(m_pEffect)]; s32 *unk118; };
        struct { u8 _hp_unk12C[T2_B15_PART(m_pOwner)]; B15Obj *unk12C; };
    };
};
#else
struct B15Obj {               /* Turok 2 layout (Rage Wars offsets in comments) */
                u8 type;
                u8 pad1;
                u16 unk4;             /* 0x002 (0x004) */
                u8 pad4[0x10];
                B15Def *def;          /* 0x014 (0x018) */
                u8 pad18[0xA0];
                u16 unkE4;            /* 0x0B8 (0x0E4): type id */
                u8 padBA[0x1A];
                u32 flags;            /* 0x0D4 (0x100) */
                u8 padD8[0x20];
                s32 *unk118;          /* 0x0F8 (0x118): parent */
                u8 padFC[0x10];
                B15Obj *unk12C;       /* 0x10C (0x12C) */
};
#endif

s32 GameObject_IsSolid(B15Obj *a);

static __inline__ s32 chk(B15Obj *a, B15Obj *t) {
    u32 f;
    if (gEngineApp_m_Settings.multiplayer != 0) {
        return 1;
    }
    if (t->unkE4 == a->unkE4) {
        return 0;
    }
    f = a->flags;
    if (!(f & 0x310000)) {
        return 0;
    }
    if (gScene.m_bStreamAll != 0 && !(t->flags & 0x300000) && !(f & 0x300000)) {
        return 0;
    }
    switch (a->def->type) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        return 1;
    }
    return 0;
}

static __inline__ s32 chk3(B15Obj *a, B15Obj *t) {
    u32 f;
    register s32 res asm("$3");
    if (gEngineApp_m_Settings.multiplayer != 0) {
        res = 1;
        goto done;
    }
    if (t->unkE4 == a->unkE4) {
        res = 0;
        goto done;
    }
    f = a->flags;
    if (!(f & 0x310000)) {
        res = 0;
        goto done;
    }
    if (gScene.m_bStreamAll != 0 && !(t->flags & 0x300000) && !(f & 0x300000)) {
        res = 0;
        goto done;
    }
    switch (a->def->type) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        res = 1;
        goto done;
    }
    res = 0;
done:
    return res;
}

s32 Blast_CanHit(B15Obj *a, B15Obj *t)
{
  B15Obj *p;
  B15Obj *u;
  if (a == t)
  {
    return 0;
  }
  if ((a->flags & 0x300000) && (!GameObject_IsSolid(a)))
  {
    return 0;
  }
  if (t->type != 1)
  {
    if (t->type != 2)
    {
      return 1;
    }
    p = t->unk12C;
    u = t;
    if (p != ((void *) 0))
    {
      if ((p->type == 1) && (!chk(a, p)))
      {
        return 0;
      }
      if ((a == u->unk12C) && ((*u->unk118) & 0x40))
      {
        return 0;
      }
    }
    if (((u->unk4 != 0x14) || (a->def->type != 1)) || (a->unkE4 != 0xCA))
    {
      return 1;
    }
    return 0;
  }
  if (chk3(a, t))
  {
    return 1;
  }
  if (a || t)
  {
 do { } while (0);
    return 0;
  }
  else
  {
    return 0;
  }
}

#include "structs/particle_event.h"

void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CMtxF__Identity(CMtxF *m);
void CMtxF__RotateVector(CMtxF *m, CVector3 *in, CVector3 *out);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *m);

/* The blast's push from `src` turned into a world vector, in the frame of `target`. */
void Blast_GetPush(CGameObjectInstance *src, CGameObjectInstance *target, CImpactBlast blast, CVector3 *out, u32 flags) {
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

#include "structs/intelligence.h"
#include "protos/scene.h"
#include "protos/game.h"
#include "protos/impact_event.h"

/* Applies a hit from an attacker to a target: marks the attacker, shakes the
 * target player's camera for heavy and medium hits, sprays blood from a
 * player attacker for blade kinds 4 and 5, reports it, and pushes the target
 * along the hit direction. */

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 offset;
} PlayerCamera;
typedef struct {
    /* 0x00 */ u8 pad0[0x66];
    /* 0x66 */ u8 kind;
} HitDef;
extern void CCamera__SetShake(PlayerCamera *camera, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos);
extern void CPlayer__TransformByView(CPlayer *player, CVector3 *v);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);       /* out = a + b */
extern f32 AI_AngleToPoint(CGameObjectInstance *actor, CVector3 point);

/* Its unused target and def parameters still take stack slots when inlined (the frame layout needs them). */
static __inline__ void sprayBlood(CGameObjectInstance *target, CGameObjectInstance *actor, CVector3 position, CVector3 direction, HitDef *def, s32 kind) {
    CPlayer *player;
    CVector3 v;
    CVector3 point;
    CVector3 zero;
    CQuaternion rotation;
    sptr region;

    player = ((CPlayer *)actor->m_AI.m_pParent);
    v.x = -direction.x;
    v.y = direction.y;
    v.z = direction.z;
    CPlayer__TransformByView(player, &v);
    v.x -= ((PlayerCamera *)player->m_pCamera)->offset.x;
    v.y -= ((PlayerCamera *)player->m_pCamera)->offset.y;
    v.z -= ((PlayerCamera *)player->m_pCamera)->offset.z;
    CQuaternion__FromDirection(&rotation, &v);
    GameObject_TraceMove(actor, position, &point, &region, 0, 1);
    point = position;
    point.y += 12.0f;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, 0, 0, 0xE9, zero, rotation, point, region, -1, kind == 5 ? 3 : 1);
    point.x = 0;
    point.y = 0;
    point.z = 0;
    CScene__StartRumbles(&gScene, player->m_ContState.m_pContState->rumblePool, 3, point, 10.24f, 1.0f);
}

void Blast_ApplyHit(CGameObjectInstance *target, CGameObjectInstance *source, s32 hit, CVector3 pos, CVector3 dir, s32 push, HitDef *def, u32 flags) {
    CGameObjectInstance *attacker;
    u32 bits;
    CPlayer *player;

    if (source->header.m_Type == 1) {
        source->m_AI.m_dwFlags |= 0x4000000;
        attacker = source;
        if (target->m_dwFlags & 0x300000) {
            bits = source->m_AI.m_ModeFlags;
            player = ((CPlayer *)target->m_AI.m_pParent);
            if (bits & 0x4000) {
                CCamera__SetShake(((PlayerCamera *)player->m_pCamera), 20.0f, 100.0f, 20.0f, 1.0f, 1, pos);
            } else if (bits & 0x2000) {
                CCamera__SetShake(((PlayerCamera *)player->m_pCamera), 10.0f, 50.0f, 10.0f, 1.0f, 1, pos);
            }
        }
        if (hit) {
            if (def != 0 && (attacker->m_dwFlags & 0x300000) && (def->kind == 4 || def->kind == 5)) {
                sprayBlood(target, source, pos, dir, def, def->kind);
            }
            dir.x = 0;
            dir.y = 0;
            dir.z = 0;
        }
        if ((attacker->m_dwFlags & 0x300000) && (flags & 0x400000)) {
            GameObject_FireTriggers(target, 0x10, attacker);
        }
    }
    if (target->header.m_pIntelligence->m_Type == 1 || target->header.m_pIntelligence->m_Type == 11) {
        CVector3__Add(&target->m_vVelocity, &target->m_vVelocity, &dir);
        if (push) {
            target->m_RotY += AI_AngleToPoint(target, source->header.m_vPos) * 0.5f;
        }
    }
}

#include "structs/collision.h"
#include "globals/collision.h"
#include "globals/player.h"

void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *actor, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *actor);
f32 CVector3__Distance(CVector3 *a, CVector3 *b);
void DamageEvent_Set(DamageEvent *out, void *source, s32 damage, f32 radius, u32 flags, HitFrame *shield);
void CAI__CallHurt(CGameObjectInstance *victim, void *ai, DamageEvent *hit);

/* How strongly the blast reaches the victim: through its shield, and with
 * falloff over the radius plus its width from the nearest point of its body. */
static __inline__ f32 blastScale(CGameObjectInstance *victim, CGameObjectInstance *source, CVector3 position, f32 radius, s32 falloff, u32 flags, HitFrame **found) {
    s32 shielded;
    s32 scaled;
    HitFrame *shield;
    f32 scale;
    f32 bottom;
    f32 top;
    f32 reach;
    f32 distance;
    CVector3 body;

    shielded = 1;
    scale = 1.0f;
    scaled = 1;
    if (gEngineApp_m_Settings.multiplayer != 0 && (flags & 0x400000)) {
        shielded = 0;
    }
    if (source->header.m_Type == 2 && (CGameObjectInstance *)gCollisionResults.m_pInstance == victim && gCollisionResults.m_nInstanceNode != -1) {
        shield = &gCollisionResults.m_Frame;
        falloff = 0;
    } else {
        shield = CGameObjectInstance__GetNearestBox(victim, position, &sBlastShieldFrame) ? &sBlastShieldFrame : NULL;
    }
    *found = shield;
    if (shielded && shield != NULL) {
        scale *= shield->damageFactor;
    }
    if (falloff != 0) {
        body = victim->header.m_vPos;
        bottom = victim->header.m_vPos.y + victim->m_CollisionYOffset;
        top = bottom + GameObject_GetCollisionHeight(victim);
        if (top < position.y) {
            body.y = top;
        } else if (position.y < bottom) {
            body.y = bottom;
        } else {
            body.y = position.y;
        }
        reach = radius + GameObject_GetCollisionRadius(victim);
        distance = CVector3__Distance(&position, &body);
        if (reach < distance) {
            return 0.0f;
        }
        if (scaled) {
            scale *= 1.0f - distance / reach;
        }
    }
    return scale;
}

/* Applies a blast from source to victim: a player's own close-range shot
 * (no weapon, or weapon 0x22) is moved most of the way back to the player;

 * then damage, push and the hit report, scaled by blastScale. */
void ImpactEvent_ApplyBlast(CGameObjectInstance *victim, CGameObjectInstance *source, s32 hit, CVector3 pos, CImpactBlast blast, s32 falloff, s32 push, u32 flags) {
    CVector3 dir;
    DamageEvent info;
    CPlayer *owner;
    HitFrame *shield;
    f32 mult;
    s32 close;
    f32 radius;
    f32 damage;

    if (Blast_CanHit(victim, source) == 0) {
        return;
    }
    hit = 0;
    if (source->header.m_Type == 1 && (source->m_dwFlags & 0x300000)) {
        owner = (CPlayer *)source->m_AI.m_pParent;
        /* The blast comes from the player's own weapon actor (round 23: the member, not the N64
         * offset 0x288, which on the host fell inside m_CharacterActor); the weapon is read
         * unsigned, then sign-extended for 0x22. */
        if (source == &owner->m_WeaponActor && ((u16)owner->m_Data.m_CurrentWeapon < 2 || (s16)(u16)owner->m_Data.m_CurrentWeapon == 0x22)) {
            CVector3 from;
            CVector3 to;

            from = pos;
            to = owner->m_CharacterActor.header.m_vPos;
            CVector3__Lerp(&pos, 0.75f, &from, &to);
            pos.y += GameObject_GetCollisionHeight(owner) * 0.5f;
            hit = 1;
        }
    }
    close = hit;
    radius = blast.radius * 0.04f;
    if (source->header.m_Type == 1) {
        mult = CGameObjectInstance__GetAverageScale(source);
    } else {
        mult = 1.0f;
    }
    radius *= mult;
    damage = blast.damage;
    damage *= blastScale(victim, source, pos, radius, falloff, flags, &shield);
    if (damage != 0.0f) {
        Blast_GetPush(victim, source, blast, &dir, flags);
        DamageEvent_Set(&info, source, damage, radius, flags, shield);
        CAI__CallHurt(victim, &victim->m_AI, &info);
        Blast_ApplyHit(victim, source, close, pos, dir, push, shield, flags);
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_80267198_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void ImpactEvent_Call(void *arg0, void *arg1, s32 arg2, CVector3 arg3, ResourceManagerState arg6);

void ImpactEvent_Call(void *arg0, void *arg1, s32 arg2, CVector3 arg3, ResourceManagerState arg6) {
    if (gImpactEventHandlers[arg2].handler != 0) {
        ((func_80267198_de_Callback) gImpactEventHandlers[arg2].handler)(arg0, arg1, arg2, arg3, arg6);
    }
}
void ImpactEvent_Call2(void *arg0, void *arg1, s32 arg2, CVector3 arg3, ResourceManagerState arg6);

void ImpactEvent_Call2(void *arg0, void *arg1, s32 arg2, CVector3 arg3, ResourceManagerState arg6) {
    if (gImpactEventHandlers[arg2].handler != 0) {
        ((func_80267198_de_Callback) gImpactEventHandlers[arg2].handler)(arg0, arg1, arg2, arg3, arg6);
    }
}

extern void CGameRegion__FloodClearFlag4(void *arg0);
extern void CGameRegion__FloodSetFlag4(void *arg0);

/* Host body: the position comes as a CVector3 (floats: SSE registers on x86-64). The Rage Wars
 * Triple is three s32, passed in integer registers, so with it every later argument of this
 * function and of GameObject_TraceMove arrived in the wrong register. */
void ImpactEvent_FloodRegionFlag4(void *arg0, CVector3 input, s32 arg4) {
    CVector3 output;
    void *region;

    GameObject_TraceMove((CGameObjectInstance *) arg0, input, &output, (s32 *) &region, 0, 0);
    if (arg4 != 0) {
        CGameRegion__FloodSetFlag4(region);
    } else {
        CGameRegion__FloodClearFlag4(region);
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

typedef struct ObjectKind {
    s32 value;
} ObjectKind;
typedef struct Object {
    u8 type;            /* 0x0 */
    u8 pad1[0x13];
    ObjectKind *kind;         /* 0x14 */
    u8 pad18[0x80 - 0x18];
    s32 value;          /* 0x80 */
} Object;
typedef struct PitchedSoundParams {
    s32 id;         /* 0x0 */
    s32 unk4;       /* 0x4 */
} PitchedSoundParams;
extern s32 Sound__PlayPitched(s32 id, CVector3 pos, s32 arg4, sptr selected, f32 scale);

void ImpactEvent_PlayPitchedSound(s32 arg0, Object *obj, s32 arg2, CVector3 pos, PitchedSoundParams params) {
    s32 *global = &gEngineApp_m_Settings.paused;
    sptr selected;
    f32 scale;

    scale = 1.0f;
    selected = -1;
    if (global[0] != 0) {
        return;
    }
    if (obj != NULL) {
        switch (obj->type) {
            case 1:
#ifdef TARGET_PORT
                /* the instance's intelligence (N64 0x14) by member (round 7 v2) */
                if (*(s32 *) ((CGameObjectInstance *) obj)->header.m_pIntelligence == obj->type) {
#else
                if (obj->kind->value == obj->type) {
#endif
                    if (global[-115] & 0x40) {
                        scale = 2.5f;
                    } else if (global[-115] & 0x20) {
                        scale = 0.35f;
                    }
                }
                selected = (sptr) obj;
                break;
            case 2:
                selected = (sptr) obj;
                break;
            case 0:
                selected = obj->value;
                break;
        }
    }
    if (params.id < 0x1CC) {
        if (params.id >= 0x1C2) {
            selected = -1;
        }
    }
    Sound__PlayPitched((s16) params.id, pos, 0, selected, scale);
}

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
} Quad; /* passed by value: starts in $a3, the rest on the stack */
extern void Sound__PlaySpeech(s32);

/* ABI (docs/port/ABI.md section 5, abicheck indirect struct_class/struct_scalar): the dispatcher
 * (ImpactEvent_Send/Call) passes (actor, actor, kind, CVector3 pos, 8-byte event value). On MIPS
 * the position and the value fill a3 and the stack words this definition names q (q.xC: the value); on the host
 * the vector travels in SSE/FP registers and the value in one general register, so the host
 * definition takes them as they are passed: the value's first word is what MIPS read. */
void ImpactEvent_PlaySound(s32 arg0, s32 arg1, s32 arg2, CVector3 pos, s32 value) {
    Sound__PlaySpeech(value);
}

#include "structs/triple.h"

extern void Actor_SpawnParticle(void *, s32, Triple, s32);

void ImpactEvent_SpawnParticle(void *actor,int unused1,int unused2,Triple input,int event) {
 Actor_SpawnParticle(actor,event,input,0);
}

extern void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);

s32 Blast_GetSourcePos(CGameObjectInstance *actor, CVector3 *pos) {
    s32 ret = 0;
    CPlayer *player;
    u16 state;
    CVector3 a;
    CVector3 b;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        player = ((CPlayer *)actor->m_AI.m_pParent);
        if (actor == &player->m_WeaponActor) {
            state = player->m_Data.m_CurrentWeapon;
            if (state < 2 || (s16) state == 0x22) {
                a = *pos;
                b = player->m_CharacterActor.header.m_vPos;
                CVector3__Lerp(pos, 0.75f, &a, &b);
                pos->y += GameObject_GetCollisionHeight(player) * 0.5f;
                ret = 1;
            }
        }
    }
    return ret;
}

typedef struct { s32 kind; } P3Kind;
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the members at their host offsets (round 11 C26: the N64 pads put the flags (0xD4) on host m_rpGeometryHeader/m_rpObjectHeader) */
typedef struct {
    union {
        struct { u8 _hpInfo[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; P3Kind *info; };
        struct { u8 _hpId[__builtin_offsetof(CGameObjectInstance, m_nTypeFlag)]; u16 id; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; u32 flags; };
    };
} P3Actor;
#else
typedef struct {
    u8 pad0[0x14];
    P3Kind *info;           /* 0x14 (0x18 in Rage Wars) */
    u8 pad18[0xB8 - 0x18];
    u16 id;                 /* 0xB8 (0xE4) */
    u8 padBA[0xD4 - 0xBA];
    u32 flags;              /* 0xD4 (0x100) */
} P3Actor;
#endif

s32 func_00238258(P3Actor *a, P3Actor *b) {
    if (gEngineApp_m_Settings.multiplayer != 0) {
        return 1;
    }
    if (a->id == b->id || !(b->flags & 0x310000)) {
        return 0;
    }
    if (gScene.m_bStreamAll != 0 && !(a->flags & 0x300000) && !(b->flags & 0x300000)) {
        return 0;
    }
    switch (b->info->kind) {
    case 1:
    case 4:
    case 7:
    case 0xB:
        return 1;
    }
    return 0;
}

#include "m2c_macros.h"

f32 Blast_GetSourceScale(u8 *arg0) {
    if (*arg0 == 1) {
        return CGameObjectInstance__GetAverageScale((sptr) arg0);
    }
    return 1.0f;
}

s32 CGameObjectInstance__GetNearestBox(CGameObjectInstance *a, CVector3 pos, HitFrame *out);
f32 GameObject_GetCollisionRadius(CGameObjectInstance *a);

/* How much of a blast at `p` reaches `a`: its armour, then (with falloff) a linear drop over
 * the distance to the nearest point of its body, out to `reach` plus its own size. */
f32 Blast_GetReach(CGameObjectInstance *a, CGameObjectInstance *t, CVector3 p, f32 reach, s32 falloff, u32 flags, HitFrame **psrc) {
    CVector3 c;
    CCollisionResults *o;
    HitFrame *sv;
    s32 armored;
    s32 ok;
    f32 m;
    f32 lo;
    f32 hi;
    f32 lim;
    f32 dist;

    m = 1.0f;
    armored = 1;
    ok = 1;
    if (gEngineApp_m_Settings.multiplayer != 0 && (flags & 0x400000)) {
        armored = 0;
    }
    if (t->header.m_Type == 2 && (CGameObjectInstance *)(o = &gCollisionResults)->m_pInstance == a && o->m_nInstanceNode != -1) {
        sv = &o->m_Frame;
        falloff = 0;
    } else if (CGameObjectInstance__GetNearestBox(a, p, &sBlastShieldFrame) != 0) {
        sv = &sBlastShieldFrame;
    } else {
        sv = NULL;
    }
    *psrc = sv;
    if (armored && sv != NULL) {
        m *= sv->damageFactor;
    }
    if (!falloff) {
        return m;
    }
    c = a->header.m_vPos;
    lo = a->header.m_vPos.y + a->m_CollisionYOffset;
    hi = lo + GameObject_GetCollisionHeight(a);
    if (hi < p.y) {
        c.y = hi;
    } else if (p.y < lo) {
        c.y = lo;
    } else {
        c.y = p.y;
    }
    lim = reach + GameObject_GetCollisionRadius(a);
    dist = CVector3__Distance(&p, &c);
    if (lim < dist) {
        return 0.0f;
    }
    if (ok) {
        m *= 1.0f - dist / lim;
    }
    return m;
}

typedef struct ViewInfo {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 eye;
} ViewInfo;
typedef struct ShotActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *player;
} ShotActor;
void CPlayer__TransformByView(CPlayer *pl, CVector3 *v);

/* Fires the player's shot of kind 0xE9 at `pos`, aimed along `dir` from the eye, and plays its sound. */
void ImpactEvent_FirePlayerShot(void *unused, ShotActor *actor, CVector3 pos, CVector3 dir, s32 mode) {
    CVector3 v;
    CVector3 p;
    CVector3 origin;
    CQuaternion q;
    sptr arg;
    CPlayer *pl;

    pl = actor->player;
    v.x = -dir.x;
    v.y = dir.y;
    v.z = dir.z;
    CPlayer__TransformByView(pl, &v);
    v.x -= ((ViewInfo *)pl->m_pCamera)->eye.x;
    v.y -= ((ViewInfo *)pl->m_pCamera)->eye.y;
    v.z -= ((ViewInfo *)pl->m_pCamera)->eye.z;
    CQuaternion__FromDirection(&q, &v);
    GameObject_TraceMove(actor, pos, &p, &arg, 0, 1);
    p = pos;
    p.y += 12.0f;
    origin.x = 0.0f;
    origin.y = 0.0f;
    origin.z = 0.0f;
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, actor, actor, NULL, 0, 0xE9, origin, q, p, arg, -1, mode == 5 ? 3 : 1);
    p.x = 0.0f;
    p.y = 0.0f;
    p.z = 0.0f;
    CScene__StartRumbles(&gScene, pl->m_ContState.m_pContState->rumblePool, 3, p, 10.24f, 1.0f);
}

void ImpactEvent_DirectDamage(unsigned char *arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    if (*arg0 == 1) {
        ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 0, 0, 0x100000);
    }
}

void ImpactEvent_AreaDamage(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 0, 0x100000);
}

void ImpactEvent_AreaDamagePush(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 1, 0x100000);
}

void ImpactEvent_Null5(s32 arg0, s32 arg1, s32 arg2, CVector3 arg3) {
}

void ImpactEvent_DoorOpen(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) { /* impact event (a door opening): the actors pointer-wide */
    ImpactEvent_FloodRegionFlag4(arg0, arg3, 1);
}

/* impact event 7 (a door closing): the actor pointer at full width */
void ImpactEvent_DoorClose(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3) {
    ImpactEvent_FloodRegionFlag4(arg0, arg3, 0);
}

#include "globals/camera.h"
#ifndef TARGET_PORT
extern void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6);
#endif

extern void CCamera__StartFlash(void *view, s32 red, s32 green, s32 blue, s32 mode, s32 alpha, s32 duration, s32 delay);
extern s32 CCamera__IsPointVisible(Node_func_80267CE4_de *arg0, Triple arg1);
#ifdef TARGET_PORT

void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, CImpactEventWords v) {
    Node_func_80267CE4_de *node;
    node = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
    while (node != 0) {
        if (CCamera__IsPointVisible(node, arg3) != 0) {
            CCamera__StartFlash(node, (u8) (v.w0 >> 24), (u8) (v.w0 >> 16), (u8) (v.w0 >> 8), (u8) v.w0,
                                (u8) (v.w1 >> 24), (u8) (v.w1 >> 16), (u8) (v.w1 >> 8));
        }
        node = node->next;
    }
}
#else
void ImpactEvent_FlashScreens(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6) {
    Node_func_80267CE4_de *node;
    node = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
    while (node != 0) {
        if (CCamera__IsPointVisible(node, arg3) != 0) {
            CCamera__StartFlash(node, arg6.a, arg6.b, arg6.c, arg6.d,
                          arg6.e, arg6.f, arg6.g);
        }
        node = node->next;
    }
}
#endif

void ImpactEvent_Null10(s32 arg0, s32 arg1, s32 arg2, CVector3 arg3) {
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_2/802661FC_2.CPP, func_80267D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */



extern void WeaponActor_FireCallback(void *actor, sptr flagsAddr, s32 arg2);

void ImpactEvent_SetWeaponFirePos(s32 arg0, void *arg1, s32 arg2, CVector3 arg3, s32 arg6)
{
    s32 t6 = arg6;
    CPlayer *player = ((CPlayer *)((CGameObjectInstance *)arg1)->m_AI.m_pParent);
    player->m_vWeaponFirePos = arg3;
    do { } while (0);
    WeaponActor_FireCallback(arg1, (sptr)&((CGameObjectInstance *)arg1)->m_AI.m_dwFlags, t6);
}

extern s32 CCameraPool__StartShake(void *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 arg5, Triple arg6);

void ImpactEvent_StartShakeType1(s32 arg0, s32 arg1, s32 arg2, Triple t, CImpactEventWords v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) (s16) (v.w0 >> 16), (f32) (s16) v.w0,
                            (f32) (s16) (v.w1 >> 16), (f32) (s16) v.w1 * 10.24f, 1, t);
}

void ImpactEvent_StartShake(s32 arg0, s32 arg1, s32 arg2, Triple t, CImpactEventWords v) {
    CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, (f32) (s16) (v.w0 >> 16), (f32) (s16) v.w0,
                            (f32) (s16) (v.w1 >> 16), (f32) (s16) v.w1 * 10.24f, 0, t);
}

#include "globals/text.h"
#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

typedef struct MessageEventParams {
    s32 index;          /* 0x0 */
    s32 unk4;           /* 0x4 */
} MessageEventParams;
/* The camera pool as bytes (the pool and its main camera at +0x40): the byte view is what keeps the ROM's code. */
#define sCameraPoolBytes ((u8 *) &gEngineApp_m_Game_m_CameraPool)

extern void CCameraPool__AddText(void *arg0, void *arg1, char *text);
extern void Sound__PlaySpeech(s32 arg0);

void ImpactEvent_PlaySoundAndEffect(s32 arg0, CGameObjectInstance *actor, s32 arg2, CVector3 pos, MessageEventParams params) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        CCameraPool__AddText(sCameraPoolBytes, ((CPlayer *)actor->m_AI.m_pParent)->m_pCamera, gLevelMessages[params.index].text);
    } else {
#ifdef TARGET_PORT
        CCameraPool__AddText(sCameraPoolBytes, sCameraPoolBytes + T2_OFFSETOF(CCameraPool, m_MainCamera), gLevelMessages[params.index].text);
#else
        CCameraPool__AddText(sCameraPoolBytes, sCameraPoolBytes + 0x40, gLevelMessages[params.index].text);
#endif
    }
    if (gLevelMessages[params.index].speech != 0) {
        Sound__PlaySpeech(gLevelMessages[params.index].speech);
    }
}

void ImpactEvent_AreaDamage200000(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 0, 0x200000);
}

void ImpactEvent_AreaDamagePush200000(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 1, 0x200000);
}

void ImpactEvent_ExplosionDamage(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 0, 0x400000);
}

void ImpactEvent_ExplosionDamagePush(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 1, 0x400000);
}

void ImpactEvent_AreaDamage800000(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 0, 0x800000);
}

void ImpactEvent_AreaDamagePush800000(sptr arg0, sptr arg1, s32 arg2, CVector3 arg3, CImpactBlast arg6) {
    ImpactEvent_ApplyBlast((CGameObjectInstance *) arg0, (CGameObjectInstance *) arg1, arg2, arg3, arg6, 1, 1, 0x800000);
}

#include "globals/rumble.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/player.h"
#endif

struct func_8024E8F0_S1;
struct func_8024E8F0_S1 {
    u8 unk0;
    char pad0[0xD4 - 0x0 - sizeof(u8)];
    s32 unkD4;
};
typedef struct func_8024E8F0_S1 func_8024E8F0_S1;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct Triple;
struct Inner;
typedef struct Inner Inner;
struct Params_func_802681A8_de;
struct Params_func_802681A8_de {
    s32 value;
    s16 angle;
    u8 scale;
    u8 pad;
};
struct Context_func_802681A8_de;
typedef struct Context_func_802681A8_de Context_func_802681A8_de;
typedef struct Params_func_802681A8_de Params_func_802681A8_de;
struct Inner {
    char pad[0xA50];
    char *resource;
};
struct Context_func_802681A8_de {
    char pad[0x1A8];
    struct Inner *inner;
};

void ImpactEvent_Rumble(func_8024E8F0_S1 *arg0, Context_func_802681A8_de *arg1, s32 arg2, CVector3 arg3, Params_func_802681A8_de arg6) {
    void *resource;
#ifdef TARGET_PORT
    /* Host members (STABILITY.md section 7): the N64 view read the actor's flags at 0xD4, its
     * player (CAI m_pParent, actor 0x1A8), the player's controller (0xA50) and the rumble pool
     * in it (0x140) at N64 offsets. */
    CGameObjectInstance *hitActor = (CGameObjectInstance *) arg0;
    if (hitActor->header.m_Type == 1 && (hitActor->m_dwFlags & 0x300000)) {
        CPlayer *player = (CPlayer *) ((CGameObjectInstance *) arg1)->m_AI.m_pParent;
        resource = player->m_ContState.m_pContState->rumblePool;
#else
    if ((arg0->unk0 == 1) && (arg0->unkD4 & 0x300000)) {
        resource = arg1->inner->resource + 0x140;
#endif
        arg3.x = 0;
        arg3.y = 0;
        arg3.z = 0;
    } else {
        resource = &gRumblePool;
    }
    CScene__StartRumbles(&gScene, (sptr) resource, arg6.value, arg3,
                  arg6.angle * 10.24f, arg6.scale * 0.003921569f);
}

typedef struct LevelSpecialParams {
    s32 unk0;
    s32 unk4;
} LevelSpecialParams;
extern void MantidQueen_SpawnMinion(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, LevelSpecialParams params);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Primagen_HandleAnimEvent Primagen_HandleAnimEvent_ev
#endif

extern void Primagen_HandleAnimEvent(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, LevelSpecialParams params);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define MotherBoss_AnimEvent MotherBoss_AnimEvent_ev
#endif

extern void MotherBoss_AnimEvent(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, LevelSpecialParams params);

/* the actors pointer-wide (sptr: s32 on the N64, the same code) */
void ImpactEvent_LevelSpecial(sptr arg0, sptr arg1, s32 arg2, CVector3 pos, LevelSpecialParams params) {
    switch (gScene_m_LevelID) {
        case 0x1D4D:
            MantidQueen_SpawnMinion(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB2:
            Primagen_HandleAnimEvent(arg0, arg1, arg2, pos, params);
            break;
        case 0x1DB1:
            MotherBoss_AnimEvent(arg0, arg1, arg2, pos, params);
            break;
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_8026826C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Kind;
typedef struct Kind Kind;
struct ParamsRecAt4;
typedef struct ParamsRecAt4 ParamsRecAt4;
struct ObjectRecAt17C;
typedef struct ObjectRecAt17C ObjectRecAt17C;
#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
struct ParamsRecAt4 {
    s16 value;  /* the low half of word 0 */
    s16 unk0;
    s32 team;
};
#else
struct ParamsRecAt4 {
    s16 unk0;
    s16 value;
    s32 team;
};
#endif

struct Kind {
    s32 value;
    u8 pad4[0xA];
    s8 team;
};
#ifdef TARGET_PORT
struct ObjectRecAt17C {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ObjectRecAt17C(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_type[T2_HOFF_ObjectRecAt17C(header.m_Type)]; u8 type; }; /* N64 0x0 */
        struct { u8 _hp_kind[T2_HOFF_ObjectRecAt17C(header.m_pIntelligence)]; struct Kind *kind; }; /* N64 0x14 */
        struct { u8 _hp_value[T2_HOFF_ObjectRecAt17C(m_mfOrient) + 40]; s32 value; }; /* N64 0x80 */
        struct { u8 _hp_mask[T2_HOFF_ObjectRecAt17C(m_AI.m_VisBits)]; u32 mask; }; /* N64 0x14C */
    };
};
#else
struct ObjectRecAt17C {
    u8 type;
    u8 pad1[0x13];
    struct Kind *kind; /* Turok 2 offsets (0x18/0xD0/0x17C in Rage Wars) */
    u8 pad18[0x80 - 0x18];
    s32 value;
    u8 pad84[0x14C - 0x84];
    u32 mask;
};
#endif

extern s32 Sound__PlayPitched(s32 arg0, CVector3 arg1, s32 arg4, sptr arg5,
                         f32 arg6);

void ImpactEvent_PlayTeamSound(ObjectRecAt17C *arg0, ObjectRecAt17C *arg1, s32 arg2, CVector3 arg3,
                   ParamsRecAt4 arg6) {
    s32 *global = &gEngineApp_m_Settings.paused;
    s32 selected;
    f32 scale;
    scale = 1.0f;
    selected = -1;
    if (global[0] != 0) {
        return;
    }
    switch (arg1->type) {
    case 0:
        if (arg0->kind->team != arg6.team) {
            return;
        }
        selected = arg1->value;
        break;
    case 1:
        if (arg1->kind->value == arg1->type) {
            if (global[-115] & 0x40) {
                scale = 2.5f;
            } else if (global[-115] & 0x20) {
                scale = 0.35f;
            }
        }
        if ((arg1->mask & (1 << arg6.team)) == 0) {
            return;
        }
        break;
    }
    Sound__PlayPitched(arg6.value, arg3, 0, selected, scale);
}

struct ObjRecAt1;
struct ObjRecAt1 {
    char pad0[0x1];
    u8 unk1;
};
typedef struct ObjRecAt1 ObjRecAt1;
struct IntPair;
struct Filter;
struct Filter {
    u8 type;
    u8 pad[0x14B];
    s32 mask;
};
typedef struct Filter Filter;

void ImpactEvent_SpawnTeamParticle(void *arg0, Filter *arg1, s32 arg2, Triple arg3, struct IntPair arg6) {
    ObjRecAt1 *object = arg0;
    switch (arg1->type) {
    case 0:
        if (object->unk1 != arg6.field_4) {
            return;
        }
        break;
    case 1:
        if (!(arg1->mask & (arg1->type << arg6.field_4))) {
            return;
        }
        break;
    }
    Actor_SpawnParticle(arg0, arg6.field_0, arg3, 0);
}

/* Static inline in the Rage Wars source file (dropped by the importer);

 * Turok 2 passes 0x400000 where Rage Wars passes 0x200. */
static inline void applyAreaBlast(CGameObjectInstance *a, sptr b, s32 c, CVector3 pos, CImpactBlast blk) {
    ImpactEvent_ApplyBlast(a, (CGameObjectInstance *) b, c, pos, blk, 1, 0, 0x400000);
}

void ImpactEvent_BlastSameHeight(CGameObjectInstance *a, sptr b, s32 c, CVector3 pos, CImpactBlast blk) {
    f32 d;
    d = a->header.m_vPos.y - pos.y;
    if (d < 0.0f ? -d <= 76.799995f : d <= 76.799995f) {
        applyAreaBlast(a, b, c, pos, blk);
    }
}

void ImpactEvent_SetHotPoint(CGameObjectInstance *arg0, s32 arg1, s32 arg2, CVector3 arg3, s32 arg4) {
    if (arg0->header.m_Type == 1) {
        arg0->m_dwHotPointFlags |= 1 << arg4;
    }
}

void ImpactEvent_ClearHotPoint(CGameObjectInstance *arg0, s32 arg1, s32 arg2, CVector3 arg3, s32 arg4) {
    if (arg0->header.m_Type == 1) {
        arg0->m_dwHotPointFlags &= ~(1 << arg4);
    }
}

typedef struct SpawnEventParams {
    s32 unk0;           /* 0x0 */
    f32 size;           /* 0x4 */
} SpawnEventParams;
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CInstancePool__SpawnAnimInstance CInstancePool__SpawnAnimInstance_i
#endif

extern void CInstancePool__SpawnAnimInstance(void *arg0, s32 arg1, sptr arg2, CVector3 pos, s32 arg6, CVector3 rot, CVector3 scale, sptr arg13);

void ImpactEvent_SpawnAnimInstance(CGameObjectInstance *actor, s32 arg1, s32 arg2, CVector3 arg3, SpawnEventParams params) {
    CVector3 scale;
    CVector3 rot;

    if (actor->header.m_Type == 0) {
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
    } else {
        rot = actor->m_vVelocity;
    }
    scale.x = params.size;
    scale.y = params.size;
    scale.z = params.size;
    CInstancePool__SpawnAnimInstance(&gScene.m_InstancePool, params.unk0, ((sptr)actor->header.m_pCurrentRegion), actor->header.m_vPos, 0, rot, scale, 0);
}

#include "structs/game.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00268768.c, func_00268768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The host (round 10 y3, docs/port/FIDELITY.md 5): an impact event's 8-byte value arrives as two
 * host-order words (the event values' convention: CEventValue in structs/particle.h, the
 * animation events' s32[2] from the archive), word 0 the N64's first big-endian word. On the
 * little-endian hosts the halves and bytes of each word sit in reverse order, so the fields
 * that read them by position are declared reversed. (With the N64 order, every particle hit
 * swapped radius and damage: demo 1's first enemy shot took 1 health instead of 8.) */
typedef struct B15Blk8 {
    s16 b, a;   /* b (the sound) is the low half of word 0 */
    s32 c;
} B15Blk8;
#else
typedef struct B15Blk8 {
    s16 a, b;
    s32 c;
} B15Blk8;
#endif

typedef struct B15Src {
               u8 type;
               u8 pad1[3];
               CVector3 pos;       /* 0x4 */
               u8 pad10[4];
               B15Def *def;     /* 0x14 */
               u8 pad18[0x68];
               void *unk80;     /* 0x80 (0xD0 in Rage Wars) */
} B15Src;
s32 Sound__PlayPitched(s32 id, CVector3 pos, s32 arg4, sptr selected, f32 scale);
s32 GameObject_GetFloorKind(void *a);

void ImpactEvent_PlayFloorSound(B15Src *act, B15Src *src, s32 c, CVector3 pos, B15Blk8 k) {
    f32 vol = 1.0f;
    void *s = (void *)-1;
    if (gEngineApp_m_Settings.paused == 0 && GameObject_GetFloorKind(act) == k.c) {
        switch (src->type) {
        case 0:
            s = src->unk80;
            break;
        case 1:
            if (act->def->type == 1) {
                if (gEngineApp_m_Settings.cheatFlags & 0x40) {
                    vol = 2.5f;
                }
                if (gEngineApp_m_Settings.cheatFlags & 0x20) {
                    vol = 0.35f;
                }
            }
            break;
        }
        Sound__PlayPitched(k.b, pos, 0, (sptr) s, vol);
    }
}

extern void AI_StartSound(u8 *, u8 *, s32);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* ABI (docs/port/ABI.md section 5, abicheck indirect struct_class/struct_scalar): the dispatcher
 * (ImpactEvent_Send/Call) passes (actor, actor, kind, CVector3 pos, 8-byte event value). On MIPS
 * the position and the value fill a3 and the stack words this definition names pair/fifth/sixth; on the host
 * the vector travels in SSE/FP registers and the value in one general register, so the host
 * definition takes them as they are passed: the value's first word is what MIPS read. */
void ImpactEvent_AIStartSound(void *unused, u8 *object, s32 third, CVector3 pos, s32 value) {
    if (gEngineApp_m_Settings.paused == 0 && object != 0 && object[0] == 1) {
        AI_StartSound(object, object + T2_OFFSETOF(CGameObjectInstance, m_AI), value);
    }
}
#else
void ImpactEvent_AIStartSound(void *unused, u8 *object, s32 third, struct IntPair pair, s32 fifth, s32 sixth) {
    if (gEngineApp_m_Settings.paused == 0 && object != 0 && object[0] == 1) {
        do {
            AI_StartSound(object, object + 0x140, sixth);
        } while (0);
    }
}
#endif

extern void AI_StopSound(u8 *, u8 *);

/* ABI (docs/port/ABI.md section 5, abicheck indirect struct_class/struct_scalar): the dispatcher
 * (ImpactEvent_Send/Call) passes (actor, actor, kind, CVector3 pos, 8-byte event value). On MIPS
 * the position and the value fill a3 and the stack words this definition names pair; on the host
 * the vector travels in SSE/FP registers and the value in one general register, so the host
 * definition takes them as they are passed: the value's first word is what MIPS read. */
void ImpactEvent_AIStopSound(void *unused, u8 *object, s32 third, CVector3 pos, s32 value) {
    if (object != 0 && object[0] == 1) {
        AI_StopSound(object, object + T2_OFFSETOF(CGameObjectInstance, m_AI));
    }
}
