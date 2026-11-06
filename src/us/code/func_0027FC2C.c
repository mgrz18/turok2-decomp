#include "common.h"
#include "structs/camera.h"
#include "globals/render.h"

extern u8 *sCinemaTextFadeOut;
extern u8 *D_800B2CD0;
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);
void CList__AddTail(CList *list, void *element);
void CCameraText__Reset(CCameraText *text);

/* CCameraPool__TakeText, inlined: takes a free text from the pool and puts it on the
 * camera's active texts. */
static __inline__ CCameraText *TakeText(CCameraPool *pool, CCamera *cam) {
    CCameraText *text = pool->m_FreeTextList.m_pHead;

    if (text != NULL) {
        CCameraText__Reset(text);
        CList__Remove(&pool->m_FreeTextList, text);
        CList__AddTail(&cam->m_ActiveTextList, text);
    }
    return text;
}

s32 Cinema_IsActive(void);

/* Shows a string over a camera for a second, one text per line, centred, 80
 * pixels above the bottom, with no id (-1). When Cinema_IsActive() says so, the
 * texts the camera shows are put in mode 4 and the new ones start in mode 1
 * instead of 2. Returns the last line's text. */
CCameraText *CCameraPool__AddText(CCameraPool *pool, CCamera *cam, u8 *string) {
    CCameraText *cur;
    CCameraText *text;
    u8 *str;
    u8 *line;
    u8 *c;
    s32 mode;
    s32 id;

    mode = 2;
    if (Cinema_IsActive()) {
        cur = cam->m_ActiveTextList.m_pHead;
        if (cur != NULL) {
            mode = 1;
            do {
                cur->m_nMode = 4;
                cur = cur->m_pNext;
            } while (cur != NULL);
        }
    }
    id = -1;
    str = string;
    text = NULL;
    if (str == sCinemaTextFadeOut) {
        str = D_800B2CD0;
    }
    c = str;
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
                    text->m_DisplayTime = 15.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = gScreenWidth / 2;
                    text->m_PositionY = gScreenHeight - 80;
                }
                line = c + 1;
            }
        }
    }
    return text;
}
