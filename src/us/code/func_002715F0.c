#include "common.h"
#include "structs/sound.h"

void func_002742F4(SndVoicePool *pool);                         /* extern */
void func_00276790(SndQueue *queue, s32);                /* extern */

void func_002715F0(SndMgr *mgr, s32 arg1) {
    func_002742F4(&mgr->pool);
    func_00276790(&mgr->queue, arg1);
}
