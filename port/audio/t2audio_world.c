/* t2audio_world.c: the game's sound world (CAudioWorld and its voices, request queue, music
 * sequence, speech player and looping sounds) typed for the host (docs/port/AUDIO.md §8).
 *
 * The game's C for these functions (src/us/code/func_0026FEA0 .. func_00277024) is the shared
 * Turok: Rage Wars sound code, imported with Rage Wars' views (rw/abu_types.h ContextRecAt2BC0,
 * Voice, RecordCode8025A3EC, M04Emitter, NodeRecAtC8 ...) or local pads: every field at its N64
 * offset, 0xCC-byte voices, 4-byte pointers, big-endian halves of words. On the host
 * CAudioWorld, SndVoice and SndNode hold 8-byte pointers, so every one of those views reads the
 * wrong bytes (r4's evtq corruption was SpeechPlayer_Construct allocating a sound on
 * "m + 0x84", the N64 offset of the sound player, inside the bank pointers). The functions
 * below are the same code on structs/sound.h's types; the originals are out of the link
 * (port/cmake/link.cmake). Each says which N64 offsets it replaces where that is not obvious.
 *
 * Endianness: the ROM code reads the low byte or half of a word through its big-endian
 * address (((u8 *)&pan)[1], *(u16 *)(m + 0x132)); here those are casts of the value.
 * Floats passed as integers on MIPS (alSndpSetPitch with the f32 bits in a1) are floats here. */
#include "common.h"
#include "structs/sound.h"
#include "structs/camera.h"
#include "structs/table.h"
#include "globals/audio.h"
#include "globals/game.h"
#include "globals/camera.h"
#include "protos/iset.h"
#include "protos/list.h"

#include <stdio.h>
#include <string.h>

/* ---- what this file calls ---- */

/* the sound player (port/audio/t2audio_sndp.c) */
ALSndId alSndpAllocate(ALSndPlayer *sndp, ALSound *sound);
void alSndpDeallocate(ALSndPlayer *sndp, ALSndId id);
s32 alSndpGetState(ALSndPlayer *sndp);
void alSndpSetSound(ALSndPlayer *sndp, ALSndId id);
void alSndpPlay(ALSndPlayer *sndp);
void alSndpStop(ALSndPlayer *sndp);
void alSndpSetPan(ALSndPlayer *sndp, ALPan pan);
void alSndpSetVol(ALSndPlayer *sndp, s16 vol);
void alSndpSetPitch(ALSndPlayer *sndp, f32 pitch);
void alSndpSetFXMix(ALSndPlayer *sndp, u8 mix);
void alSndpSetPriority(ALSndPlayer *sndp, ALSndId id, u8 priority);
/* the compressed sequence player (game C) */
void alCSeqNew(ALCSeq *seq, u8 *data);
void alCSPSetSeq(void *csp, ALCSeq *seq);
void alCSPSetBank(void *csp, ALBank *bank);
void alCSPSetChlFXMix(void *csp, s32 chan, s8 mix);
void alCSPSetVol(void *csp, s16 vol);
void alCSPPlay(void *csp);
void alCSPStop(void *csp);
s32 alCSPGetState(void *csp);

u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);

