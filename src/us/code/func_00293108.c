/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNewMarker.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00292DE0(ALSeq *seq, ALEvent *event);
void func_00293108(ALSeq *seq, ALSeqMarker *m, u32 ticks);
void func_00292DE0(ALSeq *seq, ALEvent *event);
void func_00293108(ALSeq *seq, ALSeqMarker *m, u32 ticks) {
    ALEvent evt;
    u8 *ptr;
    u16 status;
    s32 lastTicks;
    u8 *savePtr;
    s32 saveTicks;
    u16 saveStatus;
    if (ticks == 0) {
        m->curPtr = seq->trackStart;
        m->lastStatus = 0;
        m->lastTicks = 0;
        m->curTicks = 0;
    } else {
        savePtr = seq->curPtr;
        saveStatus = seq->lastStatus;
        saveTicks = seq->lastTicks;
        seq->lastStatus = 0;
        seq->lastTicks = 0;
        seq->curPtr = seq->trackStart;
        do {
            ptr = seq->curPtr;
            status = seq->lastStatus;
            lastTicks = seq->lastTicks;
            func_00292DE0(seq, &evt);
            if (evt.type == 4) {
                ptr = seq->curPtr;
                status = seq->lastStatus;
                lastTicks = seq->lastTicks;
                break;
            }
        } while (seq->lastTicks < ticks);
        m->curPtr = ptr;
        m->lastStatus = status;
        m->lastTicks = lastTicks;
        m->curTicks = seq->lastTicks;
        seq->curPtr = savePtr;
        seq->lastStatus = saveStatus;
        seq->lastTicks = saveTicks;
    }
}
