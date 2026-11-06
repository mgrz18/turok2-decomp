#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402FA0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
    /* 0x10 */ u8 pad0[0x18 - 0x10];
    /* 0x18 */ Vec3_00402FA0 vel;
    /* 0x24 */ u8 pad1[0xD8 - 0x24];
    /* 0xD8 */ f32 time;
    /* 0xDC */ s16 anim;
} Obj00402FA0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
} Target00402FA0;

typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ Target00402FA0 *target;
    /* 0x84 */ u8 pad1[0xC7 - 0x84];
    /* 0xC7 */ s8 count;
} Item00402FA0;

s32 CGameObjectInstance__PickRandomAnim(Obj00402FA0 *obj, s32 a1, s32 a2);
void CVector3__Subtract(Vec3_00402FA0 *out, Vec3_00402FA0 *from, Vec3_00402FA0 *to);
void CAI__SetMode(Obj00402FA0 *obj, Item00402FA0 *item, s32 a2);

void MantidQueen_UpdatePush(Obj00402FA0 *obj, Item00402FA0 *item) {
    Vec3_00402FA0 dir;
    s32 anim;
    WaveBossState *s = &gMantidQueen;

    anim = CGameObjectInstance__PickRandomAnim(obj, 0x1004, -1);
    if (obj->anim == anim) {
        if (s->pushed == 0 && obj->time >= 10.0f) {
            s->pushed = 1;
            if (item->target != NULL) {
                CVector3__Subtract(&dir, &item->target->pos, &obj->pos);
                obj->vel.x += dir.x * 0.75f;
                obj->vel.z += dir.z * 0.75f;
            }
        }
        if (obj->time >= 20.0f) {
            obj->vel.x = 0.0f;
            obj->vel.z = 0.0f;
        }
    }
    if (item->count != 0) {
        CAI__SetMode(obj, item, 8);
    }
}
