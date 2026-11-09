/*
 * rnc.c - RNC ProPack decompressor (methods 1 and 2) for the Turok 2 port. See rnc.h.
 *
 * Original code for this repository (same license as the repo). Written from Turok 2's own
 * routines, not from another RNC implementation:
 *   method 1  RNC_UnpackMethod1 (0x200C40) -> func_00200CB0 main loop, func_00200E1C symbol
 *             decode, func_00200EC0/func_00200EE8 bit reader, func_00200F20 table build
 *   method 2  RNC_UnpackMethod2 (0x2010C0), inner entry at +0xA8
 * The format notes are in tools/assets/rnc.py (the Python twin of this file).
 */
#include "rnc.h"

#include <string.h>

static uint32_t be32(const uint8_t *p)
{
    return (uint32_t)p[0] << 24 | (uint32_t)p[1] << 16 | (uint32_t)p[2] << 8 | p[3];
}

int Rnc_IsStream(const uint8_t *p, size_t avail)
{
    return avail >= RNC_HEADER_LEN && p[0] == 'R' && p[1] == 'N' && p[2] == 'C' &&
           (p[3] == 1 || p[3] == 2);
}

int Rnc_ReadHeader(const uint8_t *p, size_t avail, RncHeader *h)
{
    if (!Rnc_IsStream(p, avail))
        return RNC_ERR_HEADER;
    h->method = p[3];
    h->unpacked = be32(p + 4);
    h->packed = be32(p + 8);
    h->crcUnpacked = (uint16_t)(p[12] << 8 | p[13]);
    h->crcPacked = (uint16_t)(p[14] << 8 | p[15]);
    h->leeway = p[16];
    h->chunks = p[17];
    return RNC_OK;
}

uint16_t Rnc_Crc16(const uint8_t *p, size_t n)
{
    static uint16_t tab[256];
    static int init;
    uint16_t c = 0;
    size_t i;
    if (!init) {
        int k, j;
        for (k = 0; k < 256; k++) {
            uint16_t v = (uint16_t)k;
            for (j = 0; j < 8; j++)
                v = (v & 1) ? (uint16_t)((v >> 1) ^ 0xA001) : (uint16_t)(v >> 1);
            tab[k] = v;
        }
        init = 1;
    }
    for (i = 0; i < n; i++)
        c = (uint16_t)((c >> 8) ^ tab[(c ^ p[i]) & 0xFF]);
    return c;
}

const char *Rnc_ErrorString(int64_t err)
{
    switch (err) {
    case RNC_ERR_HEADER: return "bad RNC header";
    case RNC_ERR_TRUNCATED: return "RNC stream truncated";
    case RNC_ERR_DATA: return "malformed RNC data";
    case RNC_ERR_OUTPUT: return "RNC output size mismatch";
    case RNC_ERR_CRC: return "RNC CRC mismatch";
    default: return err >= 0 ? "ok" : "RNC error";
    }
}

/* ---- output with bounds -------------------------------------------------------------- */
typedef struct {
    uint8_t *dst;
    size_t pos, cap;
} Out;

/* copy `len` bytes from pos - dist, byte by byte (the source may overlap the destination) */
static int out_match(Out *o, size_t dist, size_t len)
{
    size_t i;
    uint8_t *d;
    if (dist == 0 || dist > o->pos)
        return RNC_ERR_DATA;
    if (len > o->cap - o->pos)
        return RNC_ERR_OUTPUT;
    d = o->dst + o->pos;
    for (i = 0; i < len; i++)
        d[i] = d[(ptrdiff_t)i - (ptrdiff_t)dist];
    o->pos += len;
    return RNC_OK;
}

static int out_bytes(Out *o, const uint8_t *s, size_t len)
{
    if (len > o->cap - o->pos)
        return RNC_ERR_OUTPUT;
    memcpy(o->dst + o->pos, s, len);
    o->pos += len;
    return RNC_OK;
}

/* ===================================================================================== */
/* method 1                                                                               */
/* ===================================================================================== */
typedef struct {
    uint16_t mask, code;
    uint8_t len, sym;
    uint8_t used;
} M1Code;

typedef struct {
    const uint8_t *in;
    size_t n;        /* readable input bytes */
    size_t pos;      /* look-ahead word = in[pos], in[pos + 1] */
    uint32_t buf;    /* `cnt` left-over bits, then the look-ahead word above them */
    int cnt;
    int err;
} M1Bits;

