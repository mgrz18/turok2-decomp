#include "common.h"
#include "structs/game_object.h"

extern s32 D_8012F9D8;

void Primagen_EnterMode6(CGameObjectInstance *arg0) {
    s32 *flags;

    /* through a pointer: a plain struct store gets scheduled after D_8012F9D8's */
    flags = &arg0->m_dwFlags;
    *flags = (s32) (*flags | 0x2000);
    D_8012F9D8 = 0;
}
