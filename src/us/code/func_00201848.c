#include "common.h"

typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offsets[1];
} OffsetTable;

s32 CISet__GetROMAddress(s32 arg0, s32 arg1, s32 arg2) {
    return arg1 + ((OffsetTable *)arg0)->offsets[arg2];
}
