#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x50];
    /* 0x050 */ s32 unk50;
    /* 0x054 */ u8 pad54[0x140 - 0x54];
    /* 0x140 */ u8 ai[1];
} Obj;

typedef struct {
    /* 0x000 */ Obj *obj;
} State;

extern State D_8012FB70;
extern u8 D_00431D7C[];
extern void func_00246108(Obj *obj, void *ai, CAITargets *t, s32 f1, s32 f2);
extern void func_00253D68(CPlayer *player, void *arg1);
extern void func_0024CF58(CPlayer *player, CPlayer *other);
extern void func_00243414(Obj *obj, void *ai, s32 state);

/* func_0040BD60 */
static __inline__ f32 getTargetDist(Obj *obj, CGameObjectInstance **found) {
    CAITargets t;
    f32 dist;

    dist = -1.0f;
    *found = NULL;
    func_00246108(obj, obj->ai, &t, 1, 0);
    if (t.m_Path.m_pTarget != NULL && (t.m_Path.m_pTarget->m_dwFlags & 0x300000)) {
        dist = t.m_Path.m_FlatDist;
        *found = t.m_Path.m_pTarget;
    }
    return dist;
}

void func_0040C6F4(void) {
    Obj *obj = D_8012FB70.obj;
    CGameObjectInstance *found;
    CPlayer *player;

    player = NULL;
    getTargetDist(obj, &found);
    if (found != NULL) {
        player = ((CPlayer *)found->m_AI.m_pParent);
    }
    if (player != NULL) {
        func_00253D68(player, D_00431D7C);
        func_0024CF58(player, player);
        player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY = 3.141593f;
        obj->unk50 = 0;
    }
    func_00243414(obj, obj->ai, 0xA);
}
