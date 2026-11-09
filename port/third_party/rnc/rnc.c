/*
 * rnc.c - readable C reconstruction of the hand-written Rob Northen / Propack "RNC" decompressor
 *         of Turok: Rage Wars (src/libultra/rnc*.s, VRAM 0x002B2590-0x002B3100).
 *
 * NOT a matching decompilation: gcc 2.8.1 cannot reproduce the original bytes (hand scheduled delay
 * slots, registers used as a global ABI between the routines, deliberate jumps into the middle of
 * other routines).  This file is *functionally equivalent* C, verified by differential testing
 * against the original machine code (see README.md and test/).
 *
 * Original unit            -> where it lives in this file
 * ------------------------    ---------------------------------------------------------------
 * rncUnpackMethod1          -> rncUnpackMethod1()     (register-saving wrapper in the original)
 * rncUnpackMethod1Body      -> rnc1_unpack()          (the actual method 1 decoder)
 * rncReadHuffCode           -> rnc1_read_huff()
 * rncInputBits              -> rnc1_input_bits()
 * rncRefillBitBuf           -> rnc1_refill()
 * rncMakeHuffTable          -> rnc1_make_huff_table()
 * rncUnpackMethod2          -> rncUnpackMethod2()     (wrapper that saves a0-a3/t0-t7/s0-s3/ra)
 * rncUnpackMethod2Body      -> rnc2_unpack()
 *
 * RNC file layout (18 byte header, then the packed stream):
 *   +0  "RNC" + method (1 or 2)
 *   +4  unpacked size  (u32, big endian)
 *   +8  packed size    (u32, big endian; size of the stream *without* the header)
 *   +C  crc16 of the unpacked data (poly 0xA001)    -- not checked by the game's decoders
 *   +E  crc16 of the packed data                    -- not checked by the game's decoders
 *   +10 leeway byte, +11 number of packed chunks    -- not used
 *   +12 packed stream
 * The game (func_00285150) calls  rncUnpackMethod1(hdr + 0x12, dst, packed, unpacked)  for method 1
 * and  rncUnpackMethod2(hdr, dst, packed, unpacked)  for method 2 (method 2 parses the header itself
 * and ignores the packed/unpacked arguments).
 *
 * Deliberate differences from the original (all only matter for corrupt streams):
 *   - Every input read, output write and back reference is bounds checked; on a violation the
 *     decoder stops and reports failure.  The original has no checks and would scribble over memory.
 *   - Huffman tables with more than 16 codes (impossible in a valid stream) abort; the original
 *     overruns its 16 byte "length" scratch area and smashes its caller's saved $ra.
 *   - $v0 handling: the original returns accidental register contents from method 1; we return the
 *     same value (rnc_unpack_method1 documents it) so the differential test can compare it.
 */
#include <stdint.h>
#include <stddef.h>
#include <string.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t  s32;

/* ------------------------------------------------------------------------------------------
 * Globals the original decoder writes (method 2 only)
 * ------------------------------------------------------------------------------------------ */

/* D_8014D420: method 2 stores the unpacked size from the header here.  Nothing in the game
 * reads it back (it is a leftover of the original Propack library's "last unpacked size"). */
u32 rnc_last_unpacked_size;

/* D_8014D41E: halfword just below it.  Method 2 clears it and, after the end marker, reads it as
 * the number of "tail" halfwords to append to the output (a feature of the original library whose
 * data would have to be put after D_8014D420).  Because the decoder itself zeroes the count right
 * before, the tail is always empty: dead code in the original.  Kept as a variable for documentation. */
u16 rnc_tail_count;

/* How far past the end of the packed stream the decoders may read (the 16-bit window of method 1
 * prefetches).  The differential test uses the same value for the original's memory sandbox. */
#define RNC_IN_SLACK 16

/* ------------------------------------------------------------------------------------------
 * Common helpers
 * ------------------------------------------------------------------------------------------ */

static u32 be32(const u8 *p) { return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3]; }
static u32 be16(const u8 *p) { return ((u32)p[0] << 8) | p[1]; }
static void put_be16(u8 *p, u32 v) { p[0] = (u8)(v >> 8); p[1] = (u8)v; }

