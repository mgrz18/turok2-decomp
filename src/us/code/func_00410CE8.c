#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    f32 m[4][4];
} Mtx4f;

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x34 - 0x10];
} ActorHead; /* the part of the actor saved around the collision test */

typedef struct {
    /* 0x00 */ ActorHead head;
    /* 0x34 */ u8 pad34[0x50 - 0x34];
    /* 0x50 */ f32 yaw;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u32 flags;
} CinemaSpec;

typedef struct {
    /* 0x000 */ u8 pad0[0x18];
    /* 0x018 */ Actor *target;
    /* 0x01C */ f32 time;
    /* 0x020 */ u8 pad20[0x30 - 0x20];
    /* 0x030 */ f32 start;
    /* 0x034 */ f32 end;
    /* 0x038 */ s32 active;
    /* 0x03C */ u8 pad3C[0x68 - 0x3C];
    /* 0x068 */ CinemaSpec spec;
    /* 0x078 */ u8 pad78[0xF0 - 0x78];
    /* 0x0F0 */ Vec3f position;
    /* 0x0FC */ f32 yaw;
} Cinema;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ f32 yaw;
    /* 0x2C */ f32 pitch;
    /* 0x30 */ u8 pad30[0x38 - 0x30];
    /* 0x38 */ Vec3f eye;
} Camera;

extern Cinema *D_800C1BB0;
extern Actor *D_80119890; /* current player */
extern u8 D_800F7078[];   /* world */
extern u8 D_800F1B40[];   /* collision info */
extern Vec3f D_800F1CE8;  /* collision point */

extern s32 Cinema_PathPlaying(void);
extern Vec3f func_0041057C(f32 t); /* eye path */
extern Vec3f func_00410724(f32 t); /* target path */
extern void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale); /* out = in * scale */
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = b - a */
extern void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a + b */
extern void func_0020F79C(Mtx4f *m);                         /* identity */
extern void func_0020FE00(Mtx4f *m, f32 x, f32 y, f32 z);    /* rotation */
extern void func_0020F85C(Mtx4f *m, Vec3f *in, Vec3f *out);  /* transform */
extern f32 func_0020EAA0(Vec3f *v);                          /* heading */
extern f32 func_0020F5C0(Vec3f *v);                          /* pitch */
extern void func_002216DC(void *world);
extern s32 func_00239F00(Actor *actor, Vec3f to, void *info);
extern void func_00226724(void *world);

/* Places the cinema relative to its target (flag 1) or to the player (flag 2). */
static __inline__ void Cinema_Anchor(Actor *player) {
    if ((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) {
        D_800C1BB0->position = D_800C1BB0->target->head.pos;
        D_800C1BB0->yaw = D_800C1BB0->target->yaw + 1.5707964f;
    }
    if (D_800C1BB0->spec.flags & 2) {
        if (player != NULL) {
            D_800C1BB0->position = player->head.pos;
            D_800C1BB0->yaw = player->yaw;
        } else {
            D_800C1BB0->position.x = 0.0f;
            D_800C1BB0->position.y = 0.0f;
            D_800C1BB0->position.z = 0.0f;
            D_800C1BB0->yaw = 0.0f;
        }
    }
}

/* Drives the camera along the cinema paths. */
void func_00410CE8(Camera *cam) {
    Vec3f eye;
    Vec3f target;
    Vec3f dir;
    ActorHead saved;
    Mtx4f mtx;
    Vec3f tmp;
    Actor *player;
    s32 hit;
    f32 yaw;

    if (!D_800C1BB0->active || !Cinema_PathPlaying()) {
        return;
    }
    if (D_800C1BB0->time < D_800C1BB0->start || D_800C1BB0->end < D_800C1BB0->time) {
        return;
    }
    eye = func_0041057C(D_800C1BB0->time);
    target = func_00410724(D_800C1BB0->time);
    func_0020EF60(&eye, &eye, 10.24f);
    func_0020EF60(&target, &target, 10.24f);
    cam->yaw = 0.0f;
    func_0020EF2C(&dir, &target, &eye);
    if (((D_800C1BB0->spec.flags & 1) && D_800C1BB0->target != NULL) || (D_800C1BB0->spec.flags & 2)) {
        player = D_80119890;
        Cinema_Anchor(player);
        func_0020F79C(&mtx);
        func_0020FE00(&mtx, 0.0f, D_800C1BB0->yaw, 0.0f);
        func_0020F85C(&mtx, &target, &tmp);
        target = tmp;
        func_0020F85C(&mtx, &eye, &tmp);
        eye = tmp;
        func_0020EEF8(&target, &target, &D_800C1BB0->position);
        func_0020EEF8(&eye, &eye, &D_800C1BB0->position);
        func_0020EF2C(&dir, &target, &eye);
        if (player != NULL) {
            saved = player->head;
            player->head.pos = target;
            func_002216DC(D_800F7078);
            hit = func_00239F00(player, eye, D_800F1B40);
            func_00226724(D_800F7078);
            player->head = saved;
            if (hit) {
                eye = D_800F1CE8;
            }
        }
    }
    yaw = func_0020EAA0(&dir);
    cam->pitch = -func_0020F5C0(&dir);
    cam->yaw += yaw;
    cam->eye = eye;
}
