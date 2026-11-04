#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[5];
    /* 0x05 */ s8 unk5;
} Entry;

s8 func_00276844(Entry *arg0) {
    return arg0->unk5;
}
