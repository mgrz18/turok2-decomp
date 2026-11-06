#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"

typedef struct { u8 b[64]; } E64;
typedef struct { E64 *base; s32 n; s32 cur; E64 *end; } S;

void MtxPool_Reset(S *arg0) {
    arg0->cur = arg0->n;
    arg0->end = arg0->base + arg0->n * gEvenOdd;
}
