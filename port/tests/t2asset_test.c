/*
 * t2asset_test.c - host test for port/assets/t2asset.c
 *
 *   t2asset_test ROM UNITS_TSV ARCHIVE.o2r [ARCHIVE2.o2r ...]
 *
 * 1. ROM backend (ROM + units list): reads every load unit (RNC unpacked by port/assets/rnc.c, CRC16
 *    checked) and keeps the bytes.
 * 2. Each archive: same unit set (count and keys), every unit byte-identical to (1) after
 *    (1)'s bytes go through the archive's swap plans (T2Asset_SwapUnit; ROM order when the
 *    archive has no t2/swap), swapping twice gives (1) back, T2Asset_HostSize, prefix probes
 *    (served from the swapped unit), and reads at inner addresses of the three audio units
 *    (the audio DMA pattern) against the ROM image / the swapped VM windows.
 * 3. With a format 2 archive: the ROM backend with T2Asset_LoadSwapFile(archive) gives the
 *    same host-order bytes as the archive.
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

typedef struct { u32 key, romSize, hostSize, flags; const u8 *data; u8 *host; } Ref;

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
        {   /* expected host-order bytes: (1) through this archive's plans */
            u32 dbl = 0, planned = 0;
            for (i = 0; i < n; i++) {
                Ref *r = &ref[i];
                free(r->host);
                r->host = NULL;
                if (!r->data) continue;
                r->host = (u8 *)malloc(r->hostSize + 1);
                memcpy(r->host, r->data, r->hostSize);
                if (T2Asset_SwapUnit(r->key, r->host, r->hostSize) < 0) {
                    fprintf(stderr, "%s: swap %08X: %s\n", argv[a], r->key, T2Asset_Error());
                    bad++;
                    continue;
                }
                if (memcmp(r->host, r->data, r->hostSize) != 0) {
                    u8 *twice = (u8 *)malloc(r->hostSize + 1);
                    planned++;
                    memcpy(twice, r->host, r->hostSize);
                    T2Asset_SwapUnit(r->key, twice, r->hostSize);
                    if (memcmp(twice, r->data, r->hostSize) != 0) {
                        if (dbl < 5) fprintf(stderr, "%s: %08X: swapping twice does not give the ROM bytes\n", argv[a], r->key);
                        dbl++;
                    }
                    free(twice);
                }
            }
            printf("%s: swap %s, %u units change byte order, %u double-swap errors\n", argv[a],
                   T2Asset_SwapActive() ? "active" : "off (no t2/swap or big-endian host)", planned, dbl);
            bad += dbl;
        }
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
            if (!d || !r->host || sz != r->hostSize || memcmp(d, r->host, sz) != 0) {
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
            if (!d || (sz != 4 && sz != r->hostSize) || memcmp(d, r->host, 4) != 0) {
                if (probes < 5) fprintf(stderr, "%s: probe %08X -> %X\n", argv[a], r->key, sz);
                probes++;
            }
            T2Asset_Release(d);
        }
        /* RAW_RANGE / VM_WINDOW: 20000 reads of 0x300 bytes at random inner addresses of the three
         * audio units (the wave table has no plan: ROM bytes; the two VM windows: swapped) */
        t0 = now();
        for (i = 0; i < 20000; i++) {
            static const u32 lo[3] = { T2_ROOT_AUDIO_CTL, T2_ROOT_AUDIO_DATA, T2_ROOT_SOUND_DATA };
            static const u32 hi[3] = { T2_ROOT_AUDIO_DATA, T2_ROOT_SOUND_DATA, T2_ROOT_CARTRIDGE };
            u8 buf[0x300];
            u32 k = i % 3, addr = lo[k] + rnd() % (hi[k] - lo[k] - 0x300), j;
            const u8 *want = rom + addr;
            for (j = 0; j < n; j++)
                if (ref[j].key == lo[k] && ref[j].host && addr - lo[k] + sizeof buf <= ref[j].hostSize)
                    want = ref[j].host + (addr - lo[k]);
            if (T2Asset_ReadInto(addr, 0, buf, sizeof buf) < 0 || memcmp(buf, want, sizeof buf) != 0) {
                if (raw < 5) fprintf(stderr, "%s: raw read %08X: %s\n", argv[a], addr, T2Asset_Error());
                raw++;
            }
        }
        tRaw = now() - t0;
        printf("%s: %u units, open %.3f s, read+compare all %.3f s, %u mismatches, %u HostSize errors, "
               "%u/%u 4-byte probes served as prefix (%u errors), 20000 audio reads %.3f s (%u errors)\n",
               argv[a], m, tOpen, tRead, mism, hs, prefix, probed, probes, tRaw, raw);
        bad += mism + probes + raw + hs;
        {
            u32 ap, np, rf;
            T2Asset_SwapStats(&ap, &np, &rf);
            printf("%s: swap counters (this check + the reads): %u plans applied, %u loads without a plan, %u refused\n", argv[a], ap, np, rf);
            bad += rf;
        }
        T2Asset_Close();
        /* 3. the ROM backend with this archive's plans: same host-order bytes */
        if (T2Asset_OpenRom(argv[1], argv[2]) == 0 && T2Asset_LoadSwapFile(argv[a]) == 0) {
            u32 rm = 0;
            t0 = now();
            for (i = 0; i < n; i++) {
                u32 sz = 0;
                const u8 *d = (const u8 *)T2Asset_Read(ref[i].key, &sz);
                if (!d || !ref[i].host || sz != ref[i].hostSize || memcmp(d, ref[i].host, sz) != 0) {
                    if (rm < 5) fprintf(stderr, "rom + %s plans: %08X differs\n", argv[a], ref[i].key);
                    rm++;
                }
                T2Asset_Release(d);
            }
            printf("rom backend + %s's t2/swap: %u units, %u mismatches, %.3f s\n", argv[a], n, rm, now() - t0);
            bad += rm;
            T2Asset_Close();
        } else {
            printf("rom backend + %s's t2/swap: skipped (%s)\n", argv[a], T2Asset_Error());
            T2Asset_Close();
        }
    }

    for (i = 0; i < n; i++) {
        T2Asset_Release(ref[i].data);
        free(ref[i].host);
    }
    free(ref);
    free(rom);
    printf("%s\n", bad ? "FAIL" : "OK");
    return bad ? 1 : 0;
}
