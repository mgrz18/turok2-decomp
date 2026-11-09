/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/code_802BAC64.c, func_802B5D70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern f32 _doModFunc(ALDelay *d, s32 count);
extern Acmd *_loadBuffer(ALFx *, s16 *, s32, s32, Acmd *);
Acmd *_loadOutputBuffer(ALFx *r, ALDelay *d, s32 buff, s32 incount, Acmd *p)
{
    extern s32 osVirtualToPhysical(sptr);
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
#ifdef TARGET_PORT /* u32 delays: see alFxPull */
        out_ptr = &r->input[-(s32)(d->output - d->rsdelta)];
#else
        out_ptr = &r->input[-(d->output - d->rsdelta)];
#endif
        ramalign = ((sptr)out_ptr & 0x7) >> 1;
        ptr = _loadBuffer(r, out_ptr - ramalign, rbuff, count + ramalign, ptr);
        ratio = (s32)(fratio * unity);
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(8) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(0) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(rbuff + (ramalign << 1)) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (((unsigned int)(buff) & (0xFFFFFFFFU >> (32 - (16)))) << (16)) | (((unsigned int)(incount << 1) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); };
        { Acmd *_a = (Acmd *)(ptr++); _a->words.w0 = (((unsigned int)(5) & (0xFFFFFFFFU >> (32 - (8)))) << (24)) | (((unsigned int)(d->rs->first) & (0xFFFFFFFFU >> (32 - (8)))) << (16)) | (((unsigned int)(ratio) & (0xFFFFFFFFU >> (32 - (16)))) << (0)); _a->words.w1 = (unsigned int)(osVirtualToPhysical((sptr)d->rs->state)); };
        d->rs->first = 0;
        d->rsdelta += count - incount;
    } else {
#ifdef TARGET_PORT
        out_ptr = &r->input[-(s32)d->output];
#else
        out_ptr = &r->input[-d->output];
#endif
        ptr = _loadBuffer(r, out_ptr, buff, incount, ptr);
    }
    return ptr;
}
Acmd *_loadBuffer(ALFx *r, s16 *curr_ptr, s32 buff, s32 count, Acmd *p);
f32 _doModFunc(ALDelay *d, s32 count);
