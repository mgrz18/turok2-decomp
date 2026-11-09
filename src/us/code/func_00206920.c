#include "common.h"
#include "m2c_macros.h"
#include "globals/engine.h"
#include "structs/world.h"

void MtxPool_Reset(MtxPool *arg0) {
    arg0->remaining = arg0->n;
    arg0->next = arg0->base + arg0->n * gEvenOdd;
}
