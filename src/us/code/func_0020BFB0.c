#include "common.h"

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 count;
} Handle;

void func_0020BFB0(Handle *arg0) {
    arg0->id = -1;
    arg0->count = 0;
}
