#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/camera.h"

/* An actor's facing as a quaternion: turned half a circle from its owner's
 * player (flags 0x300000), its heading plus pitch (0x24C) in a region of
 * type 4, or else its heading applied to its own rotation (0x40). */

extern f32 gQuaternionSin;

f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */
void CQuaternion__Multiply(CQuaternion *, CQuaternion *, CQuaternion *);

CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *actor) {
    CQuaternion result;
    CQuaternion pitch;
    CQuaternion yaw;
    CCamera *camera;
    f32 s;

    if (actor->m_dwFlags & 0x300000) {
        camera = ((CPlayer *) actor->m_AI.m_pParent)->m_pCamera;
        if (camera == NULL) {
            goto own;
        }
        s = func_002982F0(1.5707964f);
        gQuaternionSin = s;
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        yaw.w = func_002974C0(1.5707964f);
        CQuaternion__Multiply(&result, &yaw, &camera->m_qViewAlign);
    } else if (actor->header.m_pIntelligence->m_Type == 4) {
        s = func_002982F0(actor->m_AI.m_Turret.m_Pitch * 0.5f);
        pitch.x = s;
        pitch.y = 0.0f;
        pitch.z = 0.0f;
        gQuaternionSin = s;
        pitch.w = func_002974C0(actor->m_AI.m_Turret.m_Pitch * 0.5f);
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        gQuaternionSin = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        CQuaternion__Multiply(&result, &pitch, &yaw);
    } else {
    own:
        s = func_002982F0((actor->m_RotY + 3.1415927f) * 0.5f);
        yaw.x = 0.0f;
        yaw.y = s;
        yaw.z = 0.0f;
        gQuaternionSin = s;
        yaw.w = func_002974C0((actor->m_RotY + 3.1415927f) * 0.5f);
        CQuaternion__Multiply(&result, &yaw, &actor->m_qGround);
    }
    return result;
}
