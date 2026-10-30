#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0xC];
    /* 0x1C */ f32 velY;
} Actor;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ u8 padC[0x370];
    /* 0x37C */ s32 unk37C;
} PlayerCD0;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[0xA78 - 0x520];
    /* 0xA78 */ f32 unkA78;
    /* 0xA7C */ f32 unkA7C;
    /* 0xA80 */ u8 padA80[0xAA4 - 0xA80];
    /* 0xAA4 */ f32 lowestHeight;
    /* 0xAA8 */ u8 padAA8[0xAE4 - 0xAA8];
    /* 0xAE4 */ f32 eyeHeight;
    /* 0xAE8 */ u8 padAE8[0xBEC - 0xAE8];
    /* 0xBEC */ s32 unkBEC;
    /* 0xBF0 */ u8 padBF0[0xCD0 - 0xBF0];
    /* 0xCD0 */ PlayerCD0 unkCD0;
} Player;

extern f32 func_00268A5C(Actor *actor);
extern void func_00280EAC(void *camera);
extern void func_00275544(s16 sound, Vec3f pos, s32, s32);

/* Tracks the lowest height reached, damps the player's motion and the actor's
 * vertical speed (clamped at -51.2), resets state and plays sound 2000 there. */
void func_00255A44(Player *player, Actor *actor) {
    f32 h;
    PlayerCD0 *s;

    h = func_00268A5C(actor) - player->eyeHeight;
    if (h < player->lowestHeight) {
        player->lowestHeight = h;
    }
    player->unkA78 *= 0.5f;
    player->unkA7C *= 0.5f;
    actor->velY *= 0.75f;
    if (actor->velY < -51.199997f) {
        actor->velY = -51.199997f;
    }
    player->unkBEC = 0;
    func_00280EAC(player->camera);
    s = &player->unkCD0;
    s->unk0 = 0;
    s->unk4 = 0;
    s->unk37C = -1;
    s->unk8 = 0;
    func_00275544(2000, actor->pos, 0, -1);
}
