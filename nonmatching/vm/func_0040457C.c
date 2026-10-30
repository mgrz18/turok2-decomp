/* NON-MATCHING: 5 of 434 words differ: the do/while(0) around the player lookup (needed for the ROM's s2/s3 choice) lets the loop optimizer hoist &D_80119870 above the actor->pos loads; without it 26 words differ (s2/s3 swapped). */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_0040457C;

typedef struct {
    /* 0x00 */ u8 flag;
    /* 0x01 */ u8 pad01[0x8 - 0x1];
    /* 0x08 */ s32 count;
    /* 0x0C */ f32 delay;
    /* 0x10 */ u8 pad10[0x14 - 0x10];
    /* 0x14 */ s32 type;
    /* 0x18 */ u8 pad18[0x1C - 0x18];
} Wave0040457C;

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[1];
} Actor0040457C;

typedef struct {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad004[0x344 - 0x4];
    /* 0x344 */ s32 handle;
    /* 0x348 */ s32 spawnsLeft;
    /* 0x34C */ s32 wavesLeft;
    /* 0x350 */ s32 enemiesAlive;
    /* 0x354 */ f32 spawnTimer;
    /* 0x358 */ s32 region;
    /* 0x35C */ u8 doorsOpen;
    /* 0x35D */ u8 pad35D[0x360 - 0x35D];
    /* 0x360 */ f32 waveTimer;
    /* 0x364 */ f32 nextTimer;
    /* 0x368 */ void *actor;
    /* 0x36C */ Actor0040457C *found[5];
    /* 0x380 */ u8 pad380[0x3D0 - 0x380];
    /* 0x3D0 */ s32 unk3D0;
    /* 0x3D4 */ s32 unk3D4;
    /* 0x3D8 */ u8 pad3D8[0x400 - 0x3D8];
    /* 0x400 */ f32 unk400;
    /* 0x404 */ Wave0040457C *waves;
    /* 0x408 */ u8 pad408[0x40C - 0x408];
    /* 0x40C */ s32 wave;
    /* 0x410 */ u8 wavesDone;
    /* 0x411 */ u8 pad411[0x420 - 0x411];
    /* 0x420 */ s32 unk420;
    /* 0x424 */ s32 resetRegion;
} ModeState0040457C;

typedef struct Enemy0040457C {
    /* 0x000 */ u8 pad0[0x144];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad1[0x1EC - 0x148];
    /* 0x1EC */ s32 unk1EC;
    /* 0x1F0 */ f32 range;
    /* 0x1F4 */ s32 unk1F4;
    /* 0x1F8 */ s32 region;
    /* 0x1FC */ u8 pad2[0x28C - 0x1FC];
    /* 0x28C */ struct Enemy0040457C *next;
} Enemy0040457C;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_0040457C pos;
    /* 0x10 */ s32 region;
    /* 0x14 */ u8 pad14[0x18 - 0x14];
    /* 0x18 */ Vec3_0040457C rot;
} Entity0040457C;

typedef struct {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ u8 pad1[0x88 - 0x28];
} Info0040457C;

extern ModeState0040457C D_8012F608;
extern u8 D_80119870[];
extern u8 D_8010ABA0[];
extern Enemy0040457C *D_8010FEB4;
extern s32 D_800F1F50;
Entity0040457C *func_002532EC(void *game, Vec3_0040457C pos);
void func_00243414(void *actor, void *ai, s32 event);
f32 func_002119FC(f32 lo, f32 hi);
s32 func_00228108(void *a0, s32 a1, s32 model, s32 handle, s32 region, Vec3_0040457C pos, f32 a6,
                  Vec3_0040457C rot, Vec3_0040457C scale, s32 a9);
void func_00246108(Entity0040457C *actor, void *level, Info0040457C *info, s32 a3, s32 a4);
void func_00404FE0(void);
void func_004050E0(Entity0040457C *actor, void *level, s32 a2);
void func_00405288(void);
void func_004053C4(Entity0040457C *actor, void *level, s32 a2);

static __inline__ Wave0040457C *CurrentWave(void) {
    return (Wave0040457C *)(D_8012F608.wave * sizeof(Wave0040457C) + (s32)D_8012F608.waves);
}