s32 BinaryRange(u32 *array, s32 count, u32 key, s32 *first, s32 *last);
s32 BinarySearch(u32 *array, u32 count, u32 key);
s32 Random_Next(void);
s32 Random_NextB(void);
f32 Random_Float(f32 range);
f32 Random_Blend(f32 a, f32 b);
f32 HalfToFloat(s32 h);
u16 FloatToHalf(f32 f);
s32 Cinema_PathPlaying(void);
s32 Cinema_IsActive(void);
f32 Math_Approach(f32 value, f32 target, f32 rate);
f32 Math_AcosLookup(f32 x);
f32 func_002982F0(f32 x);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CMtxF__InverseRotateVector(void *m, CVector3 *in, CVector3 *out);
void CVector3__Normalize(CVector3 *v);
CCamera *CCameraPool__GetNearestCamera(void *pool, CVector3 *pos);
void SndVoice_Construct(SndVoice *v, CAudioWorld *m, s32 index);
void CAudioWorld__Lock(CAudioWorld *m);
void CAudioWorld__Unlock(CAudioWorld *m);
s32 CAudioWorld__CountHandleVoices(CAudioWorld *m, s32 handle);
int T2Port_AudioReady(void);
s16 SoundEffectInfo_GetGroup(SoundEffectInfo *i);
s16 SoundEffectInfo_GetSample(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPriority(SoundEffectInfo *i);
s8 SoundEffectInfo_GetChance(SoundEffectInfo *i);
u32 SoundEffectInfo_GetPan(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPitch(SoundEffectInfo *i);
s32 SoundEffectInfo_GetFlags(SoundEffectInfo *i);
s16 SoundEffectInfo_GetRandomRange0(SoundEffectInfo *i);
s16 SoundEffectInfo_GetRandomRange1(SoundEffectInfo *i);
void SndVoicePool_StopAllExcept(SndVoicePool *pool, s32 value);
void SndQueue_CancelAllExcept(SndQueue *q, s32 value);
void SndQueue_CancelHandle(SndQueue *q, s32 value);
void SndQueue_CancelSoundId(SndQueue *q, s32 value);
void SndQueue_CancelOwner(SndQueue *q, s32 value);
void SndQueue_CancelFlags(SndQueue *q, s32 value);
void SndVoicePool_StopSound(SndVoicePool *pool, s32 key);
void SndVoicePool_StopByOwner(SndVoicePool *pool, s32 key);
void SndVoicePool_StopByFlags(SndVoicePool *pool, u32 mask);
void SndVoice_UpdateVolume(SndVoice *v);
void SndVoice_UpdatePitch(SndVoice *v);
void SndSeq_FadeOut(SndSeq *s);
void SndSeq_UpdateFade(SndSeq *s);
f32 Sound_DistanceFalloff(CVector3 *a, CVector3 *b);
s32 Sound__Play(s16 id, CVector3 pos, sptr follow, s32 owner);
void Sound__Stop(s32 handle);
s32 Sound__IsPlaying(s32 handle);

/* the speech player: the game's SndFx block (structs/sound.h) with its fields named (Rage
 * Wars' PlayerRecAt24 / MusicPlayer / Shape_typemap_110 views, N64 offsets in the comments;
 * same host layout as SndFx: one pointer, then words) */
typedef struct {
    /* 0x00 */ CAudioWorld *owner;
    /* 0x04 */ s32 request;    /* last request id handed out */
    /* 0x08 */ s32 state;      /* 0 idle, 1 start, 2 stopping, 3 streaming */
    /* 0x0C */ s32 loop;       /* the group; >= 0 restarts when it ends */
    /* 0x10 */ s32 song;
    /* 0x14 */ s32 speed;      /* volume 0..32767 */
    /* 0x18 */ s32 pan;        /* the low byte is the pan */
    /* 0x1C */ s32 sndId;      /* its sound player id (low half on the N64: 0x1E) */
    /* 0x20 */ f32 tempoScale;
    /* 0x24 */ f32 volume;
} T2SpeechPlayer;
_Static_assert(sizeof(T2SpeechPlayer) <= sizeof(SndFx), "SndFx view");

static __inline__ void world_lock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (++l->count != 1) {
        __osRestoreInt(mask);
        osRecvMesg((OSMesgQueue *)l, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

static __inline__ void world_unlock(CAudioWorld *m) {
    CMutex *l = &m->lock;
    u32 mask = __osDisableInt();
    if (--l->count != 0) {
        __osRestoreInt(mask);
        osSendMesg((OSMesgQueue *)l, NULL, 1);
    } else {
        __osRestoreInt(mask);
    }
}

/* the sound effect bank's sounds: bank->instArray[0] (Rage Wars' M04Bank2.tab, SndQueue_Update's
 * font->glyphs: soundCount at 0xE, soundArray at 0x10 on the N64) */
static __inline__ ALInstrument *sfx_inst(CAudioWorld *m) {
    return m->bank->instArray[0];
}

/* the common "stop this voice" sequence (inlined in a dozen places in the ROM) */
static void voice_stop(SndVoice *v) {
    CAudioWorld *m = v->mgr;
    v->stopping = 1;
    v->vp.follow = NULL;
    if (v->vp.frame != m->frame) {
        alSndpSetSound(&m->player, m->handles[v->index]);
        if (alSndpGetState(&m->player) != 0) {
            alSndpStop(&m->player);
        }
        v->sound = -1;
    }
}

static f32 cents_to_pitch(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    if ((f32)i != pos) {
        f32 a = sPitchRatioTable[i];
        return a + (pos - i) * (sPitchRatioTable[(s16)(i + 1)] - a);
    }
    return sPitchRatioTable[i];
}

/* ======================================================================================
 * CAudioWorld
 * ====================================================================================== */

/* func_002706A0 (raw voice offsets 0x1DCC + i * 0xCC, the low bytes of fxMix and pan) */
s32 CAudioWorld__StartVoice(CAudioWorld *m, s32 id, s32 a2, CVector3 *pos);
void SndQueue_RequeueVoice(SndQueue *q, s32 index, s32 owner, CVector3 *pos);
s32 SndVoicePool_GetVoiceState(SndVoicePool *pool, s16 idx);
void SndVoicePool_PlayVoice(SndVoicePool *pool, s16 idx);

s32 CAudioWorld__StartVoice(CAudioWorld *m, s32 id, s32 a2, CVector3 *pos) {
    s32 i;
    s16 idx = -1;
    SndVoice *v;
    SndVoiceParams *p;
    ALSndPlayer *pl;
    s32 mix;
    f32 f;
    (void)a2;

    if (id == -1) {
        return -1;
    }
    for (i = 0; i < 17; i++) {
        if (m->pool.voices[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        return -1;
    }
    if (idx == 16) {
        /* the ROM passes the caller's a3 (CAudioWorld__PlayLayer's position) through */
        SndQueue_RequeueVoice(&m->queue, 16, id, pos);
        return -1;
    }
    v = &m->pool.voices[idx];
    p = &v->vp;
    pl = &m->player;
    if (p->vol == 0 && !(v->flags & 4)) {
        v->flags |= 8;
        m->pendingCue = -1;
        return 0;
    }
    if (SndVoicePool_GetVoiceState(&m->pool, idx) == 0) {
        alSndpSetPitch(pl, p->pitch.f);
        mix = (p->fxMix + m->fxMix < 0x80) ? (((u8)p->fxMix + (u8)m->fxMix) & 0xFF) : 0x7F;
        alSndpSetFXMix(pl, mix);
        alSndpSetPan(pl, (u8)p->pan);
        if (v->flags & 0x200) {
            f = m->musicVolume;
        } else {
            f = m->sfxVolume;
            if (v->unkC4 != 0 && m->muted != 0) {
                f *= 0.5f;
            }
        }
        alSndpSetVol(pl, (s16)(s32)((f32)p->vol * f * p->scale));
        if (v->delay == 0) {
            SndVoicePool_PlayVoice(&m->pool, idx);
        }
    }
    m->pendingCue = -1;
    return 0;
}

/* func_002708C0 (ContextRecAt2BC0 / Voice views) */
s32 CAudioWorld__StopHandle(CAudioWorld *m, s32 id) {
    s32 found = 0;
    s32 i;
    void SndVoicePool_StopVoice(SndVoicePool *pool, s16 idx);

    if (id == -1) {
        return 0;
    }
    world_lock(m);
    SndQueue_CancelHandle(&m->queue, id);
    for (i = 0; i < 17; i++) {
        SndVoice *v = &m->pool.voices[i];
        if (v->sound != id) {
            continue;
        }
        found++;
        if (i == 16) {
            v->sound = -1;
            v->vp.priority = 0;
            v->vp.channel = -1;
            v->id = -1;
            v->unk8 = -1;
        } else {
            if (v->delay > 0) {
                v->delay = 0;
                v->flags &= ~0x10;
            }
            if (m->handles[i] != -1) {
                SndVoicePool_StopVoice(&m->pool, i);
            }
        }
    }
    world_unlock(m);
    return found;
}

/* func_00270A60 (the position arrives as an s32 there) */
SndVoice *SndVoicePool_AllocVoice(SndVoicePool *p, SoundEffectInfo *info, s32 tag, s32 handle);
s32 SndVoicePool_IsSoundPlaying(SndVoicePool *pool, s32 value);
s32 SndVoicePool_FindSoundWithOwner(SndVoicePool *pool, s32 keyA, s32 keyB);
s32 SndVoicePool_StopGroupVoice(SndVoicePool *p, s16 tag, s16 key);
s32 SndQueue_Add(SndQueue *q, SoundEffectInfo *info, CVector3 *pos, s32 variant, s32 extra);
void SndVoice_SetPosition(SndVoice *v, CVector3 *pos, sptr follow);
void SndVoice_SetSoundId(SndVoice *v, s32 id);
void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 flags);

s32 CAudioWorld__PlayLayer(CAudioWorld *m, SoundEffectInfo *info, CVector3 *pos, s32 variant, s32 handle,
                           s32 extra, sptr follow) {
    SndVoice *v;

    m->listener = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, pos);
    if (info->flags & 1) {
        if (SndVoicePool_IsSoundPlaying(&m->pool, m->cueId) != 0) {
            return -1;
        }
        if (Sound_DistanceFalloff(pos, &m->listener->m_vPos) == 0.0f) {
            return -1;
        }
    }
    if ((info->flags & 0x1000) && m->cueArg != -1) {
        if (SndVoicePool_FindSoundWithOwner(&m->pool, m->cueId, m->cueArg) != -1) {
            return -1;
        }
    }
    if (info->sample != 0) {
        return SndQueue_Add(&m->queue, info, pos, variant, extra);
    }
    if (info->flags & 0x40) {
        SndVoicePool_StopGroupVoice(&m->pool, m->cueId, info->group);
    }
    v = SndVoicePool_AllocVoice(&m->pool, info, variant, handle);
    if (v == NULL) {
        return -1;
    }
    SndVoice_SetPosition(v, pos, follow);
    SndVoice_SetSoundId(v, m->cueId);
    SndVoice_Setup(v, info, extra);
    world_lock(m);
    CAudioWorld__StartVoice(m, v->id, 0, pos);
    world_unlock(m);
    return 0;
}

/* func_00270C80 (typed, but the last variant is read as the low half of m + 0x130 at 0x132) */
s32 CAudioWorld__PlaySound(CAudioWorld *m, s32 id, CVector3 pos, sptr follow, s32 owner) {
    s32 i;
    s32 count;
    s32 sum;
    s32 thresh;
    s32 lo;
    s32 hi;
    s32 idx;
    s32 ret;
    u32 n;
    u32 layers;
    s16 last;
    StrideTable *nd;

    if (m->muted != 0 && id == 0x44C) {
        return -1;
    }
    if (m->pendingCue != -1) {
        return -1;
    }
    if (id < 0x100) {
        count = 0;
        for (i = 0; i < 17; i++) {
            if (m->pool.voices[i].soundId == id) {
                count++;
            }
        }
        if (count >= 6) {
            return -1;
        }
    }
    last = (s16)m->lastVariant;
    idx = -1;
    if (m->rangeCount != 0 && BinaryRange(m->ranges, m->rangeCount, id, &lo, &hi) != 0) {
        i = lo;
        if (i != hi) {
            sum = 0;
            for (count = lo; count <= hi; count++) {
                sum += m->weights[count] * 10;
            }
            thresh = Random_Next() % sum;
            sum = 0;
            for (i = lo; i < hi; i++) {
                sum += m->weights[i] * 10;
                if (sum >= thresh) {
                    break;
                }
            }
            if (id < 0x100 && last != 0 && i == m->lastVariant) {
                i = (i == hi) ? i - 1 : i + 1;
            }
        }
        idx = i;
    }
    if (id < 0x100) {
        m->cueVolume = Random_Float(127.0f);
    }
    if (idx == -1) {
        return -1;
    }
    m->cue = CISet__GetBlock(CISet__GetBlock(m->variants, (s16)idx), 0);
    m->cueId = id;
    m->cueArg = owner;
    nd = CISet__GetBlock(CISet__GetBlock(m->variants, (s16)idx), 1);
    layers = nd->count;
    if (layers != 0) {
        world_lock(m);
        ret = ++m->handle;
        world_unlock(m);
        if (id < 0x100) {
            m->lastVariant = idx;
        }
    } else {
        ret = -1;
    }
    for (n = 0; n < layers; n++) {
        CAudioWorld__PlayLayer(m, (SoundEffectInfo *)(nd->entries + n * nd->stride), &pos, idx, ret, 0, follow);
    }
    m->cueArg = -1;
    return ret;
}

/* func_00271128 (typed; the PlayLayer prototype takes the position as a pointer) */
s32 CAudioWorld__PlaySoundIndex(CAudioWorld *m, s32 id, CVector3 pos, s32 follow) {
    StrideTable *rec;
    u32 i;
    s32 token;

    if (!(id < m->rangeCount)) {
        return -1;
    }
    m->cue = CISet__GetBlock(CISet__GetBlock(m->variants, (s16)id), 0);
    m->cueId = 0x100;
    m->cueArg = -1;
    rec = CISet__GetBlock(CISet__GetBlock(m->variants, (s16)id), 1);
    if (rec->count != 0) {
        world_lock(m);
        token = ++m->handle;
        world_unlock(m);
    } else {
        token = -1;
    }
    for (i = 0; i < (u32)rec->count; i++) {
        CAudioWorld__PlayLayer(m, (SoundEffectInfo *)(rec->entries + i * rec->stride), &pos, id, token, 0, follow);
    }
    return token;
}

/* func_002712E8 (ContextRecAt2BC0 view: red/green/blue are the three volumes) */
void SndVoicePool_Update(SndVoicePool *pool);
void SndQueue_Update(SndQueue *q);
void SpeechPlayer_Update(T2SpeechPlayer *sp);
void SndSeq_Update(SndSeq *s);

s32 CAudioWorld__Update(CAudioWorld *m) {
    u8 *bytes = &gEngineApp_m_Settings.unk15;

    m->sfxVolume = (f32)bytes[0] * 0.003921569f;
    m->volume2BA4 = (f32)bytes[-1] * 0.003921569f;
    m->musicVolume = (f32)bytes[1] * 0.003921569f;
    m->unk2BB0 = bytes[-2];
    world_lock(m);
    SndVoicePool_Update(&m->pool);
    world_unlock(m);
    SndQueue_Update(&m->queue);
    SpeechPlayer_Update((T2SpeechPlayer *)&m->fx);
    SndSeq_UpdateFade(&m->seq);
    SndSeq_Update(&m->seq);
    if (gAudioWorldReady != 0 && m->unk2BB4 != 0 && gEngineApp_m_Settings.mode == 13 && m->track > 0) {
        CVector3 zero = { 0.0f, 0.0f, 0.0f };
        CCamera *cam = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, cam->m_vPos, 0, -1);
        }
    }
    return ++m->frame;
}

/* func_00271630 (alpha at 0x2BBC = unk2BBC, the pitch scale new voices take) */
void CAudioWorld__PlaySoundPitched(CAudioWorld *m, s32 id, CVector3 pos, sptr follow, s32 owner, f32 pitch) {
    m->unk2BBC = pitch;
    CAudioWorld__PlaySound(m, id, pos, follow, owner);
    m->unk2BBC = 1.0f;
}

/* func_00271694 (the game mode read through a CCameraPool view at 0x13D8, the camera position
 * at 0x114) */
void CAudioWorld__UpdateAmbientTrack(CAudioWorld *m) {
    if (gAudioWorldReady != 0 && m->unk2BB4 != 0 && gEngineApp_m_Settings.mode == 13 && m->track > 0) {
        CVector3 zero = { 0.0f, 0.0f, 0.0f };
        CCamera *cam = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &zero);
        if ((m->frame & 3) == 0) {
            CAudioWorld__PlaySound(m, m->track, cam->m_vPos, 0, -1);
        }
    }
}

/* the one-liners with N64 offsets (0x2B78, 0x2BB0, 0x2BB4, 0x84, 0x1D64, 0x1DB8, 0x138, 0x2B9C,
 * 0x2BC0, 0x2B4C) */
SndEnv *CAudioWorld__GetPanEnvelope(CAudioWorld *m, s32 id) {
    return (SndEnv *)(m->table2B78 + id * 0xC);
}
void CAudioWorld__SetAmbientTrackEnabled(CAudioWorld *m, s32 on) {
    m->unk2BB4 = on;
}
s32 CAudioWorld__GetStereo(CAudioWorld *m) {
    return m->unk2BB0;
}
ALSndPlayer *CAudioWorld__GetSndPlayer(CAudioWorld *m) {
    return &m->player;
}
void CAudioWorld__StopSoundsByOwner(CAudioWorld *m, s32 owner) {
    SndVoicePool_StopByOwner(&m->pool, owner);
    SndQueue_CancelOwner(&m->queue, owner);
}
/* func_00271570 / func_002715F0: typed, but the ROM passes arg1 through to the pool call (a1) */
void CAudioWorld__StopSound(CAudioWorld *m, s32 id) {
    SndVoicePool_StopSound(&m->pool, id);
    SndQueue_CancelSoundId(&m->queue, id);
}
void CAudioWorld__StopSoundsByFlags(CAudioWorld *m, s32 flags) {
    SndVoicePool_StopByFlags(&m->pool, flags);
    SndQueue_CancelFlags(&m->queue, flags);
}
void CAudioWorld__FadeOutMusic(CAudioWorld *m) {
    SndSeq_FadeOut(&m->seq);
}
void SndSeq_Stop(SndSeq *s);
void SndSeq_Duck(SndSeq *s);
void SndSeq_Unduck(SndSeq *s);
void CAudioWorld__StopMusic(CAudioWorld *m) {
    SndSeq_Stop(&m->seq);
}
void CAudioWorld__DuckMusic(CAudioWorld *m) {
    SndSeq_Duck(&m->seq);
}
void CAudioWorld__UnduckMusic(CAudioWorld *m) {
    SndSeq_Unduck(&m->seq);
}
void SndVoicePool_ClearFollow(SndVoicePool *pool);
void CAudioWorld__DetachAllVoices(CAudioWorld *m) {
    SndVoicePool_ClearFollow(&m->pool);
}
void CAudioWorld__SetFxMix(CAudioWorld *m, f32 value) {
    f32 v = value;
    if (v > 1.0f) {
        v = 1.0f;
    } else if (v < 0.0f) {
        v = 0.0f;
    }
    m->fxMix = v * 40.0f;
}
s32 SpeechPlayer_Play(T2SpeechPlayer *sp, s32 group, f32 scale);
s32 SpeechPlayer_Stop(T2SpeechPlayer *sp);
s32 CAudioWorld__PlaySpeech(CAudioWorld *m, s32 group) {
    f32 v = 1.0f;
    if (!(v < m->musicVolume)) {
        v = m->musicVolume;
    }
    return SpeechPlayer_Play((T2SpeechPlayer *)&m->fx, group, v);
}
void CAudioWorld__StopSpeech(CAudioWorld *m) {
    SpeechPlayer_Stop((T2SpeechPlayer *)&m->fx);
}
/* func_00271B20 (SortTable: count at 0xE, u32 entries at 0x10 = an ALInstrument's sounds) */
void CAudioWorld__SortInstrumentSounds(ALInstrument *inst) {
    s32 i;
    s32 j;
    for (i = 0; i < inst->soundCount; i++) {
        for (j = i + 1; j < inst->soundCount; j++) {
            ALSound *right = inst->soundArray[j];
            ALSound *left = inst->soundArray[i];
            if ((uptr)right < (uptr)left) {
                inst->soundArray[i] = right;
                inst->soundArray[j] = left;
            }
        }
    }
}
/* func_00271BEC (SceneRecAt2B5C: lastPick 0x130, count 0x2B54, ids 0x2B58, weights 0x2B5C) */
s32 CAudioWorld__PickVariant(CAudioWorld *m, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total = 0;
    s32 entry;
    s32 draw;
    s32 i;

    if (m->rangeCount == 0) {
        return -1;
    }
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
            entry = (entry == last) ? entry - 1 : entry + 1;
        }
    }
    return entry;
}
void *CAudioWorld__GetVariantLayers(CAudioWorld *m, s16 id) {
    return CISet__GetBlock(CISet__GetBlock(m->variants, id), 1);
}
/* func_00271DB4 (muted at 0x2BB8, pendingCue at 0x102, soundId of voice i at 0x1E64 + i * 0xCC) */
s32 CAudioWorld__IsSoundBusy(CAudioWorld *m, s32 id) {
    s32 count = 0;
    s32 i;
    if ((m->muted != 0 && id == 0x44C) || m->pendingCue != -1) {
        return 1;
    }
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

/* func_00271E20 (the camera position at 0x114, its view rotation at 0x14C) */
s32 Sound_CalcPan(CVector3 *pos, CCamera *cam) {
    CVector3 r;
    CVector3 d;
    CVector3 w;
    f32 s;
    f32 zero;

    if (cam == NULL) {
        return 0x40;
    }
    CVector3__Subtract(&d, &cam->m_vPos, pos);
    if (d.x * d.x + d.z * d.z < 1.0e2f) {
        return 0x40;
    }
    CMtxF__InverseRotateVector(&cam->m_mfViewOrient, &d, &w);
    w.y = 0.0f;
    zero = w.y;
    r.y = zero;
    r.x = zero;
    r.z = 1.0f;
    CVector3__Normalize(&w);
    s = func_002982F0(Math_AcosLookup(w.x * r.x + w.y * r.y + w.z * r.z));
    if (w.x == zero) {
        return 0x40;
    }
    if (zero < w.x) {
        if (s < zero) {
            s = -s;
        }
    } else {
        s = -__builtin_fabsf(s);
    }
    return (s16)(s32)(s * 64.0f + 64.0f);
}

/* ======================================================================================
 * SndVoice
 * ====================================================================================== */

/* func_00271F6C (ViewRecAtB0: motion = vp, angles = env, presets = mgr) */
void SndVoice_InitPitch(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    SndEnv *env = &v->env;
    s16 x;
    s16 y;
    s32 turn;
    f32 diff;
    f32 r;

    v->vp.distSq = v->vp.prevDistSq = 0.0f;
    if (info->unkA != -1) {
        *env = *(SndEnv *)(v->mgr->table2B74 + info->unkA * 0xC);
    } else {
        env->level = FloatToHalf((f32)info->unk10);
    }
    x = SoundEffectInfo_GetRandomRange0(info);
    y = SoundEffectInfo_GetRandomRange1(info);
    r = Random_Blend((f32)y, (f32)x);
    turn = (s16)(s32)((f32)x + (r < 0.0f ? -r : r));
    env->level = FloatToHalf(HalfToFloat(env->level) + turn);
    if (flags & 0x40) {
        env->level = FloatToHalf(HalfToFloat(env->level) + 1.0e3f);
    }
    if (flags & 0x80) {
        env->level = FloatToHalf(HalfToFloat(env->level) - 1.0e3f);
    }
    if (HalfToFloat(env->level) >= 1.2e3f) {
        env->level = FloatToHalf(1.2e3f);
    }
    if (info->flags & 2) {
        v->vp.pitch.f = HalfToFloat(env->level);
        diff = HalfToFloat(env->unk2) - HalfToFloat(env->target);
        if (env->frames == 0) {
            v->envStep = 0.0f;
        } else {
            v->envStep = diff / env->frames;
        }
        v->envValue = HalfToFloat(env->unk2);
    }
    v->vp.pitch.f = cents_to_pitch(HalfToFloat(env->level));
    env->level = FloatToHalf(v->vp.pitch.f);
}

/* func_00272278 (M04Emitter / M04Ctx / M04Info views: the cue bytes at 0x2B88, the listener at
 * 0x2B98, envId = info->unk8, b0F = info->unkF) */
void SndVoice_InitVolume(SndVoice *o, SoundEffectInfo *info) {
    SndVoiceParams *vp = &o->vp;
    SndEnv *env = &o->env2;
    u8 *pp = o->mgr->cue;
    s32 i;
    s32 n;
    s32 t;
    f32 vf;
    f32 f;

    o->volumeScale = 1.0f;
    if (o->soundId < 0x100) {
        for (i = 0; i < 0x1F; i++) {
            if (o->soundId == (s32)sSoundKindVolumeTable[i].kind) {
                o->volumeScale = sSoundKindVolumeTable[i].scale;
                break;
            }
        }
    }
    if (Cinema_PathPlaying() != 0) {
        o->volumeScale *= 0.8f;
    }
    if (info->unk8 != -1) {
        *env = *(SndEnv *)(o->mgr->table2B70 + info->unk8 * 0xC);
    } else {
        env->level = FloatToHalf((f32)info->unkF);
    }
    if (!(info->flags & 0x800) && o->reset != 1 && o->mgr->listener != NULL) {
        f32 a = (f32)pp[0] * 0.01f;
        f32 b = (f32)pp[1] * 0.01f;
        CVector3 *p = &vp->pos;
        CVector3 *q = &o->mgr->listener->m_vPos;
        f32 d2;
        if (a == 1.0f) {
            vf = 1.0f;
        } else if (b == a) {
            vf = a;
        } else {
            vf = a + Random_Float(b - a);
        }
        d2 = (p->x - q->x) * (p->x - q->x) + (p->y - q->y) * (p->y - q->y) + (p->z - q->z) * (p->z - q->z);
        f = (sSoundHearingRangeSq <= d2) ? 0.0f : 1.0f - d2 / sSoundHearingRangeSq;
        switch (vp->distanceMode) {
        case 2:
            vp->scale = f;
            break;
        case 0:
            f = f * f;
            f = f * f;
            /* fallthrough */
        case 1:
            f = f * f;
            vp->scale = f;
            break;
        default:
            f = 0.0f;
            vp->scale = 0.0f;
            break;
        }
        vp->f20 = f;
    } else {
        vf = 1.0f;
        vp->scale = vf;
        vp->f20 = vf;
    }
    if (info->flags & 4) {
        f32 fs;
        vp->vol = (s32)HalfToFloat(env->level);
        fs = HalfToFloat(env->unk2) - HalfToFloat(env->target);
        o->env2Step = (env->frames == 0) ? 0.0f : fs / (f32)env->frames;
        o->env2Value = HalfToFloat(env->unk2);
        n = (s32)HalfToFloat(env->level);
        t = (f32)n < 0.0f ? -n : n;
        vp->vol = (s32)((f32)(s32)((f32)gSndVolumeCurve[t] * o->env2Value) * vf);
        env->level = FloatToHalf((f32)gSndVolumeCurve[t] * vf);
        vp->wC = (s32)HalfToFloat(env->level);
        return;
    }
    n = (s32)HalfToFloat(env->level);
    t = (f32)n < 0.0f ? -n : n;
    t = (s32)((f32)gSndVolumeCurve[t] * vf);
    vp->vol = t;
    vp->wC = t;
}

/* func_00272B54 (M04 views; the pan's low byte read big-endian) */
void SndVoice_UpdatePan(SndVoice *o) {
    SndVoiceParams *vp = &o->vp;
    CAudioWorld *c = o->mgr;
    s32 neg;

    if (CAudioWorld__GetStereo(c) == 0) {
        alSndpSetSound(&c->player, c->handles[o->index]);
        alSndpSetPan(&c->player, 0x40);
        return;
    }
    if (o->hasEnv != 0) {
        if (o->env3.delay > 0) {
            o->env3.delay--;
        } else {
            neg = o->env3Step < 0.0f;
            if (neg ? (o->env3Value < HalfToFloat(o->env3.target)) : (HalfToFloat(o->env3.target) < o->env3Value)) {
                o->env3Value -= o->env3Step;
                if (neg) {
                    if (HalfToFloat(o->env3.target) < o->env3Value) {
                        o->env3Value = HalfToFloat(o->env3.target);
                    }
                } else if (o->env3Value < HalfToFloat(o->env3.target)) {
                    o->env3Value = HalfToFloat(o->env3.target);
                }
            }
        }
        vp->pan = (s32)o->env3Value;
    } else if ((o->flags & 0x400) && o->reset == 0) {
        vp->pan = Sound_CalcPan(&vp->pos, c->listener);
    } else {
        vp->pan = 0x40;
        return;
    }
    alSndpSetSound(&c->player, c->handles[o->index]);
    alSndpSetPan(&c->player, (u8)vp->pan);
}

/* func_0027314C (typed, but the handles read through m + 0x7C + 0x60) */
void SndVoice_Update(SndVoice *s) {
    CAudioWorld *b;
    s32 playing;

    s->mgr->listener = CCameraPool__GetNearestCamera(&gEngineApp_m_Game_m_CameraPool, &s->vp.pos);
    b = s->mgr;
    alSndpSetSound(&b->player, b->handles[s->index]);
    playing = alSndpGetState(&b->player);
    if ((s->vp.scale <= 0.075f && s->soundId != 0x228) || ((s->flags & 4) && s->vp.vol == 0)) {
        s->stopping = 1;
    }
    if (s->hasTrack && s->mgr->track != s->soundId) {
        s->stopping = 1;
    }
    if (playing) {
        if (s->stopping) {
            voice_stop(s);
            return;
        }
    } else if (s->delay > 0) {
        s->delay--;
        s->flags |= 0x10;
        return;
    }
    if ((s->flags & 0x18) == 0x10) {
        alSndpPlay(&s->mgr->player);
        return;
    }
    if (!playing) {
        alSndpDeallocate(&b->player, b->handles[s->index]);
        b->handles[s->index] = -1;
        s->vp.priority = 0;
        s->id = -1;
        s->unk8 = -1;
        s->soundId = -1;
        s->vp.channel = -1;
        s->unkB4 = -1;
        return;
    }
    if (s->vp.follow != NULL) {
        s->vp.pos = *s->vp.follow;
    }
    SndVoice_UpdateVolume(s);
    SndVoice_UpdatePan(s);
    SndVoice_UpdatePitch(s);
}

/* func_00273E98 / func_00273EB8 (soundId at 0xA8) */
void SndVoice_SetPosition(SndVoice *v, CVector3 *pos, sptr follow) {
    v->vp.pos = *pos;
    v->vp.follow = (CVector3 *)follow;
}
void SndVoice_SetSoundId(SndVoice *v, s32 id) {
    v->soundId = id;
}

/* func_00273EC0 (View_func_8025AE1C_de: "priority" 0x28 = vp.pan, envelope 0x8C = env3,
 * sweeping 0x88 = hasEnv, trackId 0xA8 = soundId, fixed 0xC0 = reset) */
void SndVoice_Setup(SndVoice *v, SoundEffectInfo *info, s32 flags) {
    SndVoice_InitPitch(v, info, flags);
    SndVoice_InitVolume(v, info);
    v->vp.pan = 0x40;
    if (info->unkC != -1) {
        f32 first;
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, info->unkC);
        first = HalfToFloat(v->env3.unk2);
        v->env3Step = (first - HalfToFloat(v->env3.target)) / v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = (s16)(s32)HalfToFloat(v->env3.unk2);
        v->hasEnv = 1;
        return;
    }
    v->hasEnv = 0;
    if (v->soundId < 0x100) {
        if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
            v->vp.pan = (s8)v->mgr->cueVolume;
        }
    } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
        v->vp.pan = Sound_CalcPan(&v->vp.pos, v->mgr->listener);
    }
}

