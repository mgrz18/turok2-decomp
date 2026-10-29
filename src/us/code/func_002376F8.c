/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SYSTEM_2/802661FC_2.CPP, func_80267958_de (inlined here), with Turok 2's layouts. */
#include "common.h"

/* Applies a hit from an attacker to a target: marks the attacker, shakes the
 * target player's camera for heavy and medium hits, sprays blood from a
 * player attacker for blade kinds 4 and 5, reports it, and pushes the target
 * along the hit direction. */

typedef struct { f32 x, y, z; } Vec3f;
typedef struct { f32 x, y, z, w; } Quat4;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f offset;
} PlayerCamera;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ PlayerCamera *camera;
    /* 0x520 */ u8 pad520[0xA50 - 0x520];
    /* 0xA50 */ u8 *controller;
} Player;

typedef struct {
    /* 0x00 */ s32 type;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x3];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x14 - 0x10];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ Vec3f velocity;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 spin;
    /* 0x054 */ u8 pad54[0xD4 - 0x54];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x140 - 0xD8];
    /* 0x140 */ u32 aiFlags;
    /* 0x144 */ u8 pad144[0x17C - 0x144];
    /* 0x17C */ u32 hitFlags;
    /* 0x180 */ u8 pad180[0x1A8 - 0x180];
    /* 0x1A8 */ Player *player;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x66];
    /* 0x66 */ u8 kind;
} HitDef;

extern u8 D_800F8928[];
extern u8 D_800F7078[];

extern void func_002811CC(PlayerCamera *camera, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, Vec3f pos);
extern void func_00253F90(Player *player, Vec3f *v);
extern void func_0020E810(Quat4 *out, Vec3f *dir);
extern void func_002688C8(Actor *actor, Vec3f pos, Vec3f *outPos, s32 *outRegion, s32 flagA, s32 flagB);
extern s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Vec3f origin,
                         Quat4 rot, Vec3f pos, s32 arg, s32 b, s32 c);
extern void func_00225DB0(void *table, void *owner, s32 id, Vec3f pos, f32 a, f32 b);
extern void func_0026E110(Actor *actor, s32 mask, Actor *other);
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);       /* out = a + b */
extern f32 func_00246310(Actor *actor, Vec3f point);

/* Its unused target and def parameters still take stack slots when inlined (the frame layout needs them). */
static __inline__ void sprayBlood(Actor *target, Actor *actor, Vec3f position, Vec3f direction, HitDef *def, s32 kind) {
    Player *player;
    Vec3f v;
    Vec3f point;
    Vec3f zero;
    Quat4 rotation;
    s32 region;

    player = actor->player;
    v.x = -direction.x;
    v.y = direction.y;
    v.z = direction.z;
    func_00253F90(player, &v);
    v.x -= player->camera->offset.x;
    v.y -= player->camera->offset.y;
    v.z -= player->camera->offset.z;
    func_0020E810(&rotation, &v);
    func_002688C8(actor, position, &point, &region, 0, 1);
    point = position;
    point.y += 12.0f;
    zero.x = 0;
    zero.y = 0;
    zero.z = 0;
    func_0022F350(D_800F8928, actor, actor, 0, 0, 0xE9, zero, rotation, point, region, -1, kind == 5 ? 3 : 1);
    point.x = 0;
    point.y = 0;
    point.z = 0;
    func_00225DB0(D_800F7078, player->controller + 0x140, 3, point, 10.24f, 1.0f);
}

void func_002376F8(Actor *target, Actor *source, s32 hit, Vec3f pos, Vec3f dir, s32 push, HitDef *def, u32 flags) {
    Actor *attacker;
    u32 bits;
    Player *player;

    if (source->kind == 1) {
        source->aiFlags |= 0x4000000;
        attacker = source;
        if (target->flags & 0x300000) {
            bits = source->hitFlags;
            player = target->player;
            if (bits & 0x4000) {
                func_002811CC(player->camera, 20.0f, 100.0f, 20.0f, 1.0f, 1, pos);
            } else if (bits & 0x2000) {
                func_002811CC(player->camera, 10.0f, 50.0f, 10.0f, 1.0f, 1, pos);
            }
        }
        if (hit) {
            if (def != 0 && (attacker->flags & 0x300000) && (def->kind == 4 || def->kind == 5)) {
                sprayBlood(target, source, pos, dir, def, def->kind);
            }
            dir.x = 0;
            dir.y = 0;
            dir.z = 0;
        }
        if ((attacker->flags & 0x300000) && (flags & 0x400000)) {
            func_0026E110(target, 0x10, attacker);
        }
    }
    if (target->def->type == 1 || target->def->type == 11) {
        func_0020EEF8(&target->velocity, &target->velocity, &dir);
        if (push) {
            target->spin += func_00246310(target, source->pos) * 0.5f;
        }
    }
}
