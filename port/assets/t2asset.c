/*
 * t2asset.c - Turok 2 native port: asset reader. See t2asset.h and docs/assets/PORT.md
 * ("Reader").
 *
 * Front end (this file, top): unit lookup, prefix/whole-unit rules, RAW_RANGE reads at
 * inner addresses, buffers. Backends (this file, bottom): archive (miniz) and ROM
 * (image in memory + RNC unpack). A libultraship backend plugs in through
 * T2Asset_SetBackend without touching the front end.
 */
#include "t2asset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "../third_party/miniz/miniz.h"

#include "rnc.h" /* RNC methods 1 and 2: original code, written from Turok 2's routines */

#define RNC_HEADER 0x12
#define RNC_SLACK 16
#define PAD 16

/* ===================================================================================== */
/* errors                                                                                */
/* ===================================================================================== */
static char sErr[256];

static int fail(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(sErr, sizeof sErr, fmt, ap);
    va_end(ap);
    return -1;
}

const char *T2Asset_Error(void) { return sErr; }

static u32 be32(const u8 *p) { return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | p[3]; }
static u32 le32(const u8 *p) { return ((u32)p[3] << 24) | ((u32)p[2] << 16) | ((u32)p[1] << 8) | p[0]; }

/* ===================================================================================== */
/* SHA1 (FIPS 180-1), for the ROM check                                                  */
/* ===================================================================================== */
#define ROL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
static void sha1_block(u32 h[5], const u8 *p)
{
    u32 w[80], a, b, c, d, e, t;
    int i;
    for (i = 0; i < 16; i++)
        w[i] = be32(p + 4 * i);
    for (; i < 80; i++)
        w[i] = ROL(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);
    a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
    for (i = 0; i < 80; i++) {
        u32 f, k;
        if (i < 20)      f = (b & c) | (~b & d), k = 0x5A827999;
        else if (i < 40) f = b ^ c ^ d,          k = 0x6ED9EBA1;
        else if (i < 60) f = (b & c) | (b & d) | (c & d), k = 0x8F1BBCDC;
        else             f = b ^ c ^ d,          k = 0xCA62C1D6;
        t = ROL(a, 5) + f + e + k + w[i];
        e = d, d = c, c = ROL(b, 30), b = a, a = t;
    }
    h[0] += a, h[1] += b, h[2] += c, h[3] += d, h[4] += e;
}

void T2_Sha1(const void *data, size_t len, u8 out[20])
{
    u32 h[5] = { 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 };
    const u8 *p = (const u8 *)data;
    u8 tail[128];
    size_t n = len, rest, i;
    u64 bits = (u64)len * 8;
    while (n >= 64) {
        sha1_block(h, p);
        p += 64, n -= 64;
    }
    memset(tail, 0, sizeof tail);
    memcpy(tail, p, n);
    tail[n] = 0x80;
    rest = (n < 56) ? 64 : 128;
    for (i = 0; i < 8; i++)
        tail[rest - 1 - i] = (u8)(bits >> (8 * i));
    sha1_block(h, tail);
    if (rest == 128)
        sha1_block(h, tail + 64);
    for (i = 0; i < 20; i++)
        out[i] = (u8)(h[i / 4] >> (24 - 8 * (i % 4)));
}

int T2Rom_Normalize(u8 *rom, size_t len)
{
    size_t i;
    if (len < 0x40 || (len & 3))
        return -1;
    if (rom[0] == 0x80 && rom[1] == 0x37) /* .z64 */
        return 0;
    if (rom[0] == 0x37 && rom[1] == 0x80) { /* .v64: 16-bit swapped */
        for (i = 0; i < len; i += 2) {
            u8 t = rom[i]; rom[i] = rom[i + 1]; rom[i + 1] = t;
        }
        return 0;
    }
    if (rom[0] == 0x40 && rom[1] == 0x12) { /* .n64: 32-bit little endian */
        for (i = 0; i < len; i += 4) {
            u8 t0 = rom[i], t1 = rom[i + 1];
            rom[i] = rom[i + 3], rom[i + 1] = rom[i + 2], rom[i + 2] = t1, rom[i + 3] = t0;
        }
        return 0;
    }
    return -1;
}

/* ===================================================================================== */
/* RNC                                                                                   */
/* ===================================================================================== */
static int is_rnc(const u8 *p, u32 avail)
{
    return Rnc_IsStream(p, avail);
}

u32 T2Rnc_Unpack(const u8 *src, size_t avail, u8 *dst, u32 dstCap)
{
    RncHeader h;
    int64_t n;
    if (Rnc_ReadHeader(src, avail, &h) != RNC_OK || (size_t)h.packed > avail - RNC_HEADER)
        return 0;
    if (!dst)
        return h.unpacked;
    if (dstCap < h.unpacked)
        return 0;
    /* the packed CRC is checked once per ROM by tools; the unpacked one on every read */
    n = Rnc_Unpack(src, avail, dst, dstCap, 0);
    if (n < 0 || Rnc_Crc16(dst, h.unpacked) != h.crcUnpacked)
        return 0;
    return (u32)n;
}

