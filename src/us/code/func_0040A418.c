/* FAKEMATCH: identical if/else arms in fewerActorsThan's counting loop (as in func_0040C4F4). */
#include "common.h"
#include "structs/boss.h"

/* Picks the next attack anim from a weighted table: close to the target 4 (then 3 after a 4), otherwise 5
 * when fewer than 3 actors are in the D_8010FEB4 list, else 6. */

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *callback234;
    /* 0x238 */ u8 pad238[0x248 - 0x238];
    /* 0x248 */ void *callback248;
    /* 0x24C */ u8 pad24C[0x250 - 0x24C];
    /* 0x250 */ s32 unk250;
    /* 0x254 */ s32 unk254;
} LevelActor;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x28C - 0xD8];
    /* 0x28C */ struct Actor *next;
} Actor;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Actor *actor;
    /* 0x08 */ u8 pad8[0x40 - 0x8];
    /* 0x40 */ f32 dist;
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u32 flags;
} CAI;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ CAI ai;
} Obj;

typedef struct {
    s16 id;
    s16 weight;
} Pick;

extern Actor *D_8010FEB4;
extern void func_00246108(Obj *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern s16 func_0026D518(Pick *table);
extern void func_00243414(Obj *obj, CAI *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Obj *obj, Actor **found) {
    Targets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, &obj->ai, &t, 1, 0);
    if (t.b.actor != NULL && (t.b.actor->flags & 0x300000)) {
        dist = t.b.dist;
        *found = t.b.actor;
    }
    return dist;
}

/* func_0040C4F4 */
static __inline__ s32 fewerActorsThan(s32 max) {
    Actor *actor;
    s32 count = 0;

    for (actor = D_8010FEB4; actor != NULL; actor = actor->next) {
        /* both arms are merged by cross-jumping; the extra block gives the ROM's register allocation */
        if (actor->next != NULL) {
            count++;
        } else {
            count++;
        }
    }
    return count < max;
}

void func_0040A418(Obj *obj, CAI *ai) {
    Pick picks[8];
    Actor *found;
    MotherBossState *state = &D_8012FB70;
    s32 n;
    f32 dist;

    n = 0;
    dist = getTargetDist(obj, &found);
    if (found != NULL) {
        if (dist < 1228.7999f) {
            picks[n].id = 4;
            picks[n++].weight = 6;
            if (state->lastAnim == 4) {
                picks[n].id = 3;
                picks[n++].weight = 6;
            }
        } else {
            state->unk668 = 0;
            if (fewerActorsThan(3)) {
                picks[n].id = 5;
                picks[n++].weight = 5;
            } else {
                picks[n].id = 6;
                picks[n++].weight = 5;
            }
        }
        picks[n].id = -1;
        state->lastAnim = func_0026D518(picks);
        state->counter = 0;
        func_00243414(obj, ai, state->lastAnim);
    }
}
