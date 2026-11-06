#include "common.h"
#include "globals/text.h"

extern s32 gScreenWidth;              /* screen width */
extern s32 gScreenHeight;              /* screen height */
extern s32 sCreditsLine;              /* first credits line on screen */
extern f32 gCreditsScroll;              /* scroll position of that line */

extern void COnScreen__BeginText(void);
extern void COnScreen__SelectTinyFont(void);
extern void COnScreen__SetFontScale(f32, f32);
extern void COnScreen__SetFontColor(s32, s32, s32, s32, s32, s32);
extern void COnScreen__DrawTextShadowed(char *, f32, f32, f32, f32, s32, s32, s32);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Draws the scrolling credits, fading lines in at the top of the screen. */
void Credits_Draw(void) {
    s32 line;
    f32 y;
    f32 alpha;
    char *text;
    s32 a;

    line = sCreditsLine;
    y = gCreditsScroll;
    COnScreen__BeginText();
    COnScreen__SelectTinyFont();
    COnScreen__SetFontScale(1.0f, 1.0f);
    do {
        text = gCreditsLines[line];
        if (text != NULL) {
            alpha = MIN(MAX(y, 0.0f), 16.0f) * 16.0f;
            if (alpha > 255.0f) {
                alpha = 255.0f;
            }
            a = alpha;
            COnScreen__SetFontColor(0xFF, 0xFF, 0xFF, a, a, a);
            if (y >= 0.0f) {
                COnScreen__DrawTextShadowed(text, (gScreenWidth * 14) / 284, (s32)y, gScreenWidth * 0.0028169014f,
                              gScreenHeight * 0.0045045046f, 1, a, 4);
            }
            line++;
        }
        y += gScreenHeight * 0.045045048f;
    } while (y <= gScreenHeight);
}