/* ==========================================================================================
 * METHOD 1  -  Huffman + LZ77, 16 bit little-endian bit window
 * ==========================================================================================
 *
 * Stream structure (after an initial skip of 2 bits):
 *   repeat while input remains            ("chunk")
 *       3 Huffman tables:  raw-length table, distance table, match-length table
 *       16 bit count N of (match) sub-blocks in this chunk  (stored as N, used as N - 1)
 *       raw-length, then N times { distance, match length, raw-length }
 *   A raw-length of L means: L literal bytes follow *in the byte stream* (not in the bit stream).
 *
 * The bit stream is read from 16 bit little endian words that are interleaved with the literal
 * bytes: the pointer `p` always addresses the next word that has not yet been pulled into the
 * window, so literal bytes are taken straight from `p`, after which the window is re-primed.
 */

/* The register ABI shared by rncInputBits / rncRefillBitBuf / rncReadHuffCode / rncMakeHuffTable
 * and rncUnpackMethod1Body, turned into explicit state:
 *     $t6 = window     bit window, next bit = bit 0.  Holds the unread bits of the current
 *                      16 bit word in its low part and the *next* word above them.
 *     $t7 = left       number of unread bits remaining in the current 16 bit word (0..16);
 *                      the window therefore holds `left + 16` valid bits.
 *     $s3 = p          input pointer: the next 16 bit word to be loaded (also the literal source)
 *     $v0              last byte loaded << 24 (left over by the refill, returned by the unpacker)
 */
typedef struct {
    u32 window;       /* $t6 */
    s32 left;         /* $t7 */
    const u8 *p;      /* $s3 */
    u32 v0;           /* $v0 residue */
    const u8 *in_lim; /* read limit (in_end + RNC_IN_SLACK), hardening only */
    int bad;          /* set when a bounds check failed */
} Rnc1Bits;

/* rncRefillBitBuf: called when `n` bits are requested but only `left` < n remain in the current
 * word.  The caller has already subtracted n from `left` (hence the add back).  Drops the rest of
 * the current word, pulls the next 16 bit word into the top half and returns the number of bits
 * that still have to be skipped in the (new) current word. */
static s32 rnc1_refill(Rnc1Bits *b, s32 n)
{
    b->left += n;                                    /* undo the caller's subtraction */
    b->window >>= (b->left & 31);                    /* discard the rest of the old word */
    if (b->p + 4 > b->in_lim) { b->bad = 1; return 0; }
    b->v0 = (u32)b->p[3] << 24;
    b->window |= ((u32)b->p[3] << 24) | ((u32)b->p[2] << 16);   /* word at p+2 -> top half */
    b->p += 2;
    n -= b->left;                                    /* bits of the request that fall into the new word */
    b->left = 16 - n;
    return n;
}

/* rncInputBits: returns window & mask, then consumes n bits (refilling when the current word runs
 * out).  `mask` is passed separately because the original callers use masks that are not simply
 * (1 << n) - 1 (the initial 2 bit skip passes mask 2, the chunk count passes 0xFFFFFFFF). */
static u32 rnc1_input_bits(Rnc1Bits *b, u32 mask, s32 n)
{
    u32 r = b->window & mask;
    b->left -= n;
    if (b->left < 0)
        n = rnc1_refill(b, n);
    b->window >>= (n & 31);
    return r;
}

/* Huffman table layout (0x80 bytes, 16 entries, big endian halfwords):
 *   +0x00 + 4*i : u16 mask, u16 code            the code is stored bit-reversed (LSB first)
 *   +0x40 + 4*i : u8 code_length, u8 index, u16 extra_mask
 * `index` is the symbol number: 0 and 1 are literal values 0/1, index >= 2 means
 * "(1 << (index - 1)) plus (index - 1) extra bits" (Elias-gamma like), `extra_mask` =
 * (1 << (index - 1)) - 1 selects those extra bits.
 */
#define HUFF_ENTRIES 16
#define HUFF_SIZE    0x80

/* rncMakeHuffTable: read one table from the bit stream.
 *   5 bits : number of symbols n (0..16)
 *   n * 4 bit code lengths (0 = unused symbol)
 * Canonical codes are assigned shortest first; for equal lengths in symbol order. */
