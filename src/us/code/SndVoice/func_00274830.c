#include "common.h"
#include "structs/game_object.h"

s32 alSndpPlay(s32);                         /* extern */

void SndVoice_Play(CGameObjectInstance *obj) {
    alSndpPlay(obj->m_rpObjectHeader + 0x84);
}
