/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00257380.c, func_00257380, with Turok 2's layouts. */
#include "common.h"
#include "structs/sound.h"
#include "globals/frame.h"
#include "globals/audio.h"
#include "globals/engine.h"
#include "protos/ultra.h"
#include "protos/osint.h"
#include "protos/audio_world.h"

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
extern u8 gEngineApp_m_Settings_unk15[]; /* local type: the field's real type (or the member path) changes the ROM's code here */     /* volume settings (bytes from -2) */
extern void CMutex__Construct(CMutex *lock);
extern void alHeapInit(void *heap, sptr base, s32 len);    /* alHeapInit */
extern void amCreateAudioMgr(ALSynConfig *cfg, s32 a, AiCfg *ai);
void CAudioWorld__LoadSfxDataFile(CAudioWorld *m);
extern void SndSeq_Construct(void *seq, CAudioWorld *m);
extern void alSndpNew(void *player, void *cfg);           /* alSndpNew */
extern void SndQueue_Construct(void *q, CAudioWorld *m);
extern void SndVoicePool_Construct(void *pool, CAudioWorld *m);
extern void SpeechPlayer_Construct(void *fx, CAudioWorld *m);
#ifdef TARGET_PORT
/* port/audio/t2audio.h's T2AudioCtl (its first two fields): the ctl region's bank files
 * converted to host structs (docs/port/AUDIO.md) instead of the VM_Alloc copy of the ROM */
typedef struct {
    ALBankFile *musicBank;
    ALBankFile *sfxBank;
} T2AudioCtlView;
extern const T2AudioCtlView *T2Audio_LoadCtl(void);
#endif

static __inline__ void CAudioWorld_Lock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    s32 mask = __osDisableInt();

    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void CAudioWorld_Unlock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    s32 mask = __osDisableInt();

    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

/* Sound manager start-up: heap, synthesizer, banks, player and state. */
void CAudioWorld__Initialize(CAudioWorld *m) {
    ALSynConfig cfg;
    AiCfg ai;
    BankRes *r;
    ALBankFile *bk;
    ALInstrument *t;
    s32 i;
    s32 j;
    s32 n;
    uptr a; /* soundArray holds pointers: the host compares them whole */
    uptr b;
    u8 *e;

    CMutex__Construct(&m->lock);
    CAudioWorld_Lock(m);
    m->soundState = 0;
    alHeapInit(&m->heap, gAudioHeapBase, gAudioHeapSize);
    cfg.maxVVoices = 0x25;
    cfg.maxPVoices = 0x25;
    cfg.maxUpdates = 0x100;
    cfg.fxType = 6; /* AL_FX_CUSTOM */
    cfg.params = sAudioWorldReverbParams;
    ai.rate = 0x5622;
    ai.one = 1;
    cfg.dmaproc = 0;
    cfg.heap = &m->heap;
    ai.size = 0x800;
    amCreateAudioMgr(&cfg, gThreadPriorityAudio, &ai);
    gAudioRomBase = 0x20F194;
#ifdef TARGET_PORT
    {
        const T2AudioCtlView *c = T2Audio_LoadCtl();
        m->seq.data = (s32 *)c->musicBank;
        bk = c->sfxBank;
    }
#else
    r = VM_Alloc(0x2F194, 0, 0x1E0000);
    m->seq.data = (s32 *)r->f4;
    bk = r->bankFile;
#endif
    m->bankFile = bk;
    m->bank = bk->bankArray[0];
    CAudioWorld__LoadSfxDataFile(m);
    SndSeq_Construct(&m->seq, m);
    t = m->bank->instArray[0];
    for (i = 0; i < t->soundCount; i++) {
        for (j = i + 1; j < t->soundCount; j++) {
            a = t->soundArray[j];
            b = t->soundArray[i];
            if (a < b) {
                t->soundArray[i] = a;
                t->soundArray[j] = b;
            }
        }
    }
    m->playerConfig.maxSounds = 0x11;
    m->playerConfig.maxEvents = 0xC0;
    m->playerConfig.heap = &m->heap;
    alSndpNew(&m->player, &m->playerConfig);
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
    SndQueue_Construct(&m->queue, m);
    SndVoicePool_Construct(&m->pool, m);
    e = gEngineApp_m_Settings_unk15;
    m->sfxVolume = (f32)e[0] * (1.0f / 255);
    m->volume2BA4 = (f32)e[-1] * (1.0f / 255);
    m->musicVolume = (f32)e[1] * (1.0f / 255);
    m->unk2BAC = 0;
    m->unk2BB0 = e[-2];
    m->unk2BB4 = 1;
    m->unk2BBC = 1.0f;
    SpeechPlayer_Construct(&m->fx, m);
    gAudioWorldReady = 1;
    CAudioWorld_Unlock(m);
}

