/* gbi_test: the game's display-list code (include/gbi_t2.h, TARGET_PORT) must produce the
 * same 64-bit command words as libultraship's gbi.h for the same commands, addresses
 * included (host pointers above 4 GB on purpose). Exit 0 when every word matches.
 *   make -C port/tests/gbi LUS=<libultraship checkout> && port/tests/gbi/build/gbi_test */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uintptr_t w0, w1; } Words;

int gbi_test_game(void *buf, void *image, void *mask, void *dl, void *mtx, void *vtx);
int gbi_test_lus(void *buf, void *image, void *mask, void *dl, void *mtx, void *vtx);
int gbi_test_gfx_size(void);
int gbi_test_lus_gfx_size(void);

int main(void) {
    static Words game[256], ref[256];
    /* addresses that do not fit 32 bits on a 64-bit host */
    char *heap = malloc(1 << 20);
    void *image = heap + 0x1000, *mask = heap + 0x2000, *dl = heap + 0x3000, *mtx = heap + 0x4000,
         *vtx = heap + 0x5000;
    int n, m, i, bad = 0;

    printf("sizeof(Gfx): game %d, libultraship %d\n", gbi_test_gfx_size(), gbi_test_lus_gfx_size());
    if (gbi_test_gfx_size() != gbi_test_lus_gfx_size()) {
        return 1;
    }
    memset(game, 0xAA, sizeof(game));
    memset(ref, 0xAA, sizeof(ref));
    n = gbi_test_game(game, image, mask, dl, mtx, vtx);
    m = gbi_test_lus(ref, image, mask, dl, mtx, vtx);
    printf("commands: game %d, libultraship %d; test pointers at %p\n", n, m, (void *) heap);
    if (n != m) {
        bad++;
    }
    for (i = 0; i < (n > m ? n : m); i++) {
        int ok = game[i].w0 == ref[i].w0 && game[i].w1 == ref[i].w1;
        if (!ok) {
            bad++;
        }
        printf("%3d  %016llX %016llX   %016llX %016llX  %s\n", i, (unsigned long long) game[i].w0,
               (unsigned long long) game[i].w1, (unsigned long long) ref[i].w0, (unsigned long long) ref[i].w1,
               ok ? "ok" : "DIFF");
    }
    printf("%s (%d mismatches)\n", bad ? "FAIL" : "OK", bad);
    free(heap);
    return bad != 0;
}
