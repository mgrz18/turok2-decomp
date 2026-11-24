/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra_inferred/802AD870_7.CPP, func_802AE030_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

struct MeasuredRecAt18 { unsigned char padding[24]; void * value; };
struct MeasuredRecAt0V3 { u8 value; };
u8 __getTrackByte(ALCSeq *seq, u32 track); /* ABI: returns u8 (the host leaves the upper bits of the return register undefined) */
u32 __readVarLen(ALCSeq *seq, u32 track);
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 track, ALEvent *event);
u32 __alCSeqGetTrackEvent(ALCSeq *seq, u32 index, ALEvent *event) {
    u8 first;
    u8 command;
    u32 first_value;
    u32 command_value;
    first = __getTrackByte(seq, index);
    first_value = first & 0xFF;
    if (first_value == 0xFF) {
        command = __getTrackByte(seq, index);
        command_value = command & 0xFF;
        if (command_value == 0x51) {
            event->type = 3;
            event->msg.tempo.status = first;
            event->msg.tempo.type = command;
            event->msg.tempo.byte1 = __getTrackByte(seq, index);
            event->msg.tempo.byte2 = __getTrackByte(seq, index);
            event->msg.tempo.byte3 = __getTrackByte(seq, index);
            seq->lastStatus[index] = 0;
        } else if (command_value == 0x2F) {
            u32 active = seq->validTracks ^ (1U << index);
            seq->validTracks = active;
            if (active != 0) {
                event->type = 0x12;
            } else {
                event->type = 4;
            }
        } else if (command_value == 0x2E) {
            __getTrackByte(seq, index);
            __getTrackByte(seq, index);
            seq->lastStatus[index] = 0;
            event->type = 0x13;
        } else if (command_value == 0x2D) {
            void *base;
            void *p;
            u8 lead;
            u8 count_byte;
            u32 count;
            u32 byte0;
            u32 byte1;
            u32 byte2;
            u32 byte3;
            void *next;
            base = (u8 *)seq + (index * 4);
            p = ((struct MeasuredRecAt18 *)(base))->value;
            lead = ((struct MeasuredRecAt0V3 *)(p))->value;
            p = (u8 *)p + 1;
            count_byte = ((struct MeasuredRecAt0V3 *)(p))->value;
            count = count_byte & 0xFF;
            if (count == 0) {
                next = (u8 *)p + 5;
                ((struct MeasuredRecAt0V3 *)(p))->value = lead;
            } else {
                if (count != first_value) {
                    ((struct MeasuredRecAt0V3 *)(p))->value = count_byte - 1;
                }
                p = (u8 *)p + 1;
                byte0 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte1 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte2 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                byte3 = ((struct MeasuredRecAt0V3 *)(p))->value;
                p = (u8 *)p + 1;
                next = (u8 *)p - ((byte0 << 24) + (byte1 << 16) +
                                   (byte2 << 8) + byte3);
            }
            ((struct MeasuredRecAt18 *)(base))->value = next;
            seq->lastStatus[index] = 0;
            event->type = 0x14;
        }
    } else {
        event->type = 1;
        if (first & 0x80) {
            event->msg.midi.status = first;
            event->msg.midi.byte1 = __getTrackByte(seq, index);
            seq->lastStatus[index] = first;
        } else {
            u8 running_status = seq->lastStatus[index];
            event->msg.midi.byte1 = first;
            event->msg.midi.status = running_status;
        }
        command_value = event->msg.midi.status & 0xF0;
        if ((command_value != 0xC0) && (command_value != 0xD0)) {
            event->msg.midi.byte2 = __getTrackByte(seq, index);
            if ((event->msg.midi.status & 0xF0) == 0x90) {
                event->msg.midi.duration = __readVarLen(seq, index);
            }
        } else {
            event->msg.midi.byte2 = 0;
        }
    }
    return 1;
}
