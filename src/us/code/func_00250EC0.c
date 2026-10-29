#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Surface;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ void *region;
    /* 0x014 */ Surface *surface;
    /* 0x018 */ f32 pushX;
    /* 0x01C */ f32 pushY;
    /* 0x020 */ f32 pushZ;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0x140 - 0x54];
    /* 0x140 */ s32 flags;
} Body;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} Options;

typedef struct {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[4];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ u8 pad524[0xA10 - 0x524];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u8 padA14[0xA24 - 0xA14];
    /* 0x0A24 */ f32 moveScale;
    /* 0x0A28 */ u8 padA28[0xA54 - 0xA28];
    /* 0x0A54 */ f32 turnInput;
    /* 0x0A58 */ f32 lookInput;
    /* 0x0A5C */ f32 strafeInput;
    /* 0x0A60 */ f32 forwardInput;
    /* 0x0A64 */ u8 padA64[4];
    /* 0x0A68 */ u32 pressed;
    /* 0x0A6C */ u8 padA6C[0xA78 - 0xA6C];
    /* 0x0A78 */ f32 forward;
    /* 0x0A7C */ f32 strafe;
    /* 0x0A80 */ u8 padA80[0xA98 - 0xA80];
    /* 0x0A98 */ f32 bobSign;
    /* 0x0A9C */ f32 depth;
    /* 0x0AA0 */ Vec3f velocity;
    /* 0x0AAC */ u8 padAAC[0xAC8 - 0xAAC];
    /* 0x0AC8 */ f32 pitch;
    /* 0x0ACC */ f32 turn;
    /* 0x0AD0 */ f32 tilt;
    /* 0x0AD4 */ u8 padAD4[0xB8C - 0xAD4];
    /* 0x0B8C */ s32 zoomed;
    /* 0x0B90 */ u8 padB90[0xC04 - 0xB90];
    /* 0x0C04 */ s32 animation;
} Player;

typedef struct {
    u8 multiplayer;
} GameMode;

extern GameMode D_8011AAF1;
extern u8 D_800F7078[];
extern s16 D_800AE312[];        /* per character: climbing step sound (stride 4) */

s32 func_0026AFC0(void *region);
s32 func_0024E5F0(Player *player, Body *body, s32 mode);
void *func_0022425C(u8 *list, void *region);
f32 func_0026C434(void *region);
f32 func_00210E64(f32 from, f32 to);
void func_002117DC(f32 *value, f32 target, f32 rate);
f32 func_00268B50(Body *body);
f32 func_00268B98(Body *body);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
f32 func_0021170C(f32 value, f32 rate);
f32 func_0021160C(f32 value, f32 delta, f32 limit);
void func_0024FC08(Player *player, Body *body);
void func_00275624(s32 sound);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

#define ABS(x) ((x) < 0.0f ? -(x) : (x))