#include "protos/iset.h"

/* Sound manager: find the sound tables in the sound data file. */
void CAudioWorld__LoadSfxDataFile(CAudioWorld *m) {
    u32 *s3;
    u32 *t0;
    u32 *t1;
    u32 *t2;
    u32 *n3;
    u32 *n4;
    u32 *n5;
    u32 *r;
    u32 *z;

    m->soundData = VM_Alloc(0x3EA7A0, 0, 0xBE1814);
    s3 = CISet__GetBlock(m->soundData, 0);
    CISet__GetBlock(m->soundData, 1);
    t0 = CISet__GetBlock(s3, 0);
    t1 = CISet__GetBlock(s3, 1);
    t2 = CISet__GetBlock(s3, 2);
    m->variants = t2;
    m->rangeCount = t2[0];
    m->ranges = (u8 *)t0 + 8;
    m->weights = (u8 *)t1 + 8;
    n3 = CISet__GetBlock(s3, 3);
    n4 = CISet__GetBlock(s3, 4);
    n5 = CISet__GetBlock(s3, 5);
    m->count2B70 = n3[1];
    m->table2B70 = (u8 *)n3 + 8;
    m->count2B74 = n4[1];
    m->table2B74 = (u8 *)n4 + 8;
    m->count2B78 = n5[1];
    m->table2B78 = (u8 *)n5 + 8;
    r = CISet__GetBlock(m->soundData, 1);
    z = CISet__GetBlock(r, 0);
    m->table2B60 = (u8 *)z + 8;
    m->count2B64 = z[1];
    m->node2B50 = CISet__GetBlock(r, 1);
}

/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: CAudioWorld__StartVoice: 114 of 137 words. */
#include "structs/vector.h"
#include "ultra_types.h"

void alSndpSetPitch(void *p, s32 pitch);
void alSndpSetFXMix(void *p, s32 mix);
void alSndpSetPan(void *p, u8 pan);
void alSndpSetVol(void *p, s16 vol);
void SndQueue_RequeueVoice(void *q, s32 a, s32 id);
s32 SndVoicePool_GetVoiceState(void *pool, s16 idx, s32 id);
void SndVoicePool_PlayVoice(void *pool, s16 idx);

