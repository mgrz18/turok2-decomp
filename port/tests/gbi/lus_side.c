/* gbi_test, reference side: the same display list written with libultraship's own gbi.h
 * (F3DEX_GBI_2), i.e. what Fast3D expects. */
#include <stdint.h>
#include <stdbool.h>
#include <libultraship/libultra/gbi.h>

#define CLAMP0(v) ((v) > 0 ? (v) : 0)

int gbi_test_lus(Gfx *buf, void *image, void *mask, void *dl, void *mtx, void *vtx) {
    Gfx *g = buf;
    int x0 = 10, y0 = 20, x1 = 200, y1 = 120, border = 3;
    int x = 40, y = 50, width = 32, height = 32, s = 4, t = 6;

    gDPPipeSync(g++);
    gDPSetPrimColor(g++, 255, 255, 0x11, 0x22, 0x33, 0x44);
    gDPFillRectangle(g++, CLAMP0(x0), CLAMP0(y0), CLAMP0(x1), CLAMP0(y0 + border));
    gDPFillRectangle(g++, CLAMP0(x0), CLAMP0(y1 - border), CLAMP0(x1), CLAMP0(y1));
    gDPFillRectangle(g++, CLAMP0(x0), CLAMP0(y0 + border), CLAMP0(x0 + border), CLAMP0(y1 - border));
    gDPFillRectangle(g++, CLAMP0(x1 - border), CLAMP0(y0 + border), CLAMP0(x1), CLAMP0(y1 - border));

    gDPLoadMultiBlock_4b(g++, mask, 0x100, 0, G_IM_FMT_I, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPLoadTextureBlock(g++, image, G_IM_FMT_RGBA, G_IM_SIZ_16b, width, height, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
    gDPSetTile(g++, G_IM_FMT_I, G_IM_SIZ_4b, width / 16, 0x100, 1, 0, 0, 0, 0, 0, 0, 0);
    gDPSetTileSize(g++, 1, 0, 0, (width - 1) << 2, (height - 1) << 2);
    gSPTextureRectangle(g++, x << 2, y << 2, (x + (width - s)) << 2, (y + (height - t)) << 2, 0, s << 5, t << 5, 1 << 10, 1 << 10);

    __gSPDisplayList(g++, dl); /* LUS: gSPDisplayList is a function (resource paths) */
    gSPMatrix(g++, mtx, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
    __gSPSegment(g++, 2, vtx);
    __gSPVertex(g++, (Vtx *) (uintptr_t) 0x02000041, 4, 0); /* segmented, bit 0 = "resolve through the segment table" */
    gSPEndDisplayList(g++);
    return g - buf;
}

int gbi_test_lus_gfx_size(void) {
    return sizeof(Gfx);
}
