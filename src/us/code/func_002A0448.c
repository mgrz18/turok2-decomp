/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecDecodeFrame, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"
#include "globals/adec.h"

/* Acclaim compressed-audio stream decoder state (one instance) */
typedef struct {
    /* 0x00 */ u32 hdr0;
    /* 0x04 */ u32 hdr2;
    /* 0x08 */ u32 *hdr;
    /* 0x0C */ u32 nframes;
    /* 0x10 */ u32 remaining;
    /* 0x14 */ u16 *rd;         /* bitstream read pointer */
    /* 0x18 */ s16 band[16];    /* per-band scale indices of the current frame */
    /* 0x38 */ u8 pad38[0xB8 - 0x38];
    /* 0xB8 */ s16 coef[256];   /* quantized coefficients of the current frame */
} AdecStream;

extern u16 sAdecPrevScale[16];      /* previous scale indices */

extern s32 adecReadBits(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st);          /* adecReadBits */
extern s16 adecHuffDecode(AdecHuffSeg *segs, s16 *out, s16 n, s16 *cnt, AdecStream *st);    /* adecHuffDecode */

/* Decodes one frame into st: the 16 scale indices st->band[] (the very first frame raw 6 bit fields, afterwards Huffman coded
   differences against the previous frame) and the 256 coefficients st->coef[]. Returns 1, or -1 on a stream error. */
s32 adecDecodeFrame(AdecStream *st) {
    s16 cnt;
    s16 nv;
    s16 i;
    s32 rv;
    AdecHuffSeg *seg = gAdecHuffSegments;
    if (sAdecHeaderDone == 0) {
        sAdecHeaderDone = 1;
        if ((s16)adecReadBits((u16 *)st->band, 6, 16, 0, st) <= 0) {
            return -1;
        }
        seg->pos += 0x60;
        for (i = 0; i < 16; i++) {
            sAdecPrevScale[i] = st->band[i];
        }
        while (seg->kind != 0) {
            seg++;
        }
    } else {
        rv = adecHuffDecode(seg, st->band, 16, &cnt, st);
        if (rv != 16) {
            goto fail;
        }
        seg = &seg[nv = cnt];
        for (i = 0; i < 16; i++) {
            nv = sAdecPrevScale[i];
            st->band[i] += nv;
            sAdecPrevScale[i] = st->band[i];
        }
    }
    rv = adecHuffDecode(seg, st->coef, 256, &cnt, st);
    if (rv != 256) {
    fail:
        return -1;
    }
    return 1;
}
