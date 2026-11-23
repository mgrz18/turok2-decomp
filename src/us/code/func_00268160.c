#include "common.h"
#include "structs/game_object.h"

/* Host body: the instance's members instead of its N64 offsets (0xD4 m_dwFlags,
 * 0x14 header.m_pIntelligence, whose first word is the definition type). */
s32 func_00268160(u8 *arg0) {
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;

    if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)) {
        return 1;
    }
    switch (*(s32 *) obj->header.m_pIntelligence) {
        case 1:
        case 2:
        case 5:
        case 8:
        case 9:
            return 1;
    }
    return 0;
}