static __inline__ s32 find17(CAudioWorld *m, s32 id) {
    s32 i;
    SndVoice *vv;
    for (vv = m->pool.voices, i = 0; i < 17; i++, vv++) {
        if (vv->id == id) {
            return i;
        }
    }
    return -1;
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StartVoice) */
s32 CAudioWorld__StartVoice(CAudioWorld *m, s32 id)
{
  s16 idx;
  SndVoiceParams *p;
  SndVoice *vs;
  u8 *pl;
  s32 mix;
  f32 f;
  u8 *new_var;
  if (id == (-1))
  {
    return -1;
  }
  idx = find17(m, id);
  if (((s16) idx) == (-1))
  {
    return -1;
  }
  if (((s16) idx) == 16)
  {
    SndQueue_RequeueVoice(&m->queue, 16, id);
    return -1;
  }
  p = (SndVoiceParams *) ((u8 *)((((s16) idx) * 0xCC) + (uptr) m) + 0x1DCC);
  pl = &m->player;
  if (p->vol == 0)
  {
    if (!((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60))) & 4))
    {
      *((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60)) |= 8;
      m->pendingCue = -1;
      return 0;
    }
  }
  if (SndVoicePool_GetVoiceState(&m->pool, idx, id) == 0)
  {
    alSndpSetPitch(pl, p->pitch.i);
    mix = ((p->fxMix + m->fxMix) < 0x80) ? ((((u8 *) p)[0x11] + (*(((u8 *) m) + 0x2B9F))) & 0xFF) : (0x7F);
    alSndpSetFXMix(pl, mix);
    alSndpSetPan(pl, ((u8 *) (&p->pan))[1]);
    if ((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60))) & 0x200)
    {
      f = m->musicVolume;
    }
    else
    {
      f = m->sfxVolume;
      if (((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E80))) != 0) && (m->muted != 0))
      {
        if (m->sfxVolume)
        {
          f *= 0.5f;
        }
        else
        {
          f *= 0.5f;
        }
      }
    }
    alSndpSetVol(pl, (s16) ((s32) ((((f32) p->vol) * f) * p->scale)));
    if ((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E5C))) == 0)
    {
      SndVoicePool_PlayVoice(&m->pool, idx);
    }
  }
  m->pendingCue = -1;
  return 0;
}
#endif

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_15.c, func_80257A14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void SndQueue_CancelHandle(struct ChannelSet *arg0, s32 arg1);
extern void SndVoicePool_StopVoice(void *, s16);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopHandle) */
s32 CAudioWorld__StopHandle(void *arg0, s32 id) {
    void *temp_s0;
    void *var_a0;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_v0;
    s32 found;
    s32 i;
    Voice *voice;
    found = 0;
    if (id == -1) {
        return found;
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = ((MenuRules *)(temp_s0))->locked + 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    } else {
        __osRestoreInt(temp_a0);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    }
    SndQueue_CancelHandle(var_a0, id);
    for (i = 0; i < 17; i++) {
        if (((ContextRecAt2BC0 *)arg0)->voices[i].sound == id) {
            found++;
            voice = &((ContextRecAt2BC0 *)arg0)->voices[i];
            if (i == 16) {
                voice->sound = -1;
                voice->priority = 0;
                voice->channel = -1;
                voice->unkC = -1;
                voice->unk8 = -1;
            } else {
                if (voice->pending > 0) {
                    voice->pending = 0;
                    voice->flags &= ~0x10;
                }
                if ((&((ContextCode8025A3EC *)arg0)->slots)[0].ids[i] != -1) {
                    SndVoicePool_StopVoice(&((ContextRecAt2BC0 *)(arg0))->menu[0], i);
                }
            }
        }
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_v0 = __osDisableInt();
    temp_v1 = ((MenuRules *)(temp_s0))->locked - 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(temp_s0, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    return found;
}
#endif

#include "globals/camera.h"
#include "structs/camera.h"

/* Static inlines from the Rage Wars header b13_types.h (dropped by the importer). */
static inline void CAudioWorld_Lock_270A60(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static inline void CAudioWorld_Unlock_270A60(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

s32 SndVoicePool_IsSoundPlaying(void *pool, s16 a);
f32 Sound_DistanceFalloff(sptr id, sptr p);
s32 SndVoicePool_FindSoundWithOwner(void *pool, s16 a, s32 b);
s32 SndQueue_Add(void *q, SoundEffectInfo *info, sptr id, s32 a3, s32 a5);
void SndVoicePool_StopGroupVoice(void *pool, s16 a, s16 b);
SndVoice *SndVoicePool_AllocVoice(void *pool, SoundEffectInfo *info, s32 a3, s32 a4);
void SndVoice_SetPosition(SndVoice *v, sptr id, sptr a6);
void SndVoice_SetSoundId(SndVoice *v, s16 a);
void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 a5);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PlayLayer) */
s32 CAudioWorld__PlayLayer(CAudioWorld *m, SoundEffectInfo *info, sptr id, s32 a3, s32 a4, s32 a5, sptr a6) {
    SndVoice *v;
    m->listener = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, (CVector3 *)id);
    if (info->flags & 1) {
        if (SndVoicePool_IsSoundPlaying(&m->pool, m->cueId) != 0) {
            return -1;
        }
        if (Sound_DistanceFalloff(id, (sptr)&m->listener->m_vPos) == 0.0f) {
            return -1;
        }
    }
    if (info->flags & 0x1000) {
        if (m->cueArg != -1) {
            if (SndVoicePool_FindSoundWithOwner(&m->pool, m->cueId, m->cueArg) != -1) {
                return -1;
            }
        }
    }
    if (info->sample != 0) {
        return SndQueue_Add(&m->queue, info, id, a3, a5);
    }
    if (info->flags & 0x40) {
        SndVoicePool_StopGroupVoice(&m->pool, m->cueId, info->group);
    }
    v = SndVoicePool_AllocVoice(&m->pool, info, a3, a4);
    if (v == 0) {
        return -1;
    }
    SndVoice_SetPosition(v, id, a6);
    SndVoice_SetSoundId(v, m->cueId);
    SndVoice_Setup(v, info, a5);
    CAudioWorld_Lock_270A60(m);
    ((s32 (*)(CAudioWorld *, s32, s32, sptr))CAudioWorld__StartVoice)(m, v->id, 0, id);   /* the ROM passes two more arguments */
    CAudioWorld_Unlock_270A60(m);
    return 0;
}
#endif

#include "structs/table.h"

/* Plays a sound or a music cue at pos: refuses while another cue is pending,
 * when 6 of the 17 voices already play this sound id (or for cue 0x44C while
 * it is muted). Picks one of the id's variants at random by weight (a sound
 * id avoids repeating its last variant), then starts each of its layers under
 * a new play handle, which it returns (-1 when nothing plays). */

extern f32 Random_Float(f32 range);

static __inline__ void CAudioWorld_Lock_270C80(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();

    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void CAudioWorld_Unlock_270C80(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();

    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PlaySound) */
s32 CAudioWorld__PlaySound(CAudioWorld *m, s32 id, CVector3 pos, sptr a5, s32 a6) {
    s32 i;
    s32 j;
    s32 busy;
    s32 count;
    s32 sum;
    s32 r;
    s32 thresh;
    s32 lo;
    s32 hi;
    s32 lo2;
    s32 hi2;
    s32 idx;
    s32 sh;
    s32 ret;
    s32 n;
    u32 layers;
    s16 last;
    StrideTable *nd;

    if (m->muted != 0 && id == 0x44C) {
        busy = 1;
        goto chk;
    }
    if (m->pendingCue != -1) {
        busy = 1;
        goto chk;
    }
    count = 0;
    if (id < 0x100) {
        for (j = 0; j < 17; j++) {
            if (m->pool.voices[j].soundId == id) {
                count++;
            }
        }
        busy = 1;
        if (count >= 6) {
            goto chk;
        }
    }
    busy = 0;
chk:
    if (busy != 0) {
        return -1;
    }

    if (id < 0x100) {
        last = (u16) m->lastVariant; /* the N64's 0x132: the low half of the big-endian word (round 12 a5) */
        sum = 0;
        if (m->rangeCount != 0) {
            if (BinaryRange(m->ranges, m->rangeCount, (s16)id, &lo, &hi) != 0) {
                i = lo;
                if (i != hi) {
                    for (count = lo; count <= hi; count++) {
                        sum += m->weights[count] * 10;
                    }
                    r = Random_Next() % sum;
                    thresh = r;
                    sum = 0;
                    for (i = lo; i < hi; i++) {
                        sum += m->weights[i] * 10;
                        if (sum >= thresh) {
                            break;
                        }
                    }
                    if (last != 0 && i == m->lastVariant) {
                        if (i == hi) {
                            i = i - 1;
                        } else {
                            i = i + 1;
                        }
                    }
                }
                idx = i;
            } else {
                idx = -1;
            }
        } else {
            idx = -1;
        }
        m->cueVolume = Random_Float(127.0f);
    } else {
        sum = 0;
        if (m->rangeCount != 0) {
            if (BinaryRange(m->ranges, m->rangeCount, (s16)id, &lo2, &hi2) != 0) {
                i = lo2;
                if (i != hi2) {
                    for (r = lo2; r <= hi2; r++) {
                        sum += m->weights[r] * 10;
                    }
                    r = Random_Next() % sum;
                    thresh = r;
                    sum = 0;
                    for (i = lo2; i < hi2; i++) {
                        sum += m->weights[i] * 10;
                        if (sum >= thresh) {
                            break;
                        }
                    }
                }
                idx = i;
            } else {
                idx = -1;
            }
        } else {
            idx = -1;
        }
    }
    if (idx == -1) {
        return -1;
    }
    sh = (s16)idx;
    m->cue = CISet__GetBlock(CISet__GetBlock(m->variants, sh), 0);
    m->cueId = id;
    m->cueArg = a6;
    nd = CISet__GetBlock(CISet__GetBlock(m->variants, sh), 1);
    layers = nd->count;
    if (layers != 0) {
        CAudioWorld_Lock_270C80(m);
        ret = (m->handle = m->handle + 1);
        CAudioWorld_Unlock_270C80(m);
        if (id < 0x100) {
            m->lastVariant = idx;
        }
    } else {
        ret = -1;
    }
    n = 0;
    if (layers != 0) {
        do {
            CAudioWorld__PlayLayer(m, nd->entries + n * nd->stride, &pos, idx, ret, 0, a5);
            n++;
        } while ((u32)n < layers);
    }
    m->cueArg = -1;
    return ret;
}
#endif

static __inline__ void CAudioWorld_Lock_271128(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void CAudioWorld_Unlock_271128(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg(&l->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PlaySoundIndex) */
s32 CAudioWorld__PlaySoundIndex(CAudioWorld *m, s32 id, CVector3 pos, s32 flags) {
    u32 *rec;
    u32 count;
    u32 i;
    s32 token;
    void *n;
    if (!(id < m->rangeCount)) {
        return -1;
    }
    n = CISet__GetBlock(m->variants, (s16)id);
    n = CISet__GetBlock(n, 0);
    m->cue = n;
    m->cueId = 0x100;
    m->cueArg = -1;
    n = CISet__GetBlock(m->variants, (s16)id);
    rec = CISet__GetBlock(n, 1);
    count = rec[1];
    if (count != 0) {
        CAudioWorld_Lock_271128(m);
        token = ++m->handle;
        CAudioWorld_Unlock_271128(m);
    } else {
        token = -1;
    }
    i = 0;
    if (count != 0) {
        do {
            CAudioWorld__PlayLayer(m, (u8 *)rec + (i * rec[0] + 8), &pos, id, token, 0, flags);
            i++;
        } while (i < count);
    }
    return token;
}
#endif

#include "globals/game.h"

/* Once a frame: the volume options (bytes 0x13-0x16 of the settings), the voices under the lock,
 * the queue, speech and music, and the ambient track (mode 13) from the camera nearest the origin. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__Update) */
s32 CAudioWorld__Update(CAudioWorld *m) {
    u8 *bytes = &gEngineApp_m_Settings.unk15;
    CMutex *lock;
    u32 mask;
    s32 count;
    m->sfxVolume = (f32)bytes[0] * 0.003921569f;
    m->volume2BA4 = (f32)bytes[-1] * 0.003921569f;
    m->musicVolume = (f32)bytes[1] * 0.003921569f;
    lock = &m->lock;
    m->unk2BB0 = bytes[-2];
    mask = __osDisableInt();
    count = lock->count + 1;
    lock->count = count;
    if (count != 1) {
        __osRestoreInt(mask);
        osRecvMesg(&lock->queue, 0, 1);
    } else {
        __osRestoreInt(mask);
    }
    SndVoicePool_Update(&m->pool);
    {
        CMutex *lock2 = &m->lock;
        u32 mask2 = __osDisableInt();
        s32 count2 = lock2->count - 1;
        lock2->count = count2;
        if (count2 != 0) {
            __osRestoreInt(mask2);
            osSendMesg(&lock2->queue, 0, 1);
        } else {
            __osRestoreInt(mask2);
        }
    }
    SndQueue_Update(&m->queue);
    SpeechPlayer_Update(&m->fx);
    SndSeq_UpdateFade(&m->seq);
    SndSeq_Update(&m->seq);
    if (gAudioWorldReady != 0 &&
        m->unk2BB4 != 0 &&
        gEngineApp_m_Settings.mode == 13 &&
        m->track > 0) {
        CVector3 zero;
        struct CCamera *camera;
        CVector3 *vec;
        zero.z = 0.0f;
        zero.y = 0.0f;
        zero.x = 0.0f;
        camera = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        vec = &camera->m_vPos;
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, *vec, 0, -1);
        }
    }
    return ++m->frame;
}
#endif

void CAudioWorld__Lock(CAudioWorld *arg0) {
    s32 temp_a0;
    s32 temp_v1;
    CMutex *temp_s0;

    temp_s0 = &arg0->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&temp_s0->queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_a0);
}

/* Releases the sound manager lock. */
void CAudioWorld__Unlock(CAudioWorld *arg0) {
    s32 temp_v0;
    s32 temp_v1;
    CMutex *temp_s0;

    temp_s0 = &arg0->lock;
    temp_v0 = __osDisableInt();
    temp_v1 = temp_s0->count - 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&temp_s0->queue, 0, 1);
        return;
    }
    __osRestoreInt(temp_v0);
}

