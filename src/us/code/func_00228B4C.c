#include "common.h"
#include "structs/camera.h"

/* An element going back on its owner's list at 0x14. */
typedef struct {
    /* 0x00 */ u8 pad0[0x16];
    /* 0x16 */ s16 unk16;
} ListNode;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ CList m_List;
} ListOwner;

void CList__AddHead(CList *, void *);               /* extern */
void CList__Remove();                               /* extern */

void func_00228B4C(ListOwner *arg0, ListNode *arg1) {
    arg1->unk16 = 0;
    CList__Remove();
    CList__AddHead(&arg0->m_List, arg1);
}
