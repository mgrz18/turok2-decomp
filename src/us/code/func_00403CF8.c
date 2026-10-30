#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00403CF8;

typedef struct {
    u8 data[0x64];
} Light00403CF8;

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad004[0xC - 0x4];
    /* 0x00C */ f32 unk00C;
    /* 0x010 */ u8 pad010[0x18 - 0x10];
    /* 0x018 */ s32 unk018;
    /* 0x01C */ s32 unk01C;
    /* 0x020 */ u8 pad020[0x24 - 0x20];
    /* 0x024 */ Light00403CF8 lights[8];
    /* 0x344 */ void *handle;
    /* 0x348 */ s32 unk348;
    /* 0x34C */ u8 pad34C[0x350 - 0x34C];
    /* 0x350 */ s32 unk350;
    /* 0x354 */ s32 unk354;
    /* 0x358 */ u8 pad358[0x35C - 0x358];
    /* 0x35C */ u8 unk35C;
    /* 0x35D */ u8 pad35D[0x360 - 0x35D];
    /* 0x360 */ s32 unk360;
    /* 0x364 */ u8 pad364[0x368 - 0x364];
    /* 0x368 */ void *actor;
    /* 0x36C */ void *found[5];
    /* 0x380 */ u8 pad380[0x3D0 - 0x380];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x3EC - 0x3D8];
    /* 0x3EC */ s32 unk3EC;
    /* 0x3F0 */ s32 unk3F0;
    /* 0x3F4 */ u8 pad3F4[0x400 - 0x3F4];
    /* 0x400 */ s32 unk400;
    /* 0x404 */ u8 pad404[0x414 - 0x404];
    /* 0x414 */ s32 unk414;
    /* 0x418 */ u8 pad418[0x424 - 0x418];
    /* 0x424 */ s32 unk424;
    /* 0x428 */ Vec3_00403CF8 pos;
    /* 0x434 */ Vec3_00403CF8 startPos;
    /* 0x440 */ f32 unk440;
    /* 0x444 */ s32 unk444;
    /* 0x448 */ f32 unk448;
} ModeState00403CF8;

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

extern ModeState00403CF8 D_8012F608;
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
    D_8012F608.handle = func_00225EBC(D_800F7078, 1, 0x1DB2);
    for (i = 0; i < 8; i++) {
        func_002362E4(&D_8012F608.lights[i], actor);
    }
    func_00403C14(1);
    func_004070B4(1);
    D_8012F608.unk3D4 = 1;
    D_8012F608.unk3D0 = 1;
    D_8012F608.unk348 = 0;
    D_8012F608.unk350 = 0;
    D_8012F608.unk424 = 1;
    D_8012F608.unk35C = 0;
    D_8012F608.unk444 = 0;
    D_8012F608.unk00C = 100.0f;
    D_8012F608.unk440 = 5.0f;
    D_8012F608.unk448 = 180.0f;
    func_00406F10(1);
    D_8012F608.unk3F0 = 0;
    D_8012F608.unk400 = 0;
    D_8012F608.unk018 = 0;
    D_8012F608.unk01C = 0;
    D_8012F608.unk360 = 0;
    D_8012F608.unk354 = 0;
    D_8012F608.unk414 = 0;
    D_8012F608.unk3EC = 0;
    D_8012F608.startPos.x = actor->pos.x;
    D_8012F608.startPos.y = actor->pos.y;
    D_8012F608.startPos.z = actor->pos.z;
    D_8012FB48 = 0;
    D_8012F600 = 0;
    func_00243414(actor, level, 0);
    level->unk37 = 1;
}