/* func_00274948 (typed; Sound_CalcPan takes the camera) */
void SndVoice_InitPan(SndVoice *v, SoundEffectInfo *info) {
    s16 id = info->unkC;
    v->vp.pan = 0x40;
    if (id != -1) {
        f32 d;
        v->env3 = *CAudioWorld__GetPanEnvelope(v->mgr, id);
        d = HalfToFloat(v->env3.unk2);
        v->env3Step = (d - HalfToFloat(v->env3.target)) / (f32)v->env3.frames;
        v->env3Value = HalfToFloat(v->env3.unk2);
        v->vp.pan = HalfToFloat(v->env3.unk2);
        v->hasEnv = 1;
        return;
    }
    v->hasEnv = 0;
    if (v->soundId < 0x100) {
        if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
            v->vp.pan = (s8)v->mgr->cueVolume;
        }
    } else if (v->reset == 0 && CAudioWorld__GetStereo(v->mgr) != 0) {
        v->vp.pan = Sound_CalcPan(&v->vp.pos, v->mgr->listener);
    }
}

/* func_002746AC .. func_002748D0 (Model views: the owner at 0xB0, handles at 0xDC, player at
 * 0x84; RecordCode8025A3EC: doppler 0x34, pitchScale 0xB8) */
void SndVoice_SetVolume(SndVoice *v, s32 vol) {
    CAudioWorld *m = v->mgr;
    alSndpSetSound(&m->player, m->handles[v->index]);
    alSndpSetVol(&m->player, (s16)(s32)((f32)vol * v->volumeScale));
}
void SndVoice_SetPitch(SndVoice *v, f32 pitch) {
    CAudioWorld *m = v->mgr;
    alSndpSetSound(&m->player, m->handles[v->index]);
    alSndpSetPitch(&m->player, pitch * v->vp.doppler * v->pitchScale);
}
void SndVoice_SetPan(SndVoice *v, s32 pan) {
    CAudioWorld *m = v->mgr;
    alSndpSetSound(&m->player, m->handles[v->index]);
    alSndpSetPan(&m->player, pan & 0xFF);
}
s32 SndVoice_GetState(SndVoice *v) {
    CAudioWorld *m = v->mgr;
    alSndpSetSound(&m->player, m->handles[v->index]);
    return alSndpGetState(&m->player);
}
void SndVoice_Play(SndVoice *v) {
    alSndpPlay(&v->mgr->player);
}
void SndVoice_Stop(SndVoice *v) {
    voice_stop(v);
}
void SndVoice_Release(SndVoice *v) {
    CAudioWorld *m = v->mgr;
    alSndpDeallocate(&m->player, m->handles[v->index]);
    m->handles[v->index] = -1;
    v->vp.priority = 0;
    v->id = -1;
    v->unk8 = -1;
    v->soundId = -1;
    v->vp.channel = -1;
    v->unkB4 = -1;
}

