#include "common.h"
#include "m2c_macros.h"

extern s32 D_8011F290[];

void func_0028D23C(void) {
    s32 *p = D_8011F290;

    if (p[2] != 0) {
        p[2]--;
        if (p[2] == 0) {
            p[1] = 0;
        }
    }
}
