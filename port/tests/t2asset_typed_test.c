/*
 * t2asset_typed_test.c - typed units through the port's load path, in host byte order.
 *
 *   t2asset_typed_test ARCHIVE.o2r EXPECT.txt [ROM UNITS_TSV]
 *
 * EXPECT.txt comes from port/tests/t2asset_typed.py (the area tools' big-endian parsers on the
 * ROM). Each unit named there is read with T2Asset_Read (whole unit, as CCache__Request with the
 * host size) and checked:
 *   F  a field read with a native load (memcpy into u16/u32/f32) equals the parser's value:
 *      the swap plan made it host order;
 *   B  bytes the engine reads byte-wise (or that Fast3D reads as big-endian texels/TLUTs) are
 *      still in ROM order;
 *   V  a transposed Vtx record, untransposed on the host as CUSet__Transpose does, equals the
 *      parser's vertex (s16 x y z, u16 flag, s16 s t: swapped by reversing planes).
 * Passes: the archive backend (miniz, t2/swap from the archive), then, with ROM, the ROM backend
 * (ROM + units list) with T2Asset_LoadSwapFile(ARCHIVE). Also checks that with no swap table the same fields read
 * big-endian (the archive keeps ROM order). Exit 0 only if everything matched.
 */
#include "../assets/t2asset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { const u8 *d; u32 key, size; } Cache;
static Cache sCache;

static const u8 *unit(u32 key, u32 *size)
{
    if (sCache.d && sCache.key == key) {
        *size = sCache.size;
        return sCache.d;
    }
    T2Asset_Release(sCache.d);
    sCache.size = 0;
    sCache.d = (const u8 *)T2Asset_Read(key, &sCache.size);
    sCache.key = key;
    *size = sCache.size;
    return sCache.d;
}

static int hexbytes(const char *s, u8 *out, int cap)
{
    int n = 0;
    while (s[0] && s[1] && n < cap && s[0] != '\n') {
        unsigned v;
        if (sscanf(s, "%2x", &v) != 1)
            break;
        out[n++] = (u8)v;
        s += 2;
    }
    return n;
}

/* native = 1: host-order loads (swap table active); 0: big-endian reads (ROM order) */
static u32 load(const u8 *p, const char *type, int native)
{
    u32 w = (!strcmp(type, "u16") || !strcmp(type, "s16")) ? 2 : 4;
    if (native) {
        if (w == 2) { u16 v; memcpy(&v, p, 2); return v; }
        { u32 v; memcpy(&v, p, 4); return v; }
    }
    return w == 2 ? (u32)(p[0] << 8 | p[1]) : (u32)p[0] << 24 | (u32)p[1] << 16 | (u32)p[2] << 8 | p[3];
}

