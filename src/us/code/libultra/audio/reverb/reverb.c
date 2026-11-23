/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B57F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3);
Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
extern void __assert(char *, char *, s32);
Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p);
Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);

Acmd *alFxPull(ALFx *filter, s16 *outp, s32 outCount, s32 sampleOffset, Acmd *p)
{
    Acmd *ptr = p;
    ALFx *r = filter;
    ALFilter *source = (ALFilter *)r->filter.source;
    s16 i, buff1, buff2, input, output;
    s16 *in_ptr, *out_ptr, gain, *prev_out_ptr = 0;
    ALDelay *d, *pd;
    if (!source)
        __assert("EX", "audio/reverb.c", 75);
    ptr = (*source->handler)(source, outp, outCount, sampleOffset, p);
    input = 1728;
    output = 2048;
    buff1 = 0;
    buff2 = 320;
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0xda83) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(1728) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(input) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0x5a82) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(2048) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(input) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    ptr = _saveBuffer(r, r->input, input, outCount, ptr);
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(2) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (unsigned int)(outCount << 1); };
    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        in_ptr = &r->input[-(s32)d->input];
        out_ptr = &r->input[-(s32)d->output];
        if (in_ptr == prev_out_ptr) {
            { s16 t = buff2; buff2 = buff1; buff1 = t; };
        } else {
            ptr = _loadBuffer(r, in_ptr, buff1, outCount, ptr);
        }
        ptr = _loadOutputBuffer(r, d, buff2, outCount, ptr);
        if (d->ffcoef) {
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->ffcoef) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff1) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            if (!d->rs && !d->lp) {
                ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
            }
        }
        if (d->fbcoef) {
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->fbcoef) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(buff1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
            ptr = _saveBuffer(r, in_ptr, buff1, outCount, ptr);
        }
        if (d->lp)
            ptr = _filterBuffer(d->lp, buff2, outCount, ptr);
        if (!d->rs)
            ptr = _saveBuffer(r, out_ptr, buff2, outCount, ptr);
        if (d->gain)
            { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(12) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)((u16)d->gain) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff2) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        prev_out_ptr = &r->input[d->output];
    }
    r->input += outCount;
    if (r->input > &r->base[r->length])
        r->input -= r->length;
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(10) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(output) & (0xFFFFFFFFU >> (32 - (24)))) << (0)); _a->words.w1 = (((unsigned int)(1728) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(outCount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    return ptr;
}

#include "globals/libaudio.h"

extern void init_lpfilter(ALLowPass *lp);

s32 alFxParamHdl(ALFx *filter, s32 paramID, void *param)
{
    ALFx *f = filter;
    s32 p = (paramID - 2) % 8;
    s32 s = (paramID - 2) / 8;
    s32 val = *(s32 *)param;
    switch (p) {
        case 0:
            f->delay[s].input = (u32)val & 0xFFFFFFF8;
            break;
        case 1:
            f->delay[s].output = (u32)val & 0xFFFFFFF8;
            break;
        case 3:
            f->delay[s].ffcoef = (s16)val;
            break;
        case 2:
            f->delay[s].fbcoef = (s16)val;
            break;
        case 4:
            f->delay[s].gain = (s16)val;
            break;
        case 5:
            f->delay[s].rsinc = (((f32)val * 0.001f) * 2.0f) / alGlobals->drvr.outputRate;
            break;
        case 6: {
            f32 fval = (f32)val;
            s32 length = (f->delay[s].output - f->delay[s].input);
            double flength = length;
            if (length < 0)
                flength += 4294967296.0;
            f->delay[s].rsgain = fval * ((f32)flength * 5.7762263e-6f);
            break;
        }
        case 7:
            if (f->delay[s].lp) {
                f->delay[s].lp->fc = (s16)val;
                init_lpfilter(f->delay[s].lp);
            }
            break;
    }
    return 0;
}
f32 _doModFunc(ALDelay *d, s32 count);

Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p)
{
    extern u32 osVirtualToPhysical(void *);
    Acmd *ptr = p;
    s32 ratio, count, rbuff = 640;
    s16 *out_ptr;
    f32 fincount, fratio, delta;
    s32 ramalign = 0, length;
    f32 unity;
    if (d->rs) {
        length = d->output - d->input;
        delta = _doModFunc(d, incount);
        delta /= length;
        unity = 32768.0f;
        delta = (s32)(delta * unity);
        delta = delta * 3.0517578e-5f;
        fratio = 1.0f - delta;
        fincount = d->rs->delta + (fratio * (f32)incount);
        count = (s32)fincount;
        d->rs->delta = fincount - (f32)count;
        out_ptr = &r->input[-(s32)(d->output - d->rsdelta)];
        ramalign = ((sptr)out_ptr & 0x7) >> 1;
        ptr = _loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
        ratio = (s32)(fratio * unity);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(rbuff + (ramalign << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(incount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(d->rs->first) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(ratio) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(d->rs->state)); };
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
        out_ptr = &r->input[-(s32)d->output];
        ptr = _loadBuffer(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}

#include "protos/ultra.h"

Acmd *_saveBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    extern u32 osVirtualToPhysical(void *);
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;
    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(before_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(curr_ptr)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff + (before_end << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(after_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(r->base)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    } else {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(6) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(curr_ptr)); };
    }
    return ptr;
}

#include "common.h"
#include "m2c_macros.h"

s32 alFxParam(sptr *arg0, s32 arg1, sptr arg2) {
    if (arg1 == 1) {
        *arg0 = arg2;
    }
    return 0;
}

Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p)
{
    extern u32 osVirtualToPhysical(void *);
    Acmd *ptr = p;
    s32 after_end, before_end;
    s16 *updated_ptr, *delay_end;
    delay_end = &r->base[r->length];
    if (curr_ptr < r->base)
        curr_ptr += r->length;
    updated_ptr = curr_ptr + count;
    if (updated_ptr > delay_end) {
        after_end = updated_ptr - delay_end;
        before_end = delay_end - curr_ptr;
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(before_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(curr_ptr)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff + (before_end << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(after_end << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(r->base)); };
    } else {
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(4) & (0xFFFFFFFFU >> (32 - (8)))) << (24)); _a->words.w1 = (unsigned int)(osVirtualToPhysical(curr_ptr)); };
    }
    { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(count << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
    return ptr;
}

#ifdef TARGET_PORT
#include "structs/libaudio.h"
/* host: the low-pass filter's fields at the host offsets (fstate is a pointer at 0x28, first at 0x30) */
void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3)
{
  ALLowPass *lp = arg0;
  u32 *cmd = arg3;
  s32 buf = arg1 & 0xFFFF;
  cmd[0] = buf | 0x08000000;
  cmd[1] = (buf << 0x10) | ((arg2 * 2) & 0xFFFF);
  cmd[2] = 0x0B000020;
  cmd[3] = osVirtualToPhysical(&lp->fcvec);
  cmd[4] = ((lp->first & 0xFF) << 0x10) | (u16) lp->fgain | 0x0E000000;
  cmd[5] = osVirtualToPhysical(lp->fstate);
  lp->first = 0;
  return cmd + 6;
}
#else
void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3)
{
  s32 temp_a1;
  void *var_s0;
  int new_var2;
  void *var_s1;
  s8 *new_var;
  void *var_s2;
  new_var2 = 0x0E000000;
  var_s2 = arg0;
  var_s0 = arg3;
  temp_a1 = arg1 & 0xFFFF;
  *((s32 *) (((s8 *) var_s0) + 0)) = (s32) (temp_a1 | 0x08000000);
  *((s32 *) (((s8 *) var_s0) + 4)) = (s32) ((temp_a1 << 0x10) | ((arg2 * 2) & 0xFFFF));
  *((s32 *) (((s8 *) var_s0) + 8)) = 0x0B000020;
  *((s32 *) (((s8 *) var_s0) + 0xC)) = osVirtualToPhysical(var_s2 + 8);
  *((s32 *) (((s8 *) var_s0) + 0x10)) = (s32) (((*((u8 *) (((s8 *) var_s2) + 0x2F))) << 0x10) | ((*((u16 *) (((s8 *) var_s2) + 2))) | new_var2));
  var_s1 = var_s0 + 0x18;
  *((s32 *) (((s8 *) var_s0) + 0x14)) = osVirtualToPhysical(*((s32 *) (((s8 *) var_s2) + 0x28)));
  *((s32 *) ((new_var = (s8 *) var_s2) + 0x2C)) = 0;
  return var_s1;
}
#endif

f32 _doModFunc(ALDelay *d, s32 count)
{
    f32 val;
    d->rsval += d->rsinc * count;
    d->rsval = (d->rsval > 2.0f) ? d->rsval - (4.0f) : d->rsval;
    val = d->rsval;
    val = (val < 0) ? -val : val;
    val -= 1.0f;
    return d->rsgain * val;
}
