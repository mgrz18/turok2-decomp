/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FC10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/engine.h"
#include "globals/sysrc.h"

extern void __scAppendList(struct OSSched *arg0, void *arg1);
extern void __scYield(void *arg0);
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern s32 __scSchedule(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void __scExec(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern void osSetThreadPri(void *arg0, s32 arg1);
s32 __scSchedule(Ctx_func_8028F544_de *arg0, Node_func_8028F544_de **arg1, Node_func_8028F544_de **arg2, s32 arg3);
void __scAppendList(OSSched *arg0, void *arg1);
void __scExec(View_func_8028FA60_de *view, Scene_func_8028FA60_de *current, Scene_func_8028FA60_de *next);
void __scYield(void *arg0);
void Sched_HandleCmdMsg(void *arg0) {
    Node_func_8028FC10_de *node = 0;
    s32 value1 = 0;
    s32 value2 = 0;
    s32 flags;
    while (osRecvMesg(&((View_func_8028FA60_de *)arg0)->messages, &node, 0) != -1) {
        if (node->type == 1 && (node->flags & 0x20) && gDecompressorBoostRetraces == 0) {
            gDecompressorBoostRetraces = 2;
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorHigh);
        }
        __scAppendList(arg0, node);
    }
    if (((View_func_8028FA60_de *)arg0)->pending != 0 &&
        ((View_func_8028FA60_de *)arg0)->current != 0) {
        __scYield(arg0);
        return;
    }
    flags = (((View_func_8028FA60_de *)arg0)->current == 0) * 2 |
            (((View_func_8028FA60_de *)arg0)->next == 0);
    if (((s32 (*)(void *, s32 *, s32 *, s32))__scSchedule)(arg0, &value1, &value2, flags) != flags) {
        ((void (*)(void *, s32, s32))__scExec)(arg0, value1, value2);
    }
}
