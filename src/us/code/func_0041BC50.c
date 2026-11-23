#include "common.h"
#include "structs/player_options.h"


extern u8 D_800B5E80[12];
extern u8 D_800B5E8C[12];
extern u8 D_800B5E98[12];
extern u8 D_800B5EA4[12];

void PlayerOptions_SetDefaultName(CPlayerOptions *w) {
    s32 i;

    switch (w->m_nController) {
    case 0:
    default:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = D_800B5E80[i];
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = D_800B5E8C[i];
        }
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = D_800B5E98[i];
        }
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = D_800B5EA4[i];
        }
        break;
    }
}
