/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_17.CPP, func_80224C4C_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ s16 strokeSound;
} Tuning;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 flags;
} Surface;

typedef struct {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ Surface *surface;
    /* 0x018 */ u8 pad18[4];
    /* 0x01C */ f32 climb;
    /* 0x020 */ u8 pad20[0x140 - 0x20];
    /* 0x140 */ s32 flags;
} Body;

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADE70;     /* standing look tuning */
extern Tuning D_800ADEB8;   /* wading move tuning */

void func_002117DC(f32 *value, f32 target, f32 rate);
void func_0024EF70(CPlayer *player, Body *body, void *tuning);
void func_0024F174(CPlayer *player, Body *body, void *tuning);
s32 func_0024E5F0(CPlayer *player, Body *body, s32 mode);
f32 func_00268A5C(Body *body);
f32 func_002982F0(f32 x);
void func_00275544(s16 sound, Vec3f pos, s32, s32);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_nItems = 0;
    s->unk_0x4 = 0.0f;
    s->m_nLastItem = -1;
    s->unk_0x8 = 0.0f;
}

static __inline__ s32 try_jump(CPlayer *player, Body *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->flags & 0x6000)
        && (body->surface->flags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        func_0024E5F0(player, body, 4);
        return 1;
    }
    return 0;
}

/* Wading: leave the state once out of the water, dive when looking down hard
 * while lifting, else follow the water surface, sway, and pick the animation
 * from the climb; moving plays the tuning's stroke sound now and then. */
void func_002508D0(CPlayer *player, Body *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 8;
    } else {
        player->m_Mode = 9;
    }
    func_002117DC(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    func_0024EF70(player, body, &D_800ADE70);
    func_0024F174(player, body, &D_800ADEB8);
    if (!(body->flags & 0x200)) {
        reset_strokes(&player->m_Barrel);
        func_0024E5F0(player, body, 3);
        return;
    }
    if (player->m_vHeadRotOffset.x * 57.295776f < -40.0f && player->m_ContState.m_FinalMoveY > 0.0f) {
        player->m_vDesiredPos.y = func_00268A5C(body) - player->m_vEyeOffset.y - 51.199997f;
        func_0024E5F0(player, body, 0xB);
        return;
    }
    if (!(body->flags & 0xC0000)) {
        body->climb = 0.0f;
        func_002117DC(&player->m_vDesiredPos.y, func_00268A5C(body) - player->m_vEyeOffset.y * 0.6f, 0.5f);
    }
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 0.043633237f;
    if (try_jump(player, body)) {
        return;
    }
    idle = 0;
    if (player->m_CharAnim == 0x1144) {
        idle = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!idle) {
        if (1.024f <= player->m_Speed) {
            player->m_CharAnim = 0x8A2;
        } else if (player->m_Speed <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    player->m_SfxTimer += D_800B6D28;
    if (5.12f < player->m_TotalSpeed && 11.25f < player->m_SfxTimer) {
        player->m_SfxTimer = 0.0f;
        func_00275544(D_800ADEB8.strokeSound, body->pos, 0, -1);
    }
}
