#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "protos/player.h"

s32 Controller_SetRumbleOptionAll(s32);

typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0;

/* 0x1328-byte list entries (see CPlayerPool__GetIndex) linked through 0x1320. */
#ifdef TARGET_PORT
typedef struct Node002534E0 {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Node002534E0(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_attached[T2_HOFF_Node002534E0(m_ContState.m_pContState)]; Attached002534E0 *attached; }; /* N64 0xA50 */
        struct { u8 _hp_next[T2_HOFF_Node002534E0(m_pNext)]; struct Node002534E0 *next; }; /* N64 0x1320 */
    };
} Node002534E0;
#else
typedef struct Node002534E0 {
    /* 0x0000 */ u8 pad0[0xA50];
    /* 0x0A50 */ Attached002534E0 *attached;
    /* 0x0A54 */ u8 padA54[0x1320 - 0xA54];
    /* 0x1320 */ struct Node002534E0 *next;
} Node002534E0;
#endif

#ifdef TARGET_PORT
typedef struct List002534E0 {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_List002534E0(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_head[T2_HOFF_List002534E0(m_ActiveList.m_pHead)]; Node002534E0 *head; }; /* N64 0x20 */
    };
} List002534E0;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Node002534E0 *head;
} List002534E0;
#endif

void CPlayerPool__EndMatch(List002534E0 *arg0) {
    Node002534E0 *var_a0;
    Node002534E0 *var_a0_2;
    Node002534E0 *var_s0;
    List002534E0 *var_s1;

    var_s0 = (Node002534E0 *) &gEngineApp_m_Settings.match;
    var_s0 = (Node002534E0 *) &gEngineApp_m_Settings.match;
    var_s1 = arg0;
    if ((gEngineApp_m_Settings.match.over == 0) && (gEngineApp_m_Settings.match.done == 0)) {
        Controller_SetRumbleOptionAll(0);
        if (gEngineApp_m_Settings.match.teams != 0) {
            if (gEngineApp_m_Settings.match.teamOver == 0) {
                gEngineApp_m_Settings.match.teamOver = 1;
                var_s0 = var_s1->head;
                var_a0 = var_s0;
                if (var_s0 != NULL) {
                    do {
                        CPlayer__SetState(var_a0, var_s0, 0x12);
                        var_s0 = var_s0->next;
                        var_a0 = var_s0;
                    } while (var_s0 != NULL);
                }
            }
        } else {
            gEngineApp_m_Settings.match.over = 1;
            var_s0 = var_s1->head;
            var_a0_2 = var_s0;
            if (var_s0 != NULL) {
                do {
                    CPlayer__SetState(var_a0_2, var_s0, 0x12);
                    var_s0 = var_s0->next;
                    var_a0_2 = var_s0;
                } while (var_s0 != NULL);
            }
        }
    }
}
