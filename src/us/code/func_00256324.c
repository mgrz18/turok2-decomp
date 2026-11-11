#include "common.h"
#include "structs/player.h"
#include "structs/region.h"

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + (off)))
#endif

extern CRegionInfo *GameObject_GetRegionInfo(CGameObjectInstance *actor);
extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);
extern s32 Sound__Play2D(s32 sound);

void PlayerJumpPad_Enter(CPlayer *player, CGameObjectInstance *actor) {
    CVector3 pos;
    CVector3 delta;
    CRegionInfo *region = GameObject_GetRegionInfo(actor);

    P_S32(player, 0xB98) = 0;
#ifdef TARGET_PORT
    *(CRegionInfo **)((u8 *)player + T2PlayerOff(0xBA0)) = region;
#else
    *(CRegionInfo **)((u8 *)player + 0xBA0) = region;
#endif
    P_F32(player, 0xBA4) = actor->header.m_vPos.x;
    P_F32(player, 0xBA8) = actor->header.m_vPos.y;
    P_F32(player, 0xBAC) = actor->header.m_vPos.z;
    pos.x = region->m_vPos.x;
    pos.y = region->m_vPos.y;
    pos.z = region->m_vPos.z;
    CVector3__Subtract(&delta, &pos, &P_VEC(player, 0xBA4));
    delta.y = 0.0f;
    P_F32(player, 0xB9C) = CVector3__GetYaw(&delta);
    if (region == NULL || !(region->m_wFlags54 & 0x10)) {
        Sound__Play2D(0x1450);
    }
}