/* ===================================================================================== */
/* front end                                                                             */
/* ===================================================================================== */
static const T2AssetBackend *sBe;
static void *sCtx;

/* ===================================================================================== */
/* byte order: swap plans (t2/swap, docs/assets/ARCHIVE.md)                              */
/* ===================================================================================== */
/* Table layout (little-endian): 'T2SW' u16 major u16 minor u32 count u32 reserved, then count
 * x {u32 key, u32 hostSize, u32 offset, u32 runs, u32 tables} sorted by key; each plan at
 * `offset` (from the table start): runs x {u32 offset, u32 count << 2 | width code (1: 2 bytes,
 * 2: 4, 3: 8)}, then tables x {u32 offset, u32 records, u32 swaps, swaps x {u16 plane, u16 width}}
 * (a transposed table, CUSet__Transpose: a w-byte field at record offset p is planes p..p+w-1,
 * each `records` bytes long; swapping it reverses those planes). Runs first, then tables, the
 * order of tools/assets/swapcheck.py apply_plan(). */
#define SWAP_HDR 16
#define SWAP_REC 20
#define SWAP_MAJOR 2
static u8 *sSwap;
static size_t sSwapLen;
static u32 sSwapN;
static u32 sSwapApplied, sSwapNoPlan, sSwapRefused;

static int host_little(void)
{
    const u16 one = 1;
    return *(const u8 *)&one == 1;
}

static void swap_drop(void)
{
    free(sSwap);
    sSwap = NULL, sSwapLen = 0, sSwapN = 0;
    sSwapApplied = sSwapNoPlan = sSwapRefused = 0;
}

int T2Asset_SetSwapTable(const void *data, size_t len)
{
    const u8 *p = (const u8 *)data;
    u32 n, i, prev = 0;
    swap_drop();
    if (len < SWAP_HDR || memcmp(p, "T2SW", 4) != 0)
        return fail("t2/swap: bad magic");
    if ((u32)(p[4] | p[5] << 8) != SWAP_MAJOR)
        return fail("t2/swap: format %u, this reader needs %u (rebuild the archive)", (u32)(p[4] | p[5] << 8), SWAP_MAJOR);
    n = le32(p + 8);
    if ((u64)n * SWAP_REC + SWAP_HDR > len)
        return fail("t2/swap: truncated index");
    for (i = 0; i < n; i++) {
        const u8 *r = p + SWAP_HDR + (size_t)i * SWAP_REC;
        u64 need = (u64)le32(r + 12) * 8 + (u64)le32(r + 16) * 12;
        if ((i && le32(r) <= prev) || le32(r + 8) < SWAP_HDR + (u64)n * SWAP_REC || le32(r + 8) + need > len)
            return fail("t2/swap: bad record %u (key %08X)", i, le32(r));
        prev = le32(r);
    }
    sSwap = (u8 *)malloc(len);
    if (!sSwap)
        return fail("out of memory");
    memcpy(sSwap, data, len);
    sSwapLen = len, sSwapN = n;
    return 0;
}

int T2Asset_SwapActive(void) { return sSwap != NULL && host_little(); }

void T2Asset_SwapStats(u32 *applied, u32 *noPlan, u32 *refused)
{
    if (applied) *applied = sSwapApplied;
    if (noPlan) *noPlan = sSwapNoPlan;
    if (refused) *refused = sSwapRefused;
}

static const u8 *swap_find(u32 key)
{
    u32 lo = 0, hi = sSwapN;
    while (lo < hi) {
        u32 mid = (lo + hi) / 2;
        const u8 *r = sSwap + SWAP_HDR + (size_t)mid * SWAP_REC;
        if (le32(r) == key) return r;
        if (le32(r) < key) lo = mid + 1; else hi = mid;
    }
    return NULL;
}

static void rev_bytes(u8 *p, u32 w)
{
    u32 i;
    for (i = 0; i < w / 2; i++) {
        u8 t = p[i];
        p[i] = p[w - 1 - i];
        p[w - 1 - i] = t;
    }
}

