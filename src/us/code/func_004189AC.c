#include "common.h"
#include "structs/menu.h"

extern f32 D_8011AAF4;     /* D_8011AAD8 + 0x1C */
extern s32 D_800C1EA8;
extern char D_800B6698[];
extern char D_800B66B0[];
extern char D_800C0E9C[];  /* "%3d" */
extern s32 func_004160F0(MenuItem *a);
extern void func_0029E3E0(char *dst, char *fmt, ...);

s32 func_004189AC(MenuItem *a) {
    if (D_8011AAF4 == 1.0f) {
        a->text = D_800B66B0;
    } else {
        a->text = D_800B6698;
        func_0029E3E0(a->text + (func_004160F0(a) - 3), D_800C0E9C, D_800C1EA8 - 5);
    }
    return 0;
}
