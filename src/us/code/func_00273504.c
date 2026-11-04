/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0025B1E8.c, func_0025B1E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/sound.h"
#include "rw/junk_types.h" /* M04Bank2 */

extern f32 D_800A8160;
extern f32 D_800A8164;
extern f32 D_800A8168;
extern s32 D_800B2A34[];
extern s32 D_800B2A44[];
s16 func_00293470(void *p, s32 a, void *owner);
void func_00293E30(void *p, s16 id, s32 pri);
void func_002714B0(SndMgr *m);
void func_00271514(SndMgr *m);
s16 func_00273370(SndVoicePool *p, s16 a, s16 b, s16 c, s32 d);
s16 func_00276820(SoundEffectInfo *i);
s8 func_00276838(SoundEffectInfo *i);
s8 func_00276844(SoundEffectInfo *i);
s8 func_00276868(SoundEffectInfo *i);
s16 func_00276898(SoundEffectInfo *i);
s32 func_00211420();
SndVoice *func_00273504(SndVoicePool *p, SoundEffectInfo *info, int tag, s32 a3)
{
  s16 *snd;
  SndMgr *m3;
  SndMgr *m;
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
  SndMgr *m2;
  s16 *new_var;
  s32 r;
  SndVoice *v;
  pri = func_00276820(info);
  flag = func_00276838(info);
  m = p->owner;
  snd = m->handles;
  if (((M04Bank2 *) m->bank)->tab->n < pri)
  {
    return 0;
  }
  func_002714B0(m);
  new_var = &(*((s16 *) (((u8 *) info) + 2)));
  if ((func_00276844(info) == 0x64) || ((func_00211420() % 100) < func_00276844(info)))
  {
    idx = func_00273370(p, (s16) tag, pri, (s16) flag, func_00276898(info));
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
      new_var2 = m2->player;
      core = ((u8 *) m2) + 0x7C;
      snd[idx] = func_00293470(new_var2, ((M04Bank2 *) m2->bank)->tab->v[pri], m2);
      sv = snd[idx];
      if (sv == (-1))
      {
        v->id = sv;
        v->unk8 = sv;
        v->vp.priority = 0;
        p->owner->pendingCue = sv;
        fail:
        func_00271514(p->owner);
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
      func_00293E30(new_var2, snd[idx], pr & 0xFF);
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
      vp->distanceMode = func_00276868(info);
      v->flags = func_00276898(info);
      vp->follow = 0;
      func_00271514(p->owner);
      return v;
    }
  }
  goto fail;
}
