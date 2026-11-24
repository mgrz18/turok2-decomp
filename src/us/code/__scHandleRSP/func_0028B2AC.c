/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FC10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/engine.h"
#include "globals/sysrc.h"

void Sched_HandleCmdMsg(OSSched *arg0) {
    OSScTask *node = 0;
    OSScTask *value1 = 0;
    OSScTask *value2 = 0;
    s32 flags;
    while (osRecvMesg(&arg0->cmdQ, (OSMesg *) &node, 0) != -1) {
        if (node->list.t.type == 1 && (node->flags & 0x20) && gDecompressorBoostRetraces == 0) {
            gDecompressorBoostRetraces = 2;
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorHigh);
        }
        __scAppendList(arg0, node);
    }
    if (arg0->doAudio != 0 &&
        arg0->curRSPTask != 0) {
        __scYield(arg0);
        return;
    }
    flags = (arg0->curRSPTask == 0) * 2 |
            (arg0->curRDPTask == 0);
    if (__scSchedule(arg0, &value1, &value2, flags) != flags) {
        __scExec(arg0, value1, value2);
    }
}
