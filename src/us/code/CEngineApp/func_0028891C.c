#include "common.h"
#include "structs/game.h"

void CEngineApp__InitFade(CEngineApp *arg0) {
    arg0->m_Settings.fadeMode = 1;
    arg0->m_Settings.fade = 0;
    arg0->m_Settings.fastFade = 0;
}
