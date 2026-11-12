#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x78];
    /* 0x78 */ s32 unk78;
    /* 0x7C */ s32 unk7C;
} Target;

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* host body (round 10 y2): the CAI by member. The N64 view (0x78 the target, 0x7C its time) wrote
 * the target's low half and the time's bits over the host CAI's m_pParent: an enemy hurt by
 * another actor (EnemyAI_OnDamage: the Blind One's minions under the boss's own fire) got a
 * garbage parent and crashed in AI_FindBestTarget. The time stays a float's bits (the ROM's word). */
void AI_SetTargetOverride(sptr arg0, Target *arg1, sptr arg2, s32 arg3) {
    CAI *ai = (CAI *) arg1;

    ai->m_pTargetOverride = (struct CGameObjectInstance *) arg2;
    *(s32 *) &ai->m_TargetOverrideTime = arg3;
}
#else
void AI_SetTargetOverride(sptr arg0, Target *arg1, sptr arg2, s32 arg3) {
    arg1->unk78 = arg2;
    arg1->unk7C = arg3;
}
#endif
