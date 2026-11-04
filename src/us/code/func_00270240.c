/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00257380.c, func_00257380, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"

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
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 f4;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ ALBankFile *bankFile;
} BankRes;

extern s32 D_800B2680;      /* sound manager ready */
extern u8 D_800B2684[];
extern s32 D_800B6E90;
extern s32 D_800F2040;
extern u8 D_8011AAED[];     /* volume settings (bytes from -2) */
extern s32 D_80130A40;      /* audio heap base */
extern s32 D_80130A7C;      /* audio heap size */

extern s32 __osDisableInt(void);         /* disable interrupts */
extern void __osRestoreInt(s32 mask);    /* restore interrupts */
extern s32 osRecvMesg(void *mq, void *msg, s32 flag);
extern s32 osSendMesg(void *mq, void *msg, s32 flag);
extern void CMutex__Construct(CMutex *lock);
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
    CMutex *l = &m->lock;
    s32 mask = __osDisableInt();

    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(l, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void SndMgr_Unlock(SndMgr *m) {
    CMutex *l = &m->lock;
    s32 mask = __osDisableInt();

    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(l, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

/* Sound manager start-up: heap, synthesizer, banks, player and state. */
void func_00270240(SndMgr *m) {
    SynCfg cfg;
    AiCfg ai;
    BankRes *r;
    ALBankFile *bk;
    ALInstrument *t;
    s32 i;
    s32 j;
    s32 n;
    u32 a;
    u32 b;
    u8 *e;

    CMutex__Construct(&m->lock);
    SndMgr_Lock(m);
    m->soundState = 0;
    func_002912F0(&m->heap, D_80130A40, D_80130A7C);
    cfg.maxVVoices = 0x25;
    cfg.maxPVoices = 0x25;
    cfg.maxUpdates = 0x100;
    cfg.v7 = 6;
    cfg.tab = D_800B2684;
    ai.rate = 0x5622;
    ai.one = 1;
    cfg.fxType = 0;
    cfg.heap = &m->heap;
    ai.size = 0x800;
    func_0026F760(&cfg, D_800B6E90, &ai);
    D_800F2040 = 0x20F194;
    r = func_0028E60C(0x2F194, 0, 0x1E0000);
    m->seq.data = (s32 *)r->f4;
    bk = r->bankFile;
    m->bankFile = bk;
    m->bank = bk->bankArray[0];
    func_00270550(m);
    func_00274F34(&m->seq, m);
    t = m->bank->instArray[0];
    for (i = 0; i < t->soundCount; i++) {
        for (j = i + 1; j < t->soundCount; j++) {
            a = t->soundArray[j];
            b = t->soundArray[i];
            if (a < b) {
                do {
                    t->soundArray[i] = a;
                } while (0);
                t->soundArray[j] = b;
            }
        }
    }
    m->playerConfig[0] = 0x11;
    m->playerConfig[1] = 0xC0;
    m->playerConfig[2] = (s32)&m->heap;
    func_002935B0(m->player, m->playerConfig);
    for (n = 0; n < 16; n++) {
        m->handles[n] = -1;
    }
    m->pendingCue = -1;
    m->cueId = -1;
    m->soundCounter = 0;
    m->frame = 0;
    m->cue = 0;
    m->cueVolume = 0x40;
    m->listener = 0;
    m->muted = 0;
    m->fxMix = 0;
    func_002763F4(&m->queue, m);
    func_00273A94(&m->pool, m);
    e = D_8011AAED;
    m->sfxVolume = (f32)e[0] * (1.0f / 255);
    m->volume2BA4 = (f32)e[-1] * (1.0f / 255);
    m->musicVolume = (f32)e[1] * (1.0f / 255);
    m->unk2BAC = 0;
    m->unk2BB0 = e[-2];
    m->unk2BB4 = 1;
    m->unk2BBC = 1.0f;
    func_00276D98(&m->fx, m);
    D_800B2680 = 1;
    SndMgr_Unlock(m);
}
