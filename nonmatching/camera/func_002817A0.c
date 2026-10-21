#include "common.h"
#include "structs/camera.h"

extern char *D_800B35D4;
extern char *D_800B2CD0;
extern s32 D_800C2030;      /* screen width */
extern s32 D_800C2034;      /* screen height */
void CList__Remove(CList *list, void *element);
void CList__AddHead(CList *list, void *element);
void CList__AddTail(CList *list, void *element);
void func_00281B58(CCameraText *text);

/* CCameraPool__AddMultiLineText (LibTEngine's name; Turok 3's has no id):
 * shows a string over a camera for `secs`, one text per line, centred, 80
 * pixels above the bottom. Texts the camera shows under the same id go
 * first. Returns the last line's text. */
CCameraText *func_002817A0(CCameraPool *pool, CCamera *cam, char *string, s32 id, f32 secs) {
    CCameraText *text;
    CCameraText *next;
    char *line;
    char *c;

    text = cam->m_ActiveTextList.m_pHead;
    while (text != NULL) {
        next = text->m_pNext;
        if (text->m_nId == id) {
            CList__Remove(&cam->m_ActiveTextList, text);
            CList__AddHead(&pool->m_FreeTextList, text);
        }
        text = next;
    }
    text = NULL;
    if (string == D_800B35D4) {
        string = D_800B2CD0;
    }
    c = string;
    line = c;
    while (*c != '\0') {
        c++;
        if (*c == '\n' || *c == '\0') {
            if (c != line) {
                text = pool->m_FreeTextList.m_pHead;
                if (text != NULL) {
                    func_00281B58(text);
                    CList__Remove(&pool->m_FreeTextList, text);
                    CList__AddTail(&cam->m_ActiveTextList, text);
                }
                if (text != NULL) {
                    text->m_nMode = 2;
                    text->m_pString = line;
                    text->m_nAlpha = 0;
                    text->m_Scale = 1.0f;
                    text->m_nId = id;
                    text->m_bSmall = 0;
                    text->m_ModeTime = 0.0f;
                    text->m_ModeDuration = 0.0f;
                    text->m_ScaleX = 1.0f;
                    text->m_ScaleY = 1.0f;
                    text->m_PositionX = D_800C2030 / 2;
                    text->m_PositionY = D_800C2034 - 80;
                    text->m_DisplayTime = secs * 15.0f;
                }
                line = c + 1;
            }
        }
    }
    return text;
}
