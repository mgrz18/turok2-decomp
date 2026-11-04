#include "common.h"
#include "structs/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403CF8;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00403CF8 pos;
} Actor00403CF8;

typedef struct {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u8 pad1[0x2C - 0x10];
    /* 0x02C */ void *data;
    /* 0x030 */ u8 pad2[0x37 - 0x30];
    /* 0x037 */ s8 unk37;
    /* 0x038 */ u8 pad3[0xF0 - 0x38];
    /* 0x0F0 */ void *onReset;
    /* 0x0F4 */ void *onEvent;
    /* 0x0F8 */ void *onHit;
    /* 0x0FC */ u8 pad4[0x108 - 0xFC];
    /* 0x108 */ void *onDraw;
} Level00403CF8;

extern s32 D_80110048;
extern s32 D_8012FB48;
extern s32 D_8012F600;
extern u8 D_800F7078[];
extern u8 D_0043173C[];
s32 func_00225F6C(void *world, s32 key0, s32 key1, s32 key2, void **out, s32 max);
void *func_00225EBC(void *world, s32 a1, s32 id);
void func_002362E4(void *light, void *actor);
void func_00403C14(s32 mode);
void func_004070B4(s32 a0);
void func_00406F10(s32 a0);
void func_00243414(void *actor, void *level, s32 event);
void func_00403F3C();
void func_004037B0();
void func_00403AD4();
void func_00407DC0();

void func_00403CF8(Actor00403CF8 *actor, Level00403CF8 *level) {
    s32 i;

    D_80110048 = 0x1DB2;
    level->data = D_0043173C;
    level->onReset = func_00403F3C;
    level->onHit = func_004037B0;
    level->onDraw = func_00403AD4;
    level->onEvent = func_00407DC0;
    level->unkC = -1;
    D_8012F608.actor = actor;
    func_00225F6C(D_800F7078, -1, 6, 0xC8, &D_8012F608.found[1], 1);
    func_00225F6C(D_800F7078, -1, 6, 0xC9, &D_8012F608.found[0], 1);
    func_00225F6C(D_800F7078, -1, 6, 0xCA, &D_8012F608.found[2], 1);
    func_00225F6C(D_800F7078, -1, 6, 0xCB, &D_8012F608.found[3], 1);
    func_00225F6C(D_800F7078, -1, 6, 0xCC, &D_8012F608.found[4], 1);
    D_8012F608.intel = func_00225EBC(D_800F7078, 1, 0x1DB2);
    for (i = 0; i < 8; i++) {
        func_002362E4(&D_8012F608.anims[i], actor);
    }
    func_00403C14(1);
    func_004070B4(1);
    D_8012F608.bossMode = 1;
    D_8012F608.bossState = 1;
    D_8012F608.spawnsLeft = 0;
    D_8012F608.enemiesAlive = 0;
    D_8012F608.resetRegion = 1;
    D_8012F608.doorsOpen = 0;
    D_8012F608.point.y = 0;
    D_8012F608.speed = 100.0f;
    D_8012F608.point.x = 5.0f;
    D_8012F608.point.z = 180.0f;
    func_00406F10(1);
    D_8012F608.script.timer = 0;
    D_8012F608.script2.timer = 0;
    D_8012F608.spawnStep = 0;
    D_8012F608.subStage = 0;
    D_8012F608.waveTimer = 0;
    D_8012F608.spawnTimer = 0;
    D_8012F608.script3.timer = 0;
    D_8012F608.timer3EC = 0;
    D_8012F608.home.x = actor->pos.x;
    D_8012F608.home.y = actor->pos.y;
    D_8012F608.home.z = actor->pos.z;
    D_8012FB48 = 0;
    D_8012F600 = 0;
    func_00243414(actor, level, 0);
    level->unk37 = 1;
}
