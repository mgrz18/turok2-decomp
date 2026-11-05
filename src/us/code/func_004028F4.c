#include "common.h"
#include "structs/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_004028F4;

typedef struct {
    /* 0x00 */ s32 model;
    /* 0x04 */ Vec3_004028F4 offset;
    /* 0x10 */ s32 unk10;
} Part004028F4;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ Vec3_004028F4 rot;
} Obj004028F4;

extern u8 D_8010ABA0[];
extern Part004028F4 D_004307B4[];
s16 func_0026D518(struct Pick *table);
void func_002688C8(Obj004028F4 *obj, Vec3_004028F4 pos, Vec3_004028F4 *posp, s32 *out, s32 a5, s32 a6);
void func_00228108(void *a0, s32 a1, s32 model, void *intel, s32 region, Vec3_004028F4 pos, s32 a8,
                   Vec3_004028F4 rot, Vec3_004028F4 offset, s32 a9);

void func_004028F4(s32 a0, Obj004028F4 *obj, s32 a2, Vec3_004028F4 pos) {
    WaveBossState *s = &D_8012F3A0;
    s32 index;
    s32 region;

    index = func_0026D518(s->step->partPicks);
    func_002688C8(obj, pos, &pos, &region, 0, 0);
    func_00228108(D_8010ABA0, -1, D_004307B4[index].model, s->intel[index], region, pos, 0, obj->rot,
                  D_004307B4[index].offset, 0);
}
