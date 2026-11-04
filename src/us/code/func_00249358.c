#include "common.h"
#include "structs/player.h"

typedef struct { f32 x, y, z; } Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
} RegionInfo;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
} HitSource;

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
s32 func_002548FC(CPlayer *p);
RegionInfo *func_0022425C(void *level, void *region);
void func_00249118(CPlayer *p, HitSource *src, s32 arg3, s32 type);
void func_0027FC2C(Game *game, void *camera, char *text);
void func_0027580C(s32 sound);
void func_00225DB0(void *level, u8 *rumble, s32 id, Vec3f v, f32 f6, f32 f7);
void func_00281054(void *camera, s32 alpha, s32 time);
void func_00275624(s32 sound);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Hurts a player: damage (8.8) scaled by the reduced-damage state and the
 * multiplayer handicap, unless the player is protected; then the red flash,
 * the low-health warning, the rumble and the hurt sound. */
void func_00249358(CPlayer *p, HitSource *src, s32 damage, s32 arg3, s32 type) {
    s32 old;
    f32 f;
    s32 check;
    RegionInfo *info;
    s32 prev;
    s32 health;
    s32 dead;
    s32 wasDead;
    s32 alpha;
    s32 time;
    s32 amount;
    Vec3f zero;

    f = 1.0f;
    old = p->m_Data.m_Health;
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
    if ((u16)p->m_Mode < 2) {
        return;
    }
    if (p->m_pOnEnemy) {
        damage /= 5;
    }
    if (D_801198B0.settings.multiplayer) {
        if (p->m_pOptions->m_Handicap) {
            damage = damage * (1.0f / 256) / ((p->m_pOptions->m_Handicap + f) * 0.3f) * 256.0f;
        }
    }
    if (D_801198B0.settings.flags & 0x10) {
        check = 1;
    } else {
        check = p->m_Data.m_SpecialPickupCount[384] & 0xF;
    }
    if (check && p->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = func_0022425C(D_800F7078, ((void *)p->m_CharacterActor.header.m_pCurrentRegion));
        if (info != NULL && (info->flags & 0x400000)) {
            return;
        }
    }
    if (p->m_InvincibilityTime > 0.0f && type != 0x1D) {
        return;
    }
    prev = p->m_Data.m_Health;
    wasDead = prev == 0;
    health = prev - damage;
    if (health < 0) {
        health = 0;
    }
    p->m_CharacterActor.m_AI.m_Health = (p->m_Data.m_Health = health);
    p->m_WeaponActor.m_AI.m_Health = p->m_Data.m_Health;
    dead = p->m_Data.m_Health == 0;
    if (dead) {
        if (!wasDead) {
            func_00249118(p, src, arg3, type);
        }
        alpha = 0xFF;
        time = 0xC;
    } else {
        alpha = ((damage >> 8) * 0xFF) / 20;
        alpha = (MIN(alpha, 0xFF) < 0x80) ? 0x80 : MIN(alpha, 0xFF);
        time = 0x1E;
        if (old > 0xF00 && p->m_Data.m_Health <= 0xF00 && !D_801198B0.settings.multiplayer) {
            func_0027FC2C(&D_801198B0, ((void *)p->m_pCamera), D_800B66D0);
            func_0027580C(0x156);
        }
        amount = damage >> 8;
        if (amount != 0) {
            f = MIN(amount * (1.0f / 15), 1.0f);
            zero.x = 0;
            zero.y = 0;
            zero.z = 0;
            func_00225DB0(D_800F7078, ((u8 *)p->m_ContState.m_pContState) + 0x140, 0x11, zero, 51.199997f, f);
        }
    }
    if (!dead && (s16) (u16)p->m_Mode != 0xF) {
        func_00281054(((void *)p->m_pCamera), alpha & 0xFF, time);
    }
    if ((old >> 8) != (p->m_Data.m_Health >> 8)) {
        if (D_801198B0.settings.multiplayer) {
            if (p->m_pOptions->m_TagMode == 1) {
                func_00275624(0x27DA);
            }
        } else if (!(p->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
            func_00275544(0x3AC0, src->pos, 0, -1);
        }
    }
}
