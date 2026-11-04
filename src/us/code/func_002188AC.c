#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
} Entry;

s32 func_002188AC(Entry *arg0) {
    return arg0->unkC;
}
