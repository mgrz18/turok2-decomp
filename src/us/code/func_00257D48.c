#include "common.h"

typedef struct Object Object;
typedef struct AI AI;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void (*update)(Object *obj, AI *ai);
} AIState;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 animType;
} AIAnim;

struct AI {
    /* 0x000 */ u8 pad0[0x2C];
    /* 0x02C */ AIAnim *anim;
    /* 0x030 */ AIState *state;
    /* 0x034 */ u8 pad34[0x118 - 0x34];
    /* 0x118 */ f32 timer118;
    /* 0x11C */ u8 pad11C[0x128 - 0x11C];
    /* 0x128 */ s32 unk128;
    /* 0x12C */ u8 pad12C[0x130 - 0x12C];
    /* 0x130 */ f32 timer130;
};

typedef struct {
    /* 0x000 */ u8 pad0[0x996];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xA08 - 0x998];
    /* 0xA08 */ s16 pose;
    /* 0xA0A */ u8 padA0A[0xB14 - 0xA0A];
    /* 0xB14 */ s16 queuedWeapon;
    /* 0xB16 */ u8 padB16[0xB8C - 0xB16];
    /* 0xB8C */ s32 unkB8C;
} Player;

struct Object {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ Player *owner;
};

typedef struct {
    /* 0x00 */ s16 mode;
    /* 0x02 */ u8 pad2[0x16];
} PoseInfo;   /* 0x18 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 model;
    /* 0x06 */ u8 pad6[0x50 - 6];
    /* 0x50 */ void (*update)(Object *obj, AI *ai);
} WeaponInfo;

extern PoseInfo D_800ADF68[];
extern WeaponInfo *D_800AF618[];   /* weapon table */
extern f32 D_800B6D28;             /* frame time scale */
extern u8 D_800F7078[];
extern void func_00243414(Object *obj, AI *ai, s32 mode);
extern void func_002241D8(void *level, Object *obj, s32 model, s32 animType);

/* First-person weapon AI: counts its timers down, goes to mode 1 when the
 * player's pose asks for it or a weapon change is queued, runs the state's
 * and the weapon's updates, then animates the weapon model. */
void func_00257D48(Object *obj, AI *ai) {
    Player *player;
    s32 mode;

    player = obj->owner;
    mode = D_800ADF68[player->pose].mode;
    if (ai->timer130 > 0.0f) {
        ai->timer130 -= D_800B6D28;
    }
    if (player->weapon != 21) {
        ai->unk128 = 0;
    }
    if (ai->timer118 > 0.0f) {
        ai->timer118 -= D_800B6D28;
    }
    if (mode == 1) {
        func_00243414(obj, ai, 1);
    }
    if (player->queuedWeapon != player->weapon) {
        player->unkB8C = 0;
        func_00243414(obj, ai, 1);
    }
    if (ai->state != NULL && ai->state->update != NULL) {
        ai->state->update(obj, ai);
    }
    if (D_800AF618[player->weapon]->update != NULL) {
        D_800AF618[player->weapon]->update(obj, ai);
    }
    func_002241D8(D_800F7078, obj, D_800AF618[player->weapon]->model, ai->anim->animType);
}
