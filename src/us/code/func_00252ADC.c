#include "common.h"
#include "structs/game_object.h"

typedef struct Player {
    /* 0x0000 */ CGameObjectInstance actor;
    /* 0x0288 */ CGameObjectInstance weapon;
    /* 0x0510 */ u8 pad510[0x51C - 0x510];
    /* 0x051C */ void *camera;
    /* 0x0520 */ u8 pad520[0x52A - 0x520];
    /* 0x052A */ s16 lives;
    /* 0x052C */ u8 pad52C[0xC04 - 0x52C];
    /* 0x0C04 */ s32 fieldC04;
    /* 0x0C08 */ u8 padC08[0x1288 - 0xC08];
    /* 0x1288 */ void *weaponBackgroundSound;
    /* 0x128C */ void *weaponFiringSound;
    /* 0x1290 */ u8 pad1290[0x1320 - 0x1290];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

extern s32 D_8011AC8C;
extern s32 D_8011ACA4;
extern f32 D_800B6D28;          /* frame time scale */

extern s32 Cinema_PathPlaying(void);
extern void *func_00275F7C(void);
extern void func_00275D34(void *mgr, void *sound);
extern void func_0021F1B4(CGameObjectInstance *obj);
extern void func_0021A344(CGameObjectInstance *obj);

/* Whether the player's camera has a menu open. */
static __inline__ s32 menu_open(Player *p) {
    void *cam = p->camera;
    s32 open = 0;

    if (cam != NULL) {
        open = *(s32 *)((u8 *)cam + 0x54C) != 0;
    }
    return open;
}

/* Animate each living player's weapon model on its own (frame time kept),
 * then put it back on the player; stop the weapon sounds otherwise. */
void func_00252ADC(PlayerList *list) {
    Player *p;

    if (D_8011AC8C == 11 || D_8011AC8C == 8) {
        return;
    }
    for (p = list->head; p != NULL; p = p->next) {
        if (Cinema_PathPlaying() || p->lives == 0) {
            func_00275D34(func_00275F7C(), p->weaponBackgroundSound);
            p->weaponBackgroundSound = NULL;
            func_00275D34(func_00275F7C(), p->weaponFiringSound);
            p->weaponFiringSound = NULL;
        } else if (D_8011ACA4 == 0) {
            if (!menu_open(p)) {
                CGameObjectInstance *weapon = &p->weapon;
                s32 old = p->fieldC04;
                f32 frameTime = D_800B6D28;

                p->weapon.m_dwFlags |= 0x200;
                weapon->m_dwHotPointFlags = 16;
                weapon->m_dwFlags &= ~0x20000;
                func_0021F1B4(weapon);
                func_0021A344(weapon);
                D_800B6D28 = frameTime;
                if (old != p->fieldC04) {
                    p->actor.m_CurrentAnimation.m_bCycleCompleted = 0;
                }
                p->weapon.header.m_vPos = p->actor.header.m_vPos;
                p->weapon.m_RotY = p->actor.m_RotY;
                p->weapon.header.m_pCurrentRegion = p->actor.header.m_pCurrentRegion;
                p->weapon.m_qGround = p->actor.m_qGround;
                p->weapon.m_dwFlags &= ~0x200;
            }
        }
    }
}
