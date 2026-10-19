/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNextEvent.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
} ALMIDIEvent;
typedef struct {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
} ALTempoEvent;
typedef struct {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        s32 ticks;
        struct { s16 vol; } spvol;
        struct { void *seq; } spseq;
        struct { void *bank; } spbank;
        struct { struct ALSoundState_s *state; } sndp;
        struct { struct ALSoundState_s *state; u8 pan; } sndpan;
        struct { struct ALSoundState_s *state; s16 vol; } sndvol;
        struct { struct ALSoundState_s *state; f32 pitch; } sndpitch;
        struct { struct ALSoundState_s *state; u8 fxmix; } sndfx;
    } msg;
} ALEvent;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
typedef struct {
    u8 *base;
    u8 *trackStart;
    u8 *curPtr;
    s32 lastTicks;
    u32 len;
    f32 qnpt;
    s16 division;
    u16 lastStatus;
} ALSeq;
typedef struct {
    u8 pad00[0x0C];
    u8 samplePan;
    u8 sampleVolume;
} ALSound;
typedef struct ALSoundState_s {
    ALLink node;
    u8 voice[0x14];
    ALSound *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
} ALSoundState;
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
