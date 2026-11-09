/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_12.c, func_802B57F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *_filterBuffer(void *arg0, s32 arg1, s32 arg2, void *arg3);
extern Acmd *_loadBuffer(ALFx *, s16 *, s32, s32, Acmd *);
extern void __assert(char *, char *, s32);
extern Acmd *_loadOutputBuffer(ALFx *, ALDelay *, s32, s32, Acmd *);
extern Acmd *_loadBuffer(ALFx *, s16 *, s32, s32, Acmd *);
extern Acmd *_saveBuffer(ALFx *, s16 *, s32, s32, Acmd *);
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
#ifdef TARGET_PORT /* the delays are u32: negated they wrap only with 32-bit pointers */
        in_ptr = &r->input[-(s32)d->input];
        out_ptr = &r->input[-(s32)d->output];
#else
        in_ptr = &r->input[-d->input];
        out_ptr = &r->input[-d->output];
#endif
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
