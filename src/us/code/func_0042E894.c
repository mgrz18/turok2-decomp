/* CPlayer code: func_0042E894, 0x0042E894-0x0042E91C (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"

typedef struct PlayerList {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ CPlayer *head;
} PlayerList;

extern s32 D_8011AC88; /* game state */

extern s32 func_0028D0E0(void);
extern void func_0028D0F0(void);
extern void func_0028D100(void);
extern void func_0042DE74(CPlayer *p);
extern void func_0024CF58(CPlayer *p, CPlayer *q);
extern void func_00284254(s32);

void func_0042E894(PlayerList *list) {
    CPlayer *p;

    if (D_8011AC88 == 13 && func_0028D0E0() != 0) {
        func_0028D100();
        func_0028D0F0();
    }
    for (p = list->head; p != NULL; p = p->m_pNext) {
        func_0042DE74(p);
        func_0024CF58(p, p);
        func_00284254(p->m_pCamera);
    }
}