/* ======================================================================================
 * SndVoicePool
 * ====================================================================================== */

/* func_00273370 (SndCore: handles at m + 0x7C + 0x60; voice by the N64 stride) */
s16 SndVoicePool_FindSlot(SndVoicePool *p, s32 a, s32 b, s16 pri, s32 c) {
    CAudioWorld *m = p->owner;
    SndVoice *v = p->voices;
    s32 i;
    s32 best = pri;
    s32 bestIdx = -1;
    (void)a;
    (void)b;
    (void)c;

    for (i = 0; i < 16; i++, v++) {
        if (m->handles[i] == -1) {
            return i;
        }
        if (v->vp.priority < pri && !(best < v->vp.priority) && v->vp.priority != 100) {
            best = v->vp.priority;
            bestIdx = i;
        }
    }
    if (bestIdx == -1) {
        return -1;
    }
    if (m->handles[bestIdx] == -1) {
        SndNode *h = (SndNode *)m->queue.active.link.next;
        if ((SndLink *)h == &m->queue.active.link) {
            return bestIdx;
        }
        if (h->body.vp.priority > pri) {
            return -1;
        }
        return bestIdx;
    }
    voice_stop(&p->voices[bestIdx]);
    if (p->voices[16].id != -1) {
        return -1;
    }
    return 16;
}

