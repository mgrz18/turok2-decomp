#include "common.h"
#include "structs/menu.h"

extern f32 gEngineApp_m_Settings_speed;     /* gEngineApp_m_Settings + 0x1C */
extern s32 gMpPlayerSpeed;
extern char D_800B6698[];
extern char D_800B66B0[];
extern char D_800C0E9C[];  /* "%3d" */
extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

s32 MpMenu_PlayerSpeedLabel(MenuItem *a) {
    if (gEngineApp_m_Settings_speed == 1.0f) {
        a->text = D_800B66B0;
    } else {
        a->text = D_800B6698;
        sprintf(a->text + (MenuItem_TextLength(a) - 3), D_800C0E9C, gMpPlayerSpeed - 5);
    }
    return 0;
}