void func_0040457C(Entity0040457C *actor, void *level) {
    Info0040457C info;
    Vec3_0040457C scale;
    Vec3_0040457C pos;
    s32 nextWave = 0;
    Entity0040457C *player;
    Enemy0040457C *enemy;
    Wave0040457C *wave;
    s32 extra;
    f32 delay;
    s32 *flags;

    do {
        player = func_002532EC(D_80119870, actor->pos);
    } while (0);
    if (player == NULL) {
        return;
    }
    if (D_8012F608.resetRegion != 0) {
        D_8012F608.region = player->region;
        D_8012F608.resetRegion = 0;
    }
    if (!D_8012F608.doorsOpen) {
        if (D_8012F608.spawnsLeft > 0) {
            D_8012F608.doorsOpen = 1;
            func_00243414(D_8012F608.found[2], D_8012F608.found[2]->ai, 0);
            func_00243414(D_8012F608.found[3], D_8012F608.found[3]->ai, 0);
        }
    }
    if (D_8012F608.doorsOpen) {
        if (D_8012F608.spawnsLeft == 0 && D_8012F608.spawnTimer <= 0.0f) {
            D_8012F608.doorsOpen = 0;
            func_00243414(D_8012F608.found[2], D_8012F608.found[2]->ai, 1);
            func_00243414(D_8012F608.found[3], D_8012F608.found[3]->ai, 1);
        }
    }
    if (D_8012F608.wavesLeft > 0 && D_8012F608.nextTimer <= 0.0f && D_8012F608.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        D_8012F608.wavesLeft--;
        if (D_8012F608.wavesLeft == 0) {
            D_8012F608.wavesDone = 1;
            D_8012F608.wave = -1;
        } else if (extra != 3) {
            extra -= 3;
            D_8012F608.nextTimer = delay - extra * 15 + func_002119FC(0.0f, extra * 2) * 15.0f;
        } else {
            D_8012F608.nextTimer = delay;
        }
        func_00243414(D_8012F608.found[4], D_8012F608.found[4]->ai, 0);
    }
    if ((u32)(D_8012F608.spawnsLeft - 1) < 8 && D_8012F608.waveTimer <= 0.0f) {
        pos.x = 1669.12f;
        pos.y = 1064.96f;
        pos.z = 2816.0f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (func_00228108(D_8010ABA0, -1, 0x387, D_8012F608.handle, D_8012F608.region, pos, 0.0f, actor->rot,
                          scale, 0)) {
            D_8012F608.spawnsLeft--;
            if (D_8012F608.waves != NULL) {
                if (D_8012F608.spawnsLeft != 0) {
                    nextWave = 1;
                } else if (CurrentWave()->type != 1) {
                    D_8012F608.wavesDone = 1;
                    D_8012F608.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
            D_8012F608.spawnTimer = 120.0f;
        }
    }
    if ((u32)(D_8012F608.spawnsLeft - 1) < 8 && D_8012F608.waveTimer <= 0.0f) {
        pos.x = -1699.84f;
        pos.y = 1064.96f;
        pos.z = 2877.44f;
        scale.x = 2.0f;
        scale.y = 2.0f;
        scale.z = 2.0f;
        if (func_00228108(D_8010ABA0, -1, 0x387, D_8012F608.handle, D_8012F608.region, pos, 0.0f, actor->rot,
                          scale, 0)) {
            D_8012F608.spawnsLeft--;
            if (D_8012F608.waves != NULL) {
                if (D_8012F608.spawnsLeft != 0) {
                    nextWave = 1;
                } else if (CurrentWave()->type != 1) {
                    D_8012F608.wavesDone = 1;
                    D_8012F608.wave = -1;
                } else {
                    nextWave = 1;
                }
            }
        }
    }
    if (nextWave && D_8012F608.wave != -1) {
        delay = CurrentWave()->delay;
        extra = CurrentWave()->count;
        if (D_8012F608.waves != NULL) {
            if (extra != 3) {
                extra -= 3;
                D_8012F608.waveTimer = delay - extra * 15 + func_002119FC(0.0f, extra * 2) * 15.0f;
            } else {
                D_8012F608.waveTimer = delay;
            }
        }
    }
    D_8012F608.enemiesAlive = 0;
    for (enemy = D_8010FEB4; enemy != NULL; enemy = enemy->next) {
        enemy->unk1EC = 0;
        enemy->range = 768.0f;
        enemy->unk1F4 = 0;
        enemy->region = D_8012F608.region;
        if (enemy->health > 0) {
            D_8012F608.enemiesAlive++;
        }
    }
    flags = &D_800F1F50;
    *flags |= 0x8000;
    func_00246108(actor, level, &info, 0, 1);
    *flags &= ~0x8000;
    if (D_8012F608.wave == -1) {
        func_00404FE0();
    }
    func_004050E0(actor, level, info.unk24);
    if (D_8012F608.unk420 == -1) {
        func_00405288();
    }
    func_004053C4(actor, level, info.unk24);
    if (D_8012F608.waves != NULL) {
        if (D_8012F608.enemiesAlive == 0) {
            wave = CurrentWave();
            if (wave->type == 1 && wave->flag == 1 && D_8012F608.spawnsLeft == 0 && D_8012F608.unk400 <= 0.0f) {
                D_8012F608.wavesDone = 1;
                D_8012F608.wave = -1;
            }
        }
        if (D_8012F608.waves != NULL) {
            return;
        }
    }
    if (D_8012F608.unk3D0 == 1) {
        D_8012F608.unk3D4 = 0;
    }
}
