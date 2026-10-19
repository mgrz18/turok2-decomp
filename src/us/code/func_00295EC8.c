/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B4F68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s16 func_00295EC8(f64 vol, f64 tgt, s32 count, u16 *ratel);
s16 func_00295EC8(f64 vol, f64 tgt, s32 count, u16 *ratel)
{
  s16 s;
  f64 invn = (1.0f) / ((f32) count);
  f64 eps;
  f64 a;
  f64 fs;
  f64 new_var;
  f64 mant;
  s32 i_invn;
  s32 ex;
  s32 indx;
  if (count == 0)
  {
    if (tgt >= vol)
    {
      *ratel = 0xffff;
      return 0x7fff;
    }
    else
    {
      *ratel = 0;
      return 0;
    }
  }
  if (tgt < (1.0))
  {
    tgt = (1.0);
  }
  if (vol <= (0.0))
  {
    vol = (1.0);
  }
  {
    LogTab logtab = { { -0.91253697872161865, -0.75207197666168213, -0.60768300294876099, -0.47643798589706421, -0.3561440110206604, -0.24511200189590454, -0.14201900362968445, -4.580400139093399e-2 } };
    f64 absvalue;
    s32 *eptr;
    i_invn = (s32) (invn * (1073741824.0));
    tgt = tgt / vol;
    eptr = &ex;
    *eptr = 0;
    if (tgt == (0.0))
    {
      vol = tgt;
    }
    else
    {
      absvalue = __builtin_fabs(tgt);
      if (absvalue >= (1.0))
      {
        do
        {
          absvalue *= (0.5);
          ++(*eptr);
        }
        while (absvalue >= (1.0));
      }
      if (absvalue < (0.5))
      {
        do
        {
          absvalue += absvalue;
          --(*eptr);
        }
        while (absvalue < (0.5));
      }
      vol = absvalue;
      if (!(tgt > (0.0)))
      {
        vol = -vol;
      }
    }
    indx = (s32) (vol * (16.0));
    absvalue = (0.69314718246459961);
    eps = ((new_var = logtab.v[indx - 8]) + ex) * absvalue;
    mant = (1073741824.0); eps /= mant; fs = eps + (1.0); a = (1.0);
    while (i_invn)
    {
      if (i_invn & 1)
      {
        a = a * fs;
      }
      fs *= fs;
      i_invn >>= 1;
    }
  }
  if (vol)
  {
    a *= (a *= (a *= a));
    s = (s16) a;
    *ratel = (s16) ((a - ((f32) s)) * (65535.0));
    return (s16) a;
  }
  else
  {
    a *= (a *= (a *= a));
    s = (s16) a;
    *ratel = (s16) ((a - ((f32) s)) * (65535.0));
    return (s16) a;
  }
}
