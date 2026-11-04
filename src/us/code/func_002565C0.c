#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0xAB0];
    /* 0xAB0 */ f32 unkAB0;
    /* 0xAB4 */ f32 unkAB4;
    /* 0xAB8 */ s32 unkAB8;
    /* 0xABC */ s32 unkABC;
} Player;

void func_002565C0(Player *arg0) {
    arg0->unkAB8 = 0;
    arg0->unkABC = 0;
    arg0->unkAB4 = (f32) arg0->unkAB0;
}
