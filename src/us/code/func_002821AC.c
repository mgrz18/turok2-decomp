#include "common.h"
#include "structs/camera.h"

extern u8 *D_800B35D4;
extern u8 *D_800B2CD0;
extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);
void CList__AddTail(CList *list, void *element);
void func_00281B58(CCameraText *text);

/* func_00281AB4, inlined: takes a free text from the pool and puts it on the
 * camera's active texts. */
static __inline__ CCameraText *TakeText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        func_00281B58(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}

/* Shows a string over a camera for `secs`, one text per line, centred, 80
 * pixels above the bottom, starting in the given mode and with the given id.
 * Returns the last line's text. */
CCameraText *func_002821AC(CCameraPool *pool, CCamera *cam, u8 *string, s32 mode, f32 secs, s32 id) {
    CCameraText *text;
    u8 *line;
    u8 *c;

    text = NULL;
    if (string == D_800B35D4) {
        string = D_800B2CD0;
    }
    c = string;
    line = c;
    while (*c != 0) {
        c++;
        if (*c == '\n' || *c == 0) {
            if (line != c) {
                text = TakeText(pool, cam);
                if (text != NULL) {
                    text->m_nMode = mode;
                    text->m_pString = (char *)line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_DisplayTime = secs * 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = D_800C2030 / 2;
                    text->m_PositionY = D_800C2034 - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}
