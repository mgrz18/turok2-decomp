#include "common.h"
#include "structs/game_object.h"

typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
    /* 0x04 */ s8 kind;
} SweepDef;

/* An instance that can be hit: an actor (or the instance sweeping). */
typedef struct HitInst {
    /* 0x000 */ u8 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ s32 *def;
    /* 0x018 */ u8 pad18[0xA0];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[2];
    /* 0x0BC */ Box bounds;
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x20];
    /* 0x0F8 */ s32 *particleDef;
    /* 0x0FC */ u8 padFC[0x10];
    /* 0x10C */ struct HitInst *owner;
} HitInst;

/* A segment swept from an instance's position to end. */
typedef struct Sweep {
    /* 0x00 */ HitInst *inst;
    /* 0x04 */ SweepDef *def;
    /* 0x08 */ f32 maxT;
    /* 0x0C */ u8 padC[0x40];
    /* 0x4C */ CVector3 end;
} Sweep;

typedef struct World {
    /* 0x000 */ u8 pad0[0x8F0];
    /* 0x8F0 */ HitInst *actors[0x80];
    /* 0xAF0 */ s32 actorCount;
} World;

typedef struct SweepHit {
    /* 0x00 */ HitInst *inst;
    /* 0x04 */ f32 t;
} SweepHit;

typedef struct MultiState {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 enabled;
} MultiState;

extern World D_800F7078;
extern HitInst **D_800F1ADC;
extern u8 D_8011AAF1;
extern MultiState D_8011AAD8;

typedef struct SweepBox {
    u8 pad[0x30];
} SweepBox;

s32 func_0023A640(Sweep *sweep, Box *box, f32 *t);
void func_00226F90(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 func_002407D8();
s32 func_0024079C();
void func_0023E6D8(Sweep *sweep, SweepBox *box);
void func_0023DF70(Sweep *sweep, SweepBox *box, HitInst *other);
void func_0023FDF0(Sweep *sweep, HitInst *other, CVector3 *start, Box *box);

#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* Sweep against the actors whose bounds the swept segment's box touches,
 * nearest first, until the sweep's range runs out. */
void func_0024032C(Sweep *sweep) {
    Box box;
    SweepBox volume;
    SweepHit hits[0x80];
    f32 t;
    CVector3 *start;
    SweepDef *def;
    HitInst *inst;
    HitInst *other;
    World *world;
    SweepHit *hit;
    MultiState *multi;
    s32 count;
    s32 nHits;
    s32 i;
    s32 j;

    def = sweep->def;
    if (def->kind == 0 || !(def->flags & 0x40000000)) {
        return;
    }
    inst = sweep->inst;
    start = &inst->pos;
    box.min.x = MIN(sweep->end.x, inst->pos.x);
    box.max.x = MAX(sweep->end.x, inst->pos.x);
    box.min.y = MIN(sweep->end.y, start->y);
    box.max.y = MAX(sweep->end.y, start->y);
    box.min.z = MIN(sweep->end.z, start->z);
    box.max.z = MAX(sweep->end.z, start->z);
    hit = hits;
    nHits = 0;
    world = &D_800F7078;
    count = world->actorCount;
    for (i = 0; i < count && nHits < 0x80; i++) {
        other = world->actors[i];
        if (inst == other) {
            continue;
        }
        if (def->kind == 7 && *D_800F1ADC == other) {
            continue;
        }
        if ((def->flags & 0x20000000) && other->typeId == 0x64F) {
            continue;
        }
        if (inst->type == 2 && inst->owner == other) {
            if (*inst->particleDef & 0x40) {
                continue;
            }
            if (D_8011AAF1) {
                continue;
            }
            switch (*other->def) {
                case 1:
                    continue;
                case 4:
                    continue;
            }
        }
        if (!(box.max.x > other->bounds.min.x) || !(box.min.x < other->bounds.max.x) ||
            !(box.max.z > other->bounds.min.z) || !(box.min.z < other->bounds.max.z) ||
            !(box.max.y > other->bounds.min.y) || !(box.min.y < other->bounds.max.y)) {
            continue;
        }
        if (func_0023A640(sweep, &other->bounds, &t)) {
            hit->inst = other;
            hit->t = t;
            hit++;
            nHits++;
        }
    }
    if (nHits != 0) {
        func_00226F90(hits, nHits, sizeof(SweepHit), func_002407D8, func_0024079C);
        hit = hits;
        for (j = 0; j < nHits; j++, hit++) {
            if (!(hit->t < sweep->maxT)) {
                break;
            }
            other = hit->inst;
            if ((other->flags & 0x300000) && !(multi = &D_8011AAD8)->enabled) {
                func_0023E6D8(sweep, &volume);
                func_0023DF70(sweep, &volume, other);
            } else {
                func_0023FDF0(sweep, other, start, &box);
            }
        }
    }
}
