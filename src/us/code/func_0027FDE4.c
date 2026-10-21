#include "common.h"
#include "structs/camera.h"

extern s32 D_800C2034;          /* screen height */
extern f32 D_800B6D28;          /* the frame's time step */
void func_00277F6C(void);
void func_0027845C(void);
void func_00278CDC(char *string, s32 x, s32 y, s32 alpha, s32 arg4, s32 arg5, f32 scaleX, f32 scaleY);
f32 func_002982F0(f32 x);       /* sine */
f32 func_002974C0(f32 x);       /* cosine */
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);

/* A camera's on-screen texts, each frame (LibTEngine's CCameraPool
 * AdvanceText): stacked 9 pixels apart from 81% down the screen, drawn
 * last to first, faded in (mode 2) and out (mode 4) over a fifteenth of a
 * second per unit of time with a quarter sine wave; when one has faded out
 * the waiting ones (mode 1) start in. Finished texts go back to the pool. */
void func_0027FDE4(CCameraPool *pool, CCamera *cam) {
    CCameraText *text;
    CCameraText *cur;
    CCameraText *prev;
    CCameraText *other;
    f32 f;                      /* the gap between lines, then the time step */
    f32 wave;

    func_00277F6C();
    func_0027845C();
    f = 9.0f;
    prev = cam->m_ActiveTextList.m_pHead;
    if (prev != NULL) {
        prev->m_PositionY = D_800C2034 * 0.81f;
        for (text = prev->m_pNext; text != NULL; text = text->m_pNext) {
            text->m_PositionY = prev->m_PositionY + f;
            prev = text;
        }
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = text->m_pLast) {
        func_00278CDC(text->m_pString, text->m_PositionX, text->m_PositionY, text->m_nAlpha, 1, 1, 1.0f, 1.0f);
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = text->m_pLast) {
        f = D_800B6D28 * (1.0f / 15.0f);
        cur = text;
        switch (text->m_nMode) {
            case 1:                 /* waiting */
                text->m_nAlpha = 0;
                break;
            case 2:                 /* fading in */
                wave = func_002982F0(cur->m_ModeTime * 1.5707964f);
                cur->m_ModeTime += f;
                cur->m_nAlpha = wave * 255.0f;
                if (cur->m_ModeTime > 1.0f) {
                    cur->m_nMode = 3;
                    cur->m_ModeTime = 0.0f;
                }
                break;
            case 3:                 /* shown */
                cur->m_nAlpha = 255;
                break;
            case 4:                 /* fading out: the waiting ones start after */
                wave = func_002974C0(cur->m_ModeTime * 1.5707964f);
                cur->m_ModeTime += f;
                cur->m_nAlpha = wave * 255.0f;
                if (cur->m_ModeTime > 1.0f) {
                    for (other = cam->m_ActiveTextList.m_pHead; other != NULL; other = other->m_pNext) {
                        if (other->m_nMode == 1) {
                            other->m_nMode = 2;
                            other->m_ModeTime = 0.0f;
                        }
                    }
                    cur->m_nMode = 0;
                    cur->m_ModeTime = 0.0f;
                }
                break;
            default:
                cur->m_nAlpha = 0;
                cur->m_nMode = 0;
                break;
        }
    }
    for (text = cam->m_ActiveTextList.m_pTail; text != NULL; text = prev) {
        prev = text->m_pLast;
        if (text->m_nMode == 0) {
            CList__Remove(&cam->m_ActiveTextList, text);
            CList__AddHead(&pool->m_FreeTextList, text);
        }
    }
}
