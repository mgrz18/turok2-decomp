#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/cache.h"
#include "protos/player.h"
#include "protos/game.h"

typedef struct LevelState {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ void *handle98;
    /* 0x9C */ void *handle9C;
    /* 0xA0 */ void *handleA0;
    /* 0xA4 */ void *handleA4;
    /* 0xA8 */ void *handleA8;
    /* 0xAC */ void *handleAC;
    /* 0xB0 */ void *handleB0;
    /* 0xB4 */ void *handleB4;
    /* 0xB8 */ u8 padB8[0x10];
    /* 0xC8 */ void *handleC8;
    /* 0xCC */ void *handleCC;
} LevelState;


extern void CPlayer__StopLoopingSounds(void *);
extern void LoopSounds_StopAll(sptr);
extern void Sound__DetachAll(void);
extern void Sound__StopFlags(s32);

/* Tears the level down: releases its resources' handles. */
void CScene__UnloadLevel(LevelState *level) {
    void *p;

    p = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (p != NULL) {
        CPlayer__StopLoopingSounds(p);
    }
    LoopSounds_StopAll(LoopSounds_Get());
    Sound__DetachAll();
    Sound__StopFlags(0x1000);
    CCache__Deallocate(0, level->handle98);
    CCache__Deallocate(0, level->handle9C);
    CCache__Deallocate(0, level->handleA0);
    CCache__Deallocate(0, level->handleA4);
    CCache__Deallocate(0, level->handleA8);
    CCache__Deallocate(0, level->handleB0);
    CCache__Deallocate(0, level->handleB4);
    CCache__Deallocate(0, level->handleCC);
    CCache__Deallocate(0, level->handleAC);
    CCache__Deallocate(0, level->handleC8);
}
