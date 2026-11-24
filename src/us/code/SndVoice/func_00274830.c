#include "common.h"
#include "structs/game_object.h"
#include "protos/sndp.h"


void SndVoice_Play(CGameObjectInstance *obj) {
    alSndpPlay(obj->m_rpObjectHeader + 0x84);
}
