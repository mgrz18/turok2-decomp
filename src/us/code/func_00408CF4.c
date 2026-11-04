#include "common.h"
#include "structs/game_object.h"

extern CListedInstance *D_8010FEB4;  /* active actors */

typedef struct {
    u8 data[0x18];
} DamageEvent;

extern f32 func_0021149C(f32 scale);                                      /* random 0..scale */
extern void func_00246690(void *actor, s32 event, CVector3 pos, s32 arg);
extern void func_00245828(DamageEvent *event, void *source, s32 damage, f32 radius, s32 flags, void *shield);
extern void func_0025B2D4(CListedInstance *actor, void *ai, DamageEvent *event);

/* One time in four, hurt the first live player actor. */
void func_00408CF4(void) {
    DamageEvent event;
    CListedInstance *actor;

    if (func_0021149C(100.0f) > 25.0f) {
        return;
    }
    for (actor = D_8010FEB4; actor != NULL; actor = actor->m_pNext) {
        if (actor->m_Instance.header.m_pIntelligence->m_Type == 1 && actor->m_Instance.m_AI.m_Health != 0) {
            func_00246690(actor, 0x17C, actor->m_Instance.header.m_vPos, 0);
            func_00245828(&event, NULL, 0xFF00, 51.199997f, 0x100000, NULL);
            func_0025B2D4(actor, &actor->m_Instance.m_AI.m_dwFlags, &event);
            return;
        }
    }
}