void SndVoicePool_StopSound(SndVoicePool *pool);                         /* extern */
void SndQueue_CancelSoundId(SndQueue *queue, s32);                /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopSound) */
void CAudioWorld__StopSound(CAudioWorld *mgr, s32 arg1) {
    SndVoicePool_StopSound(&mgr->pool);
    SndQueue_CancelSoundId(&mgr->queue, arg1);
}
#endif

s32 SndVoicePool_StopByOwner(s32);                         /* extern */
s32 SndQueue_CancelOwner(sptr, s32);                /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopSoundsByOwner) */
void CAudioWorld__StopSoundsByOwner(sptr arg0, s32 arg1) {
    SndVoicePool_StopByOwner(arg0 + 0x1DB8);
    SndQueue_CancelOwner(arg0 + 0x138, arg1);
}
#endif

void SndVoicePool_StopByFlags(SndVoicePool *pool);                         /* extern */
void SndQueue_CancelFlags(SndQueue *queue, s32);                /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopSoundsByFlags) */
void CAudioWorld__StopSoundsByFlags(CAudioWorld *mgr, s32 arg1) {
    SndVoicePool_StopByFlags(&mgr->pool);
    SndQueue_CancelFlags(&mgr->queue, arg1);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PlaySoundPitched) */
void CAudioWorld__PlaySoundPitched(ContextRecAt2BC0 *arg0, s32 arg1, CVector3 arg2, s32 arg3, s32 arg4, f32 arg5) {
    arg0->alpha = arg5;
    CAudioWorld__PlaySound(arg0, arg1, arg2, arg3, arg4);
    arg0->alpha = 1.0f;
}
#endif

