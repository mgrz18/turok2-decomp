/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225D10.c, func_00225D10, with Turok 2's layouts (names from AbuN64/ragewars-decomp, WEAPONS/80225D10.CPP). */
/* A player travelling to a target: turn toward it and follow a parabola. */

#include "common.h"

typedef struct { f32 x, y, z; } Position;

typedef struct TravelTarget {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ Position pos;
    /* 0x40 */ f32 height;
    /* 0x44 */ u8 pad44[0x1A];
    /* 0x5E */ u8 duration;
} TravelTarget;

typedef struct TravelPlayer {
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ f32 yaw;
    /* 0x054 */ u8 pad54[0x2D8 - 0x54];
    /* 0x2D8 */ f32 bodyYaw;
    /* 0x2DC */ u8 pad2DC[0xAA0 - 0x2DC];
    /* 0xAA0 */ Position pos;
    /* 0xAAC */ u8 padAAC[0xB98 - 0xAAC];
    /* 0xB98 */ f32 elapsed;
    /* 0xB9C */ f32 targetYaw;
    /* 0xBA0 */ TravelTarget *target;
    /* 0xBA4 */ Position start;
} TravelPlayer;

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADE70;
extern void func_0024EF70(void *, void *, void *);
extern s32 func_0024E5F0(void *, void *, s32);
extern void func_00210EF0(f32 *value);

void func_00251828(TravelPlayer *player, void *body) {
    f32 yaw, targetYaw, alternate, duration, t;
    Position pos;

    func_0024EF70(player, body, &D_800ADE70);
    player->elapsed += D_800B6D28;
    yaw = player->yaw;
    targetYaw = player->targetYaw;
    if (targetYaw != yaw) {
        func_00210EF0(&yaw);
        func_00210EF0(&targetYaw);
        if (yaw > targetYaw) {
            alternate = targetYaw + 6.2831855f;
            if (alternate - yaw < yaw - targetYaw) {
                targetYaw = alternate;
            }
        } else {
            alternate = targetYaw - 6.2831855f;
            if (yaw - alternate < targetYaw - yaw) {
                targetYaw = alternate;
            }
        }
        if (targetYaw > yaw) {
            yaw += (targetYaw - yaw) * 0.2f;
            if (targetYaw < yaw) {
                yaw = targetYaw;
            }
        } else {
            yaw -= (yaw - targetYaw) * 0.2f;
            if (yaw < targetYaw) {
                yaw = targetYaw;
            }
        }
        func_00210EF0(&yaw);
    }
    player->bodyYaw = yaw;
    player->yaw = yaw;
    duration = (f32)(player->target->duration * 15);
    if (player->elapsed < duration) {
        t = player->elapsed / duration;
        pos.x = player->start.x + (player->target->pos.x - player->start.x) * t;
        pos.z = player->start.z + (player->target->pos.z - player->start.z) * t;
        pos.y = player->start.y + (player->target->pos.y - player->start.y) * t
              + player->target->height * ((t - t * t) * 4.0f);
        player->pos = pos;
        return;
    }
    func_0024E5F0(player, body, 2);
}
