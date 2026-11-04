#include "common.h"
#include "structs/sound.h"

void func_00274144(SndVoicePool *pool);                         /* extern */
void func_002766C8(SndQueue *queue, s32);                /* extern */

void func_00271570(SndMgr *mgr, s32 arg1) {
    func_00274144(&mgr->pool);
    func_002766C8(&mgr->queue, arg1);
}
