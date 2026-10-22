/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00250E10.c, func_00250E10.
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
void func_0029BB10(OSThread *thread);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void *func_00266C5C(void *s, int c, u32 n);
void func_0029B060(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri);
void func_0029B030(OSMesgQueue *mq, OSMesg *msgBuf, s32 count);
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
extern B12Mgr D_800D7BE0;
extern s32 D_800D8DCC;
extern u32 D_800AB0C0;
extern s32 D_800B6E8C;
extern u8 D_800D1CC0[];
void CMutex__Construct(void *lock);
u32 func_00266B80(void);
u8 *func_002024E0(void *heap, u32 size);
void CList__Construct(B12List *l, s32 prevOff, s32 nextOff);
void CList__AddTail(B12List *l, void *n);
void CHashTable__Construct(u32 n);
void D_002041E4(void *arg);
void func_00202BB0(s32 unused, s32 id) {
    u32 mask;
    u32 m2;
    u32 i;
    B12Node *n;
    B12Node *n1;
    u32 i1;
    s32 *t;
    CMutex__Construct(&D_800D7BE0.q11D0);
    mask = func_0029DFF0();
    if (++D_800D8DCC != 1) {
        func_0029E010(mask);
        osRecvMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        func_0029E010(mask);
    }
    D_800AB0C0 = 0x400;
    if (func_00266B80() > 0x400000) {
        D_800AB0C0 *= 2;
    }
    D_800D7BE0.nodes = (B12Node *)func_002024E0(D_800D1CC0, D_800AB0C0 * 0x28);
    D_800D7BE0.stk = (B12Node **)func_002024E0(D_800D1CC0, D_800AB0C0 * 4);
    D_800D7BE0.pool2 = func_002024E0(D_800D1CC0, D_800AB0C0 * 0x2C);
    func_0029B030(&D_800D7BE0.q11F0, &D_800D7BE0.buf11F0, 1);
    D_800D7BE0.tbl630[1] = 0;
    D_800D7BE0.tbl630[0] = 0;
    CList__Construct(&D_800D7BE0.lists[0], 0x18, 0x1C);
    CList__Construct(&D_800D7BE0.lists[1], 0x20, 0x24);
    D_800D7BE0.cnt = 0;
    for (i = 0; i < D_800AB0C0; i++) {
        n1 = D_800D7BE0.nodes;
        n1 = (B12Node *)((u8 *)n1 + i * 0x28);
        n1->flags = 0;
        D_800D7BE0.stk[i] = n1;
        D_800D7BE0.cnt++;
    }
    t = D_800D7BE0.tbl628;
    t[1] = 1;
    t[0] = 1;
    D_800D7BE0.f638 = -1;
    D_800D7BE0.tbl11C4[0] = 0;
    D_800D7BE0.tbl11C4[1] = 0;
    D_800D7BE0.cur = 0;
    D_800D7BE0.f120C = 0; /* 1 in Rage Wars */
    D_800D7BE0.stamp = 0;
    CList__Construct(&D_800D7BE0.lists3[0], 0x28, 0x24);
    CList__Construct(&D_800D7BE0.lists3[1], 0x28, 0x24);
    CList__Construct(&D_800D7BE0.lists3[2], 0x28, 0x24);
    for (i = 0; i < D_800AB0C0; i++) {
        n = (B12Node *)D_800D7BE0.pool2;
        n = (B12Node *)((u8 *)n + i * 0x2C);
        n->unk10 = 0;
        CList__AddTail(&D_800D7BE0.lists3[0], n);
    }
    CHashTable__Construct(D_800AB0C0);
    func_0029B030(&D_800D7BE0.q870, D_800D7BE0.buf870, 0x40);
    m2 = func_0029DFF0();
    if (--D_800D8DCC != 0) {
        func_0029E010(m2);
        osSendMesg(&D_800D7BE0.q11D0, 0, 1);
    } else {
        func_0029E010(m2);
    }
    func_00266C5C(D_800D7BE0.stack, (u8)id, 0x800);
    func_0029B060(&D_800D7BE0.thread, id, D_002041E4, 0, D_800D7BE0.stack + 0x800, D_800B6E8C);
    func_0029BB10(&D_800D7BE0.thread);
}
