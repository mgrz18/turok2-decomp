/* Types of the functions taken from thejunkman9000/Turok-Rage-Wars-Decomp (tools/import_ref.py),
 * gathered in one place by tools/centralize_types.py. Their names are that
 * decomp's own. */

#ifndef RW_JUNK_TYPES_H
#define RW_JUNK_TYPES_H

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;
typedef float f32;
typedef void *OSMesg;
typedef s32 OSPri;
typedef s32 OSId;
typedef union {
    struct { f32 f_odd; f32 f_even; } f;
} __OSfp;
typedef struct {
               u64 at, v0, v1, a0, a1, a2, a3;
               u64 t0, t1, t2, t3, t4, t5, t6, t7;
               u64 s0, s1, s2, s3, s4, s5, s6, s7;
               u64 t8, t9, gp, sp, s8, ra;
                u64 lo, hi;
                u32 sr, pc, cause, badvaddr, rcp;
                u32 fpcsr;
    __OSfp fp0, fp2, fp4, fp6, fp8, fp10, fp12, fp14;
    __OSfp fp16, fp18, fp20, fp22, fp24, fp26, fp28, fp30;
} __OSThreadContext;
typedef struct {
    u32 flag;
    u32 count;
    u64 time;
} __OSThreadprofile_s;
typedef struct OSThread_s {
               struct OSThread_s *next;
               OSPri priority;
               struct OSThread_s **queue;
               struct OSThread_s *tlnext;
               u16 state;
               u16 flags;
               OSId id;
               int fp;
               __OSThreadprofile_s *thprof;
               __OSThreadContext context;
} OSThread;
typedef struct OSMesgQueue_s {
               OSThread *mtqueue;
               OSThread *fullqueue;
               s32 validCount;
               s32 first;
               s32 msgCount;
               OSMesg *msg;
} OSMesgQueue;
typedef struct B12Node B12Node;
struct B12Node {
               u32 key;
               s32 unk4;
               s32 refs;
               u32 flags;
               u32 unk10;
               B12Node *link;
               B12Node *link18;
               u8 pad1C[4];
               B12Node *prev;
               B12Node *next;
};
typedef struct B12List {
               u8 *head;
               u8 *tail;
               s32 prevOff;
               s32 nextOff;
               s32 count;
} B12List;
typedef struct B12Bucket B12Bucket;
struct B12Bucket {
              u32 unk0;
              u32 unk4;
              s32 id;
              B12Bucket *next;
};
typedef struct B12Mgr {
                u8 pad0[0x5F0];
                B12Node * nodes;
                B12Node ** stk;
                B12Node * cur;
                u8 * pool2;
                B12List lists[2];
                s32 tbl628[2];
                s32 tbl630[2];
                s32 f638;
                u8 pad63C[0x4];
                OSThread thread;
                u8 pad7F0[0x80];
                OSMesgQueue q870;
                OSMesg buf870[0x40];
                u8 stack[0x800];
                 B12List lists3[3];
                 s32 tbl11C4[2];
                 s32 cnt;
                 OSMesgQueue q11D0;
                 OSMesg buf11D0;
                 s32 lockCnt;
                 OSMesgQueue q11F0;
                 OSMesg buf11F0;
                 s32 f120C;
                 u32 stamp;
                 u8 pad1214[0xC];
                 u32 hashMask;
                 B12Bucket * buckets;
} B12Mgr;
typedef struct B12Msg {
    B12Node *node;
    u8 pad4[0xC];
    u32 flags;
} B12Msg;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef signed short s16;
typedef struct R05Anim {
               f32 time;
               s16 f4;
               s16 f6;
               s16 count;
               u8 padA[2];
               u32 loopStart;
               void **data;
} R05Anim;
typedef struct R05CurveCfg {
               u8 pad0[0x18];
               f32 n;
               f32 scale;
} R05CurveCfg;
typedef struct R05Vec3i { s32 x, y, z; } R05Vec3i;
typedef struct B14Trk {
               s32 bits;
               f32 base;
               f32 range;
               u32 data;
               f32 c0;
               f32 c1;
               f32 c2;
               f32 c3;
               u32 span;
               u32 start;
               s32 cur;
               s32 f2C;
} B14Trk;
typedef struct { f32 c0, c1, c2, c3; u32 span; } B14Cubic;
typedef struct { u32 bits; f32 min; f32 range; } P3QInfo;
typedef struct { u32 pos; P3QInfo q; } P3QArr;
typedef struct B14FRange { u32 n; f32 min; f32 range; } B14FRange;
typedef struct B14FieldRd { u32 addr; B14FRange r; } B14FieldRd;
typedef struct { u32 pad; B14FRange r; } B14Tmp;
typedef struct { s32 f0; s32 f4; s32 f8; f32 fC; s32 f10; s32 f14; } B14Hdr;
typedef struct DLNode {
                struct DLNode *prev;
                struct DLNode *next;
} DLNode;
typedef struct DLList {
                DLNode *head;
                DLNode *tail;
                s32 count;
} DLList;
typedef struct NodePool {
                DLList freel;
                DLList usedl;
} NodePool;
typedef struct B19Spawn B19Spawn;
typedef struct B19PNode B19PNode;
typedef struct B19Pool B19Pool;
typedef struct B19Vec3 B19Vec3;
struct B19PNode {
              u8 pad0[0x4];
              B19PNode * next;
};
struct B19Pool {
              u8 pad0[0x14];
               B19PNode * head;
               u8 pad18[0x10];
               s32 count;
};
struct B19Vec3 {
              f32 x;
              f32 y;
              f32 z;
};
struct B19Spawn {
              u8 pad0[0x4];
              B19Spawn * next;
              void * src;
              B19Vec3 pos;
               f32 f18;
               f32 f1C;
               s32 h20;
               u8 pad24[0x8];
               s32 h2C;
               u8 pad30[0x8];
               B19Spawn ** link;
};
typedef struct B21Plane {
    f32 x, y, z, w;
} B21Plane;
typedef u32 OSEvent;
typedef struct ALLink_s {
    struct ALLink_s *next;
    struct ALLink_s *prev;
} ALLink;
typedef struct {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    void *heap;
    s32 outputRate;
    u8 fxType;
} ALSynConfig;
typedef struct B12AiCfg {
    u32 unk0;
    u32 unk4;
    s32 unk8;
} B12AiCfg;
typedef struct B12AiTask {
               s32 *cmd;
               u8 pad4[0x14];
               s32 f18;
               s32 f1C;
               u8 *f20;
               s32 f24;
               u8 *f28;
               s32 f2C;
               u8 *f30;
               s32 f34;
               s32 f38;
               s32 f3C;
               s32 f40;
               s32 f44;
               u8 pad48[8];
               s32 f50;
               s32 f54;
               OSMesgQueue *f58;
               struct B12AiTask *f5C;
               u8 pad60[0x10];
} B12AiTask;
typedef struct B12DbEnt2 {
    ALLink link;
    s32 f8;
    s32 fC;
    void *data;
} B12DbEnt2;
typedef union { s32 i; f32 f; } IF32;
typedef struct SndEnv SndEnv;
typedef struct SndCore SndCore;
typedef struct SndSeq SndSeq;
typedef struct SndFx SndFx;
typedef struct SndLink SndLink;
typedef struct SndMgr SndMgr;
typedef struct SndVoice SndVoice;
typedef struct SndNode SndNode;
typedef struct SndQueue SndQueue;
typedef struct SndVoices SndVoices;
typedef struct SndLock SndLock;
struct SndLink {
                 SndLink* prev;
                 SndLink* next;
};
struct SndEnv {
                 u16 w0;
                 u16 a;
                 u16 b;
                 s16 c;
                 u16 w8;
                 u16 wA;
};
struct SndVoice {
                 s32 idx;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 s32 unk10;
                 s32 unk14;
                 s32 unk18;
                 s32 unk1C;
                 s16 unk20;
                 u8 pad22[0x2];
                 s32 unk24;
                 s16 pan;
                 u8 pad2A[0x2];
                 f32 unk2C;
                 u8 pad30[0x4];
                 f32 unk34;
                 s16 unk38;
                 s16 unk3A;
                 s16 unk3C;
                 u8 pad3E[0x2];
                 s32 unk40;
                 Vec3f pos;
                 s32 unk50;
                 u8 unk54;
                 u8 pad55[0x3];
                 s32 unk58;
                 s32 unk5C;
                 u8 pad60[0x28];
                 s32 unk88;
                 SndEnv env;
                 f32 unk98;
                 f32 unk9C;
                 s32 unkA0;
                 s32 unkA4;
                 s32 tag;
                 s32 unkAC;
                 SndMgr* mgr;
                 SndMgr* unkB4;
                 f32 unkB8;
                 s32 unkBC;
                 s32 unkC0;
                 s32 unkC4;
                 u8 padC8[0x4];
};
struct SndNode {
                 SndLink link;
                 SndVoice body;
};
struct SndQueue {
                 SndMgr* owner;
                 SndNode active;
                 SndNode free;
                 SndNode nodes[32];
};
struct SndVoices {
                 SndMgr* owner;
                 SndVoice v[17];
};
struct SndLock {
                 OSMesgQueue q;
                 u8 pad18[0x4];
                 s32 count;
};
struct SndCore {
                 u8 pad0[0x60];
                 s16 snd[16];
};
struct SndSeq {
                 SndMgr* owner;
                 s32 unk4;
                 s32* unk8;
                 s32 unkC;
                 void* unk10;
                 void* csp;
                 s32 unk18;
                 s32 unk1C;
                 s32 unk20;
                 s32 unk24;
                 s32 unk28;
                 f32 vol;
                 f32 unk30;
                 f32 unk34;
                 s32 unk38;
                 f32 unk3C;
                 IF32 unk40;
};
struct SndFx {
                 SndMgr* owner;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 u8 pad10[0x4];
                 s32 unk14;
                 s32 unk18;
                 s32 sndId;
                 u8 pad20[0x20];
};
struct SndMgr {
                 u8 pad0[0x7C];
                 s32* unk7C;
                 u8 pad80[0x4];
                 u8 player[84];
                 u8 padD8[0x4];
                 s16 snd[16];
                 u8 padFC[0x6];
                 s16 unk102;
                 s32 unk104;
                 s32 unk108;
                 s32 unk10C;
                 SndLock lock;
                 s32 unk130;
                 s32 unk134;
                 SndQueue q;
                 SndSeq seq;
                 u8 heap[16];
                 SndVoices pool;
                 void* root2B48;
                 void* tree2B4C;
                 void* node2B50;
                 s32 unk2B54;
                 void* unk2B58;
                 s32* unk2B5C;
                 void* unk2B60;
                 s32 unk2B64;
                 u8 pad2B68[0x8];
                 u8* tab70;
                 u8* tab74;
                 u8* tab78;
                 s32 unk2B7C;
                 s32 unk2B80;
                 s32 unk2B84;
                 void* unk2B88;
                 s16 unk2B8C;
                 u8 pad2B8E[0x2];
                 s32 unk2B90;
                 u8 unk2B94;
                 u8 pad2B95[0x3];
                 s32 unk2B98;
                 s32 unk2B9C;
                 f32 unk2BA0;
                 f32 unk2BA4;
                 f32 unk2BA8;
                 s32 unk2BAC;
                 s32 unk2BB0;
                 s32 unk2BB4;
                 s32 unk2BB8;
                 u8 pad2BBC[0x4];
                 SndFx obj2BC0;
};
typedef struct SndInfo SndInfo;
struct SndInfo {
                 s16 w0;
                 s16 w2;
                 s8 b4;
                 s8 b5;
                 u16 bits6;
                 s16 w8;
                 s16 wA;
                 s16 wC;
                 s8 bE;
                 s8 bF;
                 s16 w10;
                 s16 w12;
                 s16 w14;
};
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
typedef union M04IF {
    s32 i;
    f32 f;
} M04IF;
typedef struct M04SortTab {
               u8 pad0[0xE];
               s16 n;
               u32 v[1];
} M04SortTab;
typedef struct M04Rec12 {
    u16 w0, w1, w2;
    s16 w3;
    u16 w4, w5;
} M04Rec12;
typedef struct M04Tgt {
                u8 pad0[0x128];
                f32 x;
                f32 y;
                f32 z;
                u8 pad134[0x528 - 0x134];
                f32 f528;
} M04Tgt;
typedef struct M04Ctx {
                 u8 pad0[0x84];
                 u8 player[0x58];
                 s16 snd[16];
                 u8 padFC[8];
                 s32 unk104;
                 u8 pad108[0x2B88 - 0x108];
                 u8 *tab2B88;
                 u8 pad2B8C[8];
                 s8 defaultPan;
                 u8 pad2B95[3];
                 M04Tgt *tgt;
                 u8 pad2B9C[4];
                 f32 volA;
                 f32 volB;
                 f32 volC;
                 u8 pad2BAC[0xC];
                 s32 unk2BB8;
} M04Ctx;
typedef struct M04VP {
               s32 w0;
               s32 w4;
               s32 vol;
               s32 wC;
               s16 s10;
               u8 pad12[2];
               M04IF v14;
               s16 pan;
               u8 pad1A[2];
               f32 scale;
               f32 f20;
               f32 f24;
               s16 s28;
               s16 s2A;
               s16 s2C;
               u8 pad2E[6];
               M04Vec3 pos;
               s32 f40;
               s8 mode;
               u8 pad45[3];
               f32 f48;
               f32 f4C;
} M04VP;
typedef struct M04Emitter {
               s32 w0;
               s32 w4;
               s32 w8;
               s32 id;
               M04VP vp;
               M04Rec12 env;
               f32 envA;
               f32 envSlope0;
               M04Rec12 env2;
               f32 env2A;
               f32 env2Slope;
               s32 hasEnv;
               M04Rec12 rec;
               f32 env0;
               f32 envSlope;
               s32 unkA0;
               u32 flags;
               s32 kind;
               s32 unkAC;
               M04Ctx *ctx;
               s32 unkB4;
               f32 unkB8;
               s32 unkBC;
               s32 resetFlag;
               s32 unkC4;
               f32 unkC8;
} M04Emitter;
typedef struct M04Bank2 {
    u8 pad0[0xC];
    M04SortTab *tab;
} M04Bank2;
typedef struct SndNodeRec { s32 w0; u16 h4; } SndNodeRec;
typedef struct AtlasCell {
    u32 w;
    u32 h;
} AtlasCell;
typedef struct ObjInit {
    s32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C;
    union { s32 i; f32 f; } unk20;
    Vec3f v;
    f32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
} ObjInit;
typedef struct DlCmd {
    u32 w0;
    u32 w1;
} DlCmd;
typedef struct B21Vp {
    s16 vscale[4];
    s16 vtrans[4];
} B21Vp;
typedef struct B21Hdr {
    u8 pad0[0x114];
    DlCmd *unk114;
} B21Hdr;
typedef struct B09LNode B09LNode;
struct B09LNode {
               B09LNode *next;
               B09LNode *prev;
               u16 slot;
               u8 id;
               u8 tlb;
               s32 unkC;
};
typedef struct B09SlotA {
            u16 key;
            u8 a;
            u8 b;
} B09SlotA;
typedef struct B09SlotCtl {
            u16 cur;
            u16 start;
} B09SlotCtl;
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
typedef s32 ALMicroTime;
typedef struct {
    ALLink freeList;
    ALLink allocList;
    s32 eventCount;
} ALEventQueue;
typedef struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
} ALPlayer;
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
typedef struct ALFilter_s {
    struct ALFilter_s *source;
    void *(*handler)(struct ALFilter_s *filter, s16 *outp, s32 outCount, s32 sampleOffset, void *cmdp);
    s32 (*setParam)(struct ALFilter_s *filter, s32 type, void *param);
    s16 inp;
    s16 outp;
    s32 type;
} ALFilter;
struct ALVoice_s;
typedef struct ALPVoice_s {
    ALLink node;
    struct ALVoice_s *vvoice;
    ALFilter *channelKnob;
    u8 decoder[0x48];
    u8 resampler[0x34];
    u8 envmixer[0x4C];
    s32 offset;
} ALPVoice;
typedef struct ALVoice_s {
    ALLink node;
    ALPVoice *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
} ALVoice;
typedef struct ALVoiceState_s {
    struct ALVoiceState_s *next;
    ALVoice voice;
    ALSound *sound;
    s32 envEndTime;
    f32 pitch;
    f32 vibrato;
    u8 envGain;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 envPhase;
    u8 phase;
    u8 tremelo;
    u8 flags;
} ALVoiceState;
typedef struct {
    void *inst;
    s16 bendRange;
    u8 fxId;
    u8 pan;
    u8 priority;
    u8 vol;
    u8 fxmix;
    u8 sustain;
    f32 pitchBend;
} ALChanStateFull;
typedef struct {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        s32 ticks;
        struct { s16 vol; } spvol;
        struct { void *seq; } spseq;
        struct { void *bank; } spbank;
        struct { ALVoice *voice; } voice;
        struct { ALVoiceState *state; void *oscState; u8 chan; } osc;
        struct { ALSeqMarker *loopStart; ALSeqMarker *loopEnd; s32 loopCount; } loop;
        struct { ALVoice *voice; s32 delta; u8 gain; } env;
        struct { u8 chan; u8 priority; } prio;
    } msg;
} ALEventFull;
typedef struct {
    ALLink node;
    ALMicroTime delta;
    ALEventFull evt;
} ALEventQueueItemFull;
typedef struct {
    ALPlayer node;
    void *drvr;
    ALSeq *target;
    s32 curTime;
    void *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    u8 pad36[2];
    ALEventFull nextEvent;
    ALEventQueue evtq;
    s32 frameTime;
    ALChanStateFull *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    void *initOsc;
    void *updateOsc;
    void *stopOsc;
    ALSeqMarker *loopStart;
    ALSeqMarker *loopEnd;
    s32 loopCount;
} ALSeqPlayerFull;
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
typedef struct {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
} ALHeap;
typedef struct {
    ALFilter filter;
    void *state;
    s16 f18;
    s16 f1A;
    s16 f1C;
    s16 f1E;
    s16 f20;
    s16 f22;
    union {
        s16 a;
        volatile s16 b;
    } f24;
    s16 f26;
    s16 f28;
    s16 f2A;
    s16 f2C;
    s16 f2E;
    s32 f30;
    s32 f34;
    s32 first;
    s32 f3C;
    s32 f40;
    s32 f44;
    s32 motion;
} ALEnvMixer;
typedef signed long long s64;
typedef struct {
    s64 quot;
    s64 rem;
} lldiv_t;
typedef struct O01Vec3 {
    f32 x, y, z;
} O01Vec3;
typedef struct O01SplRec {
               f32 x, y, z;
               f32 cx, cy, cz;
               f32 arc;
               f32 t;
               f32 c2;
} O01SplRec;
typedef struct {
               s32 status;
               OSMesgQueue *queue;
               s32 channel;
               u8 id[32];
               u8 label[32];
               s32 version;
               s32 dir_size;
               s32 inode_table;
               s32 minode_table;
               s32 dir_table;
               s32 inode_start_page;
               u8 banks;
               u8 activebank;
} OSPfs;
typedef struct O01Pad4 {
            u8 pad0[3];
            u8 unk3;
} O01Pad4;
typedef struct S7Light {
               u8 pad0[0x14];
               Vec3f dir;
               s32 enabled;
} S7Light;
typedef struct S5CtlEnt {
            u8 a;
            u8 b;
            u8 c;
            u8 d;
} S5CtlEnt;
typedef struct S5Ctl {
                s32 f0;
                s8 port;
                u8 pad5[3];
                s32 f8;
                s32 fC;
                s32 f10;
                s32 f14;
                s32 f18;
                s32 f1C;
                s32 f20;
                s32 f24;
                s32 f28;
                S5CtlEnt ent[32];
                u8 padAC[0xC4 - 0xAC];
                u8 fC4;
                u8 fC5;
                u8 fC6;
                u8 fC7;
                s32 fC8;
                s32 fCC;
                s32 fD0;
                s32 fD4;
                u8 padD8[0x140 - 0xD8];
                u8 blkA[0x2C];
                u8 blkB[0xB4];
                s32 f220;
} S5Ctl;
typedef struct S5PortFlags {
            u8 b[4];
} S5PortFlags;

#endif
