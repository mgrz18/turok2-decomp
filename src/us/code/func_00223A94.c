/* The two inlined helpers after AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD/8028E6FC.CPP, func_8028E860_de and func_8028E78C_de (func_00226EBC and func_00226DE8 here), with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/world.h"
#include "structs/intelligence.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/collision.h"

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

extern CListedInstance *D_8010FEB4;

void func_0020EF2C(CVector3 *out, CVector3 *a, CVector3 *b);
void func_00245828(DamageEvent *event, s32 source, s32 damage, f32 radius, s32 flags, s32 arg5);
void func_002465C8(Entity *e, void *ai, DamageEvent *event);
s32 func_00243414(Entity *e, void *ai, s32 state);
Entity *func_002532A8(void *list);

/* func_00226EBC inlined: the strength (0 or 0xFF) a blast at `target` hits `e` with. */
static __inline__ s32 blastStrength(Entity *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    if (e->kind == 1 && e->def->m_Type == 7) {
        func_0020EF2C(&d, &e->pos, target);
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

/* func_00226DE8 inlined: a living enemy within 2560 units of `target`. */
static __inline__ s32 inBlastRange(Entity *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    func_0020EF2C(&d, &e->pos, target);
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
void func_00223A94(CWorld *world, CVector3 *target) {
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
            func_00245828(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            func_002465C8(e, e->ai, &event);
        }
        if (inBlastRange(e, target)) {
            func_00243414(e, e->ai, 0x3D);
        }
    }
    for (node = D_8010FEB4; node != NULL; node = node->m_pNext) {
        actor = (Entity *)&node->m_Instance;
        strength = blastStrength(actor, target);
        if (strength != 0) {
            func_00245828(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            func_002465C8(actor, actor->ai, &event);
        }
        if (inBlastRange(actor, target)) {
            func_00243414(actor, actor->ai, 0x3D);
        }
    }
    player = func_002532A8(&D_80119870.m_PlayerList);
    func_0020EF2C(&d, &player->pos, target);
    if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
        func_00245828(&event, 0, 0x1900, 102.399994f, 0x400000, 0);
        func_002465C8(player, player->ai, &event);
    }
}
