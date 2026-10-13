#ifndef GFX_H
#define GFX_H

/* One display list command: two words, as libultra's Gfx is to the RSP. The
 * engine writes commands one at a time through D_800D8E24, the current
 * position in the frame's display list (turok3 calls its gDisplayList):
 *
 *     g = D_800D8E24++;
 *     g->w0 = 0xE7000000;      (gDPPipeSync)
 *     g->w1 = 0;
 */
typedef struct {
    /* 0x0 */ u32 w0;
    /* 0x4 */ u32 w1;
} Gfx;

extern Gfx *D_800D8E24;

#endif