int T2Asset_SwapUnit(u32 key, void *data, u32 size)
{
    u8 *buf = (u8 *)data;
    const u8 *rec, *q, *end;
    u32 runs, tables, i;
    if (!T2Asset_SwapActive())
        return 0;
    rec = swap_find(key);
    if (!rec) {
        sSwapNoPlan++;
        return 0;
    }
    if (le32(rec + 4) != size) { /* a mod entry with another size: no plan fits it */
        sSwapRefused++;
        return fail("0x%08X: swap plan is for 0x%X bytes, unit has 0x%X", key, le32(rec + 4), size);
    }
    q = sSwap + le32(rec + 8);
    end = sSwap + sSwapLen;
    runs = le32(rec + 12), tables = le32(rec + 16);
    /* check the whole plan before touching the buffer: a unit is swapped completely or not at all */
    {
        const u8 *c = q + (size_t)runs * 8;
        for (i = 0; i < runs; i++) {
            u32 o = le32(q + 8 * i), cw = le32(q + 8 * i + 4), w = 1u << (cw & 3), n = cw >> 2;
            if ((cw & 3) == 0 || (u64)o + (u64)w * n > size)
                return fail("0x%08X: swap run %u outside the unit", key, i);
        }
        for (i = 0; i < tables; i++) {
            u32 d, n, k, ns;
            if (c + 12 > end) return fail("0x%08X: swap plan truncated", key);
            d = le32(c), n = le32(c + 4), ns = le32(c + 8);
            c += 12;
            if (c + (size_t)ns * 4 > end) return fail("0x%08X: swap plan truncated", key);
            for (k = 0; k < ns; k++) {
                u32 pl = (u32)(c[4 * k] | c[4 * k + 1] << 8), w = (u32)(c[4 * k + 2] | c[4 * k + 3] << 8);
                if ((u64)d + (u64)(pl + w) * n > size)
                    return fail("0x%08X: swap table %u outside the unit", key, i);
            }
            c += (size_t)ns * 4;
        }
    }
    for (i = 0; i < runs; i++, q += 8) {
        u32 o = le32(q), cw = le32(q + 4), w = 1u << (cw & 3), n = cw >> 2, k;
        u8 *p = buf + o;
        switch (w) {
        case 2:
            for (k = 0; k < n; k++, p += 2) { u8 t = p[0]; p[0] = p[1]; p[1] = t; }
            break;
        case 4:
            for (k = 0; k < n; k++, p += 4) {
                u8 t0 = p[0], t1 = p[1];
                p[0] = p[3], p[1] = p[2], p[2] = t1, p[3] = t0;
            }
            break;
        default:
            for (k = 0; k < n; k++, p += w) rev_bytes(p, w);
            break;
        }
    }
    for (i = 0; i < tables; i++) {
        u32 d = le32(q), n = le32(q + 4), ns = le32(q + 8), k;
        q += 12;
        for (k = 0; k < ns; k++, q += 4) {
            u32 pl = (u32)(q[0] | q[1] << 8), w = (u32)(q[2] | q[3] << 8), j;
            for (j = 0; j < w / 2; j++) { /* swap planes pl+j and pl+w-1-j, n bytes each */
                u8 *a = buf + d + (size_t)(pl + j) * n, *b = buf + d + (size_t)(pl + w - 1 - j) * n;
                u32 x;
                for (x = 0; x < n; x++) { u8 t = a[x]; a[x] = b[x]; b[x] = t; }
            }
        }
    }
    sSwapApplied++;
    return 1;
}

int T2Asset_LoadSwapFile(const char *path)
{
    FILE *f;
    long n;
    u8 *buf;
    int rc;
    mz_zip_archive zip;
    memset(&zip, 0, sizeof zip);
    if (mz_zip_reader_init_file(&zip, path, 0)) {
        size_t len = 0;
        void *p = mz_zip_reader_extract_file_to_heap(&zip, "t2/swap", &len, 0);
        mz_zip_reader_end(&zip);
        if (!p) return fail("%s: no t2/swap entry", path);
        rc = T2Asset_SetSwapTable(p, len);
        mz_free(p);
        return rc;
    }
    f = fopen(path, "rb");
    if (!f) return fail("%s: cannot open", path);
    fseek(f, 0, SEEK_END);
    n = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = n > 0 ? (u8 *)malloc((size_t)n) : NULL;
    if (!buf || fread(buf, 1, (size_t)n, f) != (size_t)n) {
        fclose(f);
        free(buf);
        return fail("%s: read error", path);
    }
    fclose(f);
    rc = T2Asset_SetSwapTable(buf, (size_t)n);
    free(buf);
    return rc;
}

/* backend load + byte order: every unit the front end hands out goes through here */
static int load_unit(u32 key, void *dst, u32 hostSize)
{
    if (sBe->load(sCtx, key, dst, hostSize) < 0)
        return -1;
    return T2Asset_SwapUnit(key, dst, hostSize) < 0 ? -1 : 0;
}

/* resident copies of RAW_RANGE units, for reads at inner addresses (archive backend) */
typedef struct { u32 key, size; u8 *data; } Resident;
static Resident sRes[4];

static void drop_residents(void)
{
    int i;
    for (i = 0; i < 4; i++) {
        free(sRes[i].data);
        memset(&sRes[i], 0, sizeof sRes[i]);
    }
}

