#include "common.h"

typedef struct {
    /* 0x0 */ s32 a;
    /* 0x4 */ s32 b;
    /* 0x8 */ s32 c;
    /* 0xC */ u16 d;
} Src29;

typedef struct {
    /* 0x00 */ char pad0[8];
    /* 0x08 */ s32 a;
    /* 0x0C */ s32 b;
    /* 0x10 */ char pad10[0xA];
    /* 0x1A */ u16 d;
} Dst29;

void func_0029321C(Dst29 *dst, Src29 *src) {
    dst->a = src->a;
    dst->d = src->d;
    dst->b = src->b;
}
