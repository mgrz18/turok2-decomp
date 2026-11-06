/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seqplayer.c, __seqpReleaseVoice.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void alSynSetVol(void *drvr, ALVoice *voice, s16 vol, s32 time);
void alSynSetPriority(void *drvr, ALVoice *voice, s16 priority);
void alEvtqPostEvent(ALEventQueue *evtq, ALEvent *evt, ALMicroTime delta);
void alLink(ALLink *ln, ALLink *to);
void alUnlink(ALLink *ln);
void __seqpReleaseVoice(ALSeqPlayer *seqp, ALVoice *voice, s32 deltaTime);
void __seqpReleaseVoice(ALSeqPlayer *seqp, ALVoice *voice, s32 deltaTime) {
    ALEvent evt;
    ALVoiceState *vs = voice->clientPrivate;
    ALLink *thisNode;
    ALLink *nextNode;
    ALEventListItem *thisItem;
    ALEventListItem *nextItem;
    if (vs->envPhase == 0) {
        thisNode = seqp->evtq.allocList.next;
        while (thisNode != 0) {
            nextNode = thisNode->next;
            thisItem = (ALEventListItem *)thisNode;
            nextItem = (ALEventListItem *)nextNode;
            if (thisItem->evt.type == 6 && thisItem->evt.msg.note.voice == voice) {
                if (nextItem != 0) {
                    nextItem->delta += thisItem->delta;
                }
                alUnlink(thisNode);
                alLink(thisNode, &seqp->evtq.freeList);
            }
            thisNode = nextNode;
        }
    }
    vs->velocity = 0;
    vs->envPhase = 3;
    vs->envGain = 0;
    vs->envEndTime = seqp->curTime + deltaTime;
    alSynSetPriority(seqp->drvr, voice, 0);
    alSynSetVol(seqp->drvr, voice, 0, deltaTime);
    evt.type = 5;
    evt.msg.note.voice = voice;
    alEvtqPostEvent(&seqp->evtq, &evt, deltaTime);
}
