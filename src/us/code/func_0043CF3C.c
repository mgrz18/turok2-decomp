#include "common.h"
#include "structs/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_0043CF3C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_0043CF3C pos;
} Actor0043CF3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(Actor0043CF3C *actor, void *level);
} Def0043CF3C;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Def0043CF3C *def;
    /* 0x34 */ u8 pad1[0x54 - 0x34];
    /* 0x54 */ Vec3_0043CF3C spawn;
} Level0043CF3C;

extern f32 D_800B6D28;
extern u8 D_00431BF4[];
extern u8 D_00431C10[];
extern u8 D_00431C2C[];
extern u8 D_00431C34[];
extern u8 D_801100F0[];
void func_0025AD90(Actor0043CF3C *actor, Level0043CF3C *level, s32 a2);
void func_00235510(void *light);
s16 func_0026D518(void *table);
void func_00406FE4(s16 type, s32 a1, Vec3_0043CF3C offset);
void func_00404450(Actor0043CF3C *actor, Level0043CF3C *level);
void func_0040457C(Actor0043CF3C *actor, Level0043CF3C *level);
void func_0027AD88(void *a0, s32 percent);
void func_00243414(Actor0043CF3C *actor, Level0043CF3C *level, s32 event);

#define COUNT_DOWN(t)              \
    if ((t) > 0.0f) {              \
        (t) -= D_800B6D28;         \
    }

void func_0043CF3C(Actor0043CF3C *actor, Level0043CF3C *level) {
    Vec3_0043CF3C offset;
    s32 type;
    s32 i;

    func_0025AD90(actor, level, 0);
    if (level->def != NULL && level->def->onReset != NULL) {
        level->def->onReset(actor, level);
    }
    for (i = 0; i < 8; i++) {
        func_00235510(&D_8012F608.anims[i]);
    }
    if (D_8012F608.bossState == 1) {
        level->spawn.x = D_8012F608.home.x;
        level->spawn.y = D_8012F608.home.y;
        level->spawn.z = D_8012F608.home.z;
    }
    COUNT_DOWN(D_8012F608.waveTimer);
    COUNT_DOWN(D_8012F608.script.timer);
    COUNT_DOWN(D_8012F608.script2.timer);
    COUNT_DOWN(D_8012F608.script3.timer);
    COUNT_DOWN(D_8012F608.spawnTimer);
    COUNT_DOWN(D_8012F608.swayTimer);
    COUNT_DOWN(D_8012F608.nextTimer);
    COUNT_DOWN(D_8012F608.timer3EC);
    if (D_8012F608.stage == 3) {
        if (D_8012F608.values[0][1] <= 0x95FF && D_8012F608.subStage == 0) {
            D_8012F608.subStage = 1;
        }
        if (D_8012F608.values[0][1] < 0x6400 && D_8012F608.subStage == 1) {
            D_8012F608.subStage = 2;
        }
        if (D_8012F608.values[0][1] < 0x3200 && D_8012F608.subStage == 2) {
            D_8012F608.subStage = 3;
        }
    }
    if (D_8012F608.timer3EC <= 0.0f) {
        offset.x = -2385.92f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.stage == 3) {
            type = func_0026D518(D_00431C10);
        } else {
            type = func_0026D518(D_00431BF4);
        }
        func_00406FE4(type, 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = -2324.48f;
        if (D_8012F608.stage == 3) {
            type = func_0026D518(D_00431C10);
        } else {
            type = func_0026D518(D_00431BF4);
        }
        func_00406FE4(type, 0x11D, offset);
        offset.x = 2314.24f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        if (D_8012F608.stage == 3) {
            type = func_0026D518(D_00431C34);
        } else {
            type = func_0026D518(D_00431C2C);
        }
        func_00406FE4(type, 0x11D, offset);
        offset.x = -460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        func_00406FE4(func_0026D518(D_00431BF4), 0x11D, offset);
        offset.x = 460.8f;
        offset.y = 0.0f;
        offset.z = -460.8f;
        func_00406FE4(func_0026D518(D_00431BF4), 0x11D, offset);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 614.39996f;
        func_00406FE4(func_0026D518(D_00431BF4), 0x11D, offset);
        D_8012F608.timer3EC = 300.0f;
    }
    func_00404450(actor, level);
    func_0040457C(actor, level);
    func_0027AD88(D_801100F0, D_8012F608.total * 100 / D_8012F608.bonus);
    if (actor->pos.y < -204.79999f) {
        D_8012F608.bossMode = 11;
        D_8012F608.dest.x = 0.0f;
        D_8012F608.dest.y = 0.0f;
        D_8012F608.dest.z = 0.0f;
        func_00243414(actor, level, 0x19);
    }
}