/* func_00273504 (M04Bank2: the sfx instrument; core + 0x80 = the u16 at 0xFC; sample at info+2,
 * the FX mix byte at info+0xE) */
extern s32 D_800B2A34[];
extern s32 D_800B2A44[];
SndVoice *SndVoicePool_AllocVoice(SndVoicePool *p, SoundEffectInfo *info, s32 tag, s32 handle) {
    CAudioWorld *m = p->owner;
    s16 pri = SoundEffectInfo_GetGroup(info);
    u16 flag = SoundEffectInfo_GetPriority(info);
    s16 idx;
    s32 i;
    SndVoice *v;
    SndVoiceParams *vp;

    if (sfx_inst(m)->soundCount < pri) {
        return NULL;
    }
    CAudioWorld__Lock(m);
    if (!(SoundEffectInfo_GetChance(info) == 0x64 || (Random_NextB() % 100) < SoundEffectInfo_GetChance(info))) {
        goto fail;
    }
    idx = SndVoicePool_FindSlot(p, (s16)tag, pri, (s16)flag, SoundEffectInfo_GetFlags(info));
    if (idx == -1) {
        goto fail;
    }
    v = &p->voices[idx];
    if (idx == 16) {
        v->sound = handle;
        v->id = m->soundCounter++;
        v->unk8 = 0x63;
    } else {
        f32 f;
        s32 pr;
        m->pendingCue = idx;
        m->handles[idx] = alSndpAllocate(&m->player, sfx_inst(m)->soundArray[pri]);
        if (m->handles[idx] == -1) {
            v->id = -1;
            v->unk8 = -1;
            v->vp.priority = 0;
            m->pendingCue = -1;
            goto fail;
        }
        f = (f32)(s16)flag * 1.27f;
        (*(u16 *)m->padFC)++; /* a counter at 0xFC nothing reads */
        v->sound = handle;
        v->id = m->soundCounter++;
        v->unk8 = m->handles[idx];
        v->delay = info->sample;
        pr = (f >= 2.1474836e9f) ? ((s32)(f - 2.1474836e9f) | 0x80000000) : (s32)f;
        alSndpSetPriority(&m->player, m->handles[idx], pr & 0xFF);
    }
    v->soundId = m->cueId;
    v->reset = m->unk2BAC;
    v->hasTrack = 0;
    for (i = 0; i < 4; i++) {
        if (v->soundId == D_800B2A34[i]) {
            v->hasTrack = 1;
            break;
        }
    }
    v->unkC4 = 1;
    for (i = 0; i < 6; i++) {
        if (v->soundId == D_800B2A44[i]) {
            v->unkC4 = 0;
            break;
        }
    }
    v->unkB4 = m->cueArg;
    v->stopping = 0;
    vp = &v->vp;
    v->pitchScale = m->unk2BBC;
    vp->priority = flag;
    vp->vol = vp->wC = 0x7D00;
    vp->pitch.i = 0;
    vp->fxMix = info->pitch; /* the s8 at 0xE */
    vp->pan = 0x40;
    vp->channel = pri;
    vp->s2C = tag;
    vp->frame = m->frame;
    vp->doppler = 1.0f;
    vp->distanceMode = SoundEffectInfo_GetPan(info);
    v->flags = SoundEffectInfo_GetFlags(info);
    vp->follow = NULL;
    CAudioWorld__Unlock(m);
    return v;
fail:
    CAudioWorld__Unlock(m);
    return NULL;
}