static void rnc1_make_huff_table(Rnc1Bits *b, u8 *tab)
{
    u8 lens[32];
    u32 n = rnc1_input_bits(b, 0x1F, 5);
    u32 code = 0;
    u32 inc = 0x80000000u;      /* $a1: code increment for length 1, halved per length */
    u32 len, i;
    u8 *out = tab;

    if (n == 0)
        return;                 /* table left untouched (the original does the same) */
    if (n > HUFF_ENTRIES) {     /* original: smashes the stack */
        b->bad = 1;
        return;
    }
    for (i = 0; i < n; i++)
        lens[i] = (u8)rnc1_input_bits(b, 0xF, 4);

    for (len = 1; len <= 16; len++, inc >>= 1) {
        for (i = 0; i < n; i++) {
            u32 rev, k, top;
            if (lens[i] != len)
                continue;
            /* mask, bit-reversed code (the top `len` bits of `code` read MSB first) */
            put_be16(out, (1u << len) - 1);
            top = code >> 16;
            rev = 0;
            for (k = 0; k < len; k++) {
                rev |= ((top >> (15 - k)) & 1u) << k;
            }
            put_be16(out + 2, rev);
            out += 4;
            /* second array, 0x40 bytes after the entry */
            out[0x3C] = (u8)len;
            out[0x3D] = (u8)i;
            put_be16(out + 0x3E, (1u << ((i - 1) & 31)) - 1);   /* i == 0: 0xFFFF, unused */
            code += inc;
        }
    }
}

/* rncReadHuffCode: decode one symbol with table `tab`.  Linear search of the (mask, code) pairs
 * (no end marker: a valid table always matches).  `tab_end` is only a hardening bound. */
static u32 rnc1_read_huff(Rnc1Bits *b, const u8 *tab, const u8 *tab_end)
{
    const u8 *s0 = tab;
    u32 len, index, value;

    for (;;) {
        if (s0 + 4 > tab_end || s0 + 0x40 > tab_end) { b->bad = 1; return 0; }
        u32 m = be16(s0), c = be16(s0 + 2);
        s0 += 4;
        if ((m & b->window) == c)
            break;
    }
    /* here s0 points just past the matching pair: fields of the entry are at +0x3C.. */
    len = s0[0x3C];
    b->left -= (s32)len;
    {
        s32 n = (s32)len;
        if (b->left < 0)
            n = rnc1_refill(b, n);
        b->window >>= (n & 31);
    }
    index = s0[0x3D];
    if (index < 2)
        return index;                       /* symbols 0 and 1 are themselves */
    {
        s32 nbits = (s32)index - 1;
        s32 n = nbits;
        value = be16(s0 + 0x3E) & b->window;
        b->left -= n;
        if (b->left < 0)
            n = rnc1_refill(b, n);
        b->window >>= (n & 31);
        return value | (1u << nbits);
    }
}

/*
 * rncUnpackMethod1Body / rncUnpackMethod1
 *   in      packed stream *without* the 18 byte header ($s0)
 *   out     destination                                 ($s1)
 *   in_len  packed size from the header                 ($s2)
 *   out_len unpacked size from the header              ($s3 -> $s5 = end of output, unused by the original)
 * Returns the accidental $v0 of the original (last byte loaded << 24); *status is 0 on success,
 * -1 if the hardening checks fired (corrupt stream).
 */
