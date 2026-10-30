#include "common.h"

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

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 pushed;
} State00402FA0;

extern State00402FA0 D_8012F3A0;
s32 func_00219F74(Obj00402FA0 *obj, s32 a1, s32 a2);
void func_0020EF2C(Vec3_00402FA0 *out, Vec3_00402FA0 *from, Vec3_00402FA0 *to);
void func_00243414(Obj00402FA0 *obj, Item00402FA0 *item, s32 a2);

void func_00402FA0(Obj00402FA0 *obj, Item00402FA0 *item) {
    Vec3_00402FA0 dir;
    s32 anim;
    State00402FA0 *s = &D_8012F3A0;

    anim = func_00219F74(obj, 0x1004, -1);
    if (obj->anim == anim) {
        if (s->pushed == 0 && obj->time >= 10.0f) {
            s->pushed = 1;
            if (item->target != NULL) {
                func_0020EF2C(&dir, &item->target->pos, &obj->pos);
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
        func_00243414(obj, item, 8);
    }
}