/* func_00273B90 / func_00273BF8 (voice by the N64 stride; owner at 0xB4 - 4) */
s32 SndVoicePool_GetVoiceState(SndVoicePool *pool, s16 idx) {
    return SndVoice_GetState(&pool->voices[idx]);
}
void SndVoicePool_PlayVoice(SndVoicePool *pool, s16 idx) {
    SndVoice *v = &pool->voices[idx];
    alSndpPlay(&v->mgr->player);
    v->flags |= 8;
}
/* func_00273D88 (SlotRecAtA8: used = unk8, value = soundId; header->local = pendingCue) */
s32 SndVoicePool_IsSoundPlaying(SndVoicePool *pool, s32 value) {
    s32 i;
    for (i = 0; i < 16; i++) {
        SndVoice *v = &pool->voices[i];
        if (v->unk8 != -1 && pool->owner->pendingCue != i && v->soundId == value) {
            return 1;
        }
    }
    return 0;
}
/* func_00273DD8 (voice by the N64 stride) */
void SndVoicePool_StopVoice(SndVoicePool *pool, s16 idx) {
    voice_stop(&pool->voices[idx]);
}
/* func_0027403C (Slot0027403C: unk8 at 8, stride 0xCC; skipIndex = pendingCue) */
void SndVoicePool_Update(SndVoicePool *pool) {
    s32 i;
    s32 n = 0;
    for (i = 0; i < 16; i++) {
        SndVoice *v = &pool->voices[i];
        if (v->unk8 != -1 && pool->owner->pendingCue != i) {
            SndVoice_Update(v);
            n++;
        }
    }
    sSndVoicesActive = n;
    if (sSndVoicesPeak < n) {
        sSndVoicesPeak = n;
    }
}
/* func_002740E8 (ObjRecAtCV4: unk8, soundId and unkB4 read as words 0x33 apart) */
s32 SndVoicePool_FindSoundWithOwner(SndVoicePool *pool, s32 keyA, s32 keyB) {
    s32 i;
    for (i = 0; i < 16; i++) {
        SndVoice *v = &pool->voices[i];
        if (v->unk8 != -1 && pool->owner->pendingCue != i && v->soundId == keyA && v->unkB4 == keyB) {
            return i;
        }
    }
    return -1;
}
/* func_00274A98 (SndCore handles; voice by the N64 stride) */
s32 SndVoicePool_ReleaseVoice(SndVoicePool *p, s16 idx) {
    SndVoice_Release(&p->voices[idx]);
    return 0;
}
/* func_00273C5C (typed, voice by the N64 stride) */
s32 SndVoicePool_StopGroupVoice(SndVoicePool *p, s16 tag, s16 key) {
    s32 i;
    for (i = 0; i < 16; i++) {
        SndVoice *v = &p->voices[i];
        if (v->unk8 != -1 && p->owner->pendingCue != i && v->soundId == tag && (v->flags & 0x40) != 0 &&
            v->vp.channel == key) {
            voice_stop(v);
            return (s16)i;
        }
    }
    return -1;
}

/* ======================================================================================
 * SndQueue (requests waiting for a voice): active list at 0x04, free list at 0xD8
 * ====================================================================================== */

static __inline__ void link_remove(SndLink *n) {
    n->prev->next = n->next;
    n->next->prev = n->prev;
}
static __inline__ void link_append(SndLink *head, SndLink *n) {
    n->prev = head->prev;
    n->next = head;
    head->prev->next = n;
    head->prev = n;
}
/* inserted before the first active request of a lower priority */
static void queue_insert(SndQueue *q, SndNode *node) {
    SndLink *at = q->active.link.next;
    while (at != &q->active.link) {
        if (((SndNode *)at)->body.vp.priority < node->body.vp.priority) {
            break;
        }
        at = at->next;
    }
    link_append(at, &node->link);
}

/* func_00275FA0 (ManagerRecAtD8V1 / NodeRecAtC8; node offsets - 8 = body offsets) */
s32 SndQueue_Add(SndQueue *q, SoundEffectInfo *info, CVector3 *pos, s32 variant, s32 extra) {
    SndNode *node;
    CAudioWorld *m = q->owner;

    if (q->free.link.next == &q->free.link) {
        return -1;
    }
    CAudioWorld__Lock(m);
    node = (SndNode *)q->free.link.next;
    link_remove(&node->link);
    node->body.id = m->soundCounter++;
    node->body.sound = variant;
    node->body.vp.unk30 = -1;
    node->body.vp.s2C = variant;
    node->body.vp.channel = SoundEffectInfo_GetGroup(info);
    node->body.vp.priority = SoundEffectInfo_GetPriority(info);
    node->body.vp.w4 = SoundEffectInfo_GetSample(info);
    node->body.vp.distanceMode = SoundEffectInfo_GetPan(info);
    node->body.flags = SoundEffectInfo_GetFlags(info);
    node->body.vp.fxMix = SoundEffectInfo_GetPitch(info);
    node->body.vp.wC = 0x7D00;
    node->body.vp.vol = 0x7D00;
    node->body.vp.pitch.i = 0;
    node->body.vp.frame = m->frame;
    node->body.vp.pos = *pos;
    node->body.vp.follow = NULL;
    node->body.stopping = 0;
    SndVoice_SetSoundId(&node->body, m->cueId);
    node->body.reset = m->unk2BAC;
    SndVoice_Setup(&node->body, info, extra);
    queue_insert(q, node);
    CAudioWorld__Unlock(m);
    return node->body.id;
}

/* func_00276180 (Cache / Entry_func_80259440_de / CellCode802591C0 views: entry delay = vp.w4,
 * glyph/font/style = vp.priority/channel/s2C, sequence = vp.unk30, fieldA8 = delay; the bank's
 * "glyphs" are the sfx instrument's sounds) */
void SndQueue_Update(SndQueue *q) {
    CAudioWorld *m = q->owner;
    SndLink *l;
    SndLink *next;

    CAudioWorld__Lock(m);
    for (l = q->active.link.next; l != &q->active.link; l = next) {
        SndNode *e = (SndNode *)l;
        s16 slot;
        SndVoice *cell;
        next = l->next;
        if (e->body.vp.w4 != 0) {
            if (--e->body.vp.w4 == 0) {
                e->body.vp.frame = m->frame;
                e->body.vp.unk30 = m->soundCounter++;
                e->body.delay = 0;
            }
            continue;
        }
        if (m->frame - e->body.vp.frame >= 16) {
            link_remove(l);
            link_append(&q->free.link, l);
            continue;
        }
        slot = SndVoicePool_FindSlot(&m->pool, e->body.vp.s2C, e->body.vp.channel, e->body.vp.priority, 0);
        if (slot == -1 || slot == 16) {
            break;
        }
        if (m->handles[slot] != -1) {
            break;
        }
        m->handles[slot] = alSndpAllocate(&m->player, sfx_inst(m)->soundArray[e->body.vp.channel]);
        if (m->handles[slot] == -1) {
            break;
        }
        cell = &m->pool.voices[slot];
        memcpy(cell, &e->body, sizeof(SndVoice));
        cell->id = e->body.vp.unk30;
        cell->unk8 = m->handles[slot];
        cell->vp.frame = m->frame;
        cell->index = slot;
        CAudioWorld__StartVoice(m, cell->id, 0, &cell->vp.pos);
        e->body.vp.unk30 = -1;
        e->body.vp.priority = -1;
        link_remove(l);
        link_append(&q->free.link, l);
    }
    CAudioWorld__Unlock(m);
}

/* func_002763AC (ListHead: the voice at 8) */
void SndQueue_InitNode(SndNode *n, CAudioWorld *m) {
    n->link.prev = &n->link;
    n->link.next = &n->link;
    SndVoice_Construct(&n->body, m, -1);
}

/* func_002764C4 (ManagerRecAtD8V2 / NodeRecAt4C / SceneCode802591C0: the voice at
 * 0x1DBC + i * 0xCC, busy = stopping, timer = vp.w4, owner = vp.unk30; the selection fields at
 * 0x2A84.. are voice 16's unk8, id, vp.priority and vp.channel) */
void SndQueue_RequeueVoice(SndQueue *q, s32 index, s32 owner, CVector3 *pos) {
    CAudioWorld *m = q->owner;
    SndVoice *v = &m->pool.voices[index];
    SndVoice *v16 = &m->pool.voices[16];

    if (v->stopping != 0) {
        return;
    }
    if (q->free.link.next != &q->free.link) {
        SndNode *node = (SndNode *)q->free.link.next;
        link_remove(&node->link);
        memcpy(&node->body, v, sizeof(SndVoice));
        node->body.vp.unk30 = owner;
        if (pos != NULL) {
            node->body.vp.pos = *pos;
        }
        node->body.vp.w4 = 0;
        queue_insert(q, node);
    }
    v16->vp.priority = 0;
    v16->vp.channel = -1;
    v16->unk8 = -1;
    v16->id = -1;
}

