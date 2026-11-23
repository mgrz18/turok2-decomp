#include "common.h"
#include "m2c_macros.h"

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
