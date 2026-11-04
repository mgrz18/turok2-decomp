/* Second VM module copy of func_0042E894 (same source, linked 0x0042E7B4). */
#include "common.h"
#include "structs/player.h"

extern s32 D_8011AC88; /* game state */

extern s32 func_0028D0E0(void);
extern void func_0028D0F0(void);
extern void func_0028D100(void);
extern void func_0042DD94(CPlayer *p);
extern void func_0024CF58(CPlayer *p, CPlayer *q);
extern void func_00284254(s32);

void func_004677B4(CPlayerList *list) {
    CPlayer *p;

    if (D_8011AC88 == 13 && func_0028D0E0() != 0) {
        func_0028D100();
        func_0028D0F0();
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        func_0042DD94(p);
        func_0024CF58(p, p);
        func_00284254(p->m_pCamera);
    }
}