/* ======================================================================================
 * SndSeq (the music)
 * ====================================================================================== */

/* func_00274B40 (Track view: data = the music ALBankFile (its bankArray[0] at 4 on the N64),
 * unkC the song, unk10 the ALCSeq, unk14 the player; ContextRecAt2BB8 = the owner) */
void SndSeq_Update(SndSeq *s) {
    CAudioWorld *m = s->owner;
    f32 f;
    s32 i;

    switch (s->unk18) {
    case 0x20:
        if (s->flags & 4) {
            s->flags = 0;
            sSndSeqCurrentId = -1;
            s->unk18 = (s->track >= 0) ? 0x40 : 0x10;
        }
        return;
    case 0x100:
        if (s->flags & 8) {
            s->unk18 = 0x80;
            s->flags = 0;
            return;
        }
        if (++sSndSeqLoadRetries >= 0xA) {
            sSndSeqCurrentId = -1;
            s->unk18 = 0x40;
        }
        return;
    case 0x40:
        if (s->track == -1) {
            s->unk18 = 0x10;
            return;
        }
        i = BinarySearch(m->table2B60, m->count2B64, s->track);
        if (i != -1) {
            u8 *n;
            i *= 2;
            s->unkC = (sptr)CISet__GetBlock(m->node2B50, i | 1);
            n = CISet__GetBlock(m->node2B50, i);
            s->unk24 = *(s32 *)n;
            s->unk30 = HalfToFloat(*(u16 *)(n + 4));
            if (s->unk30 <= 0.0f) {
                s->unk30 = 0.1f;
            }
        } else {
            s->unkC = 0;
        }
        if (s->unkC == 0) {
            s->unk18 = 0x100;
            sSndSeqLoadRetries = 0;
            s->flags = 0;
            return;
        }
        s->unk18 = 0x80;
        s->unk4 = 1;
        /* fallthrough */
    case 0x80:
        alCSeqNew(s->unk10, (u8 *)s->unkC);
        alCSPSetSeq((void *)s->csp, s->unk10);
        alCSPSetBank((void *)s->csp, ((ALBankFile *)s->data)->bankArray[0]);
        for (i = 0; i < 0x14; i++) {
            alCSPSetChlFXMix((void *)s->csp, i & 0xFF, 0);
        }
        f = (f32)s->unk24 * (m->volume2BA4 * 0.01f);
        alCSPSetVol((void *)s->csp, (s16)(s32)(f * 32767.0f));
        s->vol = f;
        alCSPPlay((void *)s->csp);
        s->flags = 0;
        s->unk18 = 0x10;
        sSndSeqCurrentId = s->track;
        return;
    case 0x10:
    default:
        if (sSndSeqCurrentId > 0 && alCSPGetState((void *)s->csp) == 0) {
            sSndSeqCurrentId = 0;
            s->track = 0;
        } else {
            if (s->sliding != 0) {
                f = (f32)s->unk24 * (m->volume2BA4 * 0.01f) * s->current;
            } else {
                f = (f32)s->unk24 * (m->volume2BA4 * 0.01f);
                if (m->muted != 0) {
                    f *= 0.7f;
                }
            }
            if (s->sliding != 0 || f != s->vol) {
                alCSPSetVol((void *)s->csp, (s16)(s32)(f * 32767.0f));
                s->vol = f;
            }
        }
        if (s->track != sSndSeqCurrentId) {
            if (sSndSeqCurrentId > 0) {
                f32 t = s->unk30;
                s->flags = 2;
                s->unk18 = 0x20;
                s->volume.w = (s32)(s->vol * 32767.0f);
                if (!(t >= 0.1f)) {
                    t = 0.1f;
                }
                s->unk30 = t;
                s->fadeStep = (f32)s->volume.w / (t * 60.0f);
                return;
            }
            s->flags = 0;
            s->unk18 = 0x40;
        }
        return;
    }
}

/* func_00275018 (typed; the player as a pointer, the volume's low half) */
void SndSeq_UpdateFade(SndSeq *ch) {
    if (ch->flags & 2) {
        alCSPSetVol((void *)ch->csp, (s16)ch->volume.w);
        ch->volume.w = (f32)ch->volume.w - ch->fadeStep;
        if (ch->volume.w <= 0) {
            ch->volume.w = 0;
            alCSPStop((void *)ch->csp);
            ch->flags |= 4;
        }
    } else if (ch->sliding) {
        f32 current = ch->current;
        f32 target = ch->target;
        f32 next;
        if (target < current) {
            next = current - 1.0f / 60;
            if (!(target <= next)) {
                next = target;
            }
        } else if (current < target) {
            next = current + 1.0f / 60;
            if (!(next <= target)) {
                next = target;
            }
        } else {
            return;
        }
        ch->current = next;
    }
}

/* func_00275100 (typed; the song pointer kept whole) */
s32 SndSeq_SelectSong(SndSeq *s, s32 key) {
    s32 i = BinarySearch(s->owner->table2B60, s->owner->count2B64, key);
    if (i != -1) {
        u8 *n;
        i <<= 1;
        s->unkC = (sptr)CISet__GetBlock(s->owner->node2B50, i | 1);
        n = CISet__GetBlock(s->owner->node2B50, i);
        s->unk24 = *(s32 *)n;
        s->unk30 = HalfToFloat(*(u16 *)(n + 4));
        if (s->unk30 <= 0.0f) {
            s->unk30 = 0.1f;
        }
    } else {
        s->unkC = 0;
    }
    return s->unkC != 0;
}
/* func_00275224 .. func_00275450 (pads at 0x14, 0x28, 0x2C, 0x38, 0x40) */
void SndSeq_Duck(SndSeq *s) {
    s->sliding = 1;
    s->target = 0.0f;
}
void SndSeq_Unduck(SndSeq *s) {
    s->sliding = 0;
    s->target = 1.0f;
}
void SndSeq_Stop(SndSeq *s) {
    s->track = -1;
    alCSPStop((void *)s->csp);
}
void *SndSeq_GetSongData(SndSeq *s, s32 index) {
    s32 i = BinarySearch(s->owner->table2B60, s->owner->count2B64, index);
    if (i != -1) {
        u8 *n;
        void *r;
        i *= 2;
        r = CISet__GetBlock(s->owner->node2B50, i | 1);
        n = CISet__GetBlock(s->owner->node2B50, i);
        s->unk24 = *(s32 *)n;
        s->unk30 = HalfToFloat(*(u16 *)(n + 4));
        if (s->unk30 <= 0.0f) {
            s->unk30 = 0.1f;
        }
        return r;
    }
    return NULL;
}
void SndSeq_SetVolume(SndSeq *s, f32 v) {
    alCSPSetVol((void *)s->csp, (s16)(s32)(v * 32767.0f));
    s->vol = v;
}
void SndSeq_Load(SndSeq *s) {
    s32 i;
    f32 f;
    alCSeqNew(s->unk10, (u8 *)s->unkC);
    alCSPSetSeq((void *)s->csp, s->unk10);
    alCSPSetBank((void *)s->csp, ((ALBankFile *)s->data)->bankArray[0]);
    for (i = 0; i < 0x14; i++) {
        alCSPSetChlFXMix((void *)s->csp, i & 0xFF, 0);
    }
    f = (f32)s->unk24 * (s->owner->volume2BA4 * 0.01f);
    alCSPSetVol((void *)s->csp, (s16)(s32)(f * 32767.0f));
    s->vol = f;
}
/* func_00275378 (typed; the player as a pointer) */
void SndSeq_UpdateVolume(SndSeq *s) {
    f32 f;
    if (s->sliding != 0) {
        f = (f32)s->unk24 * (s->owner->volume2BA4 * 0.01f) * s->current;
    } else {
        f = (f32)s->unk24 * (s->owner->volume2BA4 * 0.01f);
        if (s->owner->muted != 0) {
            f *= 0.7f;
        }
        if (f == s->vol) {
            return;
        }
    }
    alCSPSetVol((void *)s->csp, f * 32767.0f);
    s->vol = f;
}

/* ======================================================================================
 * The speech player (adec streams; SndFx at N64 0x2BC0)
 * ====================================================================================== */

/* func_00276D98 (Shape_typemap_110: the sound player through GetSndPlayer(m) = m + 0x84; with
 * the N64 offset that allocation wrote into the bank pointers and the sound player's event
 * queue: r4's evtq corruption) */