void T2Asset_SetBackend(const T2AssetBackend *backend, void *ctx)
{
    T2Asset_Close();
    sBe = backend;
    sCtx = ctx;
}

void T2Asset_Close(void)
{
    drop_residents();
    swap_drop();
    if (sBe && sBe->close)
        sBe->close(sCtx);
    sBe = NULL;
    sCtx = NULL;
}

const char *T2Asset_BackendName(void) { return sBe ? sBe->name : "none"; }

u32 T2Asset_UnitCount(void) { return sBe && sBe->count ? sBe->count(sCtx) : 0; }
u32 T2Asset_UnitKey(u32 i) { return sBe && sBe->key_at ? sBe->key_at(sCtx, i) : 0; }

int T2Asset_UnitInfo(u32 key, u32 *romSize, u32 *hostSize, u32 *flags)
{
    u32 r = 0, h = 0, f = 0;
    if (!sBe || !sBe->lookup(sCtx, key, &r, &h, &f))
        return 0;
    if (romSize) *romSize = r;
    if (hostSize) *hostSize = h;
    if (flags) *flags = f;
    return 1;
}

static u8 *alloc_buf(u32 n)
{
    /* malloc is 16-byte aligned on the 64-bit hosts we target (glibc, macOS, MSVC x64) */
    u8 *p = (u8 *)malloc((size_t)n + PAD);
    if (p)
        memset(p + n, 0, PAD);
    return p;
}

u32 T2Asset_HostSize(u32 key, u32 size)
{
    u32 r, h, f;
    if (!T2Asset_UnitInfo(key, &r, &h, &f))
        return size;
    if (size == 0 || r == 0 || size >= r)
        return h;
    return size;
}

/* raw ROM bytes at an inner address: backend's own raw reader, else a resident RAW_RANGE unit */
static int read_raw(u32 addr, u8 *dst, u32 len)
{
    u32 i, n = T2Asset_UnitCount();
    int slot = -1;
    /* the backend's raw reader gives ROM order: only for bytes no swap plan covers */
    if (sBe->read_raw && (!T2Asset_SwapActive() || n == 0))
        return sBe->read_raw(sCtx, addr, dst, len);
    for (i = 0; i < 4; i++)
        if (sRes[i].data && addr >= sRes[i].key && addr - sRes[i].key < sRes[i].size)
            slot = (int)i;
    if (slot < 0) {
        /* find the RAW_RANGE unit containing addr (few units; keys sorted) */
        u32 lo = 0, hi = n, key = 0, r = 0, h = 0, f = 0;
        while (lo < hi) { /* last key <= addr */
            u32 mid = (lo + hi) / 2;
            if (T2Asset_UnitKey(mid) <= addr) lo = mid + 1; else hi = mid;
        }
        if (lo == 0)
            return sBe->read_raw ? sBe->read_raw(sCtx, addr, dst, len) : fail("0x%08X: no unit", addr);
        key = T2Asset_UnitKey(lo - 1);
        if (!sBe->lookup(sCtx, key, &r, &h, &f) || !(f & T2U_INNER) || addr - key >= h) {
            if (sBe->read_raw) /* outside every unit (a raw ROM read): ROM bytes */
                return sBe->read_raw(sCtx, addr, dst, len);
            return fail("0x%08X: not inside a RAW_RANGE unit", addr);
        }
        if (sBe->read_raw && !swap_find(key)) /* nothing to swap in it: no resident copy needed */
            return sBe->read_raw(sCtx, addr, dst, len);
        for (i = 0; i < 4 && sRes[i].data; i++)
            ;
        if (i == 4) { free(sRes[3].data); i = 3; }
        sRes[i].data = alloc_buf(h);
        if (!sRes[i].data || load_unit(key, sRes[i].data, h) < 0) {
            free(sRes[i].data);
            sRes[i].data = NULL;
            return fail("0x%08X: cannot load RAW_RANGE unit", key);
        }
        sRes[i].key = key, sRes[i].size = h;
        slot = (int)i;
    }
    {
        Resident *r = &sRes[slot];
        u32 off = addr - r->key, avail = r->size - off;
        memcpy(dst, r->data + off, len < avail ? len : avail);
        if (len > avail)
            memset(dst + avail, 0, len - avail);
    }
    return 0;
}

const void *T2Asset_Read(u32 romKey, u32 *size)
{
    u32 r, h, f, want = size ? *size : 0;
    u8 *buf;
    if (!sBe) {
        fail("no backend open");
        return NULL;
    }
    if (sBe->lookup(sCtx, romKey, &r, &h, &f) && !((f & T2U_INNER) && want != 0 && want < h)) {
        buf = alloc_buf(h);
        if (!buf || load_unit(romKey, buf, h) < 0) {
            free(buf);
            if (!sErr[0]) fail("0x%08X: load failed", romKey);
            return NULL;
        }
        if (want != 0 && r != 0 && want < r) { /* prefix probe */
            if (want < h) memset(buf + want, 0, h - want < PAD ? h - want : PAD);
            h = want;
        }
        if (size) *size = h;
        return buf;
    }
    if (want == 0) {
        fail("0x%08X: no unit starts here", romKey);
        return NULL;
    }
    buf = alloc_buf(want);
    if (!buf || read_raw(romKey, buf, want) < 0) {
        free(buf);
        return NULL;
    }
    return buf;
}

