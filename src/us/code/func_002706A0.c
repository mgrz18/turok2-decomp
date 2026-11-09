#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00257814.c, func_00257814.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sound.h"
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
