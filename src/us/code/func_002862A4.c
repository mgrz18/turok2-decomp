#include "common.h"

extern s32 D_8011AD00;              /* credits finished */
extern f32 D_8011B11C;              /* time left before leaving the credits */
extern f32 D_800B6D28;              /* frame time */
extern u8 D_80119870[];
extern s32 D_800B6D74;
extern s32 D_800B6D70;
extern s32 D_8011ACF8;
extern s32 D_800C2034;              /* screen height */
extern s32 D_800F6CA0;              /* first credits line on screen */
extern f32 D_8011F120;              /* scroll position of that line */
extern char *D_800B66DC[];          /* credits lines */

extern void func_002532A8(void *);
extern void func_002886D0(s32, s32);
extern void func_00288C5C(s32, s32);
extern void func_0027580C(s32);

/* Scrolls the credits up; once past the last line, waits and leaves. */
void func_002862A4(s32 arg0) {
    f32 spacing;

    if (D_8011AD00 != 0) {
        D_8011B11C -= D_800B6D28;
        if (D_8011B11C <= 0.0f) {
            func_002532A8(D_80119870);
            if (D_800B6D74 != 0) {
                if (D_800B6D70 == 0) {
                    func_002886D0(arg0, 0x1B57);
                    return;
                }
            } else if (D_800B6D70 == 0) {
                return;
            }
            func_00288C5C(arg0, 1);
            D_8011ACF8 = 1;
        }
        return;
    }
    if (D_800C2034 >= 223) {
        D_8011F120 -= 1.0f;
    } else {
        D_8011F120 -= 0.5f;
    }
    spacing = D_800C2034 * 0.045045048f;
    if (D_8011F120 <= -spacing) {
        D_800F6CA0++;
        D_8011F120 += spacing;
        if (D_800B66DC[D_800F6CA0] == NULL) {
            D_8011AD00 = 1;
            func_0027580C(0x266);
            D_8011B11C = 67.5f;
        }
    }
}
