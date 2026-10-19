/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AD270_de_11.c, func_802AEAB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char func_0028FB80;
extern void func_002928F0(void *);
extern void *D_800B7760;
extern s32 func_002912A0(s32, s32, void *, s32, s32);
extern void func_00290F20(void *, void *, s32);
extern void func_00293F10(s32, void *);
void func_0028FA10(void *arg0, InitParams *arg1) {
    ALSeqPlayer *obj = arg0;
    InitParams *params = arg1;
    s32 context = params->context;
    s32 global = D_800B7760;
    s32 allocation;
    s32 i;
    obj->chanMask = 0xFF;
    obj->uspt = 0x1E8;
    obj->vol = 0x7FFF;
    obj->bank = 0;
    obj->target = 0;
    obj->nextDelta = 0;
    obj->state = 0;
    obj->frameTime = 0x3E80;
    obj->curTime = 0;
    obj->drvr = global;
    obj->initOsc = params->value10;
    obj->updateOsc = params->value14;
    obj->stopOsc = params->value18;
    obj->nextEvent.type = 9;
    obj->maxChannels = params->kind;
    obj->chanState = func_002912A0(0, 0, (void *)context, params->kind, 0x10);
    func_002928F0(obj);
    allocation = func_002912A0(0, 0, (void *)context, params->count38, 0x38);
    obj->vFreeList = 0;
    i = 0;
    if (params->count38 > 0) {
        {
            ALVoiceState *records = (ALVoiceState *)allocation;
            do {
                records->next = obj->vFreeList;
                obj->vFreeList = records;
                i++;
                records++;
            } while (i < params->count38);
        }
    }
    obj->vAllocHead = 0;
    obj->vAllocTail = 0;
    allocation = func_002912A0(0, 0, (void *)context, params->count1C, 0x1C);
    func_00290F20(&obj->evtq, (void *)allocation, params->count1C);
    {
        ALPlayer_s *node = &obj->node;
        node->next = 0;
        node->handler = &func_0028FB80;
        node->clientData = node;
        func_00293F10(D_800B7760, node);
    }
}
