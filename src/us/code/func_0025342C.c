#ifdef TARGET_PORT
/* The port: the view put the active list head at N64 0x20 and the player's controller and link at
 * 0xA50/0x1320; the typed CPlayerPool/CPlayer (structs/player_list.h, structs/player.h) at their host
 * offsets. GameNoteMenu_SelectYes (loading a save) crashed on the NULL it returned. */
#include "common.h"
#include "structs/player.h"
void *CPlayerPool__FindByController(char *object, sptr arg1) {
    CPlayer *p;

    for (p = ((CPlayerPool *) object)->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if ((sptr) p->m_ContState.m_pContState == arg1) {
            return p;
        }
    }
    return 0;
}
#else
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022A5C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
#ifndef COMMON_H
#ifdef TARGET_PORT
typedef __INTPTR_TYPE__ sptr;
typedef __UINTPTR_TYPE__ uptr;
#else
typedef signed int sptr;
typedef unsigned int uptr;
#endif
#endif

/* Turok 2 player record (Rage Wars: controller 0x698, next 0x16E0). */
typedef struct PlayerRecord {
    char pad0[0xA50];
    s32 controller;             /* 0xA50 */
    char padA54[0x1320 - 0xA54];
    struct PlayerRecord *next;  /* 0x1320 */
} PlayerRecord;

typedef struct Round {
    char pad0[0x20];
    PlayerRecord *players;      /* 0x20 */
} Round;

/* Find the player record driven by controller arg1. */
void *CPlayerPool__FindByController(char *object, sptr arg1) {
    Round *round = (Round *)object;
    PlayerRecord *record = round->players;
    if (record != 0) {
        do {
            if (record->controller == arg1) {
                return record;
            }
            record = record->next;
        } while (record != 0);
    }
    return 0;
}
#endif