u32 rnc_unpack_method1(const u8 *in, u8 *out, s32 in_len, s32 out_len, int *status)
{
    Rnc1Bits b;
    u8 tables[3 * HUFF_SIZE];                 /* the original keeps them on its stack: 0x180 bytes */
    u8 *raw_tab = tables, *dist_tab = tables + HUFF_SIZE, *len_tab = tables + 2 * HUFF_SIZE;
    const u8 *in_end = in + in_len;           /* $s6 */
    u8 *dst = out;                            /* $s4 */
    u8 *out_end = out + out_len;

    memset(tables, 0, sizeof tables);
    b.p = in;
    b.in_lim = in_end + RNC_IN_SLACK;
    b.bad = 0;
    b.v0 = 0;
    b.window = (u32)in[0] | ((u32)in[1] << 8);   /* first word, little endian */
    b.left = 0;                                   /* "current word used up": next read refills */
    (void)rnc1_input_bits(&b, 2, 2);              /* skip 2 bits (lock / flags in the original format) */

    do {
        u32 sub_blocks;
        rnc1_make_huff_table(&b, raw_tab);
        rnc1_make_huff_table(&b, dist_tab);
        rnc1_make_huff_table(&b, len_tab);
        sub_blocks = (rnc1_input_bits(&b, 0xFFFFFFFFu, 16) - 1) & 0xFFFF;
        if (b.bad) goto fail;

        for (;;) {
            /* literal run: `n` bytes copied straight from the byte stream */
            u32 n = rnc1_read_huff(&b, raw_tab, tables + sizeof tables);
            if (b.bad) goto fail;
            if (n != 0) {
                if (n > (u32)(out_end - dst) || b.p + n + 2 > b.in_lim) goto fail;
                while (n--)
                    *dst++ = *b.p++;
                /* re-prime the window: the word now at p goes above the unread bits */
                {
                    u32 w = (u32)b.p[0] | ((u32)b.p[1] << 8);
                    b.window = (b.window & ((1u << (b.left & 31)) - 1)) | (w << (b.left & 31));
                }
            }
            if (sub_blocks-- == 0)
                break;
            /* back reference: distance, then length (stored as length - 2) */
            {
                u32 dist = rnc1_read_huff(&b, dist_tab, tables + sizeof tables);
                const u8 *from = dst - dist - 1;
                u32 len = rnc1_read_huff(&b, len_tab, tables + sizeof tables);
                if (b.bad) goto fail;
                if (from < out || len + 2 > (u32)(out_end - dst)) goto fail;
                *dst++ = *from++;
                do { *dst++ = *from++; } while (len-- != 0);   /* len + 1 more bytes */
            }
        }
        /* next chunk if the input pointer has not reached the end of the packed data */
    } while ((s32)(b.p - in_end) < 0);

    if (status) *status = 0;
    return b.v0;
fail:
    if (status) *status = -1;
    return b.v0;
}

/* Same signature as the game's call (src/game/func_00285150.c).  `src` points just behind the
 * 18 byte header.  Returns nothing meaningful (the original's $v0 is garbage). */
void rncUnpackMethod1(const u8 *src, u8 *dst, s32 packed, s32 unpacked)
{
    int st;
    (void)rnc_unpack_method1(src, dst, packed, unpacked, &st);
}

/* ==========================================================================================
 * METHOD 2  -  byte-oriented LZ (flag bits in a 9 bit shift register), no Huffman
 * ==========================================================================================
 *
 * Bit reader: $t2 is an 8 bit shift register with a sentinel.  A byte b is loaded as
 * (b << 1) | 1; the 1 below the data is shifted out last, and t2 & 0xFF == 0 signals "refill".
 * $t7 (here `bit`) is the bit most recently shifted out.
 * (The original unrolls the refill check into ~20 copies and, inside a run of literals, only
 * tests for the end of the register when it meets a 1 flag; this is equivalent.)
 *
 * Stream (after the 18 byte header, the first 2 bits of the stream are skipped):
 *   loop
 *     while flag == 0: copy one literal byte
 *     match, decoded as:                                   length   distance
 *       1 0           (flag 1 then 0)                          2    one byte only
 *       1 1 0         -> 3, then distance code
 *       1 1 1 B       -> B + 8 (B != 0), then distance code; B == 0: end/continue marker
 *       0 a           -> 4 + a ...  0 a 0           : len 4/5
 *       0 a 1 b       -> 2*(3 + a) + b   (6..9);  9 is an escape: 4 bits n, copy 4*(n + 3) raw bytes
 *     distance code: 0 -> (hi = 0)  else a variable length code for hi (0..15); lo is the next byte
 *       distance = (hi << 8 | lo); the source is  out - distance - 1
 *   B == 0 marker: one more bit, 1 = keep decoding literals, 0 = end of data.
 */
typedef struct {
    const u8 *p;
    const u8 *lim;      /* in_end + RNC_IN_SLACK, hardening */
    u32 t2;             /* shift register with sentinel */
    int bad;
} Rnc2Bits;

static u32 rnc2_getbit(Rnc2Bits *b)
{
    u32 bit;
    b->t2 <<= 1;
    bit = (b->t2 >> 8) & 1;
    b->t2 &= 0xFF;
    if (b->t2 == 0) {                       /* sentinel gone: load the next byte, sentinel = shifted-out 1 */
        if (b->p >= b->lim) { b->bad = 1; return 0; }
        b->t2 = ((u32)*b->p++ << 1) + bit;
        bit = (b->t2 >> 8) & 1;
        b->t2 &= 0xFF;
    }
    return bit;
}