void T2Asset_Release(const void *data) { free((void *)data); }

int T2Asset_ReadInto(u32 address, u32 offset, void *dst, u32 len)
{
    u32 r, h, f;
    if (!sBe)
        return fail("no backend open");
    /* RAW_RANGE units (audio) are read piecemeal at any inner address: raw path */
    if (sBe->lookup(sCtx, address, &r, &h, &f) && !(f & T2U_INNER)) {
        if (offset == 0 && len >= h) {
            if (load_unit(address, dst, h) < 0)
                return -1;
            memset((u8 *)dst + h, 0, len - h);
            return 0;
        }
        {
            u8 *tmp = alloc_buf(h);
            if (!tmp || load_unit(address, tmp, h) < 0) {
                free(tmp);
                return -1;
            }
            if (offset < h) {
                u32 n = h - offset < len ? h - offset : len;
                memcpy(dst, tmp + offset, n);
                memset((u8 *)dst + n, 0, len - n);
            } else {
                memset(dst, 0, len);
            }
            free(tmp);
            return 0;
        }
    }
    return read_raw(address + offset, (u8 *)dst, len);
}

/* ===================================================================================== */
/* unit table (shared by both backends)                                                  */
/* ===================================================================================== */
typedef struct { u32 key, romSize, hostSize, flags, index; } Unit;
typedef struct { Unit *u; u32 n, cap; } Units;

static int units_add(Units *t, u32 key, u32 rs, u32 hs, u32 fl, u32 idx)
{
    if (t->n == t->cap) {
        u32 cap = t->cap ? t->cap * 2 : 1024;
        Unit *nu = (Unit *)realloc(t->u, cap * sizeof(Unit));
        if (!nu) return -1;
        t->u = nu, t->cap = cap;
    }
    t->u[t->n].key = key, t->u[t->n].romSize = rs, t->u[t->n].hostSize = hs;
    t->u[t->n].flags = fl, t->u[t->n].index = idx;
    t->n++;
    return 0;
}

static int unit_cmp(const void *a, const void *b)
{
    u32 x = ((const Unit *)a)->key, y = ((const Unit *)b)->key;
    return x < y ? -1 : x > y;
}

static Unit *units_find(const Units *t, u32 key)
{
    u32 lo = 0, hi = t->n;
    while (lo < hi) {
        u32 mid = (lo + hi) / 2;
        if (t->u[mid].key == key) return &t->u[mid];
        if (t->u[mid].key < key) lo = mid + 1; else hi = mid;
    }
    return NULL;
}

/* flags of the three audio roots when no manifest says otherwise */
static u32 raw_range_flag(u32 key)
{
    if (key == T2_ROOT_AUDIO_DATA) return T2U_RAW_RANGE;
    if (key == T2_ROOT_AUDIO_CTL || key == T2_ROOT_SOUND_DATA) return T2U_VM_WINDOW;
    return 0;
}

/* ===================================================================================== */
/* archive backend (miniz)                                                               */
/* ===================================================================================== */
/* Entry formats accepted (detected once per archive from the first unit entry):
 *   OBLB   what tools/assets/mkarchive.py writes (docs/assets/ARCHIVE.md): the 64-byte
 *          libultraship resource header, type Blob ('OBLB') v0, with a T2 block in the
 *          header's unused bytes ('T2LU' at 0x14; u32 key, romSize, hostSize, u16 type,
 *          flags at 0x18), then u32 size + the unit bytes. LUS's BlobFactory loads it as is.
 *   raw    the unit bytes and nothing else (a hand-made mod / folder entry).
 * Header integers use the byte order named by header byte 0 (0 little, 1 big). */
#define FMT_RAW 0
#define FMT_OBLB 1
#define LUS_HDR 0x40
#define TYPE_OBLB 0x4F424C42u

typedef struct {
    mz_zip_archive zip;
    Units units;
    int fmt;
    u32 hdr;      /* bytes before the unit data */
    int bigHdr;   /* header byte order */
    u32 manifest; /* records in t2/units (0: none) */
    u8 *scratch;
    size_t scratchCap;
} ArcCtx;

static u32 hdr32(const ArcCtx *c, const u8 *p) { return c->bigHdr ? be32(p) : le32(p); }

