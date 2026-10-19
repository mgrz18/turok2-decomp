/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00252450.c, func_00252450.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

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
u32 func_0029DFF0(void);
void func_0029E010(u32 mask);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
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
extern B12Mgr D_800D7BE0;
extern OSMesgQueue D_800D8DB0;
extern s32 D_800D8DCC;
extern u8 D_800D6D68[];
extern u8 D_800D1CC0[];
void func_002067C4(s32 a, u32 b);
s32 func_00201BDC(void *port, B12Msg *n, void *q);
void CList__Remove(void *l, B12Msg *n);
void CList__AddHead(void *l, B12Msg *n);
void func_002065F8(s32 a, B12Node *n);
void func_002026EC(void *heap, s32 key);
void func_002062F8(s32 a, B12Node *n);
void func_002066B0(s32 a, B12Msg *n);
void func_002044A8(s32 a, B12Msg *n, s32 b);
void func_002041E4(void) {
    B12Msg *msg;
    B12Node *p;
    u32 mask;
    u32 m2;
    while (1) {
        osRecvMesg(&D_800D7BE0.q870, (OSMesg *)&msg, 1);
        if ((u32)msg < 2) {
            mask = func_0029DFF0();
            if (++D_800D8DCC != 1) {
                func_0029E010(mask);
                osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
            } else {
                func_0029E010(mask);
            }
            func_002067C4(0, (u32)msg);
            m2 = func_0029DFF0();
            if (--D_800D8DCC != 0) {
                func_0029E010(m2);
                osSendMesg(&D_800D8DB0, 0, 1);
            } else {
                func_0029E010(m2);
            }
        } else {
            msg->flags &= ~2;
            if (msg->flags & 8) {
                msg->flags &= ~8;
                if (func_00201BDC(D_800D6D68, msg, &D_800D7BE0.q870) != 0) {
                    continue;
                }
                mask = func_0029DFF0();
                if (++D_800D8DCC != 1) {
                    func_0029E010(mask);
                    osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
                } else {
                    func_0029E010(mask);
                }
                CList__Remove(&D_800D7BE0.lists3[2], msg);
                CList__AddHead(&D_800D7BE0.lists3[1], msg);
                p = msg->node;
                p->flags &= ~2;
                while (p->refs != 0) {
                    p->refs--;
                    if (p->refs == 0) {
                        p->flags &= ~0x100;
                    }
                }
                if (!(p->flags & 0x702)) {
                    func_002065F8(0, p);
                    func_002026EC(D_800D1CC0, p->key);
                    func_002062F8(0, p);
                }
                func_002066B0(0, msg);
                m2 = func_0029DFF0();
                if (--D_800D8DCC != 0) {
                    func_0029E010(m2);
                    osSendMesg(&D_800D8DB0, 0, 1);
                } else {
                    func_0029E010(m2);
                }
            } else {
                msg->flags &= ~4;
                mask = func_0029DFF0();
                if (++D_800D8DCC != 1) {
                    func_0029E010(mask);
                    osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
                } else {
                    func_0029E010(mask);
                }
                func_002044A8(0, msg, 1);
                m2 = func_0029DFF0();
                if (--D_800D8DCC != 0) {
                    func_0029E010(m2);
                    osSendMesg(&D_800D8DB0, 0, 1);
                } else {
                    func_0029E010(m2);
                }
            }
        }
    }
}
