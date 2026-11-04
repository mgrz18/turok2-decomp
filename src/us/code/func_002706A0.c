#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00257814.c, func_00257814.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/sound.h"
extern f32 D_800A80C8;
void func_00293DE0(void *p, s32 pitch);
void func_00293D40(void *p, s32 mix);
void func_00293D90(void *p, u8 pan);
void func_00293E70(void *p, s16 vol);
void func_002764C4(void *q, s32 a, s32 id);
s32 func_00273B90(void *pool, s16 idx, s32 id);
void func_00273BF8(void *pool, s16 idx);
static __inline__ s32 find17(SndMgr *m, s32 id) {
    s32 i;
    SndVoice *vv;
    for (vv = m->pool.voices, i = 0; i < 17; i++, vv++) {
        if (vv->id == id) {
            return i;
        }
    }
    return -1;
}
s32 func_002706A0(SndMgr *m, s32 id)
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
    func_002764C4(&m->queue, 16, id);
    return -1;
  }
  p = (SndVoiceParams *) ((u8 *)((((s16) idx) * 0xCC) + (u32) m) + 0x1DCC);
  pl = m->player;
  if (p->vol == 0)
  {
    if (!((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60))) & 4))
    {
      *((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60)) |= 8;
      m->pendingCue = -1;
      return 0;
    }
  }
  if (func_00273B90(&m->pool, idx, id) == 0)
  {
    func_00293DE0(pl, p->pitch.i);
    mix = ((p->fxMix + m->fxMix) < 0x80) ? ((((u8 *) p)[0x11] + (*(((u8 *) m) + 0x2B9F))) & 0xFF) : (0x7F);
    func_00293D40(pl, mix);
    func_00293D90(pl, ((u8 *) (&p->pan))[1]);
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
          f *= D_800A80C8;
        }
        else
        {
          f *= D_800A80C8;
        }
      }
    }
    func_00293E70(pl, (s16) ((s32) ((((f32) p->vol) * f) * p->scale)));
    if ((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E5C))) == 0)
    {
      func_00273BF8(&m->pool, idx);
    }
  }
  m->pendingCue = -1;
  return 0;
}