static int arc_lookup(void *ctx, u32 key, u32 *rs, u32 *hs, u32 *fl)
{
    Unit *u = units_find(&((ArcCtx *)ctx)->units, key);
    if (!u) return 0;
    *rs = u->romSize, *hs = u->hostSize, *fl = u->flags;
    return 1;
}

static u8 *arc_scratch(ArcCtx *c, size_t n)
{
    if (n > c->scratchCap) {
        u8 *p = (u8 *)realloc(c->scratch, n);
        if (!p) return NULL;
        c->scratch = p, c->scratchCap = n;
    }
    return c->scratch;
}

static int arc_load(void *ctx, u32 key, void *dst, u32 hostSize)
{
    ArcCtx *c = (ArcCtx *)ctx;
    Unit *u = units_find(&c->units, key);
    mz_zip_archive_file_stat st;
    u8 *tmp;
    if (!u) return fail("0x%08X: not in archive", key);
    if (!mz_zip_reader_file_stat(&c->zip, u->index, &st))
        return fail("0x%08X: stat failed", key);
    if (c->fmt == FMT_RAW && st.m_uncomp_size == hostSize) {
        if (!mz_zip_reader_extract_to_mem(&c->zip, u->index, dst, hostSize, 0))
            return fail("0x%08X: extract failed (%s)", key, mz_zip_get_error_string(mz_zip_get_last_error(&c->zip)));
        return 0;
    }
    tmp = arc_scratch(c, (size_t)st.m_uncomp_size);
    if (!tmp || !mz_zip_reader_extract_to_mem(&c->zip, u->index, tmp, (size_t)st.m_uncomp_size, 0))
        return fail("0x%08X: extract failed", key);
    if (st.m_uncomp_size < c->hdr + (mz_uint64)hostSize)
        return fail("0x%08X: entry too short", key);
    /* the Blob size field (and the T2 block, when present) must agree with the entry */
    if (c->fmt == FMT_OBLB && (hdr32(c, tmp + LUS_HDR) != hostSize ||
        (memcmp(tmp + 0x14, "T2LU", 4) == 0 && (hdr32(c, tmp + 0x18) != key || hdr32(c, tmp + 0x20) != hostSize))))
        return fail("0x%08X: entry header disagrees with its size", key);
    memcpy(dst, tmp + c->hdr, hostSize);
    return 0;
}

static u32 arc_count(void *ctx) { return ((ArcCtx *)ctx)->units.n; }
static u32 arc_key_at(void *ctx, u32 i) { return ((ArcCtx *)ctx)->units.u[i].key; }

static void arc_close(void *ctx)
{
    ArcCtx *c = (ArcCtx *)ctx;
    mz_zip_reader_end(&c->zip);
    free(c->units.u);
    free(c->scratch);
    free(c);
}

static const T2AssetBackend kArchiveBackend = {
    "archive", arc_lookup, arc_load, NULL, arc_count, arc_key_at, arc_close,
};

static int parse_unit_name(const char *name, u32 *key)
{
    u32 k = 0;
    int i;
    if (strncmp(name, "t2/u/", 5) != 0 || strlen(name) != 13)
        return 0;
    for (i = 5; i < 13; i++) {
        char ch = name[i];
        u32 d;
        if (ch >= '0' && ch <= '9') d = (u32)(ch - '0');
        else if (ch >= 'A' && ch <= 'F') d = (u32)(ch - 'A' + 10);
        else if (ch >= 'a' && ch <= 'f') d = (u32)(ch - 'a' + 10);
        else return 0;
        k = k * 16 + d;
    }
    *key = k;
    return 1;
}

/* Detect the entry format from one unit entry's first bytes. */
static void arc_detect(ArcCtx *c, const u8 *p, size_t n)
{
    c->fmt = FMT_RAW, c->hdr = 0, c->bigHdr = 0;
    if (n >= LUS_HDR + 4 && (p[0] == 0 || p[0] == 1) && p[1] <= 1 && p[2] == 0 && p[3] == 0) {
        int big = p[0] == 1;
        u32 type = big ? be32(p + 4) : le32(p + 4);
        if (type == TYPE_OBLB) c->fmt = FMT_OBLB, c->hdr = LUS_HDR + 4, c->bigHdr = big;
    }
}

/* Manifest t2/units (mkarchive.py, little-endian): 'T2UT' u16 major u16 minor u32 count
 * u32 typeCount, then count x {u32 key, romSize, hostSize; u16 type, flags}. Gives each
 * unit its ROM size (prefix-probe rule) and flags. Records that disagree with the entry
 * sizes are ignored (a mod entry that shadows a unit with a new size keeps romSize 0 =
 * "every request reads it whole"). */