void SpeechPlayer_Construct(T2SpeechPlayer *sp, CAudioWorld *m) {
    sp->owner = m;
    sp->request = -1;
    sp->loop = -1;
    sp->state = 0;
    sp->speed = 0;
    sp->pan = 0x40;
    sp->sndId = alSndpAllocate(CAudioWorld__GetSndPlayer(m), &sSpeechSound);
}

static s32 speech_pick(CAudioWorld *m, s32 group) {
    s32 first;
    s32 last;
    s32 i;
    s32 total = 0;
    s32 roll;
    if (BinaryRange(m->table2B60, m->count2B64, group, &first, &last) == 0) {
        return -1;
    }
    if (first == last) {
        return first;
    }
    for (i = first; i <= last; i++) {
        total += 100;
    }
    roll = Random_Next() % total;
    total = 0;
    for (i = first; i < last; i++) {
        total += 100;
        if (roll <= total) {
            return i;
        }
    }
    return i;
}

/* func_00277024 / func_00276DFC (SoundSlot views: state 0x08, the sound id's low half 0x1E) */
void SpeechPlayer_StopUnlocked(T2SpeechPlayer *sp) {
    if (sp->state != 2 && sp->state != 0) {
        ALSndPlayer *pl = CAudioWorld__GetSndPlayer(sp->owner);
        sp->state = 2;
        alSndpSetSound(pl, (s16)sp->sndId);
        alSndpStop(pl);
    }
}
s32 SpeechPlayer_Stop(T2SpeechPlayer *sp) {
    CAudioWorld__Lock(sp->owner);
    SpeechPlayer_StopUnlocked(sp);
    CAudioWorld__Unlock(sp->owner);
    return 0;
}

/* func_00276B10 (MusicBank / MusicPlayer views) */
s32 SpeechPlayer_Play(T2SpeechPlayer *sp, s32 group, f32 scale) {
    s32 song = speech_pick(sp->owner, (s16)group);
    s32 request;
    if (song == -1) {
        return -1;
    }
    CAudioWorld__Lock(sp->owner);
    sp->loop = group;
    sp->song = song;
    request = ++sp->request;
    speech_pick(sp->owner, (s16)sp->loop); /* drawn again and dropped: keeps the random sequence */
    sp->speed = scale * 32767.0f;
    sp->pan = 0x40;
    if (sp->state == 3 || sp->state == 1) {
        SpeechPlayer_StopUnlocked(sp);
    } else {
        sp->state = 1;
    }
    CAudioWorld__Unlock(sp->owner);
    return request;
}

/* func_00276E78 (ContextRecAt2BB8: the song table at 0x2B60/0x2B64) */
s32 SpeechPlayer_PickEntry(T2SpeechPlayer *sp, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total = 0;
    s32 entry;
    s32 draw;
    CAudioWorld *m = sp->owner;
    if (BinaryRange(m->table2B60, m->count2B64, id, &first, &last) == 0) {
        return -1;
    }
    if (first == last) {
        return first;
    }
    for (entry = first; entry <= last; entry++) {
        total += 100;
    }
    draw = Random_Next() % total;
    total = 0;
    for (entry = first; entry < last; entry++) {
        total += 100;
        if (total >= draw) {
            break;
        }
    }
    if (avoid != 0 && entry == avoid) {
        entry = (entry == last) ? entry - 1 : entry + 1;
    }
    return entry;
}

/* func_002768D0 (PlayerRecAt24 view). State 1 opens an adec stream (adecStreamOpen and the
 * adec globals D_800F55F4, written through N64-offset views): the adec player is not typed for
 * the host yet, so a speech request ends at once (state 0) and says so once. */
void SpeechPlayer_Update(T2SpeechPlayer *sp) {
    static int warned;
    ALSndPlayer *pl;
    CAudioWorld__Lock(sp->owner);
    pl = CAudioWorld__GetSndPlayer(sp->owner);
    switch (sp->state) {
    case 2:
        alSndpSetSound(pl, (s16)sp->sndId);
        if (alSndpGetState(pl) == 0) {
            sp->state = (sp->loop >= 0) ? 1 : 0;
        }
        break;
    case 1:
        if (!warned) {
            warned = 1;
            fprintf(stderr, "t2audio: speech (adec stream, song %d) not played on the host yet\n", sp->song);
        }
        sp->loop = -1;
        sp->state = 0;
        break;
    default:
        break;
    }
    CAudioWorld__Unlock(sp->owner);
}

/* ======================================================================================
 * LoopSounds (the level's looping sounds: free list at 0, playing list at 0x14, paused 0x28)
 * ====================================================================================== */

/* func_00275BE8 (SoundVoicePool view: its state after two N64-sized CLists) */
void LoopSounds_Construct(LoopSounds *ls, SndSource *src, s32 n) {
    s32 i;
    CList__Construct(&ls->m_FreeList, 0, sizeof(void *));
    CList__Construct(&ls->m_UsedList, 0, sizeof(void *));
    for (i = 0; i < n; i++) {
        CList__AddTail(&ls->m_FreeList, &src[i]);
    }
    ls->m_Paused = 0;
}
/* func_00275C6C (SoundSourceNode: value = handle, state = sound, owner = flags) */
SndSource *LoopSounds_Start(LoopSounds *ls, s32 sound, CVector3 *pos, void *follow, s32 owner) {
    SndSource *node = ls->m_FreeList.m_pHead;
    if (node != NULL) {
        s32 result;
        CList__Remove(&ls->m_FreeList, node);
        CList__AddTail(&ls->m_UsedList, node);
        node->sound.w = -1;
        node->handle = -1;
        result = Sound__Play((s16)sound, *pos, (sptr)follow, owner);
        node->handle = result;
        sLoopSoundsLastStarted = result;
        node->sound.w = sound;
        node->pos = *pos;
        node->flags = (sptr)follow;
    }
    return node;
}
/* func_00275D34 */
s32 LoopSounds_Stop(LoopSounds *ls, SndSource *src) {
    SndSource *n;
    if (src == NULL) {
        return 0;
    }
    for (n = ls->m_UsedList.m_pHead; n != NULL; n = n->next) {
        if (n == src) {
            Sound__Stop(n->handle);
            sLoopSoundsLastStopped = n->handle;
            CList__Remove(&ls->m_UsedList, n);
            CList__AddTail(&ls->m_FreeList, n);
            return 1;
        }
    }
    return 0;
}
/* func_00275DC0 / func_00275E1C / func_00275E98 (the playing list at 0x14, paused at 0x28) */
void LoopSounds_Pause(LoopSounds *ls) {
    SndSource *n;
    for (n = ls->m_UsedList.m_pHead; n != NULL; n = n->next) {
        Sound__Stop(n->handle);
        sLoopSoundsLastPaused = n->handle;
    }
    ls->m_Paused = 1;
}
void LoopSounds_Resume(LoopSounds *ls) {
    SndSource *n;
    for (n = ls->m_UsedList.m_pHead; n != NULL; n = n->next) {
        n->handle = Sound__Play(n->sound.h.lo, n->pos, n->flags, -1);
        sLoopSoundsLastResumed = n->handle;
    }
    ls->m_Paused = 0;
}
void LoopSounds_Update(LoopSounds *ls) {
    SndSource *n;
    if (ls->m_Paused != 0) {
        return;
    }
    for (n = ls->m_UsedList.m_pHead; n != NULL; n = n->next) {
        if (Sound__IsPlaying(n->handle) == 0) {
            n->sound.w = -1;
            n->handle = -1;
        }
    }
}
/* func_00275EFC (typed; the free list is the LoopSounds itself) */
void LoopSounds_StopAll(LoopSounds *ls) {
    SndSource *n = ls->m_UsedList.m_pHead;
    while (n != NULL) {
        SndSource *next = n->next;
        Sound__Stop(n->handle);
        n->sound.w = -1;
        n->handle = -1;
        CList__Remove(&ls->m_UsedList, n);
        CList__AddTail(&ls->m_FreeList, n);
        n = next;
    }
}

/* func_002757E4 (declared void: the ROM returns CAudioWorld__CountHandleVoices' v0, which
 * LoopSounds_Update reads) */
s32 Sound__IsPlaying(s32 handle) {
    if (!T2Port_AudioReady()) {
        return 0;
    }
    return CAudioWorld__CountHandleVoices(&gAudioWorld, handle);
}

/* Debug only (game.cpp's T2_TEST_SFX / T2_TEST_SONG): the volume options are 0 until the game
 * loads its settings (after the pak check), so CAudioWorld__Update would mix every voice at 0;
 * give the three volume bytes a middle value when they are still 0. */
void T2Audio_TestVolumes(void) {
    u8 *bytes = &gEngineApp_m_Settings.unk15;
    s32 i;
    for (i = -1; i <= 1; i++) {
        if (bytes[i] == 0) {
            bytes[i] = 0xC0;
        }
    }
}