/* rncUnpackMethod2Body.  `rnc` = start of the file including the header, `out` = destination.
 * Returns 0 (the original's only reachable return value; its -1 "bad crc" / -2 "not RNC" exits
 * are dead code), or -1 when the hardening checks fired. */
static s32 rnc2_unpack(const u8 *rnc, u8 *out)
{
    u32 unpacked = be32(rnc + 4);                       /* D_8014D420 receives this */
    u32 packed = be32(rnc + 8);
    const u8 *in = rnc + 0x12;                          /* $v1 */
    const u8 *in_end = in + packed;
    u8 *dst = out;                                      /* $a1 */
    u8 *out_end = out + unpacked;
    Rnc2Bits b;

    rnc_last_unpacked_size = unpacked;
    rnc_tail_count = 0;

    b.p = in;
    b.lim = in_end + RNC_IN_SLACK;
    b.bad = 0;
    b.t2 = ((((u32)*b.p++) << 1) + 1) << 1;            /* first byte, one extra shift: 2 bits skipped */

    for (;;) {
        u32 len, dist_hi = 0, dist;

        /* literal run */
        while (rnc2_getbit(&b) == 0) {
            if (b.bad || dst >= out_end || b.p >= b.lim) return -1;
            *dst++ = *b.p++;
        }
        if (b.bad) return -1;

        len = 2;
        if (rnc2_getbit(&b) == 0) {
            /* flags "0 ...": length 4..9 */
            len = 4 + rnc2_getbit(&b);
            if (rnc2_getbit(&b)) {
                len = (len - 1) * 2 + rnc2_getbit(&b);
                if (len == 9) {                         /* escape: long raw run */
                    u32 n = 0, i;
                    for (i = 0; i < 4; i++)
                        n = n * 2 + rnc2_getbit(&b);
                    n = 4 * (n + 3);
                    if (b.bad || n > (u32)(out_end - dst) || b.p + n > b.lim) return -1;
                    while (n--)
                        *dst++ = *b.p++;
                    continue;
                }
            }
            goto distance;
        }
        /* flags "1 ...": length 2, 3 or 8+ */
        if (rnc2_getbit(&b) == 0) {
            len = 2;                                    /* distance = one byte, no high part */
            goto copy;
        }
        len = 3;
        if (rnc2_getbit(&b)) {
            if (b.p >= b.lim) return -1;
            len = *b.p++;
            if (len == 0) {                             /* end / continue marker */
                if (rnc2_getbit(&b) != 0)
                    continue;                           /* 1: more literals follow */
                break;                                  /* 0: end of data */
            }
            len += 8;
        }
    distance:
        if (rnc2_getbit(&b)) {
            dist_hi = rnc2_getbit(&b);
            if (rnc2_getbit(&b)) {
                dist_hi = dist_hi * 2 + rnc2_getbit(&b);
                dist_hi |= 4;
                if (rnc2_getbit(&b) == 0)
                    dist_hi = dist_hi * 2 + rnc2_getbit(&b);
            } else if (dist_hi == 0) {
                dist_hi = 2 + rnc2_getbit(&b);
            }
            /* (dist_hi == 1 && !bit: distance high part is 1, no further bits) */
            dist_hi = ((dist_hi >> 8) | (dist_hi << 8)) & 0xFFFF;   /* original swaps the two bytes */
        }
    copy:
        if (b.bad || b.p >= b.lim) return -1;
        dist = (dist_hi & 0xFF00) | *b.p++;
        {
            const u8 *from = dst - dist - 1;
            if (from < out || len > (u32)(out_end - dst)) return -1;
            while (len--)
                *dst++ = *from++;                       /* forward copy; distance 0 repeats the last byte */
        }
    }
    /* tail halfwords (rnc_tail_count) is always 0, see its definition */
    return 0;
}

/* rncUnpackMethod2: the wrapper saves a0-a3, t0-t7, s0-s3, ra around the body (so the caller's
 * registers survive) and returns 0 in $v0.  packed/unpacked are ignored: both come from the
 * header.  Returns 0 on success, -1 if the hardening checks fired. */
s32 rncUnpackMethod2(const u8 *rnc, u8 *dst, s32 packed, s32 unpacked)
{
    (void)packed; (void)unpacked;
    return rnc2_unpack(rnc, dst);
}
