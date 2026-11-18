/* t2tex.c (round 19 h2): textures libultraship's Fast3D cannot draw as the game loads them.
 *
 * Texture kind 3 (Texture_Load, func_00206B30.c) is CI4 texels with an IA16 palette
 * (gDPSetTextureLUT(G_TT_IA16)): each 16-bit palette entry is an 8-bit intensity and an 8-bit
 * alpha. LUS decodes every CI palette as RGBA5551 and ignores the TLUT type, so these textures
 * came out as random saturated colours (the light shaft and the mist of the Mother's arena,
 * the hub pad's cut-scene 7601: cyan, green, magenta and blue where the N64 draws white-yellow
 * and brown-grey; docs/port/FIDELITY.md 11-h2). Here the CI4 texels and the palette are
 * expanded into an IA16 image (the same intensity and the full 8-bit alpha), which Fast3D
 * decodes as the RDP does; Texture_Load then loads that image as G_IM_FMT_IA 16b.
 *
 * Fast3D caches textures by their address, so each image keeps its own buffer for the life of
 * the process: a (texels, palette) pair whose bytes change (the game loaded another texture at
 * the same address) gets a new buffer, never an old address with new contents. The buffers are
 * small (at most 64x64 texels, 8 KB) and there are few of these textures. */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const uint8_t *tex;
    const uint8_t *pal;
    uint32_t hash;
    int w, h;
    uint8_t *img;
} T2TexIA16;

#define T2TEX_SLOTS 512
static T2TexIA16 sSlots[T2TEX_SLOTS];

static uint32_t fnv(uint32_t h, const uint8_t *p, size_t n) {
    size_t i;
    for (i = 0; i < n; i++) {
        h = (h ^ p[i]) * 16777619u;
    }
    return h;
}

/* The IA16 image (big-endian 16-bit texels, as the RDP reads them) of a w x h CI4 texture with
 * a 16-entry IA16 palette; NULL when it does not fit (w * h > 4096: more than a LoadBlock). */
const void *T2Port_TextureCI4IA16(const void *texels, const void *palette, int w, int h) {
    const uint8_t *tex = (const uint8_t *) texels;
    const uint8_t *pal = (const uint8_t *) palette;
    size_t n = (size_t) w * (size_t) h;
    uint32_t hash;
    uintptr_t k;
    int i, probe;
    T2TexIA16 *s = NULL;

    if (tex == NULL || pal == NULL || w <= 0 || h <= 0 || n > 4096) {
        return NULL;
    }
    hash = fnv(fnv(2166136261u, tex, n / 2), pal, 32);
    k = ((uintptr_t) tex >> 3) ^ ((uintptr_t) pal >> 5);
    for (probe = 0; probe < T2TEX_SLOTS; probe++) {
        T2TexIA16 *c = &sSlots[(k + probe) % T2TEX_SLOTS];
        if (c->tex == tex && c->pal == pal && c->w == w && c->h == h) {
            if (c->hash == hash) {
                return c->img;
            }
            s = c; /* new contents at the same addresses: a new buffer below */
            break;
        }
        if (c->tex == NULL) {
            s = c;
            break;
        }
    }
    if (s == NULL) {
        s = &sSlots[k % T2TEX_SLOTS]; /* full: reuse a slot (its old buffer stays allocated) */
    }
    s->img = (uint8_t *) malloc(n * 2);
    if (s->img == NULL) {
        s->tex = NULL;
        return NULL;
    }
    for (i = 0; i < (int) n; i++) {
        int idx = (tex[i >> 1] >> ((i & 1) ? 0 : 4)) & 0xF;
        s->img[i * 2] = pal[idx * 2];         /* intensity */
        s->img[i * 2 + 1] = pal[idx * 2 + 1]; /* alpha */
    }
    s->tex = tex;
    s->pal = pal;
    s->w = w;
    s->h = h;
    s->hash = hash;
    return s->img;
}
