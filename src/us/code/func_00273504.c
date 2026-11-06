/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B1E8.c, func_0025B1E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"
#include "rw/junk_types.h" /* M04Bank2 */

extern f32 D_800A8160;
extern f32 D_800A8164;
extern f32 D_800A8168;
extern s32 D_800B2A34[];
extern s32 D_800B2A44[];
s16 alSndpAllocate(void *p, s32 a, void *owner);
void alSndpSetPriority(void *p, s16 id, s32 pri);
void CAudioWorld__Lock(CAudioWorld *m);
void CAudioWorld__Unlock(CAudioWorld *m);
s16 SndVoicePool_FindSlot(SndVoicePool *p, s16 a, s16 b, s16 c, s32 d);
s16 SoundEffectInfo_GetGroup(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPriority(SoundEffectInfo *i);
s8 SoundEffectInfo_GetChance(SoundEffectInfo *i);
s8 SoundEffectInfo_GetPan(SoundEffectInfo *i);
s16 SoundEffectInfo_GetFlags(SoundEffectInfo *i);
s32 Random_NextB();
SndVoice *SndVoicePool_AllocVoice(SndVoicePool *p, SoundEffectInfo *info, int tag, s32 a3)
{
  s16 *snd;
  CAudioWorld *m3;
  CAudioWorld *m;
  s32 i;
  s16 idx;
  u8 *core;
  s32 pr;
  SndVoiceParams *vp;
  s32 pri;
  unsigned int sv;
  u16 flag;
  f32 f;
  u8 *new_var2;
  CAudioWorld *m2;
  s16 *new_var;
  s32 r;
  SndVoice *v;
  pri = SoundEffectInfo_GetGroup(info);
  flag = SoundEffectInfo_GetPriority(info);
  m = p->owner;
  snd = m->handles;
  if (((M04Bank2 *) m->bank)->tab->n < pri)
  {
    return 0;
  }
  CAudioWorld__Lock(m);
  new_var = &(*((s16 *) (((u8 *) info) + 2)));
  if ((SoundEffectInfo_GetChance(info) == 0x64) || ((Random_NextB() % 100) < SoundEffectInfo_GetChance(info)))
  {
    idx = SndVoicePool_FindSlot(p, (s16) tag, pri, (s16) flag, SoundEffectInfo_GetFlags(info));
    if (idx != (-1))
    {
      v = (SndVoice *) (&p->voices[idx]);
      if (idx == 0x10)
      {
        v->sound = a3;
        v->id = p->owner->soundCounter++;
        v->unk8 = 0x63;
        goto blk16;
      }
      p->owner->pendingCue = idx;
      m2 = p->owner;
      new_var2 = &m2->player;
      core = ((u8 *) m2) + 0x7C;
      snd[idx] = alSndpAllocate(new_var2, ((M04Bank2 *) m2->bank)->tab->v[pri], m2);
      sv = snd[idx];
      if (sv == (-1))
      {
        v->id = sv;
        v->unk8 = sv;
        v->vp.priority = 0;
        p->owner->pendingCue = sv;
        fail:
        CAudioWorld__Unlock(p->owner);
        return 0;
      }
      f = ((f32) ((s16) flag)) * D_800A8160;
      (*((u16 *) (core + 0x80)))++;
      v->sound = a3;
      m3 = p->owner;
      v->id = m3->soundCounter++;
      v->unk8 = snd[idx];
      v->delay = *new_var;
      if (!(D_800A8164 <= f))
      {
        pr = (s32) f;
      }
      else
      {
        pr = (s32) (f - D_800A8164);
        pr = pr | 0x80000000;
      }
      alSndpSetPriority(new_var2, snd[idx], pr & 0xFF);
      blk16:
      v->soundId = p->owner->cueId;
      v->reset = p->owner->unk2BAC;
      v->hasTrack = 0;
      for (i = 0; i < 4; i++) { if (v->soundId == D_800B2A34[i]) { v->hasTrack = 1; break; } }
      v->unkC4 = 1;
      for (i = 0; i < 6; i++) { if (v->soundId == D_800B2A44[i]) { v->unkC4 = 0; break; } }
      v->unkB4 = p->owner->cueArg;
      v->stopping = 0;
      vp = &v->vp;
      v->pitchScale = p->owner->unk2BBC;
      vp->priority = flag;
      vp->vol = (vp->wC = 0x7D00);
      vp->pitch.i = 0;
      vp->fxMix = *((s8 *) (((u8 *) info) + 0xE));
      vp->pan = 0x40;
      vp->channel = pri;
      vp->s2C = tag;
      vp->frame = p->owner->frame;
      vp->doppler = D_800A8168;
      vp->distanceMode = SoundEffectInfo_GetPan(info);
      v->flags = SoundEffectInfo_GetFlags(info);
      vp->follow = 0;
      CAudioWorld__Unlock(p->owner);
      return v;
    }
  }
  goto fail;
}
