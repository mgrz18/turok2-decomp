#include "common.h"

void Controller_UnwatchPak(s32);
s32 CScene__RequestWarp(s32, s32, s32);

extern s32 D_800F7078;
extern s32 D_80110054;

void func_00421A20(s32 arg0, s32 arg1) {
    Controller_UnwatchPak(arg1);
    CScene__RequestWarp((s32) &D_800F7078, D_80110054, 2);
}
