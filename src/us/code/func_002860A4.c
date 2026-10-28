#include "common.h"

extern s32 D_800C2030;              /* screen width */
extern s32 D_800C2034;              /* screen height */
extern s32 D_800F6CA0;              /* first credits line on screen */
extern f32 D_8011F120;              /* scroll position of that line */
extern char *D_800B66DC[];          /* credits lines */

extern void func_00277F6C(void);
extern void func_0027845C(void);
extern void COnScreen__SetFontScale(f32, f32);
extern void COnScreen__SetFontColor(s32, s32, s32, s32, s32, s32);
extern void func_0027B274(char *, f32, f32, f32, f32, s32, s32, s32);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Draws the scrolling credits, fading lines in at the top of the screen. */
void func_002860A4(void) {
    s32 line;
    f32 y;
    f32 alpha;
    char *text;
    s32 a;

    line = D_800F6CA0;
    y = D_8011F120;
    func_00277F6C();
    func_0027845C();
    COnScreen__SetFontScale(1.0f, 1.0f);
    do {
        text = D_800B66DC[line];
        if (text != NULL) {
            alpha = MIN(MAX(y, 0.0f), 16.0f) * 16.0f;
            if (alpha > 255.0f) {
                alpha = 255.0f;
            }
            a = alpha;
            COnScreen__SetFontColor(0xFF, 0xFF, 0xFF, a, a, a);
            if (y >= 0.0f) {
                func_0027B274(text, (D_800C2030 * 14) / 284, (s32)y, D_800C2030 * 0.0028169014f,
                              D_800C2034 * 0.0045045046f, 1, a, 4);
            }
            line++;
        }
        y += D_800C2034 * 0.045045048f;
    } while (y <= D_800C2034);
}
