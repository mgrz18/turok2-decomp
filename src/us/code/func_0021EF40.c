#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Player {
    u8 pad0[0x51C];
    u8 *unk51C;         /* 0x51C */
} Player;

typedef struct Actor {
    u8 pad0[4];
    Vec3 pos;           /* 0x4 */
    u8 pad10[0xD4 - 0x10];
    s32 flags;          /* 0xD4 */
    u8 padD8[0x1A8 - 0xD8];
    Player *owner;      /* 0x1A8 */
} Actor;

extern void func_0020F85C(void *matrix, Vec3 *in, Vec3 *out);
extern void func_0021F00C(Vec3 *out, Actor *actor, Vec3 v);
extern void func_0020EEF8(Vec3 *out, Vec3 *a, Vec3 *b);
extern void func_002688C8(Actor *actor, Vec3 pos, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void func_0021EF40(Actor *actor, Vec3 v, s32 arg4, s32 arg5) {
    Vec3 local;
    Vec3 out;

    if (actor->flags & 0x300000) {
        v.x = -v.x;
        v.z = -v.z;
        func_0020F85C(actor->owner->unk51C + 0x14C, &v, &out);
    } else {
        func_0021F00C(&local, actor, v);
        func_0020EEF8(&out, &actor->pos, &local);
    }
    func_002688C8(actor, out, arg4, arg5, 1, 1);
}
