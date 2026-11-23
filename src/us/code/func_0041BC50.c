#include "common.h"
#include "structs/player_options.h"


extern u8 sPlayerNameText_Player1[12];
extern u8 sPlayerNameText_Player2[12];
extern u8 sPlayerNameText_Player3[12];
extern u8 sPlayerNameText_Player4[12];

void PlayerOptions_SetDefaultName(CPlayerOptions *w) {
    s32 i;

    switch (w->m_nController) {
    case 0:
    default:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player1[i];
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player2[i];
        }
        break;
    case 2:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player3[i];
        }
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            w->m_Name[i] = sPlayerNameText_Player4[i];
        }
        break;
    }
}
