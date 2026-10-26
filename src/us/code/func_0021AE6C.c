#include "common.h"

/* An actor's facing as a quaternion: turned half a circle from its owner's
 * player (flags 0x300000), its heading plus pitch (0x24C) in a region of
 * type 4, or else its heading applied to its own rotation (0x40). */

typedef struct {
    f32 x, y, z, w;
} Quat;

typedef struct {
    u8 pad0[0x12C];
    Quat rot;               /* 0x12C */
} Player;

typedef struct {
    u8 pad0[0x51C];
    Player *player;         /* 0x51C */
} Owner;

typedef struct {
    u8 pad0[0x14];
    s32 *region;            /* 0x014 */
    u8 pad18[0x28];
    Quat rot;               /* 0x040 */
    f32 heading;            /* 0x050 */
    u8 pad54[0x80];
    u32 flags;              /* 0x0D4 */
    u8 padD8[0xD0];
    Owner *owner;           /* 0x1A8 */
    u8 pad1AC[0xA0];
    f32 pitch;              /* 0x24C */
} Actor;

extern f32 D_800DD78C;

f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void func_00210F68(Quat *, Quat *, Quat *);

Quat func_0021AE6C(Actor *actor) {
    Quat result;
    Quat pitch;
    Quat yaw;
    Player *player;
    f32 s;

    if (actor->flags & 0x300000) {
        player = actor->owner->player;
        if (player == NULL) {
            goto own;
        }
        s = func_002982F0(1.5707964f);
        D_800DD78C = s;
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        yaw.w = func_002974C0(1.5707964f);
        func_00210F68(&result, &yaw, &player->rot);
    } else if (*actor->region == 4) {
        s = func_002982F0(actor->pitch * 0.5f);
        pitch.x = s;
        pitch.y = 0.0f;
        pitch.z = 0.0f;
        D_800DD78C = s;
        pitch.w = func_002974C0(actor->pitch * 0.5f);
        s = func_002982F0((actor->heading + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        D_800DD78C = s;
        yaw.w = func_002974C0((actor->heading + 3.1415927f) * 0.5f);
        func_00210F68(&result, &pitch, &yaw);
    } else {
    own:
        s = func_002982F0((actor->heading + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        D_800DD78C = s;
        yaw.w = func_002974C0((actor->heading + 3.1415927f) * 0.5f);
        func_00210F68(&result, &yaw, &actor->rot);
    }
    return result;
}
