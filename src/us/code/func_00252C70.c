#include "common.h"
#include "structs/game_object.h"
#include "structs/camera.h"

typedef struct Player {
    /* 0x0000 */ CGameObjectInstance actor;
    /* 0x0288 */ CGameObjectInstance weapon;
    /* 0x0510 */ u8 pad510[0x51C - 0x510];
    /* 0x051C */ CCamera *camera;
    /* 0x0520 */ u8 pad520[0x1320 - 0x520];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Player *head;
} PlayerList;

/* The weapon actor's owner, at 0x1A8 (inside m_AI). */
#define OWNER(obj) (*(Player **)((u8 *)(obj) + 0x1A8))

extern char D_800A7094[];       /* "player anim object info" */

extern void **func_0020367C(s32, s32, s32, s32, s32, s32, void *, s32);
extern void *func_002017D4(void *data, s32 arg1);
extern void func_0021E7F8(CGameObjectInstance *obj, void *info);
extern void func_0021E03C(CGameObjectInstance *obj, CVector3 *offset, s32 arg2);
extern void func_002051F4(s32 arg0, void **res);

/* Reload the animation object info of every player's actor that has one. For
 * the player viewing through cam (while cam's word 0x24 is clear) it is the
 * weapon model instead, turned toward its owner's camera. */
void func_00252C70(PlayerList *list, CCamera *cam) {
    Player *p;
    CGameObjectInstance *obj;
    void **res;
    Player *owner;
    CVector3 d;

    for (p = list->head; p != NULL; p = p->next) {
        if (p->camera == cam && *(s32 *)((u8 *)cam + 0x24) == 0) {
            obj = &p->weapon;
            if (obj->m_dwFlags & 0x40000) {
                res = func_0020367C(0, (s32)obj->m_rpObject, obj->m_rpObjectHeader, 4, 0, 0, D_800A7094, 1);
                if (res != NULL) {
                    owner = OWNER(obj);
                    func_0021E7F8(obj, func_002017D4(*res, 0));
                    d.x = owner->camera->m_vPos.x - obj->header.m_vPos.x;
                    d.y = 0.0f;
                    d.z = owner->camera->m_vPos.z - obj->header.m_vPos.z;
                    func_0021E03C(obj, &d, 1);
                    func_002051F4(0, res);
                }
            }
        } else if (p->actor.m_dwFlags & 0x40000) {
            void **info = func_0020367C(0, (s32)p->actor.m_rpObject, p->actor.m_rpObjectHeader, 4, 0, 0, D_800A7094, 1);

            if (info != NULL) {
                func_0021E7F8(&p->actor, func_002017D4(*info, 0));
                func_002051F4(0, info);
            }
        }
    }
}
