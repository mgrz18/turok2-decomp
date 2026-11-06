/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNextEvent.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void alSeqNextEvent(ALSeq *seq, ALEvent *event);
s32 __alSeqReadVarLen(ALSeq *seq);
u8 __alSeqReadByte(ALSeq *seq);
void alSeqNextEvent(ALSeq *seq, ALEvent *event) {
    u8 status;
    u8 type;
    s32 deltaTicks;
    deltaTicks = __alSeqReadVarLen(seq);
    seq->lastTicks += deltaTicks;
    status = __alSeqReadByte(seq);
    if (status == 0xFF) {
        type = __alSeqReadByte(seq);
        if (type == 0x51) {
            event->type = 3;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = __alSeqReadByte(seq);
            event->msg.tempo.byte1 = __alSeqReadByte(seq);
            event->msg.tempo.byte2 = __alSeqReadByte(seq);
            event->msg.tempo.byte3 = __alSeqReadByte(seq);
        } else if (type == 0x2F) {
            event->type = 4;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = __alSeqReadByte(seq);
        }
        seq->lastStatus = 0;
    } else {
        event->type = 1;
        event->msg.midi.ticks = deltaTicks;
        if (status & 0x80) {
            event->msg.midi.status = status;
            event->msg.midi.byte1 = __alSeqReadByte(seq);
            seq->lastStatus = status;
        } else {
            event->msg.midi.status = seq->lastStatus;
            event->msg.midi.byte1 = status;
        }
        if ((event->msg.midi.status & 0xF0) != 0xC0 && (event->msg.midi.status & 0xF0) != 0xD0) {
            event->msg.midi.byte2 = __alSeqReadByte(seq);
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
}
s32 __alSeqReadVarLen(ALSeq *seq);
u8 __alSeqReadByte(ALSeq *seq);
