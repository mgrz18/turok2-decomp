#include "common.h"

/* End of the fight: when the module object has a target, gives its owner (the player) D_00431D7C, refreshes
 * its view and turns it around; then sends the module object to anim state 0xA. */

typedef struct {
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ f32 rotY;
    /* 0x054 */ u8 pad54[0x2D8 - 0x54];
    /* 0x2D8 */ f32 viewRotY;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0xD4];
    /* 0x0D4 */ s32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ Player *owner;
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
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ s32 unk50;
    /* 0x054 */ u8 pad54[0x140 - 0x54];
    /* 0x140 */ u8 ai[1];
} Obj;

typedef struct {
    /* 0x000 */ Obj *obj;
} State;

extern State D_8012FB70;
extern u8 D_00431D7C[];
extern void func_00246108(Obj *obj, void *ai, Targets *t, s32 f1, s32 f2);
extern void func_00253D68(Player *player, void *arg1);
extern void func_0024CF58(Player *player, Player *other);
extern void func_00243414(Obj *obj, void *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Obj *obj, Actor **found) {
    Targets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, obj->ai, &t, 1, 0);
    if (t.b.actor != NULL && (t.b.actor->flags & 0x300000)) {
        dist = t.b.dist;
        *found = t.b.actor;
    }
    return dist;
}

void func_0040C6F4(void) {
    Obj *obj = D_8012FB70.obj;
    Actor *found;
    Player *player;

    player = NULL;
    getTargetDist(obj, &found);
    if (found != NULL) {
        player = found->owner;
    }
    if (player != NULL) {
        func_00253D68(player, D_00431D7C);
        func_0024CF58(player, player);
        player->viewRotY = player->rotY = 3.141593f;
        obj->unk50 = 0;
    }
    func_00243414(obj, obj->ai, 0xA);
}
