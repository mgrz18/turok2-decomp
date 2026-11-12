#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"
#include "structs/camera.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* The weapon actor's owner, at 0x1A8 (inside m_AI). */
#define OWNER(obj) ((*(CPlayer **)&((CGameObjectInstance *)(obj))->m_AI.m_pParent))

extern char D_800A7094[];       /* "player anim object info" */

extern void CGameObjectInstance__UpdateBounds(CGameObjectInstance *obj, void *info);
extern void Shadow_DrawBlob(CGameObjectInstance *obj, CVector3 *offset, s32 arg2);

/* Reload the animation object info of every player's actor that has one. For
 * the player viewing through cam (while cam's word 0x24 is clear) it is the
 * weapon model instead, turned toward its owner's camera. */
void CPlayerPool__ReloadAnimInfo(CPlayerPool *list, CCamera *cam) {
    CPlayer *p;
    CGameObjectInstance *obj;
    void **res;
    CPlayer *owner;
    CVector3 d;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
#ifdef TARGET_PORT
        if (p->m_pCamera == cam && cam->m_ViewParams.m_nType == 0) { /* N64 0x24 (round 12 a5) */
#else
        if (p->m_pCamera == cam && *(s32 *)((u8 *)cam + 0x24) == 0) {
#endif
            obj = &p->m_WeaponActor;
            if (obj->m_dwFlags & 0x40000) {
                res = CCache__Request(0, (sptr)obj->m_rpObject, obj->m_rpObjectHeader, 4, 0, 0, D_800A7094, 1);
                if (res != NULL) {
                    owner = OWNER(obj);
                    CGameObjectInstance__UpdateBounds(obj, CISet__GetBlock(*res, 0));
                    d.x = owner->m_pCamera->m_vPos.x - obj->header.m_vPos.x;
                    d.y = 0.0f;
                    d.z = owner->m_pCamera->m_vPos.z - obj->header.m_vPos.z;
                    Shadow_DrawBlob(obj, &d, 1);
                    CCache__Unlock(0, res);
                }
            }
        } else if (p->m_CharacterActor.m_dwFlags & 0x40000) {
            void **info = CCache__Request(0, (sptr)p->m_CharacterActor.m_rpObject, p->m_CharacterActor.m_rpObjectHeader, 4, 0, 0, D_800A7094, 1);

            if (info != NULL) {
                CGameObjectInstance__UpdateBounds(&p->m_CharacterActor, CISet__GetBlock(*info, 0));
                CCache__Unlock(0, info);
            }
        }
    }
}
