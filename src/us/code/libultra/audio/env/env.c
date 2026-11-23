/* FAKEMATCH (round 21 audit): 1 register pin (`register ... asm("$n")`); without them: alEnvmixerPull 6 of
 * 339 words. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/alEnvmixerPull.c, alEnvmixerPull (libaudio env.c), with Turok 2's layouts.
 * Kept from that source: `one`/`eq` set at the top of the loop for the ROM's hoisting order, and the
 * HACK `register ALParam *pp asm("$2")` in the type 11 arm (ROM keeps the pointer in v0). */
#include "common.h"
#include "structs/libaudio.h"
#include "globals/libaudio.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B4C7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/strings.h"
#include "protos/ultra.h"
/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: _getRate: 1 of 148 words. */
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B4F68_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B450C_de_9.c, func_802B51B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "common.h"
#include "m2c_macros.h"

void __assert(const char *exp, const char *file, s32 line); /* __assert */
#define assert_ln(EX, ln) ((EX) ? ((void)0) : __assert("EX", "audio/env.c", ln))

void _freePVoice(ALSynth *drvr, PVoice *pvoice); /* _freePVoice */
void __freeParam(ALParam *param); /* __freeParam */
Acmd *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p);
f32 _getVol(f32 value, s32 shift, s32 whole, s32 frac);

void *alEnvmixerPull(void *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p) {
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    s16 inp;
    s16 loutp;
    s32 delta;
    s32 cur = sampleOffset;
    s32 old;
    ALParam *tmp;
    s16 *eq;
    s32 one;
    loutp = 0;
    inp = 0;
    if (e->ctrlList != 0) {
        while (e->ctrlList != 0) {
            one = 1;
            eq = eqpower;
            old = cur;
            cur = e->ctrlList->delta;
            delta = cur - old;
            if (delta > outCount) break;
            assert_ln(delta >= 0, 0x67);
            assert_ln(delta <= 0xA0, 0x68);
            switch (e->ctrlList->type) {
            case 13: {
                ALStartParamAlt *s1 = (ALStartParamAlt *)e->ctrlList;
                ALFilter *f = (ALFilter *)e;
                if (s1->unity) {
                    (*f->setParam)(f, 8, 0);
                }
                (*f->setParam)(f, 5, s1->wave);
                (*f->setParam)(f, 9, 0);
                e->first = one;
                e->delta = 0;
                e->segEnd = s1->samples;
                e->volume = (s1->volume * s1->volume) >> 15;
                e->pan = s1->pan;
                e->dryamt = eq[s1->fxMix];
                e->wetamt = eq[0x7F - s1->fxMix];
                if (s1->samples != 0) {
                    e->cvolL = one;
                    e->cvolR = one;
                } else {
                    e->cvolL = (e->volume * eq[e->pan]) >> 15;
                    e->cvolR = (e->volume * eq[0x7F - e->pan]) >> 15;
                }
                if (f->source) {
                    (*f->source->setParam)(f->source, 7, (void *) *(s32 *) &s1->pitch);
                }
                break; }
            case 11:
            case 12:
            case 16:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                if (e->delta >= e->segEnd) {
                    e->ltgt = (e->volume * eq[e->pan]) >> 15;
                    e->rtgt = (e->volume * eq[0x7F - e->pan]) >> 15;
                    e->cvolL = e->ltgt;
                    e->cvolR = e->rtgt;
                    e->delta = e->segEnd;
                } else {
                    e->cvolL = _getVol((f32)e->cvolL, e->delta, e->lratm, e->lratl);
                    e->cvolR = _getVol((f32)e->cvolR, e->delta, e->rratm, e->rratl);
                }
                if (e->cvolL == 0) e->cvolL = one;
                if (e->cvolR == 0) e->cvolR = one;
                if (e->ctrlList->type == 12) {
                    e->pan = e->ctrlList->data.i;
                }
                if (e->ctrlList->type == 11) {
                    register ALParam *pp __asm__("$2");
                    e->delta = 0;
                    pp = e->ctrlList;
                    e->volume = (pp->data.i * pp->data.i) >> 15;
                    e->segEnd = pp->moredata.i;
                }
                if (e->ctrlList->type == 16) {
                    e->dryamt = eq[e->ctrlList->data.i];
                    e->wetamt = eq[0x7F - e->ctrlList->data.i];
                }
                e->first = one;
                break;
            case 14: {
                ALStartParam *s1 = (ALStartParam *)e->ctrlList;
                if (s1->unity) {
                    (*e->filter.setParam)(e, 8, 0);
                }
                (*e->filter.setParam)(e, 5, s1->wave);
                (*e->filter.setParam)(e, 9, 0);
                break; }
            case 15:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, 4, 0);
                break;
            case 0: {
                ALSynth *drvr = &alGlobals->drvr;
                ALFreeParam *pp = (ALFreeParam *)e->ctrlList;
                pp->pvoice->offset = 0;
                _freePVoice(drvr, pp->pvoice);
                break; }
            default:
                ptr = _pullSubFrame(e, &inp, &loutp, delta, sampleOffset, ptr);
                (*e->filter.setParam)(e, e->ctrlList->type, (void *) e->ctrlList->data.i);
                break;
            }
            outCount -= delta;
            loutp += delta << 1;
            tmp = e->ctrlList;
            e->ctrlList = tmp->next;
            if (e->ctrlList == 0) {
                e->ctrlTail = 0;
            }
            __freeParam(tmp);
        }
    }
    ptr = _pullSubFrame(e, &inp, &loutp, outCount, sampleOffset, ptr);
    if (e->delta > e->segEnd) {
        e->delta = e->segEnd;
    }
    return ptr;
}
s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel);
extern void __assert(const char *, const char *, s32);