static void arc_manifest(ArcCtx *c)
{
    size_t n = 0, i;
    u8 *m;
    int idx = mz_zip_reader_locate_file(&c->zip, "t2/units", NULL, 0);
    if (idx < 0) return;
    m = (u8 *)mz_zip_reader_extract_to_heap(&c->zip, (mz_uint)idx, &n, 0);
    if (!m) return;
    if (n >= 16 && memcmp(m, "T2UT", 4) == 0 && (u64)le32(m + 8) * 16 + 16 <= n) {
        u32 cnt = le32(m + 8);
        for (i = 0; i < cnt; i++) {
            const u8 *r = m + 16 + 16 * i;
            Unit *u = units_find(&c->units, le32(r));
            if (!u || le32(r + 8) != u->hostSize) continue;
            u->romSize = le32(r + 4);
            u->flags = (u32)r[14] | ((u32)r[15] << 8);
        }
        c->manifest = cnt;
    }
    mz_free(m);
}

int T2Asset_OpenArchive(const char *path)
{
    ArcCtx *c = (ArcCtx *)calloc(1, sizeof *c);
    mz_uint i, n;
    int detected = 0;
    sErr[0] = 0;
    if (!c) return fail("out of memory");
    if (!mz_zip_reader_init_file(&c->zip, path, 0)) {
        free(c);
        return fail("%s: not a zip archive", path);
    }
    n = mz_zip_reader_get_num_files(&c->zip);
    for (i = 0; i < n; i++) {
        char name[64];
        mz_zip_archive_file_stat st;
        u32 key;
        if (!mz_zip_reader_get_filename(&c->zip, i, name, sizeof name) || !parse_unit_name(name, &key))
            continue;
        if (!mz_zip_reader_file_stat(&c->zip, i, &st))
            continue;
        if (!detected) {
            u8 *head = arc_scratch(c, (size_t)st.m_uncomp_size);
            if (!head || !mz_zip_reader_extract_to_mem(&c->zip, i, head, (size_t)st.m_uncomp_size, 0)) {
                arc_close(c);
                return fail("%s: cannot read %s", path, name);
            }
            arc_detect(c, head, (size_t)st.m_uncomp_size);
            detected = 1;
        }
        if (st.m_uncomp_size < c->hdr) {
            arc_close(c);
            return fail("%s: %s shorter than its header", path, name);
        }
        {
            u32 hs = (u32)(st.m_uncomp_size - c->hdr);
            /* without a manifest: romSize unknown (0) = every request reads the whole unit */
            if (units_add(&c->units, key, 0, hs, raw_range_flag(key), i) < 0) {
                arc_close(c);
                return fail("out of memory");
            }
        }
    }
    if (c->units.n == 0) {
        arc_close(c);
        return fail("%s: no t2/u/ entries", path);
    }
    qsort(c->units.u, c->units.n, sizeof(Unit), unit_cmp);
    for (i = 1; i < c->units.n; i++)
        if (c->units.u[i].key == c->units.u[i - 1].key) {
            u32 dup = c->units.u[i].key;
            arc_close(c);
            return fail("%s: duplicate key %08X", path, dup);
        }
    arc_manifest(c);
    T2Asset_SetBackend(&kArchiveBackend, c);
    {   /* byte-swap plans (format 2); a 1.x archive has none and is served in ROM order */
        size_t len = 0;
        void *p = mz_zip_reader_extract_file_to_heap(&c->zip, "t2/swap", &len, 0);
        if (p) {
            int rc = T2Asset_SetSwapTable(p, len);
            mz_free(p);
            if (rc < 0) {
                T2Asset_Close();
                return -1;
            }
        }
    }
    return 0;
}

/* ===================================================================================== */
/* ROM backend                                                                           */
/* ===================================================================================== */
typedef struct {
    u8 *rom;      /* normalized image + RNC_SLACK zero bytes */
    size_t len;
    Units units;  /* optional unit table */
} RomCtx;

static int rom_lookup(void *ctx, u32 key, u32 *rs, u32 *hs, u32 *fl)
{
    RomCtx *c = (RomCtx *)ctx;
    Unit *u = units_find(&c->units, key);
    if (u) {
        *rs = u->romSize, *hs = u->hostSize, *fl = u->flags;
        return 1;
    }
    if (c->units.n == 0 && key < c->len && is_rnc(c->rom + key, (u32)(c->len - key))) {
        /* no table: an RNC stream at key is a unit by itself */
        u32 h = T2Rnc_Unpack(c->rom + key, c->len + RNC_SLACK - key, NULL, 0);
        if (h) {
            *rs = RNC_HEADER + be32(c->rom + key + 8), *hs = h, *fl = T2U_RNC_UNPACKED;
            return 1;
        }
    }
    return 0;
}

static int rom_read_raw(void *ctx, u32 addr, void *dst, u32 len)
{
    RomCtx *c = (RomCtx *)ctx;
    size_t avail = addr < c->len ? c->len - addr : 0;
    if (avail == 0) return fail("0x%08X: past the end of the ROM", addr);
    memcpy(dst, c->rom + addr, len < avail ? len : avail);
    if (len > avail) memset((u8 *)dst + avail, 0, len - avail);
    return 0;
}