/* The game mode (gEngineApp_m_Settings_mode, 13: the ambient track plays) reached from the camera
 * pool's symbol, 0x13D8 past it: one %hi for both, as in the ROM (a call in Rage Wars). */
#define CAMERA_POOL_MODE (*(s32 *)((u8 *)&gEngineApp_m_Game_m_CameraPool + 0x13D8))

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__UpdateAmbientTrack) */
void CAudioWorld__UpdateAmbientTrack(CAudioWorld *m) {
    CVector3 zero;
    struct CCamera *camera;
    CVector3 *vec;
    if (gAudioWorldReady != 0 &&
        m->unk2BB4 != 0 &&
        CAMERA_POOL_MODE == 0xD &&
        m->track > 0) {
        {
            register f32 value = 0.0f;
            zero.z = value;
            zero.y = value;
            zero.x = value;
        }
        camera = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        vec = &camera->m_vPos;
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, *vec, 0, -1);
        }
    }
}
#endif

void CAudioWorld__SetSfxVolume(CAudioWorld *arg0, f32 arg1) {
    arg0->sfxVolume = arg1;
}

void CAudioWorld__SetSequenceVolume(CAudioWorld *arg0, u32 arg1) {
    arg0->volume2BA4 = arg1;
}

void CAudioWorld__SetMusicVolume(CAudioWorld *arg0, f32 arg1) {
    arg0->musicVolume = arg1;
}

