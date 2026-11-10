#include "common.h"
#include "structs/game_object.h"
#include "rw/abu_protos.h"

f32 Random_Blend(f32 arg0, f32 arg1);
s32 EnemyRam_Collision(sptr, sptr);


void EnemyRam_Setup(sptr arg0, CAI *arg1) {
    f32 temp_f20;
    f32 temp_f21;

    temp_f20 = -40.96f;
    temp_f21 = 40.96f;
    arg1->m_pfCollision = (void *) EnemyRam_Collision;
    arg1->m_vTargetOffset.x = Random_Blend(temp_f20, temp_f21);
    arg1->m_vTargetOffset.y = Random_Blend(temp_f20, temp_f21);
    arg1->m_vTargetOffset.z = Random_Blend(temp_f20, temp_f21);
}