static uint32_t m1_word(M1Bits *b, size_t at)
{
    if (at + 1 >= b->n) {
        b->err = RNC_ERR_TRUNCATED;
        return 0;
    }
    return (uint32_t)b->in[at] | (uint32_t)b->in[at + 1] << 8;
}

/* func_00200EC0 + func_00200EE8: take n (1..16) bits from the low end */
static uint32_t m1_bits(M1Bits *b, int n)
{
    uint32_t v = b->buf & ((n >= 32) ? 0xFFFFFFFFu : ((1u << n) - 1));
    b->cnt -= n;
    if (b->cnt < 0) {
        int left = b->cnt + n, k;
        b->buf >>= left;
        b->pos += 2;
        b->buf |= m1_word(b, b->pos) << 16;
        k = n - left;
        b->cnt = 16 - k;
        b->buf >>= k;
    } else {
        b->buf >>= n;
    }
    return v;
}

/* func_00200F20. Slots a chunk does not define keep their previous contents. */
static void m1_table(M1Bits *b, M1Code tab[16])
{
    uint8_t lens[32];
    int num = (int)m1_bits(b, 5), i, len, slot = 0;
    uint32_t acc = 0, step = 0x80000000u;
    if (num == 0)
        return;
    for (i = 0; i < num; i++)
        lens[i] = (uint8_t)m1_bits(b, 4);
    for (len = 1; len <= 16; len++, step >>= 1) {
        for (i = 0; i < num; i++) {
            uint32_t c, r = 0;
            int k;
            if (lens[i] != len)
                continue;
            if (slot >= 16) {
                b->err = RNC_ERR_DATA;
                return;
            }
            c = acc >> (32 - len);
            for (k = 0; k < len; k++)
                r |= ((c >> k) & 1) << (len - 1 - k);
            tab[slot].mask = (uint16_t)((1u << len) - 1);
            tab[slot].code = (uint16_t)r;
            tab[slot].len = (uint8_t)len;
            tab[slot].sym = (uint8_t)i;
            tab[slot].used = 1;
            slot++;
            acc += step;
        }
    }
}

/* func_00200E1C */
static uint32_t m1_symbol(M1Bits *b, const M1Code tab[16])
{
    int i;
    for (i = 0; i < 16 && tab[i].used; i++) {
        if ((b->buf & tab[i].mask) == tab[i].code) {
            uint32_t s = tab[i].sym;
            m1_bits(b, tab[i].len);
            if (s < 2)
                return s;
            return (1u << (s - 1)) | m1_bits(b, (int)s - 1);
        }
    }
    b->err = RNC_ERR_DATA;
    return 0;
}

static int unpack_m1(const uint8_t *in, size_t avail, uint32_t packed, Out *o)
{
    M1Bits b;
    M1Code tabs[3][16];
    uint32_t n;

    memset(tabs, 0, sizeof(tabs));
    memset(&b, 0, sizeof(b));
    b.in = in;
    b.n = avail;
    b.buf = m1_word(&b, 0);
    m1_bits(&b, 2);
    do {
        m1_table(&b, tabs[0]);
        m1_table(&b, tabs[1]);
        m1_table(&b, tabs[2]);
        n = (m1_bits(&b, 16) - 1) & 0xFFFF;
        for (;;) {
            uint32_t run = m1_symbol(&b, tabs[0]), dist, len;
            int st;
            if (b.err)
                return b.err;
            if (run) {
                if (b.pos + run + 1 >= b.n)
                    return RNC_ERR_TRUNCATED;
                if ((st = out_bytes(o, b.in + b.pos, run)) != RNC_OK)
                    return st;
                b.pos += run;
                b.buf = (b.buf & ((1u << b.cnt) - 1)) | (m1_word(&b, b.pos) << b.cnt);
            }
            if (n == 0)
                break;
            n--;
            dist = m1_symbol(&b, tabs[1]) + 1;
            len = m1_symbol(&b, tabs[2]) + 2;
            if (b.err)
                return b.err;
            if ((st = out_match(o, dist, len)) != RNC_OK)
                return st;
        }
        if (b.err)
            return b.err;
    } while (b.pos < packed);
    return RNC_OK;
}

/* ===================================================================================== */
/* method 2                                                                               */
/* ===================================================================================== */
typedef struct {
    const uint8_t *in;
    size_t n, pos;
    uint32_t v;      /* remaining flag bits above a sentinel 1, in the low 8 bits */
    int err;
} M2Bits;