SndEnv *CAudioWorld__GetVolumeEnvelope(CAudioWorld *arg0, s32 arg1) {   /* the arg1-th 0xC-byte SndEnv of its table */
    return (SndEnv *)(arg0->table2B70 + (arg1 * 0xC));
}

SndEnv *CAudioWorld__GetPitchEnvelope(CAudioWorld *arg0, s32 arg1) {   /* the arg1-th 0xC-byte SndEnv of its table */
    return (SndEnv *)(arg0->table2B74 + (arg1 * 0xC));
}

/* The arg1-th 0xC-byte entry of table2B78. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__GetPanEnvelope) */
SndEnv *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s32 arg1) {   /* the arg1-th 0xC-byte SndEnv of its table */
    return (SndEnv *)(m->table2B78 + (arg1 * 0xC));
}
#endif

s32 SndVoicePool_StopAllExcept(void *, s32);                /* extern */
s32 SndQueue_CancelAllExcept(void *, s32);                /* extern */

void CAudioWorld__StopAllSounds(CAudioWorld *arg0, s32 arg1) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    void *var_a0;
    CMutex *temp_s0;
    CMutex *temp_s0_2;

#ifdef TARGET_PORT
    if (arg0->bank == NULL) {
        return; /* no sound bank: audio off on the host (Sound__Initialize, t2boot.c) */
    }
