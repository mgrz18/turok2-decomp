/* NON-MATCHING: 304 of 325 words differ (mostly a shift): frame 0x50 vs 0x48. In the ROM the 0.0f, the constant 3 and &timer3EC are not kept across the debris spawn calls (sw $zero / lui reloads), here they live in f21/s0/s1; everything else lines up. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403F3C;

typedef struct {
    u8 data[0x64];
} Light00403F3C;

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ s32 total;
    /* 0x008 */ s32 bonus;
    /* 0x00C */ u8 pad00C[0x1C - 0xC];
    /* 0x01C */ s32 stage;
    /* 0x020 */ u8 pad020[0x24 - 0x20];
    /* 0x024 */ Light00403F3C lights[8];
    /* 0x344 */ u8 pad344[0x354 - 0x344];
    /* 0x354 */ f32 timer354;
    /* 0x358 */ u8 pad358[0x360 - 0x358];
    /* 0x360 */ f32 timer360;
    /* 0x364 */ f32 timer364;
    /* 0x368 */ u8 pad368[0x380 - 0x368];
    /* 0x380 */ s32 values[2][8];
    /* 0x3C0 */ u8 pad3C0[0x3D0 - 0x3C0];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x3E0 - 0x3D8];
    /* 0x3E0 */ f32 timer3E0;
    /* 0x3E4 */ u8 pad3E4[0x3EC - 0x3E4];
    /* 0x3EC */ f32 timer3EC;
    /* 0x3F0 */ f32 timer3F0;
    /* 0x3F4 */ u8 pad3F4[0x400 - 0x3F4];
    /* 0x400 */ f32 timer400;
    /* 0x404 */ u8 pad404[0x414 - 0x404];
    /* 0x414 */ f32 timer414;
    /* 0x418 */ u8 pad418[0x428 - 0x418];
    /* 0x428 */ Vec3_00403F3C pos;
    /* 0x434 */ Vec3_00403F3C startPos;
} ModeState00403F3C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00403F3C pos;
} Actor00403F3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(Actor00403F3C *actor, void *level);
} Def00403F3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Def00403F3C *def;
    /* 0x34 */ u8 pad1[0x54 - 0x34];
    /* 0x54 */ Vec3_00403F3C spawn;
} Level00403F3C;

extern ModeState00403F3C D_8012F608;
extern f32 D_800B6D28;
extern u8 D_00431CD4[];
extern u8 D_00431CF0[];
extern u8 D_00431D0C[];
extern u8 D_00431D14[];
extern u8 D_801100F0[];
void func_0025AD90(Actor00403F3C *actor, Level00403F3C *level, s32 a2);
void func_00235510(void *light);
s16 func_0026D518(void *table);
void func_00406FE4(s16 type, s32 a1, Vec3_00403F3C offset);
void func_00404450(Actor00403F3C *actor, Level00403F3C *level);
void func_0040457C(Actor00403F3C *actor, Level00403F3C *level);
void func_0027AD88(void *a0, s32 percent);
void func_00243414(Actor00403F3C *actor, Level00403F3C *level, s32 event);

#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= D_800B6D28;         \
    }

void func_00403F3C(Actor00403F3C *actor, Level00403F3C *level) {
    Vec3_00403F3C offset;
    void *table;
    s32 i;

    func_0025AD90(actor, level, 0);
    if (level->def != NULL && level->def->onReset != NULL) {
        level->def->onReset(actor, level);
    }
    for (i = 0; i < 8; i++) {
        func_00235510(&D_8012F608.lights[i]);
    }
    if (D_8012F608.unk3D0 == 1) {
        level->spawn.x = D_8012F608.startPos.x;
        level->spawn.y = D_8012F608.startPos.y;
        level->spawn.z = D_8012F608.startPos.z;
    }
    COUNT_DOWN(D_8012F608.timer360);
    COUNT_DOWN(D_8012F608.timer3F0);
    COUNT_DOWN(D_8012F608.timer400);
    COUNT_DOWN(D_8012F608.timer414);
    COUNT_DOWN(D_8012F608.timer354);
    COUNT_DOWN(D_8012F608.timer3E0);
    COUNT_DOWN(D_8012F608.timer364);
    COUNT_DOWN(D_8012F608.timer3EC);
    if (D_8012F608.mode == 3) {
        if (D_8012F608.values[0][1] <= 0x95FF && D_8012F608.stage == 0) {
            D_8012F608.stage = 1;
        }
        if (D_8012F608.values[0][1] < 0x6400 && D_8012F608.stage == 1) {
            D_8012F608.stage = 2;
        }
        if (D_8012F608.values[0][1] < 0x3200 && D_8012F608.stage == 2) {
            D_8012F608.stage = 3;
        }
    }
    if (D_8012F608.timer3EC <= 0.0f) {
        offset.x = -2385.92f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.mode == 3) {
            table = D_00431CF0;
        } else {
            table = D_00431CD4;
        }
        func_00406FE4(func_0026D518(table), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -2324.48f;
        if (D_8012F608.mode == 3) {
            table = D_00431CF0;
        } else {
            table = D_00431CD4;
        }
        func_00406FE4(func_0026D518(table), 0x11D, offset);
        offset.x = 2314.24f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.mode == 3) {
            table = D_00431D14;
        } else {
            table = D_00431D0C;
        }
        func_00406FE4(func_0026D518(table), 0x11D, offset);
        offset.x = -460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        func_00406FE4(func_0026D518(D_00431CD4), 0x11D, offset);
        offset.x = 460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        func_00406FE4(func_0026D518(D_00431CD4), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 614.39996f;
        func_00406FE4(func_0026D518(D_00431CD4), 0x11D, offset);
        D_8012F608.timer3EC = 300.0f;
    }
    func_00404450(actor, level);
    func_0040457C(actor, level);
    func_0027AD88(D_801100F0, D_8012F608.total * 100 / D_8012F608.bonus);
    if (actor->pos.y < -204.79999f) {
        D_8012F608.unk3D4 = 11;
        D_8012F608.pos.x = 0.0f;
        D_8012F608.pos.y = 0.0f;
        D_8012F608.pos.z = 0.0f;
        func_00243414(actor, level, 0x19);
    }
}
