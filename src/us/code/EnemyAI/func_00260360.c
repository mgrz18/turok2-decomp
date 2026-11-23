#ifdef TARGET_PORT
#include "common.h"
#include "structs/game_object.h"

/* host body (round 8 w2): the N64 text below reads the definition at the instance's N64 0x14
 * and the AI's mode and behaviour index at the N64 CAI 0x34 / 0x133. The flags are the
 * body's 0x7E (u16), read the same way. */
s32 EnemyAI_CurrentHasComboEndAnim(CGameObjectInstance *arg0, CAI *arg1) {
    u16 flags = *(u16 *) ((u8 *) &arg0->header.m_pIntelligence->m_Enemy + 0x7E);
    s8 c = arg1->m_Mode;

    if (c == 0x20 || c == 0x3D) {
        return 0;
    }
    switch (arg1->m_Enemy.m_BehaviorIndex) {
        case 0:
            return (flags & 0x1F) != 0;
        case 1:
            return (flags & 0x3E0) != 0;
        case 2:
            return (flags & 0x7C00) != 0;
    }
    return 0;
}
#else
typedef signed int s32;
typedef signed char s8;
typedef unsigned short u16;

s32 EnemyAI_CurrentHasComboEndAnim(char *arg0, char *arg1) {
    char *p = *(char **) (arg0 + 0x14) + 0x14;
    s8 c = *(s8 *) (arg1 + 0x34);

    if (c == 0x20 || c == 0x3D) {
        return 0;
    }
    switch (*(s8 *) (arg1 + 0x133)) {
        case 0:
            return (*(u16 *) (p + 0x7E) & 0x1F) != 0;
        case 1:
            return (*(u16 *) (p + 0x7E) & 0x3E0) != 0;
        case 2:
            return (*(u16 *) (p + 0x7E) & 0x7C00) != 0;
    }
    return 0;
}
#endif
