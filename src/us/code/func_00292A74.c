/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seqplayer.c, __seqpReleaseVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void alSynSetVol(void *drvr, ALVoice *voice, s16 vol, s32 time);
void func_002943D0(void *drvr, ALVoice *voice, s16 priority);
void func_00291034(ALEventQueue *evtq, ALEventFull *evt, ALMicroTime delta);
void func_002933E0(ALLink *ln, ALLink *to);
void func_002933B0(ALLink *ln);
void func_00292A74(ALSeqPlayerFull *seqp, ALVoice *voice, s32 deltaTime);
void func_00292A74(ALSeqPlayerFull *seqp, ALVoice *voice, s32 deltaTime) {
    ALEventFull evt;
    ALVoiceState *vs = voice->clientPrivate;
    ALLink *thisNode;
    ALLink *nextNode;
    ALEventQueueItemFull *thisItem;
    ALEventQueueItemFull *nextItem;
    if (vs->envPhase == 0) {
        thisNode = seqp->evtq.allocList.next;
        while (thisNode != 0) {
            nextNode = thisNode->next;
            thisItem = (ALEventQueueItemFull *)thisNode;
            nextItem = (ALEventQueueItemFull *)nextNode;
            if (thisItem->evt.type == 6 && thisItem->evt.msg.voice.voice == voice) {
                if (nextItem != 0) {
                    nextItem->delta += thisItem->delta;
                }
                func_002933B0(thisNode);
                func_002933E0(thisNode, &seqp->evtq.freeList);
            }
            thisNode = nextNode;
        }
    }
    vs->velocity = 0;
    vs->envPhase = 3;
    vs->envGain = 0;
    vs->envEndTime = seqp->curTime + deltaTime;
    func_002943D0(seqp->drvr, voice, 0);
    alSynSetVol(seqp->drvr, voice, 0, deltaTime);
    evt.type = 5;
    evt.msg.voice.voice = voice;
    func_00291034(&seqp->evtq, &evt, deltaTime);
}
