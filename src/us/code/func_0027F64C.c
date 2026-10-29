#include "common.h"
#include "gfx.h"
#include "structs/camera.h"

void func_0020B5B8(void);

#define GFX(a, b) { Gfx *_g = D_800D8E24++; _g->w0 = (a); _g->w1 = (b); }
/* gDPFillRectangle with float corners, each converted to an unsigned int. */
#define FILL_RECT(ulx, uly, lrx, lry)                                               \
    {                                                                               \
        Gfx *_g = D_800D8E24++;                                                     \
        _g->w0 = 0xF6000000 | ((u32) (lrx) & 0x3FF) << 14 | ((u32) (lry) & 0x3FF) << 2;   \
        _g->w1 = ((u32) (ulx) & 0x3FF) << 14 | ((u32) (uly) & 0x3FF) << 2;          \
    }

/* Draws a one-pixel frame around a camera's viewport (the borders of a
 * split screen): four fill rectangles in fill mode, colour 0x10001. */
void func_0027F64C(CCamera *cam) {
    f32 x, y, width, height;

    func_0020B5B8();
    x = cam->m_Viewport.m_ScreenPosX;
    y = cam->m_Viewport.m_ScreenPosY;
    width = cam->m_Viewport.m_Width;
    height = cam->m_Viewport.m_Height;
    GFX(0xE7000000, 0);                     /* gDPPipeSync */
    GFX(0xE3000A01, 0x300000);              /* gDPSetCycleType(G_CYC_FILL) */
    GFX(0xFCFFFFFF, 0xFFFE793C);            /* gDPSetCombineMode */
    GFX(0xE200001C, 0);                     /* gDPSetRenderMode */
    GFX(0xF7000000, 0x10001);               /* gDPSetFillColor */
    FILL_RECT(x, y, x + width - 1.0f, y);
    FILL_RECT(x, y + height - 1.0f, x + width - 1.0f, y + height - 1.0f);
    FILL_RECT(x, y, x, y + height - 1.0f);
    FILL_RECT(x + width - 1.0f, y, x + width, y + height - 1.0f);
}