static uint32_t m2_byte(M2Bits *b)
{
    if (b->pos >= b->n) {
        b->err = RNC_ERR_TRUNCATED;
        return 0;
    }
    return b->in[b->pos++];
}

static uint32_t m2_bit(M2Bits *b)
{
    uint32_t r;
    b->v <<= 1;
    r = (b->v >> 8) & 1;
    b->v &= 0xFF;
    if (b->v == 0) {          /* the sentinel went out: load the next flag byte */
        b->v = (m2_byte(b) << 1) | 1;
        r = (b->v >> 8) & 1;
        b->v &= 0xFF;
    }
    return r;
}

static int unpack_m2(const uint8_t *src, size_t avail, Out *o)
{
    M2Bits b;
    int st;

    memset(&b, 0, sizeof(b));
    b.in = src;
    b.n = avail;
    b.pos = RNC_HEADER_LEN;
    b.v = ((m2_byte(&b) << 1) | 1) & 0xFF;   /* first flag bit dropped */
    m2_bit(&b);                               /* second flag bit */
    for (;;) {
        uint32_t len = 2, dist;
        int hi = -1;
        if (b.err)
            return b.err;
        if (!m2_bit(&b)) {
            uint8_t c = (uint8_t)m2_byte(&b);
            if (b.err)
                return b.err;
            if ((st = out_bytes(o, &c, 1)) != RNC_OK)
                return st;
            continue;
        }
        if (m2_bit(&b)) {
            if (!m2_bit(&b)) {
                hi = 0;
            } else {
                len = 3;
                if (m2_bit(&b)) {
                    uint32_t x = m2_byte(&b);
                    if (x == 0) {             /* end of chunk */
                        if (m2_bit(&b))
                            continue;
                        break;
                    }
                    len = x + 8;
                }
            }
        } else {
            len = 4 + m2_bit(&b);
            if (m2_bit(&b)) {
                len = (len - 1) * 2 + m2_bit(&b);
                if (len == 9) {               /* literal run of 4 * (x + 3) bytes */
                    uint32_t x = 0, k;
                    int i;
                    for (i = 0; i < 4; i++)
                        x = x * 2 + m2_bit(&b);
                    k = 4 * (x + 3);
                    if (b.err)
                        return b.err;
                    if (k > b.n - b.pos)
                        return RNC_ERR_TRUNCATED;
                    if ((st = out_bytes(o, b.in + b.pos, k)) != RNC_OK)
                        return st;
                    b.pos += k;
                    continue;
                }
            }
        }
        if (hi < 0) {
            hi = 0;
            if (m2_bit(&b)) {
                int h = (int)m2_bit(&b);
                if (!m2_bit(&b)) {
                    if (h == 0)
                        h = 2 + (int)m2_bit(&b);
                } else {
                    h = (h * 2 + (int)m2_bit(&b)) | 4;
                    if (!m2_bit(&b))
                        h = h * 2 + (int)m2_bit(&b);
                }
                hi = h;
            }
        }
        dist = (((uint32_t)hi << 8) | m2_byte(&b)) + 1;
        if (b.err)
            return b.err;
        if ((st = out_match(o, dist, len)) != RNC_OK)
            return st;
    }
    return b.err;
}

/* ===================================================================================== */
int64_t Rnc_Unpack(const uint8_t *src, size_t avail, uint8_t *dst, size_t dstCap, int verify)
{
    RncHeader h;
    Out o;
    int st;

    if (Rnc_ReadHeader(src, avail, &h) != RNC_OK)
        return RNC_ERR_HEADER;
    if ((size_t)h.packed > avail - RNC_HEADER_LEN)
        return RNC_ERR_TRUNCATED;
    if (h.unpacked > dstCap)
        return RNC_ERR_OUTPUT;
    if (verify && Rnc_Crc16(src + RNC_HEADER_LEN, h.packed) != h.crcPacked)
        return RNC_ERR_CRC;
    o.dst = dst;
    o.pos = 0;
    o.cap = h.unpacked;
    if (h.method == 1)
        st = unpack_m1(src + RNC_HEADER_LEN, avail - RNC_HEADER_LEN, h.packed, &o);
    else
        st = unpack_m2(src, avail, &o);
    if (st != RNC_OK)
        return st;
    if (o.pos != h.unpacked)
        return RNC_ERR_OUTPUT;
    if (verify && Rnc_Crc16(dst, h.unpacked) != h.crcUnpacked)
        return RNC_ERR_CRC;
    return (int64_t)h.unpacked;
}
