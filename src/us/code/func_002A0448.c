/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/libultra/adecReadBits.c, adecDecodeFrame, with Turok 2's layouts. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -fforce-addr */

#include "common.h"

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

/* one entry of the Huffman segment table, stride 0x14 */
typedef struct {
    /* 0x00 */ s32 kind;        /* 0 terminates the table */
    /* 0x04 */ s32 start;
    /* 0x08 */ s32 len;
    /* 0x0C */ s32 pos;
    /* 0x10 */ void *tree;
} AdecHuffSeg;

extern s16 D_800B8BAE;          /* first frame (raw header) done */
extern AdecHuffSeg D_800B8C54[]; /* segment table */
extern u16 D_80125010[16];      /* previous scale indices */

extern s32 func_0029FE70(u16 *dst, u16 nbits, u16 count, s16 sign, AdecStream *st);          /* adecReadBits */
extern s16 func_002A0044(AdecHuffSeg *segs, s16 *out, s16 n, s16 *cnt, AdecStream *st);    /* adecHuffDecode */

/* Decodes one frame into st: the 16 scale indices st->band[] (the very first frame raw 6 bit fields, afterwards Huffman coded
   differences against the previous frame) and the 256 coefficients st->coef[]. Returns 1, or -1 on a stream error. */
s32 func_002A0448(AdecStream *st) {
    s16 cnt;
    s16 nv;
    s16 i;
    s32 rv;
    AdecHuffSeg *seg = D_800B8C54;
    if (D_800B8BAE == 0) {
        D_800B8BAE = 1;
        if ((s16)func_0029FE70((u16 *)st->band, 6, 16, 0, st) <= 0) {
            return -1;
        }
        seg->pos += 0x60;
        for (i = 0; i < 16; i++) {
            D_80125010[i] = st->band[i];
        }
        while (seg->kind != 0) {
            seg++;
        }
    } else {
        rv = func_002A0044(seg, st->band, 16, &cnt, st);
        if (rv != 16) {
            goto fail;
        }
        seg = &seg[nv = cnt];
        for (i = 0; i < 16; i++) {
            nv = D_80125010[i];
            st->band[i] += nv;
            D_80125010[i] = st->band[i];
        }
    }
    rv = func_002A0044(seg, st->coef, 256, &cnt, st);
    if (rv != 256) {
    fail:
        return -1;
    }
    return 1;
}
