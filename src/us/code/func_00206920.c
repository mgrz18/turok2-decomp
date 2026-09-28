#include "common.h"
#include "m2c_macros.h"

typedef struct { s32 x, y, z; } V3;

typedef struct { u8 b[64]; } E64;
typedef struct { E64 *base; s32 n; s32 cur; E64 *end; } S;
extern s32 D_800B6D1C;

void func_00206920(S *arg0) {
    arg0->cur = arg0->n;
    arg0->end = arg0->base + arg0->n * D_800B6D1C;
}
