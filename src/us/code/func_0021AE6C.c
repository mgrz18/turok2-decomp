#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"

/* An actor's facing as a quaternion: turned half a circle from its owner's
 * player (flags 0x300000), its heading plus pitch (0x24C) in a region of
 * type 4, or else its heading applied to its own rotation (0x40). */

typedef struct {
    u8 pad0[0x12C];
    CQuaternion rot;               /* 0x12C */
} Player;

typedef struct {
    u8 pad0[0x51C];
    Player *player;         /* 0x51C */
} Owner;

extern f32 D_800DD78C;

f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void func_00210F68(CQuaternion *, CQuaternion *, CQuaternion *);

CQuaternion func_0021AE6C(CGameObjectInstance *actor) {
    CQuaternion result;
    CQuaternion pitch;
    CQuaternion yaw;
    Player *player;
    f32 s;

    if (actor->m_dwFlags & 0x300000) {
        player = ((Owner *) actor->m_AI.m_pParent)->player;
        if (player == NULL) {
            goto own;
        }
        s = func_002982F0(1.5707964f);
        D_800DD78C = s;
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        yaw.w = func_002974C0(1.5707964f);
        func_00210F68(&result, &yaw, &player->rot);
    } else if (actor->header.m_pIntelligence->m_Type == 4) {
        s = func_002982F0(actor->m_AI.m_VarF32[0] * 0.5f);
        pitch.x = s;
        pitch.y = 0.0f;
        pitch.z = 0.0f;
        D_800DD78C = s;
        pitch.w = func_002974C0(actor->m_AI.m_VarF32[0] * 0.5f);
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        D_800DD78C = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        func_00210F68(&result, &pitch, &yaw);
    } else {
    own:
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        D_800DD78C = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        func_00210F68(&result, &yaw, &actor->m_qGround);
    }
    return result;
}
