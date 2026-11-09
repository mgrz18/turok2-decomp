/* gbi_test, game side: display lists built by the game's own code and include/gbi_t2.h,
 * compiled exactly as turok2_game compiles the game (TARGET_PORT, port_shim.h). */
#include "common.h"
#include "gfx.h"
#include "gbi_t2.h"

Gfx *gDList;

void Hud_DrawBoxOutline(s32 x0, s32 y0, s32 x1, s32 y1, s32 border, u8 r, u8 g, u8 b, u8 a);
void Hud_DrawMaskedBlockAt(u16 *image, u8 *mask, s32 x, s32 y, s32 width, s32 height, s32 s, s32 t);

/* Writes the test sequence into buf; returns the number of commands. */
int gbi_test_game(Gfx *buf, void *image, void *mask, void *dl, void *mtx, void *vtx) {
    gDList = buf;
    Hud_DrawBoxOutline(10, 20, 200, 120, 3, 0x11, 0x22, 0x33, 0x44);
    Hud_DrawMaskedBlockAt(image, mask, 40, 50, 32, 32, 4, 6);
    gSPDisplayList(gDList++, dl);
    gSPMatrixLoad(gDList++, mtx);
    GFX_CMD(0xDB060008, GBI_ADDR(vtx));                                  /* gSPSegment(2, vtx) */
    GFX_CMD(0x01000000 | (4 << 12) | (4 << 1), GBI_SEGADDR(0x02000040)); /* gSPVertex(seg 2 + 0x40, 4, 0) */
    GFX_CMD(0xDF000000, 0);
    return gDList - buf;
}

int gbi_test_gfx_size(void) {
    return GFX_SIZE;
}
