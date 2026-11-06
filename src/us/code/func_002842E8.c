#include "common.h"
#include "structs/cinema.h"

extern Cinema *D_800C1BB0;

void Cinema_ClearFinished(void) {
    D_800C1BB0->finished = 0;
}
