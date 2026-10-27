#include "common.h"

/* Draws a weapon actor: its muzzle child (copying the parent's render block), then the model. */

typedef struct {
    s32 w[6];
} RenderBlock; /* 0x18 bytes */

typedef struct {
    /* 0x000 */ u8 pad0[0x98];
    /* 0x098 */ s32 model;
    /* 0x09C */ u8 pad9C[0xBC - 0x9C];
    /* 0x0BC */ RenderBlock render;
    /* 0x0D4 */ u8 padD4[0x110 - 0xD4];
    /* 0x110 */ u8 slots[2][0x18];
    /* 0x140 */ u8 pad140[0x1A8 - 0x140];
    /* 0x1A8 */ struct WeaponPlayer *owner;
    /* 0x1AC */ u8 pad1AC[0x234 - 0x1AC];
    /* 0x234 */ void *drawHook;
} WeaponActor;

typedef struct WeaponPlayer {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *object;
    /* 0x520 */ u8 pad520[0x996 - 0x520];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xBB0 - 0x998];
    /* 0xBB0 */ WeaponActor *muzzle;
} WeaponPlayer;

typedef struct {
    /* 0x000 */ u8 pad0[0x128];
    /* 0x128 */ s32 unk_128;
    /* 0x12C */ s32 unk_12C;
} WeaponState;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ s32 camera;
    /* 0x0C */ s32 *hit;
} DrawArgs;

extern u8 D_8011AAF1; /* multiplayer */
extern s32 D_800B6D1C;

extern s32 func_00416AF8(void *arg0);
extern void func_0025C018(WeaponActor *actor, void *ai);
extern s32 func_0021D7BC(s32, s32, s32);
extern void func_0021D39C(WeaponActor *actor, s32 camera);
extern void func_0020B6A0(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

void func_0025883C(WeaponActor *obj, WeaponState *state, DrawArgs *args) {
    WeaponPlayer *player = obj->owner;
    WeaponActor *muzzle;
    s32 arg6 = -1;

    if (D_8011AAF1 != 0 && func_00416AF8(player->object + 0x53C) != 0) {
        return;
    }
    muzzle = player->muzzle;
    if (muzzle != NULL) {
        func_0025C018(muzzle, (u8 *)muzzle + 0x140);
        muzzle->render = obj->render;
        muzzle->drawHook = func_0021D7BC;
        func_0021D39C(muzzle, args->camera);
        muzzle->drawHook = NULL;
    }
    if (player->weapon == 0x15) {
        arg6 = state->unk_12C;
    }
    func_0020B6A0(args->hit, obj->model, 1, obj->slots[D_800B6D1C], 0, state->unk_128, arg6);
}