#endif
    temp_s0 = &arg0->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = temp_s0->count + 1;
    temp_s0->count = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(&temp_s0->queue, 0, 1);
        var_a0 = &arg0->pool;
    } else {
        __osRestoreInt(temp_a0);
        var_a0 = &arg0->pool;
    }
    SndVoicePool_StopAllExcept(var_a0, arg1);
    SndQueue_CancelAllExcept(&arg0->queue, arg1);
    temp_s0_2 = &arg0->lock;
    arg0->fxMix = 0;
    temp_v0 = __osDisableInt();
    temp_v1_2 = temp_s0_2->count - 1;
    temp_s0_2->count = temp_v1_2;
    if (temp_v1_2 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(&temp_s0_2->queue, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    SpeechPlayer_Stop(&arg0->fx);
}

void SndSeq_FadeOut(SndSeq *);                   /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__FadeOutMusic) */
void CAudioWorld__FadeOutMusic(CAudioWorld *arg0) {
    SndSeq_FadeOut(&arg0->seq);
}
#endif

void CAudioWorld__SetForce2D(CAudioWorld *arg0, s32 arg1) {
    arg0->unk2BAC = arg1;
}

s32 CAudioWorld__GetForce2D(CAudioWorld *arg0) {
    return arg0->unk2BAC;
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__SetAmbientTrackEnabled) */
void CAudioWorld__SetAmbientTrackEnabled(CAudioWorld *m, s32 arg1) {
    m->unk2BB4 = arg1;
}
#endif

void CAudioWorld__SetListener(CAudioWorld *arg0, struct CCamera *arg1) {
    arg0->listener = arg1;
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__GetStereo) */
s32 CAudioWorld__GetStereo(CAudioWorld *m) {
    return m->unk2BB0;
}
#endif

void CAudioWorld__SetStereo(CAudioWorld *arg0, s32 arg1) {
    arg0->unk2BB0 = arg1;
}

#include "m2c_macros.h"

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__GetSndPlayer) */
s32 CAudioWorld__GetSndPlayer(sptr arg0) {
    return arg0 + 0x84;
}
#endif

void CAudioWorld__SetMuted(CAudioWorld *arg0, s32 arg1) {
    arg0->muted = arg1;
}

/* How many of the manager's voices play the given sound (CMutex taken
 * around the scan). */
s32 CAudioWorld__CountHandleVoices(CAudioWorld *mgr, s32 sound)
{
  s32 mask;
  s32 mask2;
  s32 count;
  s32 count2;
  s32 i;
  s32 n;
  CMutex *lock;
  CMutex *lock2;
  CAudioWorld *m;
  lock = &mgr->lock;
  n = 0;
  mask = __osDisableInt();
  count = lock->count + 1;
  lock->count = count;
  if (count != 1)
  {
    __osRestoreInt(mask);
    osRecvMesg(&lock->queue, 0, 1);
    i = 0;
  }
  else
  {
    __osRestoreInt(mask);
    i = 0;
  }
 do { m = mgr; } while (0); /* walked by whole voices, the sound field read at a fixed offset: the ROM's loop */
  do
  {
    if (m->pool.voices[0].sound == sound)
    {
      n += 1;
    }
    i += 1;
    m = (CAudioWorld *) ((u8 *) m + sizeof(SndVoice));
  }
  while (i < 0x11);
  lock2 = &mgr->lock;
  mask2 = __osDisableInt();
  count2 = lock2->count - 1;
  lock2->count = count2;
  if (count2 != 0)
  {
    __osRestoreInt(mask2);
    osSendMesg(&lock2->queue, 0, 1);
  }
  else
  {
    __osRestoreInt(mask2);
  }
  return n;
}

void SndSeq_Stop(s32);                         /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopMusic) */
void CAudioWorld__StopMusic(sptr arg0) {
    SndSeq_Stop(arg0 + 0x1D64);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__SetFxMix) */
void CAudioWorld__SetFxMix(ObjRecAt2B9C *obj, float value) {
    float v;
    float max = 1.0f;
    if (value > max || !(value < 0.0f)) {
        v = value;
        if (v > max) {
            v = max;
        }
    } else {
        v = 0.0f;
    }
    obj->level = v * 4.0e1f;
}
#endif