static __inline__ s32 try_jump(Player *player, Body *body) {
    if (player->zoomed == 0 && !(11.25f < player->depth) && !(body->flags & 0x6000)
        && (body->surface->flags & 2) && (player->pressed & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Climbing: face the wall, look up or down with the stick (tilting the view
 * back near the top and the bottom of the climb), jump off, climb with the
 * stick and play a step sound every 11.25 time units. */
void func_00250EC0(Player *player, Body *body) {
    f32 angle;
    f32 side;
    f32 sideRaw;
    f32 scale;
    f32 toTop;
    f32 toBottom;
    f32 lastForward;
    f32 s;
    f32 bob;
    f32 time;
    f32 c;
    s32 nearTop;
    s32 nearBottom;
    s32 up;
    s32 down;
    s32 anim;

    if (!func_0026AFC0(body->region) || !(body->flags & 0x1000)) {
        player->pitch = 0.0f;
        func_0024E5F0(player, body, 2);
        return;
    }
    func_0022425C(D_800F7078, body->region);
    func_002117DC(&body->yaw, body->yaw - func_00210E64(body->yaw, func_0026C434(body->region)), 0.9f);
    func_002117DC(&player->turn, player->turnInput * 3.4906588f, 0.25f);
    angle = -player->lookInput * 80.0f;
    if (angle > 0.0f) {
        angle *= 0.1875f;
    } else {
        angle *= 2.25f;
    }
    /* |turn * 80| * 1.625, written as one product of two picked factors */
    side = player->turnInput;
    sideRaw = side * 80.0f;
    if (sideRaw < 0.0f) {
        side = 1.625f;
        scale = -sideRaw;
    } else {
        scale = 130.0f;
    }
    angle -= side * scale;
    angle += 90.0f;
    toTop = func_00268B50(body);
    if (toTop < 0.0f) {
        toTop = 0.0f;
    }
    nearTop = 1;
    if (!(toTop <= 184.31999f)) {
        nearTop = 0;
    }
    toBottom = func_00268B98(body);
    if (toBottom < 0.0f) {
        toBottom = 0.0f;
    }
    nearBottom = 1;
    if (!(toBottom <= 81.92f)) {
        nearBottom = 0;
    }
    if (nearTop && nearBottom) {
        if (toTop < toBottom) {
            nearBottom = 0;
        } else {
            nearTop = 0;
        }
    }
    if (nearTop) {
        angle -= (0.5f - func_002974C0((1.0f - toTop * 0.0054253475f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (nearBottom) {
        angle -= (0.5f - func_002974C0((1.0f - toBottom * 0.012207031f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (angle < -90.0f) {
        angle = -90.0f;
    }
    func_002117DC(&player->pitch, angle * 0.017453294f, 0.5f);
    if (try_jump(player, body)) {
        s = func_002982F0(body->yaw + 6.2831855f);
        c = func_002974C0(body->yaw + 6.2831855f);
        body->pushX += s * 384.0f;
        body->pushZ += c * 384.0f;
        body->yaw += player->turn;
        player->turn = 0.0f;
        return;
    }
    bob = player->bobSign * player->forward * func_002982F0(player->stateTime * 0.13962635f);
    if (player->forwardInput > 0.0f || player->forwardInput < 0.0f) {
        func_002117DC(&player->tilt, bob, 0.25f);
    } else {
        func_002117DC(&player->tilt, 0.0f, 0.9f);
    }
    up = 1;
    if (!(player->forwardInput > 0.0f)) {
        up = 0;
    }
    down = 0;
    if (!D_8011AAF1.multiplayer && player->forwardInput < 0.0f) {
        down = 1;
    }
    lastForward = player->forward;
    if (up || down) {
        player->forward = func_0021160C(player->forward, player->forwardInput * 2.048f,
                                        ABS(player->forwardInput) * 14.3359995f);
    } else {
        player->forward = func_0021170C(lastForward, 4.096f);
    }
    if (player->strafeInput != 0.0f) {
        player->strafe = func_0021160C(player->strafe, player->strafeInput * 2.048f,
                                       ABS(player->strafeInput) * 14.3359995f);
    } else {
        player->strafe = func_0021170C(player->strafe, 4.096f);
    }
    if (player->forward == 0.0f && player->strafe == 0.0f) {
        player->stateTime = 0.0f;
        if (lastForward != 0.0f) {
            player->bobSign = -player->bobSign;
        }
        func_0024FC08(player, body);
    } else if ((time = player->stateTime) > 11.25f) {
        player->bobSign = -player->bobSign;
        player->stateTime = time - 11.25f;
        player->forward *= 0.1f;
        player->strafe *= 0.1f;
        if (D_8011AAF1.multiplayer) {
            if (player->options->tagMode == 1) {
                func_00275624(0x27DB);
            } else {
                func_00275544(D_800AE312[player->character * 2], body->pos, 0, -1);
            }
        } else {
            func_00275544(0x3AB7, body->pos, 0, -1);
        }
    }
    s = func_002982F0(body->yaw - 1.5707964f);
    c = func_002974C0(body->yaw - 1.5707964f);
    player->velocity.x += s * player->strafe * player->moveScale;
    player->velocity.z += c * player->strafe * player->moveScale;
    if (player->forward >= 0.0f) {
        s = func_002982F0(body->yaw + 3.1415927f);
        c = func_002974C0(body->yaw + 3.1415927f);
        player->velocity.x += s * player->forward * player->moveScale;
        player->velocity.z += c * player->forward * player->moveScale;
    } else {
        player->velocity.y += player->forward * 0.5f;
    }
    anim = 0xA46;
    if (!(player->forward > 0.0f)) {
        anim = 0xA3C;
    }
    player->animation = anim;
}
