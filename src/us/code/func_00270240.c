/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00257380.c, func_00257380, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 q[0x18];          /* OSMesgQueue */
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 count;
} SndLock;

typedef struct {
    /* 0x00 */ s32 maxVVoices;
    /* 0x04 */ s32 maxPVoices;
    /* 0x08 */ s32 maxUpdates;
    /* 0x0C */ s32 dmaproc;
    /* 0x10 */ s32 fxType;
    /* 0x14 */ void *heap;
    /* 0x18 */ s32 pad18;
    /* 0x1C */ s8 v7;
    /* 0x1D */ u8 pad1D[3];
    /* 0x20 */ void *tab;
} SynCfg;

typedef struct {
    s32 rate;
    s32 one;
    s32 size;
} AiCfg;

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s16 n;
    /* 0x10 */ u32 v[1];
} SortTab;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 f4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ void *fC;
} BankRes;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 f4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ SortTab *tab;
} Bank;

typedef struct {
    /* 0x0000 */ u8 pad0[0x24];
    /* 0x0024 */ s32 playerCfg[3];      /* ALSndpConfig: maxSounds, maxEvents, heap */
    /* 0x0030 */ u8 pad30[0x4C];
    /* 0x007C */ Bank *bank;
    /* 0x0080 */ BankRes *bankRes;
    /* 0x0084 */ u8 player[0x54];
    /* 0x00D8 */ s32 unkD8;
    /* 0x00DC */ s16 snd[16];
    /* 0x00FC */ u8 padFC[6];
    /* 0x0102 */ s16 unk102;
    /* 0x0104 */ s32 unk104;
    /* 0x0108 */ s32 unk108;
    /* 0x010C */ s32 unk10C;
    /* 0x0110 */ SndLock lock;
    /* 0x0130 */ u8 pad130[8];
    /* 0x0138 */ u8 q[0x1D64 - 0x138];
    /* 0x1D64 */ u8 seq[8];
    /* 0x1D6C */ s32 seqData;
    /* 0x1D70 */ u8 pad1D70[0x38];
    /* 0x1DA8 */ u8 heap[0x10];
    /* 0x1DB8 */ u8 pool[0x2B88 - 0x1DB8];
    /* 0x2B88 */ s32 unk2B88;
    /* 0x2B8C */ s16 unk2B8C;
    /* 0x2B8E */ u8 pad2B8E[6];
    /* 0x2B94 */ u8 unk2B94;
    /* 0x2B95 */ u8 pad2B95[3];
    /* 0x2B98 */ s32 unk2B98;
    /* 0x2B9C */ s32 unk2B9C;
    /* 0x2BA0 */ f32 unk2BA0;
    /* 0x2BA4 */ f32 unk2BA4;
    /* 0x2BA8 */ f32 unk2BA8;
    /* 0x2BAC */ s32 unk2BAC;
    /* 0x2BB0 */ s32 unk2BB0;
    /* 0x2BB4 */ s32 unk2BB4;
    /* 0x2BB8 */ s32 unk2BB8;
    /* 0x2BBC */ f32 unk2BBC;
    /* 0x2BC0 */ u8 fx[1];
} SndMgr;

extern s32 D_800B2680;      /* sound manager ready */
extern u8 D_800B2684[];
extern s32 D_800B6E90;
extern s32 D_800F2040;
extern u8 D_8011AAED[];     /* volume settings (bytes from -2) */
extern s32 D_80130A40;      /* audio heap base */
extern s32 D_80130A7C;      /* audio heap size */

