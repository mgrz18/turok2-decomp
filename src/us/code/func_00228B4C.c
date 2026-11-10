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

#ifdef TARGET_PORT
/* The port: CList__Remove gets its arguments (the N64 C called it with none and relied on
 * $a0/$a1 still holding the set and the light: the set's used list is at its start). */
#include "structs/particle.h"
void CDLightSet__Free(CDLightSet *arg0, CDLight *arg1) {
    arg1->m_bEnabled = 0;
    CList__Remove(&arg0->m_UsedList, arg1);
    CList__AddHead(&arg0->m_FreeList, arg1);
}
#else
void CDLightSet__Free(ListOwner *arg0, ListNode *arg1) {
    arg1->unk16 = 0;
    CList__Remove();
    CList__AddHead(&arg0->m_List, arg1);
}
#endif
