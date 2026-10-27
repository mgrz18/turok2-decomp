#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 data[0x18];
} DamageEvent;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f pos;
} Camera;

typedef struct {
    /* 0x0000 */ u8 pad0[0xD4];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0x1A4 - 0xD8];
    /* 0x01A4 */ f32 shockTime;
    /* 0x01A8 */ u8 pad1A8[0x51C - 0x1A8];
    /* 0x051C */ Camera *camera;
    /* 0x0520 */ u8 pad520[4];
    /* 0x0524 */ s32 alive;
    /* 0x0528 */ u8 pad528[0x12A8 - 0x528];
    /* 0x12A8 */ void *shocker;
} Player;

extern f32 D_800B6D28;      /* frame time */
extern char D_801198B0[];

f32 func_0021149C(f32 scale);
void func_00281060(Camera *cam, s32 maxOpacity, s32 red, s32 green, s32 blue, s32 attack, s32 sustain, s32 decay);
void func_0028135C(void *pool, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, Vec3f pos);
s32 func_0024E5F0(Player *player, Player *body, s32 mode);
void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
void func_00247C10(Player *actor, void *ai, DamageEvent *hit);

/* Being shocked: flash and shake the view at random; when the time runs out
 * (or the player dies) drop back to standing and take the shock's damage. */
void func_00251E28(Player *player) {
    DamageEvent event;
    Camera *cam;

    if (func_0021149C(100.0f) > 80.0f) {
        cam = player->camera;
        func_00281060(cam, 0xFF, 0xFF, 0, 0, (u8) (u32) func_0021149C(7.5f), 3, 7);
        func_0028135C(D_801198B0, 0.0f, 204.79999f, 0.0f, 512.0f, 0, cam->pos);
    }
    player->shockTime -= D_800B6D28;
    if (player->shockTime <= 0.0f || player->alive == 0) {
        player->flags &= ~0x800000;
        func_0024E5F0(player, player, 2);
        func_00245828(&event, player->shocker, 0x3200, 25.599998f, 0x100000, 0);
        player->shocker = NULL;
        func_00247C10(player, (u8 *) player + 0x140, &event);
    }
}
