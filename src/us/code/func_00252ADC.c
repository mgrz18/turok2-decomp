#include "common.h"
#include "structs/player.h"
#include "structs/game_object.h"

extern s32 D_8011AC8C;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;          /* frame time scale */

extern s32 Cinema_PathPlaying(void);
extern void *func_00275F7C(void);
extern void func_00275D34(void *mgr, void *sound);
extern void func_0021F1B4(CGameObjectInstance *obj);
extern void func_0021A344(CGameObjectInstance *obj);

/* Whether the player's camera has a menu open. */
static __inline__ s32 menu_open(CPlayer *p) {
    void *cam = p->m_pCamera;
    s32 open = 0;

    if (cam != NULL) {
        open = *(s32 *)((u8 *)cam + 0x54C) != 0;
    }
    return open;
}

/* Animate each living player's weapon model on its own (frame time kept),
 * then put it back on the player; stop the weapon sounds otherwise. */
void func_00252ADC(CPlayerList *list) {
    CPlayer *p;

    if (D_8011AC8C == 11 || D_8011AC8C == 8) {
        return;
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if (Cinema_PathPlaying() || p->m_Data.m_Lives == 0) {
            func_00275D34(func_00275F7C(), p->m_pLoopingSound_WeaponBackground);
            p->m_pLoopingSound_WeaponBackground = NULL;
            func_00275D34(func_00275F7C(), p->m_pLoopingSound_WeaponFiring);
            p->m_pLoopingSound_WeaponFiring = NULL;
        } else if (D_8011ACA4 == 0) {
            if (!menu_open(p)) {
                CGameObjectInstance *weapon = &p->m_WeaponActor;
                s32 old = p->m_CharAnim;
                f32 frameTime = D_800B6D28;

                p->m_WeaponActor.m_dwFlags |= 0x200;
                weapon->m_dwHotPointFlags = 16;
                weapon->m_dwFlags &= ~0x20000;
                func_0021F1B4(weapon);
                func_0021A344(weapon);
                D_800B6D28 = frameTime;
                if (old != p->m_CharAnim) {
                    p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
                }
                p->m_WeaponActor.header.m_vPos = p->m_CharacterActor.header.m_vPos;
                p->m_WeaponActor.m_RotY = p->m_CharacterActor.m_RotY;
                p->m_WeaponActor.header.m_pCurrentRegion = p->m_CharacterActor.header.m_pCurrentRegion;
                p->m_WeaponActor.m_qGround = p->m_CharacterActor.m_qGround;
                p->m_WeaponActor.m_dwFlags &= ~0x200;
            }
        }
    }
}
