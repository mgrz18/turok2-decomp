#include "common.h"
#include "structs/controller.h"
#include "structs/game.h"

s32 func_00285ABC();                            /* extern */
s32 func_00288C5C(CEngineApp *, s32);                /* extern */
s32 func_00416644(void *, s32 *, s32, ControllerProfile *, s32); /* extern */
extern s32 D_0043B738;
extern f32 D_800B6D28;
extern s32 D_800B6D64;
extern f32 D_8011AD40;
extern s32 D_8011B118;
extern s32 D_8011F148;

void func_0028917C(CEngineApp *app) {
    if (D_800B6D64 != 0) {
        if (D_8011AD40 > 0.0f) {
            D_8011AD40 -= D_800B6D28;
        }
        if ((D_8011AD40 <= 0.0f) && (D_8011B118 == 0)) {
            D_8011AD40 = 0.0f;
            D_8011B118 = 1;
            func_00416644(app->m_Game.m_MenuPool, &D_0043B738, 0, &D_800F5C78, 0);
        }
        if (D_8011F148 != 0) {
            func_00288C5C(app, 1);
            func_00285ABC();
        }
    }
}
