#include "common.h"
#include "structs/camera.h"

extern s32 gScreenWidth;          /* screen width */
extern s32 gScreenHeight;          /* screen height */
extern f32 gFrameIncrement;          /* the frame's time step */
s32 Cinema_PathPlaying(void);
void CCameraPool__AdvanceCinemaText(CCameraPool *pool, CCamera *cam);
void COnScreen__BeginText(void);
void COnScreen__SelectSmallFont(void);
void COnScreen__DrawString(char *string, s32 x, s32 y, s32 alpha, s32 arg4, s32 arg5, f32 scaleX, f32 scaleY);
f32 func_002982F0(f32 x);       /* sine */
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);

/* A camera's on-screen texts, each frame (outside a cinema path; there
 * CCameraPool__AdvanceCinemaText does it): stacked upwards from 80% down the camera's
 * viewport, centred, drawn half size unless the viewport is smaller than the
 * screen or the text is small, last to first. A text grows in (mode 2), stays
 * its display time (mode 3) and shrinks away (mode 4); finished texts go back
 * to the pool. */
void CCameraPool__AdvanceText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text;
    CCameraText *next;
    f32 xScale;
    f32 yScale;
    f32 size;
    f32 width;
    f32 height;
    f32 left;
    f32 top;
    f32 y;
    f32 x;
    s32 small;

    if (Cinema_PathPlaying()) {
        CCameraPool__AdvanceCinemaText(pool, cam);
        return;
    }
    size = 1.0f;
    height = cam->m_Viewport.m_Height;
    width = cam->m_Viewport.m_Width;
    left = cam->m_Viewport.m_ScreenPosX;
    top = cam->m_Viewport.m_ScreenPosY;
    yScale = height / gScreenHeight;
    xScale = width / gScreenWidth;
    small = 1;
    if (!(xScale < size) && !(yScale < size)) {
        small = 0;
    }
    x = width * 0.5f + left;
    y = height * 0.8f + top - yScale * 18.0f;
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = next) {
        next = text->m_pLast;
        if (text->m_nMode != 0) {
            COnScreen__BeginText();
            if (small || text->m_bSmall) {
                size = 1.0f;
                yScale = size;
                COnScreen__SelectSmallFont();
            } else {
                size = 0.5f;
            }
            COnScreen__DrawString(text->m_pString, x, y, text->m_nAlpha, 1, 1,
                          text->m_Scale * xScale * size, text->m_Scale * yScale * size);
            switch (text->m_nMode) {
                case 2:
                    text->m_nAlpha += 31;
                    if (text->m_nAlpha >= 255) {
                        text->m_nAlpha = 255;
                        text->m_nMode = 3;
                        text->m_Time = text->m_DisplayTime;
                    }
                    text->m_Scale = func_002982F0(text->m_nAlpha * 0.006159986f);
                    break;
                case 3:
                    text->m_nAlpha = 255;
                    text->m_Scale = 1.0f;
                    text->m_Time -= gFrameIncrement;
                    if (text->m_Time < 0.0f) {
                        text->m_nMode = 4;
                        text->m_nAlpha = 128;
                    }
                    break;
                case 4:
                    text->m_Scale -= func_002982F0(255.0f - text->m_nAlpha * 0.012319972f) * (1.0f / 3);
                    if (text->m_Scale < 0.0f) {
                        text->m_Scale = 0.0f;
                    }
                    text->m_nAlpha -= 12;
                    if (text->m_nAlpha < 0) {
                        text->m_nAlpha = 0;
                        text->m_nMode = 0;
                    }
                    break;
            }
            if (text->m_nMode != 0) {
                goto next_line;
            }
        }
        CList__Remove(&cam->m_ActiveTextList, text);
        CList__AddHead(&pool->m_FreeTextList, text);
    next_line:
        y -= yScale * 9.0f / size;
    }
}
