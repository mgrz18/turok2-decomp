/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225F20.c, func_00225F20, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"

typedef CVector3 Vec3f;

typedef struct {
    f32 m[4][4];
} Mtx4f;

/* The ridden creature (an AI actor). */
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x34];
    /* 0x174 */ s8 aiState;
} Mount;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ u8 pad10[0x50 - 0x10];
    /* 0x50 */ f32 yaw;
} Body;

extern f32 D_800B6D28;          /* frame time */
extern f32 D_800ADDE4;          /* charge speed */
extern f32 D_800AE344;          /* step length */
extern Vec3f D_800ADCB0;        /* rider view offset */
extern Vec3f D_800ADCBC;        /* rider seat */
extern char D_800ADEA0;         /* riding look tuning */
extern char D_800ADDE0;         /* riding forward move tuning */
extern char D_800ADE04;         /* riding backward move tuning */

f32 func_002974C0(f32 x);
void func_002117DC(f32 *value, f32 target, f32 rate);
void func_0024EF70(CPlayer *player, Body *body, void *tuning);
void func_0024F174(CPlayer *player, Body *body, void *tuning);
void func_00243414(Mount *actor, void *ai, s32 state);
void func_002811CC(void *ui, f32 a, f32 b, f32 c, f32 d, s32 e, Vec3f pos);
void func_002105A4(Mtx4f *m, f32 angle);
void func_0020F85C(Mtx4f *m, Vec3f *in, Vec3f *out);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);

/* Riding: the mount sways the view as it turns, charges on button 8, shakes
 * the screen at each step and sets the rider's view offset. */
void func_00251A2C(CPlayer *player, Body *body) {
    Mtx4f m;
    Mount *mount;
    Vec3f *offset;
    f32 turn;
    f32 yaw;
    f32 fwd;
    f32 t;
    f32 step;
    f32 dist;
    f32 next;
    f32 shake;

    mount = ((Mount *)player->m_pOnEnemy);
    turn = player->m_ContState.m_FinalLookX * (((func_002974C0(player->m_RideEnemyTime * 0.2792527f) + 1.0f) * 0.01f + 0.06f) * 80.0f);
    player->m_RideEnemyTime += D_800B6D28;
    turn *= 0.017453294f;
    yaw = body->yaw;
    func_002117DC(&body->yaw, yaw + turn, 0.9f);
    func_002117DC(&player->m_vHeadRotOffset.z, ((yaw - body->yaw) + player->m_ContState.m_FinalLookX * 8.0f) * 0.017453294f, 0.1f);
    func_0024EF70(player, body, &D_800ADEA0);

    offset = NULL;
    if (player->m_ContState.m_dwButtonsDouble & 8) {
        func_00243414(mount, mount->ai, 0x2E);
        player->m_FleeTime = 90.0f;
        player->m_Speed = D_800ADDE4 * 2;
    }

    fwd = player->m_Speed;
    dist = player->m_OnEnemyStompTime;
    if (fwd < 0.0f) {
        fwd = -fwd;
    }
    if ((player->m_SideSpeed * 2 < 0.0f ? -(player->m_SideSpeed * 2) : player->m_SideSpeed * 2) < fwd) {
        step = player->m_Speed;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    } else {
        step = player->m_SideSpeed * 2;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    }
    player->m_OnEnemyStompTime = next;
    if (D_800AE344 < next) {
        player->m_OnEnemyStompTime = next - D_800AE344;
        if (mount->aiState != 0x2E) {
            player->m_Speed *= 0.5f;
            player->m_SideSpeed *= 0.5f;
        }
        t = player->m_Speed;
        if (t < 0.0f) {
            shake = -t * 2.5f;
        } else {
            shake = t * 2.5f;
        }
        if (50.0f < shake) {
            shake = 50.0f;
        }
        func_002811CC(player->m_pCamera, 0.0f, shake, 0.0f, 1.0f, 1, body->pos);
    }

    if (player->m_Speed > 0.0f) {
        func_0024F174(player, body, &D_800ADDE0);
    } else {
        func_0024F174(player, body, &D_800ADE04);
    }
    t = player->m_Speed;
    if ((t < 0.0f ? -t : t) > 10.24f) {
        player->m_FleeTime = 15.0f;
    }

    if (player->m_OnEnemyObjectType == 0x136) {
        offset = &D_800ADCB0;
        player->m_vOnEnemyScale = D_800ADCBC;
    }
    if (offset != NULL) {
        func_002105A4(&m, body->yaw);
        func_0020F85C(&m, offset, &player->m_vEyeOffset);
        func_0020EF60(&player->m_vEyeOffset, &player->m_vEyeOffset, 10.24f);
    }
    player->m_RequestedWeapon = 0x13;
}
