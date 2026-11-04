#include "common.h"
#include "structs/player.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Quat { f32 x, y, z, w; } Quat;

typedef struct ViewInfo {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ Vec3f eye;
} ViewInfo;

typedef struct ShotActor {
    /* 0x000 */ u8 pad0[0x1A8];
    /* 0x1A8 */ CPlayer *player;
} ShotActor;

extern u8 D_800F7078[];
extern u8 D_800F8928[];

void func_0020E810(Quat *out, Vec3f *dir);
s32 func_0022F350(void *table, void *actor, void *parent, void *slot, s32 a, s32 kind, Vec3f origin,
                  Quat rot, Vec3f pos, s32 arg, s32 b, s32 c);
void func_00225DB0(void *table, void *owner, s32 id, Vec3f pos, f32 a, f32 b);
void func_00253F90(CPlayer *pl, Vec3f *v);
void func_002688C8(ShotActor *a, Vec3f pos, Vec3f *outPos, s32 *outArg, s32 x, s32 y);

/* Fires the player's shot of kind 0xE9 at `pos`, aimed along `dir` from the eye, and plays its sound. */
void func_00238500(void *unused, ShotActor *actor, Vec3f pos, Vec3f dir, s32 mode) {
    Vec3f v;
    Vec3f p;
    Vec3f origin;
    Quat q;
    s32 arg;
    CPlayer *pl;

    pl = actor->player;
    v.x = -dir.x;
    v.y = dir.y;
    v.z = dir.z;
    func_00253F90(pl, &v);
    v.x -= ((ViewInfo *)pl->m_pCamera)->eye.x;
    v.y -= ((ViewInfo *)pl->m_pCamera)->eye.y;
    v.z -= ((ViewInfo *)pl->m_pCamera)->eye.z;
    func_0020E810(&q, &v);
    func_002688C8(actor, pos, &p, &arg, 0, 1);
    p = pos;
    p.y += 12.0f;
    origin.x = 0.0f;
    origin.y = 0.0f;
    origin.z = 0.0f;
    func_0022F350(D_800F8928, actor, actor, NULL, 0, 0xE9, origin, q, p, arg, -1, mode == 5 ? 3 : 1);
    p.x = 0.0f;
    p.y = 0.0f;
    p.z = 0.0f;
    func_00225DB0(D_800F7078, ((u8 *)pl->m_ContState.m_pContState) + 0x140, 3, p, 10.24f, 1.0f);
}
