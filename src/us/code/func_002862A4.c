#include "common.h"
#include "structs/game.h"

extern s32 D_8011AD00;              /* credits finished */
extern f32 D_8011B11C;              /* time left before leaving the credits */
extern f32 D_800B6D28;              /* frame time */
extern s32 D_800B6D74;
extern s32 D_800B6D70;
extern s32 D_8011ACF8;
extern s32 D_800C2034;              /* screen height */
extern s32 D_800F6CA0;              /* first credits line on screen */
extern f32 D_8011F120;              /* scroll position of that line */
extern char *D_800B66DC[];          /* credits lines */

extern void CPlayerPool__GetFirstActive(void *);
extern void CEngineApp__StartGameAtWarp(s32, s32);
extern void CEngineApp__SetupFadeTo(s32, s32);
extern void Sound__PlaySpeech(s32);

/* Scrolls the credits up; once past the last line, waits and leaves. */
void Credits_Update(s32 arg0) {
    f32 spacing;

    if (D_8011AD00 != 0) {
        D_8011B11C -= D_800B6D28;
        if (D_8011B11C <= 0.0f) {
            CPlayerPool__GetFirstActive(&D_80119870.m_PlayerList);
            if (D_800B6D74 != 0) {
                if (D_800B6D70 == 0) {
                    CEngineApp__StartGameAtWarp(arg0, 0x1B57);
                    return;
                }
            } else if (D_800B6D70 == 0) {
                return;
            }
            CEngineApp__SetupFadeTo(arg0, 1);
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
            Sound__PlaySpeech(0x266);
            D_8011B11C = 67.5f;
        }
    }
}
