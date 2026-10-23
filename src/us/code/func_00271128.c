/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025828C.c, func_0025828C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
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
u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef union { s32 i; f32 f; } IF32;
typedef struct SndEnv SndEnv;
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
static __inline__ void SndMgr_Lock(SndMgr *m) {
    SndLock *l = &m->lock;
    u32 mask = func_0029DFF0();
    if (++l->count != 1) {
        func_0029E010(mask);
        osRecvMesg(&l->q, 0, 1);
    } else {
        func_0029E010(mask);
    }
}
static __inline__ void SndMgr_Unlock(SndMgr *m) {
    SndLock *l = &m->lock;
    u32 mask = func_0029DFF0();
    if (--l->count != 0) {
        func_0029E010(mask);
        osSendMesg(&l->q, 0, 1);
    } else {
        func_0029E010(mask);
    }
}
void *func_002017D4(void *node, s32 idx);
void func_00270A60(SndMgr *m, void *rec, Vec3f *pos, s32 id, s32 token, s32 a, s32 flags);
s32 func_00271128(SndMgr *m, s32 id, Vec3f pos, s32 flags) {
    u32 *rec;
    u32 count;
    u32 i;
    s32 token;
    void *n;
    if (!(id < m->unk2B54)) {
        return -1;
    }
    n = func_002017D4(m->tree2B4C, (s16)id);
    n = func_002017D4(n, 0);
    m->unk2B88 = n;
    m->unk2B8C = 0x100;
    m->unk2B90 = -1;
    n = func_002017D4(m->tree2B4C, (s16)id);
    rec = func_002017D4(n, 1);
    count = rec[1];
    if (count != 0) {
        SndMgr_Lock(m);
        token = ++m->unk10C;
        SndMgr_Unlock(m);
    } else {
        token = -1;
    }
    i = 0;
    if (count != 0) {
        do {
            func_00270A60(m, (u8 *)rec + (i * rec[0] + 8), &pos, id, token, 0, flags);
            i++;
        } while (i < count);
    }
    return token;
}
