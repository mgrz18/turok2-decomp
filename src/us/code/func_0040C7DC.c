#include "common.h"
#include "structs/player.h"
#include "structs/boss.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ u32 flags;
} GameFlags;

/* The game state at 0x80119870; its flags at +0x1268 are also D_8011AAD8. */
typedef struct {
    /* 0x0000 */ u8 pad0000[0x1268];
    /* 0x1268 */ GameFlags flags;
} Game;

s32 func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);
s32 func_00248BA8(s32, s32, s32, s32);

CPlayer *func_002532A8(Game *);                     /* extern */
extern Game D_80119870;

void func_0040C7DC(void) {
    CPlayer *temp_v0;
    GameFlags *temp_v1;

    func_00243414(D_8012FB70.obj, &D_8012FB70.obj->m_AI, 1);
    temp_v0 = func_002532A8(&D_80119870);
    if (temp_v0 != NULL) {
        temp_v1 = &D_80119870.flags;
        temp_v0->m_Data.m_bMotherBossComplete = 1;
        temp_v1->flags = (s32) (temp_v1->flags | 4);
        func_00248BA8((s32) temp_v0, 0x1B57, 2, 0);
    }
}
