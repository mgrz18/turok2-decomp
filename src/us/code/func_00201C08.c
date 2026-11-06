#include "common.h"
#include "globals/engine.h"
#include "structs/decompressor.h"

void osSetThreadPri(OSThread *thread, OSPri pri);

void CDecompressor__Flush(CDecompressor *thisx) {
    if (thisx->queue.validCount != 0) {
        do {
            osSetThreadPri(&thisx->thread, gThreadPriorityDecompressorHigh);
        } while (thisx->queue.validCount != 0);
    }
    osSetThreadPri(&thisx->thread, gThreadPriorityDecompressorLow);
}