static int rom_load(void *ctx, u32 key, void *dst, u32 hostSize)
{
    RomCtx *c = (RomCtx *)ctx;
    u32 rs, hs, fl;
    if (!rom_lookup(ctx, key, &rs, &hs, &fl))
        return fail("0x%08X: no unit", key);
    if (fl & T2U_RNC_UNPACKED) {
        if (hostSize < hs || T2Rnc_Unpack(c->rom + key, c->len + RNC_SLACK - key, (u8 *)dst, hostSize) != hs)
            return fail("0x%08X: RNC unpack failed", key);
        return 0;
    }
    return rom_read_raw(ctx, key, dst, hostSize);
}

static u32 rom_count(void *ctx) { return ((RomCtx *)ctx)->units.n; }
static u32 rom_key_at(void *ctx, u32 i) { return ((RomCtx *)ctx)->units.u[i].key; }

static void rom_close(void *ctx)
{
    RomCtx *c = (RomCtx *)ctx;
    free(c->rom);
    free(c->units.u);
    free(c);
}

static const T2AssetBackend kRomBackend = {
    "rom", rom_lookup, rom_load, rom_read_raw, rom_count, rom_key_at, rom_close,
};

/* units TSV: first two columns hex KEY and ROMSIZE; '#' comments and a header line allowed */
static int rom_load_units(RomCtx *c, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return fail("%s: cannot open", path);
    while (fgets(line, sizeof line, f)) {
        char *e1, *e2;
        unsigned long k, s;
        u32 fl, hs;
        if (line[0] == '#') continue;
        k = strtoul(line[0] == '0' && line[1] == 'x' ? line + 2 : line, &e1, 16);
        if (e1 == line || (*e1 != '\t' && *e1 != ' ')) continue; /* header line */
        while (*e1 == '\t' || *e1 == ' ') e1++;
        if (e1[0] == '0' && e1[1] == 'x') e1 += 2;
        s = strtoul(e1, &e2, 16);
        if (e2 == e1 || k + s > c->len) { fclose(f); return fail("%s: bad line: %s", path, line); }
        fl = raw_range_flag((u32)k);
        {   /* flags column (6th) if present */
            char *q = e2;
            int col;
            for (col = 2; col < 6 && q; col++) {
                q = strchr(q, '\t');
                if (q) q++;
            }
            if (q && *q) fl = (u32)strtoul(q, NULL, 16) & ~T2U_RNC_UNPACKED;
        }
        hs = (u32)s;
        if (!(fl & T2U_INNER) && is_rnc(c->rom + k, (u32)s)) {
            hs = T2Rnc_Unpack(c->rom + k, c->len + RNC_SLACK - k, NULL, 0);
            if (!hs) { fclose(f); return fail("0x%08lX: bad RNC header", k); }
            fl |= T2U_RNC_UNPACKED;
        }
        if (units_add(&c->units, (u32)k, (u32)s, hs, fl, 0) < 0) { fclose(f); return fail("out of memory"); }
    }
    fclose(f);
    qsort(c->units.u, c->units.n, sizeof(Unit), unit_cmp);
    return 0;
}

int T2Asset_OpenRom(const char *romPath, const char *unitsTsv)
{
    RomCtx *c;
    FILE *f;
    long len;
    u8 sha[20];
    char hex[41];
    int i;
    sErr[0] = 0;
    f = fopen(romPath, "rb");
    if (!f) return fail("%s: cannot open", romPath);
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    c = (RomCtx *)calloc(1, sizeof *c);
    if (!c || len <= 0 || !(c->rom = (u8 *)calloc(1, (size_t)len + RNC_SLACK))) {
        fclose(f);
        free(c);
        return fail("out of memory");
    }
    c->len = (size_t)len;
    if (fread(c->rom, 1, c->len, f) != c->len) {
        fclose(f);
        rom_close(c);
        return fail("%s: read error", romPath);
    }
    fclose(f);
    if (T2Rom_Normalize(c->rom, c->len) < 0) {
        rom_close(c);
        return fail("%s: not an N64 ROM image", romPath);
    }
    T2_Sha1(c->rom, c->len, sha);
    for (i = 0; i < 20; i++)
        sprintf(hex + 2 * i, "%02x", sha[i]);
    if (strcmp(hex, T2_ROM_SHA1) != 0) {
        rom_close(c);
        return fail("%s: SHA1 %s is not Turok 2 (USA) " T2_ROM_SHA1, romPath, hex);
    }
    if (unitsTsv && rom_load_units(c, unitsTsv) < 0) {
        rom_close(c);
        return -1;
    }
    T2Asset_SetBackend(&kRomBackend, c);
    return 0;
}
