/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNextEvent.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

void func_00292DE0(ALSeq *seq, ALEvent *event);
s32 func_002932BC(ALSeq *seq);
u8 func_00293310(ALSeq *seq);
void func_00292DE0(ALSeq *seq, ALEvent *event) {
    u8 status;
    u8 type;
    s32 deltaTicks;
    deltaTicks = func_002932BC(seq);
    seq->lastTicks += deltaTicks;
    status = func_00293310(seq);
    if (status == 0xFF) {
        type = func_00293310(seq);
        if (type == 0x51) {
            event->type = 3;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = func_00293310(seq);
            event->msg.tempo.byte1 = func_00293310(seq);
            event->msg.tempo.byte2 = func_00293310(seq);
            event->msg.tempo.byte3 = func_00293310(seq);
        } else if (type == 0x2F) {
            event->type = 4;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = func_00293310(seq);
        }
        seq->lastStatus = 0;
    } else {
        event->type = 1;
        event->msg.midi.ticks = deltaTicks;
        if (status & 0x80) {
            event->msg.midi.status = status;
            event->msg.midi.byte1 = func_00293310(seq);
            seq->lastStatus = status;
        } else {
            event->msg.midi.status = seq->lastStatus;
            event->msg.midi.byte1 = status;
        }
        if ((event->msg.midi.status & 0xF0) != 0xC0 && (event->msg.midi.status & 0xF0) != 0xD0) {
            event->msg.midi.byte2 = func_00293310(seq);
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
}
s32 func_002932BC(ALSeq *seq);
u8 func_00293310(ALSeq *seq);
