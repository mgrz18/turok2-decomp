/* testdl.c - a display list built with libultraship's GBI (F3DEX2, the game's ucode
 * family) to prove the Fast3D path end to end before any game code runs: clear the
 * frame with a fill rectangle and draw one smaller rectangle on top. */
#include <libultraship/libultraship.h>

#define RGBA16_FILL(r, g, b) ((GPACK_RGBA5551(r, g, b, 1) << 16) | GPACK_RGBA5551(r, g, b, 1))

/* Fast3D treats a fill whose colour image equals the depth image as a Z clear (and skips
 * it), so both must be set, to different buffers, as the game's own frame setup does. */
static u16 sColorImage[320 * 240];
static u16 sDepthImage[320 * 240];

Gfx gT2TestDL[] = {
    gsDPPipeSync(),
    gsDPSetDepthImage(sDepthImage),
    gsDPSetColorImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, sColorImage),
    gsDPSetCycleType(G_CYC_FILL),
    gsDPSetRenderMode(G_RM_NOOP, G_RM_NOOP2),
    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, 320, 240),
    gsDPSetFillColor(RGBA16_FILL(0x20, 0x60, 0x20)), /* Turok green */
    gsDPFillRectangle(0, 0, 319, 239),
    gsDPPipeSync(),
    gsDPSetFillColor(RGBA16_FILL(0xF8, 0xB0, 0x00)),
    gsDPFillRectangle(120, 80, 199, 159),
    gsDPPipeSync(),
    gsSPEndDisplayList(),
};
