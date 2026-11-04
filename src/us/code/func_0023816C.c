#include "common.h"
#include "structs/vector.h"

typedef struct Actor {
    u8 type;            /* 0x0 */
    u8 pad1[0xD4 - 1];
    s32 flags;          /* 0xD4 */
    u8 padD8[0x1A8 - 0xD8];
    struct Player *owner; /* 0x1A8 */
} Actor;

typedef struct Player {
    u8 pad0[4];
    CVector3 pos;           /* 0x4 */
    u8 pad10[0x288 - 0x10];
    Actor actor;        /* 0x288 */
    u8 pad[0x996 - 0x288 - sizeof(Actor)];
    u16 state;          /* 0x996 */
} Player;

extern void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
extern f32 func_002671B4(Player *player);
extern f32 D_800A65B0;

s32 func_0023816C(Actor *actor, CVector3 *pos) {
    s32 ret = 0;
    Player *player;
    u16 state;
    CVector3 a;
    CVector3 b;

    if (actor->type == 1 && (actor->flags & 0x300000)) {
        player = actor->owner;
        if (actor == &player->actor) {
            state = player->state;
            if (state < 2 || (s16) state == 0x22) {
                a = *pos;
                b = player->pos;
                func_0020EF8C(pos, 0.75f, &a, &b);
                pos->y += func_002671B4(player) * D_800A65B0;
                ret = 1;
            }
        }
    }
    return ret;
}
