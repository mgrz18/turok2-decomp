#include "common.h"
#include "structs/game_object.h"

s32 func_00243414(s32, s32, s32);
s32 func_00402404(s32, s32, s32);
s32 func_00402A10(s32, s32);
s32 func_00403660(s32, s32, s32);

extern s32 D_00430CA4;
extern s32 D_80110048;
extern s32 func_0025E4B8;

void func_00402888(s32 arg0, CAI *arg1) {
    D_80110048 = 0x1D4D;
    arg1->m_pModeTable = (CAIModeInfo *) &D_00430CA4;
    arg1->m_pfStep = func_00402A10;
    arg1->m_pfHurt = func_00402404;
    arg1->m_pfEffects = &func_0025E4B8;
    arg1->m_pfDraw = func_00403660;
    arg1->m_VisBits = -1;
    func_00243414(arg0, (s32) arg1, 0);
}
