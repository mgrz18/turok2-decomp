#include "common.h"
/* float -> u32 as the R4300 does it (cvt.w.s: a negative value wraps). arm64's fcvtzu saturates
 * negatives to 0 (MACOS.md section 9, tools/port/f2ucheck.py); x86-64 happens to wrap. */
#ifdef TARGET_PORT
#define T2_F2U(x) ((u32)(s32)(x))
#else
#define T2_F2U(x) ((u32)(x))
#endif
#include "gfx.h"
#include "gbi_t2.h"
#include "structs/camera.h"

void Render_InvalidateModeCache(void);

/* gDPFillRectangle with float corners, each converted to an unsigned int. */
#define FILL_RECT(ulx, uly, lrx, lry)                                               \
    {                                                                               \
        Gfx *_g = gDList++;                                                     \
        _g->w0 = 0xF6000000 | (T2_F2U(lrx) & 0x3FF) << 14 | (T2_F2U(lry) & 0x3FF) << 2;   \
        _g->w1 = (T2_F2U(ulx) & 0x3FF) << 14 | (T2_F2U(uly) & 0x3FF) << 2;          \
    }

/* Draws a one-pixel frame around a camera's viewport (the borders of a
 * split screen): four fill rectangles in fill mode, colour 0x10001. */
void CCamera__DrawBorder(CCamera *cam) {
    f32 x, y, width, height;

    Render_InvalidateModeCache();
    x = cam->m_Viewport.m_ScreenPosX;
    y = cam->m_Viewport.m_ScreenPosY;
    width = cam->m_Viewport.m_Width;
    height = cam->m_Viewport.m_Height;
    GFX_CMD(0xE7000000, 0);                     /* gDPPipeSync */
    GFX_CMD(0xE3000A01, 0x300000);              /* gDPSetCycleType(G_CYC_FILL) */
    GFX_CMD(0xFCFFFFFF, 0xFFFE793C);            /* gDPSetCombineMode */
    GFX_CMD(0xE200001C, 0);                     /* gDPSetRenderMode */
    GFX_CMD(0xF7000000, 0x10001);               /* gDPSetFillColor */
    FILL_RECT(x, y, x + width - 1.0f, y);
    FILL_RECT(x, y + height - 1.0f, x + width - 1.0f, y + height - 1.0f);
    FILL_RECT(x, y, x, y + height - 1.0f);
    FILL_RECT(x + width - 1.0f, y, x + width, y + height - 1.0f);
}