Acmd *_pullSubFrame(void *filter, s16 *inp, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    ALFilter *source = e->filter.source;
    if (e->motion != 1 || !outCount)
        return ptr;
    ((source) ? ((void)0) : __assert(sEnvAssertExpr, sEnvAssertFile, 366));
    ptr = (*source->handler)(source, inp, outCount, sampleOffset, p);
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(*inp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1088 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(1408 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1728 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(2048 + *outp) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    if (e->first) {
        e->first = 0;
        e->ltgt = (e->volume * eqpower[e->pan]) >> 15;
        e->lratm = _getRate((f64)e->cvolL, (f64)e->ltgt, e->segEnd, &(e->lratl));
        e->rtgt = (e->volume * eqpower[128 - e->pan - 1]) >> 15;
        e->rratm = _getRate((f64)e->cvolR, (f64)e->rtgt, e->segEnd, &(e->rratl));
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x02 | 0x04) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->cvolL) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x04) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->cvolR) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x02 | 0x00) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->ltgt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(e->lratm) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->lratl) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x00) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->rtgt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(e->rratm) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->rratl) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(9) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x08) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->dryamt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(e->wetamt) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x01 | 0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(e->state)); };
    }
    else
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(3) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0x00 | 0x08) & (0xFFFFFFFFU >> (32 - (8)))) << (16)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(e->state)); };
    *inp += (outCount << 1);
    e->delta += outCount;
    return ptr;
}

s16 _getRate(f64 vol, f64 tgt, s32 count, u16 *ratel)
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

s32 alEnvmixerParam(void *filter, s32 paramID, void *param)
{
    ALFilter *f = (ALFilter *)filter;
    ALEnvMixer *e = (ALEnvMixer *)filter;
    switch (paramID) {
      case (3):
          if (e->ctrlTail) {
              e->ctrlTail->next = (ALParam *)param;
          } else {
              e->ctrlList = (ALParam *)param;
          }
          e->ctrlTail = (ALParam *)param;
          break;
      case (4):
          e->first = 1;
          e->motion = 0;
          e->volume = 1;
          if (f->source)
              (*f->source->setParam)(f->source, 4, param);
          break;
      case (9):
          e->motion = 1;
          if (f->source)
              (*f->source->setParam)(f->source, 9, param);
          break;
      case (1):
          f->source = (ALFilter *)param;
          break;
      default:
          if (f->source)
              (*f->source->setParam)(f->source, paramID, param);
    }
    return 0;
}

f32 _getVol(f32 value, s32 shift, s32 whole, s32 frac) {
    s32 n = shift >> 3;
    s32 i = 0;
    f32 base;
    f32 result;

    if (n == 0) {
        return value;
    }
    base = ((f32)(whole << 16) + (f32)(frac & 0xFFFF)) * 1.5258789e-5f;
    result = 1.0f;
    do {
        if (n & 1) {
            result *= base;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        i++;
        base *= base;
    } while (i < 32);
    return value * result;
}

extern f64 fabs(f64);

/* frexp(): splits x into a mantissa in [0.5, 1) and a power of two in *exp. */
f64 _frexpf(f64 x, s32 *exp) {
    f64 m;

    *exp = 0;
    if (x == 0.0) {
        return x;
    }
    m = fabs(x);
    while (m >= 1.0) {
        m *= 0.5;
        (*exp)++;
    }
    while (m < 0.5) {
        m += m;
        (*exp)--;
    }
    if (!(x > 0.0)) {
        m = -m;
    }
    return m;
}

f64 _ldexpf(f64 arg0, s32 arg2) {
    f64 var_f12;

    var_f12 = arg0;
    if (arg2 != 0) {
        var_f12 *= (f64) (1 << arg2);
    }
    return var_f12;
}
