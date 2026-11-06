#include "common.h"
#include "m2c_macros.h"

extern s32 Render_DrawTextLines(s32, s32, void *, s32, s32, s32, f32, f32);
extern void EngineMode13_Game_Draw(void);
extern f32 D_800A9DF8;
extern u8 sPakCheckText[];
extern s32 gPakCheckStarted;
extern s32 gScreenWidth;
extern s32 gPakCheckHasPak;

void EngineMode00_PakCheck_Draw(s32 arg0) {
    if (gPakCheckStarted != 0) {
        if (gPakCheckHasPak != 0) {
            Render_DrawTextLines(arg0, 1, sPakCheckText, gScreenWidth / 2, 0x5A, 0xA, D_800A9DF8, D_800A9DF8);
            return;
        }
        EngineMode13_Game_Draw();
    }
}
