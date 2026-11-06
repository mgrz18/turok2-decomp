/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/pickup.h"
#include "globals/camera.h"

typedef struct PickupParams {
    char pad0[0xC];
    f32 amount;     /* 0x0C */
} PickupParams;

typedef struct PickupDef {
    char pad0[0x14];
    PickupParams params; /* 0x14 */
} PickupDef;

typedef struct PickupObject {
    char pad0[0x14];
    PickupDef *def; /* 0x14 */
} PickupObject;


void CPlayer__CollectKeyPiece(CPlayer *player, s32 id);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, s32 arg4, s32 arg5);
void Sound__PlaySpeech(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, resource);
    }
    if (sound != 0) {
        Sound__Play(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
}

/* Item pickups selected by the entry's id. */
s32 Pickup_GiveItem(CPlayer *player, CPickupEntry *entry, PickupObject *obj) {
    void *resource;
    s32 sound;
    s32 callback;
    PickupParams *params;

    params = &obj->def->params;
    switch (entry->m_Id) {
        case 0x708:
            player->m_InvincibilityTime = 120.0f;
            break;
        case 0x709:
            player->m_PlaneWalkerTime = 120.0f;
            break;
        case 0x70A:
        case 0x70B:
            player->m_FlashlightTime += params->amount;
            break;
        case 0x111C:
        case 0x111D:
        case 0x111E:
        case 0x111F:
        case 0x1120:
        case 0x1130:
        case 0x1131:
        case 0x1132:
        case 0x1133:
        case 0x1134:
        case 0x1194:
            CPlayer__CollectKeyPiece(player, entry->m_Id);
            break;
    }
    sound = entry->m_Sound;
    callback = entry->m_Callback;
    resource = entry->m_pMessage;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}
