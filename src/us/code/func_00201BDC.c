#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 arg;
} Request;

s32 osSendMesg(s32, s32, s32);

s32 CDecompressor__DecompressAsync(s32 arg0, Request *arg1, s32 arg2) {
    arg1->arg = arg2;
    return ~osSendMesg(arg0 + 0x230, (s32) arg1, 0) != 0;
}
