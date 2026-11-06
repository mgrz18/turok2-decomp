/* The two inlined helpers after AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD/8028E6FC.CPP, func_8028E860_de and func_8028E78C_de (Blast_GetStrength and Blast_IsLivingEnemyInRange here), with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/game.h"
#include "globals/scene.h"

typedef struct Entity Entity;
struct Entity {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0x140 - 0xBA];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad175[0x28C - 0x175];
    /* 0x28C */ Entity *next;
};


void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void DamageEvent_Set(DamageEvent *event, s32 source, s32 damage, f32 radius, s32 flags, s32 arg5);
void CAI__CallHurt(Entity *e, void *ai, DamageEvent *event);
s32 CAI__SetMode(Entity *e, void *ai, s32 state);
Entity *CPlayerPool__GetFirstActive(void *list);

/* Blast_GetStrength inlined: the strength (0 or 0xFF) a blast at `target` hits `e` with. */
static __inline__ s32 blastStrength(Entity *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    if (e->kind == 1 && e->def->m_Type == 7) {
        CVector3__Subtract(&d, &e->pos, target);
        dist = d.x * d.x + d.z * d.z;
        d.y = 0;
        if (262144.0f < dist) {
            return 0;
        }
        return 0xFF;
    }
    if (e->def->m_Type == 1) {
        t = e->typeId;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0xFF;
        }
    }
    return 0;
}

/* Blast_IsLivingEnemyInRange inlined: a living enemy within 2560 units of `target`. */
static __inline__ s32 inBlastRange(Entity *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    CVector3__Subtract(&d, &e->pos, target);
    dist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (e->def->m_Type == 1) {
        t = e->typeId;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0;
        }
        if (e->health != 0) {
            if (e->state != 0x3D) {
                if (dist <= 6553600.0f) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* A blast at `target`: damages every entity of the world and of the actor list, then damages
 * the current player when within 512 units. */
void CScene__Blast(CScene *world, CVector3 *target) {
    DamageEvent event;
    CVector3 d;
    Entity *e;
    CListedInstance *node;
    Entity *actor;
    Entity *player;
    s32 count;
    s32 i;
    s32 strength;

    count = world->m_nKind1;
    for (i = 0; i < count; i++) {
        e = (Entity *)world->m_pKind1[i];
        strength = blastStrength(e, target);
        if (strength != 0) {
            DamageEvent_Set(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            CAI__CallHurt(e, e->ai, &event);
        }
        if (inBlastRange(e, target)) {
            CAI__SetMode(e, e->ai, 0x3D);
        }
    }
    for (node = gScene.m_InstancePool.m_ActiveList.m_pHead; node != NULL; node = node->m_pNext) {
        actor = (Entity *)&node->m_Instance;
        strength = blastStrength(actor, target);
        if (strength != 0) {
            DamageEvent_Set(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            CAI__CallHurt(actor, actor->ai, &event);
        }
        if (inBlastRange(actor, target)) {
            CAI__SetMode(actor, actor->ai, 0x3D);
        }
    }
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    CVector3__Subtract(&d, &player->pos, target);
    if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
        DamageEvent_Set(&event, 0, 0x1900, 102.399994f, 0x400000, 0);
        CAI__CallHurt(player, player->ai, &event);
    }
}