static int run(const char *what, const char *expect, int native, u32 *checks)
{
    FILE *f = fopen(expect, "r");
    char line[512];
    int bad = 0;
    u32 n = 0;
    if (!f) { perror(expect); return 1; }
    while (fgets(line, sizeof line, f)) {
        char kind, type[8];
        unsigned key, off;
        u32 size;
        const u8 *d;
        if (line[0] == '#')
            continue;
        if (sscanf(line, "%c %x %x", &kind, &key, &off) != 3)
            continue;
        d = unit(key, &size);
        if (!d) {
            if (bad++ < 10) fprintf(stderr, "%s: %08X: %s\n", what, key, T2Asset_Error());
            continue;
        }
        n++;
        if (kind == 'F') {
            unsigned want;
            if (sscanf(line, "%*c %*x %*x %7s %x", type, &want) != 2 || off + 4 > size) {
                bad++;
                continue;
            }
            if (!strcmp(type, "u16") || !strcmp(type, "s16"))
                want &= 0xFFFF;
            if (load(d + off, type, native) != want && bad++ < 10)
                fprintf(stderr, "%s: %08X+%X %s = %08X, parser %08X\n", what, key, off, type, load(d + off, type, native), want);
        } else if (kind == 'B') {
            u8 want[64];
            const char *h = strchr(strchr(strchr(line, ' ') + 1, ' ') + 1, ' ') + 1;
            int m = hexbytes(h, want, sizeof want);
            if ((off + (u32)m > size || memcmp(d + off, want, (size_t)m) != 0) && bad++ < 10)
                fprintf(stderr, "%s: %08X+%X bytes not in ROM order\n", what, key, off);
        } else if (kind == 'V') {
            int idx, v[11], i;
            u8 rec[16];
            int32_t w, h;
            if (sscanf(line, "%*c %*x %*x %d %d %d %d %d %d %d %d %d %d %d", &idx, &v[0], &v[1], &v[2], &v[3], &v[4],
                       &v[5], &v[6], &v[7], &v[8], &v[9]) != 11) {
                bad++;
                continue;
            }
            w = (int32_t)load(d + off, "s32", native), h = (int32_t)load(d + off + 4, "s32", native);
            if (w != 16 || idx >= h || off + 8 + 16u * (u32)h > size) {
                if (bad++ < 10) fprintf(stderr, "%s: %08X+%X: Vtx table %d x %d\n", what, key, off, w, h);
                continue;
            }
            for (i = 0; i < 16; i++) /* CUSet__Transpose: byte i of record idx is plane i */
                rec[i] = d[off + 8 + (u32)i * (u32)h + (u32)idx];
            {
                int16_t x = (int16_t)load(rec, "s16", native), y = (int16_t)load(rec + 2, "s16", native), z = (int16_t)load(rec + 4, "s16", native);
                u16 fl = (u16)load(rec + 6, "u16", native);
                int16_t s = (int16_t)load(rec + 8, "s16", native), t = (int16_t)load(rec + 10, "s16", native);
                if ((x != v[0] || y != v[1] || z != v[2] || fl != v[3] || s != v[4] || t != v[5] || rec[12] != v[6] ||
                     rec[13] != v[7] || rec[14] != v[8] || rec[15] != v[9]) && bad++ < 10)
                    fprintf(stderr, "%s: %08X Vtx[%d] = %d %d %d %d %d %d, parser %d %d %d %d %d %d\n", what, key, idx, x, y, z,
                            fl, s, t, v[0], v[1], v[2], v[3], v[4], v[5]);
            }
        }
    }
    fclose(f);
    T2Asset_Release(sCache.d);
    memset(&sCache, 0, sizeof sCache);
    *checks = n;
    return bad;
}

static void report(const char *what, int bad, u32 n)
{
    u32 ap, np, rf;
    T2Asset_SwapStats(&ap, &np, &rf);
    printf("%-28s %u checks, %d errors (swap %s: %u plans applied, %u units without a plan, %u refused)\n", what, n, bad,
           T2Asset_SwapActive() ? "on" : "off", ap, np, rf);
}

int main(int argc, char **argv)
{
    int bad = 0, b;
    u32 n;
    if (argc < 3) {
        fprintf(stderr, "usage: %s ARCHIVE.o2r EXPECT.txt [ROM UNITS_TSV]\n", argv[0]);
        return 2;
    }
    if (T2Asset_OpenArchive(argv[1]) < 0) {
        fprintf(stderr, "OpenArchive: %s\n", T2Asset_Error());
        return 1;
    }
    if (!T2Asset_SwapActive()) {
        fprintf(stderr, "%s: no swap table (format 1 archive?) or big-endian host\n", argv[1]);
        return 1;
    }
    b = run("archive", argv[2], 1, &n);
    report("archive (host order):", b, n);
    bad += b;
    T2Asset_SetSwapTable("", 0); /* drop the table: the same units read big-endian */
    b = run("archive, no swap", argv[2], 0, &n);
    report("archive (ROM order):", b, n);
    bad += b;
    T2Asset_Close();
    if (argc > 4) {
        if (T2Asset_OpenRom(argv[3], argv[4]) < 0 || T2Asset_LoadSwapFile(argv[1]) < 0) {
            fprintf(stderr, "rom: %s\n", T2Asset_Error());
            return 1;
        }
        b = run("rom", argv[2], 1, &n);
        report("rom + t2/swap (host order):", b, n);
        bad += b;
        T2Asset_Close();
    }
    printf("%s\n", bad ? "FAIL" : "OK");
    return bad ? 1 : 0;
}
