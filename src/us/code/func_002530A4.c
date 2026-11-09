#include "common.h"
#include "m2c_macros.h"

#ifdef TARGET_PORT
/* The port: the same through CPlayerPool / CPlayer / CScene (host layouts). */
#include "structs/player.h"
#include "structs/world.h"

s32 GameObject_IsSolid(CGameObjectInstance *);
void CPlayerPool__RegisterActors(CPlayerPool *pool, CScene *w) {
    CPlayer *p;
    s32 n;

    for (p = pool->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        p->m_CharacterActor.m_CollisionYOffset = 0;
        p->m_WeaponActor.m_CollisionYOffset = 0;
        if (GameObject_IsSolid(&p->m_CharacterActor) != 0) {
            n = w->m_nDrawActors;
            if (n != 0x200) {
                w->m_pDrawActors[n] = &p->m_CharacterActor;
                w->m_nDrawActors = n + 1;
            }
            n = w->m_nMovers;
            if (n != 0x80) {
                w->m_pMovers[n] = &p->m_CharacterActor;
                w->m_nMovers = n + 1;
            }
        }
    }
}
#else
s32 GameObject_IsSolid(s32);

void CPlayerPool__RegisterActors(void *arg0, void *arg1)
{
  s32 temp_v1;
  s32 temp_v1_2;
  void *var_a0;
  void *var_s0;
  var_s0 = *((void **) (((s8 *) arg0) + 0x20));
  if (var_s0 != 0)
  {
    ;
    do
    {
      *((s32 *) (((s8 *) var_s0) + 0x54)) = 0;
      *((s32 *) (((s8 *) var_s0) + 0x2DC)) = 0;
      if (GameObject_IsSolid((s32) var_s0) != 0)
      {
        temp_v1 = *((s32 *) (((s8 *) arg1) + 0x8EC));
        if (temp_v1 != 0x200)
        {
          *((void **) (((s8 *) (arg1 + (temp_v1 << 2))) + 0xEC)) = var_s0;
          *((s32 *) (((s8 *) arg1) + 0x8EC)) = (s32) (temp_v1 + 1);
        }
        temp_v1_2 = *((s32 *) (((s8 *) arg1) + 0xAF0));
        if (temp_v1_2 != 0x80)
        {
          *((void **) (((s8 *) (arg1 + (temp_v1_2 << 2))) + 0x8F0)) = var_s0;
          *((s32 *) (((s8 *) arg1) + 0xAF0)) = (s32) (temp_v1_2 + 1);
        }
      }
      var_s0 = *((void **) (((s8 *) var_s0) + 0x1320));
      var_a0 = var_s0;
    }
    while (var_s0 != 0);
  }
}
#endif
