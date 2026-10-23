#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Actor {
    u8 type;            /* 0x0 */
    u8 pad1[3];
    Vec3 pos;           /* 0x4 */
    s32 unk10;          /* 0x10 */
    u8 pad14[4];
    Vec3 unk18;         /* 0x18 */
} Actor;

typedef struct EventParams {
    s32 unk0;           /* 0x0 */
    f32 size;           /* 0x4 */
} EventParams;

extern u8 D_8010ABA0[];
extern void func_00227C10(void *arg0, s32 arg1, s32 arg2, Vec3 pos, s32 arg6, Vec3 rot, Vec3 scale, s32 arg13);

void func_00239194(Actor *actor, s32 arg1, s32 arg2, Vec3 arg3, EventParams params) {
    Vec3 scale;
    Vec3 rot;

    if (actor->type == 0) {
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
    } else {
        rot = actor->unk18;
    }
    scale.x = params.size;
    scale.y = params.size;
    scale.z = params.size;
    func_00227C10(D_8010ABA0, params.unk0, actor->unk10, actor->pos, 0, rot, scale, 0);
}
