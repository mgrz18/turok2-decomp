/* NON-MATCHING: 153 of 245 words differ: the health update (prev - damage clamped, stored to 0x524/0x144/0x3CC through two register copies) and the flash clamp shape differ; the code after shifts */
#include "common.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x32];
    /* 0x32 */ u8 handicap;
    /* 0x33 */ u8 pad33[0xB];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
} RegionInfo;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
} HitSource;

typedef struct {
    /* 0x000 */ u8 pad0[0x10];
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x140 - 0x14];
    /* 0x140 */ u32 aiFlags;
    /* 0x144 */ s32 aiHealth;
    /* 0x148 */ u8 pad148[0x3CC - 0x148];
    /* 0x3CC */ s32 shownHealth;
    /* 0x3D0 */ u8 pad3D0[0x518 - 0x3D0];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ void *camera;
    /* 0x520 */ u8 pad520[4];
    /* 0x524 */ s32 health;         /* 8.8 */
    /* 0x528 */ u8 pad528[0x6AC - 0x528];
    /* 0x6AC */ u8 surface;
    /* 0x6AD */ u8 pad6AD[0xA08 - 0x6AD];
    /* 0xA08 */ u16 state;
    /* 0xA0A */ u8 padA0A[0xA28 - 0xA0A];
    /* 0xA28 */ f32 invulnerable;
    /* 0xA2C */ u8 padA2C[0xA50 - 0xA2C];
    /* 0xA50 */ u8 *controller;
    /* 0xA54 */ u8 padA54[0xBB0 - 0xA54];
    /* 0xBB0 */ s32 reduced;
} DamagePlayer;

typedef struct {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x15];
    /* 0x19 */ u8 multiplayer;
} Settings;

/* The global at D_801198B0: what this reads of it. */
typedef struct Game {
    /* 0x0000 */ u8 pad0[0x1228];
    /* 0x1228 */ Settings settings;    /* D_8011AAD8 */
} Game;

extern Game D_801198B0;
extern s32 D_80110028;
extern u8 D_800F7078[];         /* the level */
extern char D_800B66D0[];

s32 func_00284174(void);
s32 func_002548FC(DamagePlayer *p);
RegionInfo *func_0022425C(void *level, void *region);
void func_00249118(DamagePlayer *p, HitSource *src, s32 arg3, s32 type);
void func_0027FC2C(Game *game, void *camera, char *text);
void func_0027580C(s32 sound);
void func_00225DB0(void *level, u8 *rumble, s32 id, Vec3f v, f32 f6, f32 f7);
void func_00281054(void *camera, s32 alpha, s32 time);
void func_00275624(s32 sound);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Hurts a player: damage (8.8) scaled by the reduced-damage state and the
 * multiplayer handicap, unless the player is protected; then the red flash,
 * the low-health warning, the rumble and the hurt sound. */
void func_00249358(DamagePlayer *p, HitSource *src, s32 damage, s32 arg3, s32 type) {
    s32 old;
    f32 f;
    s32 handicap;
    s32 check;
    RegionInfo *info;
    s32 prev;
    s32 health;
    s32 dead;
    s32 alpha;
    s32 time;
    s32 amount;
    Vec3f zero;

    f = 1.0f;
    old = p->health;
    if (func_00284174()) {
        return;
    }
    if (func_002548FC(p)) {
        return;
    }
    if (D_80110028) {
        return;
    }
    if ((D_801198B0.settings.flags & 1) && type != 0x1D) {
        return;
    }
    if (p->state < 2) {
        return;
    }
    if (p->reduced) {
        damage /= 5;
    }
    if (D_801198B0.settings.multiplayer) {
        handicap = p->options->handicap;
        if (handicap) {
            damage = damage * (1.0f / 256) / ((handicap + f) * 0.3f) * 256.0f;
        }
    }
    check = 1;
    if (!(D_801198B0.settings.flags & 0x10)) {
        check = p->surface & 0xF;
    }
    if (check && p->region != NULL) {
        info = func_0022425C(D_800F7078, p->region);
        if (info != NULL && (info->flags & 0x400000)) {
            return;
        }
    }
    if (p->invulnerable > 0.0f && type != 0x1D) {
        return;
    }
    prev = p->health;
    health = prev - damage;
    if (health < 0) {
        health = 0;
    }
    p->health = health;
    p->aiHealth = health;
    p->shownHealth = health;
    dead = health == 0;
    if (dead) {
        if (prev != 0) {
            func_00249118(p, src, arg3, type);
        }
        alpha = 0xFF;
        time = 0xC;
    } else {
        alpha = ((damage >> 8) * 0xFF) / 20;
        alpha = (MIN(alpha, 0xFF) < 0x80) ? 0x80 : MIN(alpha, 0xFF);
        time = 0x1E;
        if (old > 0xF00 && p->health <= 0xF00 && !D_801198B0.settings.multiplayer) {
            func_0027FC2C(&D_801198B0, p->camera, D_800B66D0);
            func_0027580C(0x156);
        }
        amount = damage >> 8;
        if (amount != 0) {
            f = amount * (1.0f / 15);
            if (f > 1.0f) {
                f = 1.0f;
            }
            zero.x = 0;
            zero.y = 0;
            zero.z = 0;
            func_00225DB0(D_800F7078, p->controller + 0x140, 0x11, zero, 51.199997f, f);
        }
    }
    if (!dead && (s16) p->state != 0xF) {
        func_00281054(p->camera, alpha & 0xFF, time);
    }
    if ((old >> 8) != (p->health >> 8)) {
        if (D_801198B0.settings.multiplayer) {
            if (p->options->tagMode == 1) {
                func_00275624(0x27DA);
            }
        } else if (!(p->aiFlags & 0x80)) {
            func_00275544(0x3AC0, src->pos, 0, -1);
        }
    }
}
