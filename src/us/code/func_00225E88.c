#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
    /* 0x64 */ s32 file;
} Resource;

extern s32 CISet__GetBlock(s32, s32);

s32 CScene__GetIntelligence(Resource *arg0, s32 arg1) {
    if (arg1 == -1) {
        return 0;
    }
    return CISet__GetBlock(arg0->file, arg1);
}
