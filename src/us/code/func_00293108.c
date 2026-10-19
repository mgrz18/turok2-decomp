/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/libultra/seq.c, alSeqNewMarker.
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
    u8 *curPtr;
    s32 lastTicks;
    s32 curTicks;
    u16 lastStatus;
} ALSeqMarker;
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
