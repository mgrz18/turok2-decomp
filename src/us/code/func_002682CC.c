#include "common.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

s32 func_002682CC(void *arg0) {
#ifdef TARGET_PORT
    /* the instance header's m_pIntelligence (N64 0x14) by member: its m_Type */
    switch (*(s32 *) ((CInstanceHeader *) arg0)->m_pIntelligence) {
#else
    switch (**(s32 **)((u8 *)arg0 + 0x14)) {
#endif
        case 0:
        case 12:
        default:
            return 1;
        case 1:
        case 4:
        case 9:
        case 11:
            return 0;
    }
}