s32 SndSeq_Duck(s32);                         /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__DuckMusic) */
void CAudioWorld__DuckMusic(sptr arg0) {
    SndSeq_Duck(arg0 + 0x1D64);
}
#endif

s32 SndSeq_Unduck(s32);                         /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__UnduckMusic) */
void CAudioWorld__UnduckMusic(sptr arg0) {
    SndSeq_Unduck(arg0 + 0x1D64);
}
#endif

s32 SndVoicePool_ClearFollow(s32);                         /* extern */

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__DetachAllVoices) */
void CAudioWorld__DetachAllVoices(sptr arg0) {
    SndVoicePool_ClearFollow(arg0 + 0x1DB8);
}
#endif

/* Speech at the music volume, at most 1. The ROM passes a fourth argument (0x40) the definition
 * does not take. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PlaySpeech) */
s32 CAudioWorld__PlaySpeech(CAudioWorld *m, s32 group) {
    f32 var_f0 = (1.0f);
    f32 temp_f1 = m->musicVolume;
    if (!(var_f0 < temp_f1)) {
        var_f0 = temp_f1;
    }
    return ((s32 (*)(void *, s32, f32, s32))SpeechPlayer_Play)(&m->fx, group, var_f0, 0x40);
}
#endif

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__StopSpeech) */
void CAudioWorld__StopSpeech(CAudioWorld *m) {
    SpeechPlayer_Stop(&m->fx);
}
#endif
void CAudioWorld__SortInstrumentSounds(SortTable *table);

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__SortInstrumentSounds) */
void CAudioWorld__SortInstrumentSounds(SortTable *table) {
    int i;
    int j;
    u32 right;
    u32 left;
    for (i = 0; i < table->count; i++) {
        for (j = i + 1; j < table->count; j++) {
            right = table->entries[j];
            left = table->entries[i];
            if (right < left) {
                table->entries[i] = right;
                table->entries[j] = left;
            }
        }
    }
}
#endif

/* Index of the first of the seventeen voices playing id, -1 if none (or id is -1). */
s32 CAudioWorld__FindVoice(CAudioWorld *ctx, s32 id) {
    SndVoice *slot;
    s32 i;

    slot = ctx->pool.voices;
    if (id == -1) {
        return -1;
    }
    for (i = 0; i < 17; i++, slot++) {
        if (slot->id == id) {
            return i;
        }
    }
    return -1;
}

/* A weighted random entry of sound id's range (avoid: not the last one picked). */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__PickVariant) */
s32 CAudioWorld__PickVariant(CAudioWorld *m, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total;
    s32 entry;
    s32 draw;
    s32 i;
    total = 0;
    if (m->rangeCount != 0) {
        if (BinaryRange(m->ranges, m->rangeCount, id, &first, &last) == 0) {
            return -1;
        }
        entry = first;
        if (entry != last) {
            for (i = first; i <= last; i++) {
                total += m->weights[i] * 10;
            }
            draw = Random_Next() % total;
            total = 0;
            for (entry = first; entry < last; entry++) {
                total += m->weights[entry] * 10;
                if (total >= draw) {
                        break;
                }
            }
            if (avoid != 0 && entry == m->lastVariant) {
                if (entry == last) {
                        entry--;
                } else {
                        entry++;
                }
            }
        }
        return entry;
    }
    return -1;
}
#endif

void CAudioWorld__GetVariantCue(CAudioWorld *arg0, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(arg0->variants, arg1), 0);
}

#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__GetVariantLayers) */
void CAudioWorld__GetVariantLayers(CAudioWorld *m, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(m->variants, arg1), 1);
}
#endif

/* Busy: the mute flag with sound 0x44C, a pending cue, or 6 voices already playing sound id. */
#ifndef TARGET_PORT /* the port's in port/ (port/cmake: CAudioWorld__IsSoundBusy) */
s32 CAudioWorld__IsSoundBusy(CAudioWorld *m, s32 id) {
    s32 count;
    s32 i;

    if ((m->muted != 0 && id == 0x44C) || m->pendingCue != -1) {
        return 1;
    }
    count = 0;
    if (id < 0x100) {
        for (i = 0; i < 17; i++) {
            if (m->pool.voices[i].soundId == id) {
                count++;
            }
        }
        if (count >= 6) {
            return 1;
        }
    }
    return 0;
}
#endif
