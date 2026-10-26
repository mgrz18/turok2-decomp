#include "common.h"

/* Advance an actor's animation (func_0021A7EC) and move it by the root motion
 * that produced: the new position is taken back out, turned by the actor's
 * rotation, collided along (func_0021A658), then the animation is applied. */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct Actor Actor;

struct Actor {
    u8 pad0[0x4];
    Vec3 pos;               /* 0x004 */
    u8 pad10[0x4];
    s32 *region;            /* 0x014 */
    u8 pad18[0x40];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    u8 padA8[0x8];
    s32 unkB0;              /* 0x0B0 */
    u8 padB4[0x8];
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x68];
    u8 ai[0x54];            /* 0x140 */
    u8 unk194[0xB0];        /* 0x194 */
    void (*init)(Actor *, void *);  /* 0x244 */
};

extern u8 D_800A5AC4[];     /* "anim object info" */
extern s32 D_800F7078;

s32 *func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
void *func_002017D4(s32, s32);
void func_002051F4(s32, s32 *);
void func_0020C0D0(void *, void *, void *);
void func_0020EEF8(Vec3 *, Vec3 *, void *);
void func_0020EF2C(Vec3 *, Vec3 *, Vec3 *);
void func_0021A500(Actor *);
void func_0021A658(Actor *, Vec3 *);
void func_0021A7EC(Actor *);
void func_002233A8(s32 *, Actor *, Vec3 *, Vec3 *);

void func_0021A344(Actor *actor) {
    Vec3 oldPos;
    Vec3 newPos;
    s32 *set;
    void *data;
    s32 *game;
    u32 flags;
    u32 moving;

    flags = actor->flags;
    if (!(flags & 0x40000)) {
        return;
    }
    game = &D_800F7078;
    if (*game != 4 || (flags & 0x2000)) {
        moving = flags & 0x300000;
        oldPos = actor->pos;
        func_0021A7EC(actor);
        newPos = actor->pos;
        actor->pos = oldPos;
        if (!moving && *actor->region != 0) {
            func_0020EF2C(&newPos, &newPos, &oldPos);
            func_0020EEF8(&newPos, &newPos, actor->unk194);
            func_0021A658(actor, &newPos);
            if (*actor->region == 1) {
                func_002233A8(game, actor, &oldPos, &actor->pos);
            }
        }
        func_0021A500(actor);
        set = func_0020367C(0, actor->unkA4, actor->unkB0, 0, 0, 0, (s32) D_800A5AC4, 1);
        if (set != NULL) {
            data = func_002017D4(*set, 0);
            actor->init(actor, actor->ai);
            func_0020C0D0(actor->unk58, data, actor->unkBC);
            func_002051F4(0, set);
        }
    }
}
