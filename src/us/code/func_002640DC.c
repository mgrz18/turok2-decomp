#include "common.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the AI's m_Action (N64 0x37) and m_Mode (0x34) by member. */
s32 Platform_GetModeAnim(sptr arg0, void *arg1) {
    CAI *ai = (CAI *) arg1;

    switch (ai->m_Action) {
        case 3:
            return 0x5280;
        case 1:
        case 2:
            return 0x5294;
        case 0:
            switch (ai->m_Mode) {
                case 0:
                case 2:
                    return 0x526C;
                case 1:
                case 3:
                    return 0x5276;
            }
            break;
        case 4:
            return 0x528A;
        default:
            return 0x526C;
    }
    return 0x526C;
}
#else
s32 Platform_GetModeAnim(sptr arg0, void *arg1) {
    switch (*(s8 *)((u8 *)arg1 + 0x37)) {
        case 3:
            return 0x5280;
        case 1:
        case 2:
            return 0x5294;
        case 0:
            switch (*(s8 *)((u8 *)arg1 + 0x34)) {
                case 0:
                case 2:
                    return 0x526C;
                case 1:
                case 3:
                    return 0x5276;
            }
            break;
        case 4:
            return 0x528A;
        default:
            return 0x526C;
    }
    return 0x526C;
}
#endif
