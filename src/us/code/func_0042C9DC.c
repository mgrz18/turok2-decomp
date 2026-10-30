#include "common.h"

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

extern u8 D_80119870[];

extern void *func_002532A8(void *);
extern void func_0042EB04(void *);
extern s32 func_00275F7C(void);
extern void func_00275EFC(s32);
extern void func_00275B58(void);
extern void func_002758DC(s32);
extern void func_002052D8(s32, void *);

/* Tears the level down: releases its resources' handles. */
void func_0042C9DC(LevelState *level) {
    void *p;

    p = func_002532A8(D_80119870);
    if (p != NULL) {
        func_0042EB04(p);
    }
    func_00275EFC(func_00275F7C());
    func_00275B58();
    func_002758DC(0x1000);
    func_002052D8(0, level->handle98);
    func_002052D8(0, level->handle9C);
    func_002052D8(0, level->handleA0);
    func_002052D8(0, level->handleA4);
    func_002052D8(0, level->handleA8);
    func_002052D8(0, level->handleB0);
    func_002052D8(0, level->handleB4);
    func_002052D8(0, level->handleCC);
    func_002052D8(0, level->handleAC);
    func_002052D8(0, level->handleC8);
}
