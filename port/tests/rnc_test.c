/*
 * rnc_test.c - checks port/assets/rnc.c on a ROM: every 'RNC' + method 1/2 header whose
 * packed CRC-16 matches is unpacked (both CRCs verified). Optionally writes
 * "offset sha1-less crc" lines for comparison with tools/assets/rnc.py.
 *   rnc_test baserom.us.z64
 */
#include "../assets/rnc.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *f;
    long len;
    uint8_t *rom, *out = NULL;
    size_t cap = 0, i;
    unsigned streams = 0, bad = 0, m[3] = {0, 0, 0};
    unsigned long long bytes = 0;

    if (argc < 2) {
        fprintf(stderr, "usage: %s ROM\n", argv[0]);
        return 2;
    }
    if (!(f = fopen(argv[1], "rb")))
        return perror(argv[1]), 2;
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    rom = (uint8_t *)malloc((size_t)len);
    if (!rom || fread(rom, 1, (size_t)len, f) != (size_t)len)
        return fprintf(stderr, "read failed\n"), 2;
    fclose(f);
    for (i = 0; i + RNC_HEADER_LEN <= (size_t)len; i++) {
        RncHeader h;
        int64_t n;
        if (rom[i] != 'R' || Rnc_ReadHeader(rom + i, (size_t)len - i, &h) != RNC_OK)
            continue;
        if (h.packed > (size_t)len - i - RNC_HEADER_LEN ||
            Rnc_Crc16(rom + i + RNC_HEADER_LEN, h.packed) != h.crcPacked)
            continue;
        if (h.unpacked > cap) {
            cap = h.unpacked;
            out = (uint8_t *)realloc(out, cap);
        }
        n = Rnc_Unpack(rom + i, (size_t)len - i, out, cap, 1);
        streams++;
        m[h.method]++;
        if (n < 0) {
            bad++;
            fprintf(stderr, "0x%08zX: %s\n", i, Rnc_ErrorString(n));
        } else {
            bytes += (unsigned long long)n;
        }
    }
    printf("%u RNC streams (method 1: %u, method 2: %u), %llu bytes unpacked, %u failed\n",
           streams, m[1], m[2], bytes, bad);
    free(out);
    free(rom);
    return bad ? 1 : 0;
}