extern s32 func_0029DFF0(void);         /* disable interrupts */
extern void func_0029E010(s32 mask);    /* restore interrupts */
extern s32 osRecvMesg(void *mq, void *msg, s32 flag);
extern s32 osSendMesg(void *mq, void *msg, s32 flag);
extern void CMutex__Construct(SndLock *lock);
extern void func_002912F0(void *heap, s32 base, s32 len);    /* alHeapInit */
extern void func_0026F760(SynCfg *cfg, s32 a, AiCfg *ai);
extern BankRes *func_0028E60C(s32 a, s32 b, s32 c);
extern void func_00270550(SndMgr *m);
extern void func_00274F34(void *seq, SndMgr *m);
extern void func_002935B0(void *player, void *cfg);           /* alSndpNew */
extern void func_002763F4(void *q, SndMgr *m);
extern void func_00273A94(void *pool, SndMgr *m);
extern void func_00276D98(void *fx, SndMgr *m);

static __inline__ void SndMgr_Lock(SndMgr *m) {
    SndLock *l = &m->lock;
    s32 mask = func_0029DFF0();

    if (++l->count != 1) {
        func_0029E010(mask);
        osRecvMesg(l, NULL, 1);
    } else {
        func_0029E010(mask);
    }
}

static __inline__ void SndMgr_Unlock(SndMgr *m) {
    SndLock *l = &m->lock;
    s32 mask = func_0029DFF0();

    if (--l->count != 0) {
        func_0029E010(mask);
        osSendMesg(l, NULL, 1);
    } else {
        func_0029E010(mask);
    }
}

/* Sound manager start-up: heap, synthesizer, banks, player and state. */
void func_00270240(SndMgr *m) {
    SynCfg cfg;
    AiCfg ai;
    BankRes *r;
    Bank *bk;
    SortTab *t;
    s32 i;
    s32 j;
    s32 n;
    u32 a;
    u32 b;
    u8 *e;

    CMutex__Construct(&m->lock);
    SndMgr_Lock(m);
    m->unkD8 = 0;
    func_002912F0(m->heap, D_80130A40, D_80130A7C);
    cfg.maxVVoices = 0x25;
    cfg.maxPVoices = 0x25;
    cfg.maxUpdates = 0x100;
    cfg.v7 = 6;
    cfg.tab = D_800B2684;
    ai.rate = 0x5622;
    ai.one = 1;
    cfg.fxType = 0;
    cfg.heap = m->heap;
    ai.size = 0x800;
    func_0026F760(&cfg, D_800B6E90, &ai);
    D_800F2040 = 0x20F194;
    r = func_0028E60C(0x2F194, 0, 0x1E0000);
    m->seqData = r->f4;
    bk = r->fC;
    m->bankRes = (BankRes *)bk;
    m->bank = (Bank *)bk->f4;
    func_00270550(m);
    func_00274F34(m->seq, m);
    t = m->bank->tab;
    for (i = 0; i < t->n; i++) {
        for (j = i + 1; j < t->n; j++) {
            a = t->v[j];
            b = t->v[i];
            if (a < b) {
                do {
                    t->v[i] = a;
                } while (0);
                t->v[j] = b;
            }
        }
    }
    m->playerCfg[0] = 0x11;
    m->playerCfg[1] = 0xC0;
    m->playerCfg[2] = (s32)m->heap;
    func_002935B0(m->player, m->playerCfg);
    for (n = 0; n < 16; n++) {
        m->snd[n] = -1;
    }
    m->unk102 = -1;
    m->unk2B8C = -1;
    m->unk108 = 0;
    m->unk104 = 0;
    m->unk2B88 = 0;
    m->unk2B94 = 0x40;
    m->unk2B98 = 0;
    m->unk2BB8 = 0;
    m->unk2B9C = 0;
    func_002763F4(m->q, m);
    func_00273A94(m->pool, m);
    e = D_8011AAED;
    m->unk2BA0 = (f32)e[0] * (1.0f / 255);
    m->unk2BA4 = (f32)e[-1] * (1.0f / 255);
    m->unk2BA8 = (f32)e[1] * (1.0f / 255);
    m->unk2BAC = 0;
    m->unk2BB0 = e[-2];
    m->unk2BB4 = 1;
    m->unk2BBC = 1.0f;
    func_00276D98(m->fx, m);
    D_800B2680 = 1;
    SndMgr_Unlock(m);
}
