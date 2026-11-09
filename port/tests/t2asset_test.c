/*
 * t2asset_test.c - host test for port/assets/t2asset.c
 *
 *   t2asset_test ROM UNITS_TSV ARCHIVE.o2r [ARCHIVE2.o2r ...]
 *
 * 1. ROM backend (ROM + units list): reads every load unit (RNC unpacked by port/assets/rnc.c, CRC16
 *    checked) and keeps the bytes.
 * 2. Each archive: same unit set (count and keys), every unit byte-identical to (1),
 *    T2Asset_HostSize, prefix probes, and RAW_RANGE reads at inner addresses (the audio
 *    DMA pattern) against the ROM image.
 * Prints counts and times; exit status 0 only if everything matched.
 */
#include "../assets/t2asset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double now(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

typedef struct { u32 key, romSize, hostSize, flags; const u8 *data; } Ref;

static u32 sSeed = 12345;
static u32 rnd(void) { sSeed = sSeed * 1103515245u + 12345u; return sSeed >> 8; }

int main(int argc, char **argv)
{
    Ref *ref;
    u32 n, i, a;
    double t0, tRom;
    u64 bytes = 0, rnc = 0;
    int bad = 0;
    u8 *rom;
    size_t romLen;
    FILE *f;

    if (argc < 4) {
        fprintf(stderr, "usage: %s ROM UNITS_TSV ARCHIVE.o2r [...]\n", argv[0]);
        return 2;
    }

    /* the ROM itself, for RAW_RANGE checks */
    f = fopen(argv[1], "rb");
    if (!f) { perror(argv[1]); return 2; }
    fseek(f, 0, SEEK_END);
    romLen = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    rom = (u8 *)malloc(romLen);
    if (!rom || fread(rom, 1, romLen, f) != romLen) { fprintf(stderr, "read error\n"); return 2; }
    fclose(f);
    T2Rom_Normalize(rom, romLen);

    /* ---- ROM backend ---- */
    t0 = now();
    if (T2Asset_OpenRom(argv[1], argv[2]) < 0) {
        fprintf(stderr, "OpenRom: %s\n", T2Asset_Error());
        return 1;
    }
    n = T2Asset_UnitCount();
    ref = (Ref *)calloc(n, sizeof *ref);
    for (i = 0; i < n; i++) {
        Ref *r = &ref[i];
        u32 sz;
        r->key = T2Asset_UnitKey(i);
        T2Asset_UnitInfo(r->key, &r->romSize, &r->hostSize, &r->flags);
        sz = r->romSize;
        r->data = (const u8 *)T2Asset_Read(r->key, &sz);
        if (!r->data || sz != r->hostSize) {
            fprintf(stderr, "rom: %08X: %s (size %X want %X)\n", r->key, r->data ? "size" : T2Asset_Error(), sz, r->hostSize);
            bad++;
            continue;
        }
        bytes += sz;
        rnc += (r->flags & T2U_RNC_UNPACKED) != 0;
    }
    tRom = now() - t0;
    printf("rom backend:     %u units (%llu RNC unpacked), %llu bytes, %.3f s (incl. SHA1 + open)\n",
           n, (unsigned long long)rnc, (unsigned long long)bytes, tRom);
    T2Asset_Close();

    /* ---- archives ---- */
    for (a = 3; a < (u32)argc; a++) {
        u32 m, mism = 0, probes = 0, probed = 0, prefix = 0, raw = 0, hs = 0;
        double tOpen, tRead, tRaw;
        t0 = now();
        if (T2Asset_OpenArchive(argv[a]) < 0) {
            fprintf(stderr, "OpenArchive: %s\n", T2Asset_Error());
            bad++;
            continue;
        }
        tOpen = now() - t0;
        m = T2Asset_UnitCount();
        if (m != n) {
            fprintf(stderr, "%s: %u units, ROM backend has %u\n", argv[a], m, n);
            bad++;
        }
        t0 = now();
        for (i = 0; i < n && i < m; i++) {
            const Ref *r = &ref[i];
            u32 sz = r->romSize, key = T2Asset_UnitKey(i), h;
            const u8 *d;
            if (key != r->key) {
                fprintf(stderr, "%s: unit %u key %08X, ROM backend %08X\n", argv[a], i, key, r->key);
                bad++, mism++;
                break;
            }
            d = (const u8 *)T2Asset_Read(key, &sz);
            if (!d || !r->data || sz != r->hostSize || memcmp(d, r->data, sz) != 0) {
                if (mism < 10)
                    fprintf(stderr, "%s: %08X differs (size %X vs %X)%s\n", argv[a], key, sz, r->hostSize, d ? "" : T2Asset_Error());
                mism++;
            }
            T2Asset_Release(d);
            h = T2Asset_HostSize(key, r->romSize);
            if (h != r->hostSize) {
                if (hs < 5) fprintf(stderr, "%s: HostSize(%08X, %X) = %X, want %X\n", argv[a], key, r->romSize, h, r->hostSize);
                hs++;
            }
        }
        tRead = now() - t0;
        /* prefix probes: the 4-byte count probe on every raw (non-RNC, non-audio) unit */
        for (i = 0; i < n; i++) {
            const Ref *r = &ref[i];
            u32 sz = 4;
            const u8 *d;
            if ((r->flags & (T2U_RNC_UNPACKED | T2U_INNER)) || r->romSize < 8 || !r->data)
                continue;
            d = (const u8 *)T2Asset_Read(r->key, &sz);
            probed++;
            prefix += sz == 4;
            /* with a manifest the probe returns 4 bytes; without one the whole unit */
            if (!d || (sz != 4 && sz != r->hostSize) || memcmp(d, r->data, 4) != 0) {
                if (probes < 5) fprintf(stderr, "%s: probe %08X -> %X\n", argv[a], r->key, sz);
                probes++;
            }
            T2Asset_Release(d);
        }
        /* RAW_RANGE: 20000 reads of 0x300 bytes at random inner addresses of the three audio units */
        t0 = now();
        for (i = 0; i < 20000; i++) {
            static const u32 lo[3] = { T2_ROOT_AUDIO_CTL, T2_ROOT_AUDIO_DATA, T2_ROOT_SOUND_DATA };
            static const u32 hi[3] = { T2_ROOT_AUDIO_DATA, T2_ROOT_SOUND_DATA, T2_ROOT_CARTRIDGE };
            u8 buf[0x300];
            u32 k = i % 3, addr = lo[k] + rnd() % (hi[k] - lo[k] - 0x300);
            if (T2Asset_ReadInto(addr, 0, buf, sizeof buf) < 0 || memcmp(buf, rom + addr, sizeof buf) != 0) {
                if (raw < 5) fprintf(stderr, "%s: raw read %08X: %s\n", argv[a], addr, T2Asset_Error());
                raw++;
            }
        }
        tRaw = now() - t0;
        printf("%s: %u units, open %.3f s, read+compare all %.3f s, %u mismatches, %u HostSize errors, "
               "%u/%u 4-byte probes served as prefix (%u errors), 20000 audio reads %.3f s (%u errors)\n",
               argv[a], m, tOpen, tRead, mism, hs, prefix, probed, probes, tRaw, raw);
        bad += mism + probes + raw + hs;
        T2Asset_Close();
    }

    for (i = 0; i < n; i++)
        T2Asset_Release(ref[i].data);
    free(ref);
    free(rom);
    printf("%s\n", bad ? "FAIL" : "OK");
    return bad ? 1 : 0;
}
