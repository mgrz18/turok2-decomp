#include "common.h"
#include "globals/strings.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/pickup.h"
#include "structs/vector.h"
#include "structs/player.h"

s32 CPlayer__IsSpecialPickup(CPlayer *player, s32 item);
void CPlayer__CollectKeyPiece(CPlayer *player, s32 id);
s32 Level_GetHub(s32 arg0);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, sptr arg4, sptr arg5);
void Sound__PlaySpeech(s32 callback);

/* Pickup_FindWeaponEntry */
static __inline__ CPickupRow18 *FindRowA(s32 id) {
    s32 i;
    CPickupRow18 *p = sWeaponPickups;

    for (i = 0x1E; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* Pickup_FindAmmoEntry */
static __inline__ CPickupRow14 *FindRowB(s32 id) {
    s32 i;
    CPickupRow14 *p = sAmmoPickups;

    for (i = 0x23; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* Pickup_FindHealthEntry */
static __inline__ CPickupRow18 *FindRowC(s32 id) {
    s32 i;
    CPickupRow18 *p = (CPickupRow18 *) sHealthPickups; /* typed for gendata (structs/pickup.h) */

    for (i = 6; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* Pickup_FindLifeForceEntry */
static __inline__ CPickupRow14 *FindRowD(s32 id) {
    s32 i;
    CPickupRow14 *p = sLifeForcePickups;

    for (i = 1; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* Pickup_FindItemEntry */
static __inline__ CPickupRow10 *FindRowE(s32 id) {
    s32 i;
    CPickupRow10 *p = sItemPickups;

    for (i = 14; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* Pick up an item of type id: the tables' handler takes it if one has it;

 * otherwise a generic pickup with its message, sound and effect. */
s32 CPlayer__CollectPickup(CPlayer *player, void *pickup, s32 id) {
    CPickupRow18 *row18;
    CPickupRow14 *row14;
    CPickupRow10 *row10;
    u8 *msg;
    s32 sound;
    s32 effect;
    s32 result;
    u32 key;

    result = 0;
    effect = 0;
    row18 = FindRowA(id);
    if (row18 != NULL) {
        result = row18->m_pHandler(player, row18);
    }
    if (result != 0) {
        return result;
    }
    row14 = FindRowB(id);
    if (row14 != NULL) {
        result = row14->m_pHandler(player, row14);
    }
    if (result != 0) {
        return result;
    }
    row18 = FindRowC(id);
    if (row18 != NULL) {
        result = row18->m_pHandler(player, row18);
    }
    if (result != 0) {
        return result;
    }
    row14 = FindRowD(id);
    if (row14 != NULL) {
        result = row14->m_pHandler(player, row14);
    }
    if (result != 0) {
        return result;
    }
    row10 = FindRowE(id);
    if (row10 != NULL) {
        result = row10->m_pHandler(player, row10, pickup);
    }
    if (result != 0) {
        return result;
    }
    if (CPlayer__IsSpecialPickup(player, id)) {
        sound = 0x1388;
        CPlayer__CollectKeyPiece(player, id);
        if ((u32) (id - 0xFA0) < 0x20) {
            switch (id) {
                case 0xFB4:
                    msg = sPickupNameText_CaveDoorKey;
                    break;
                case 0xFB9:
                    msg = sPickupNameText_GraveyardKey;
                    break;
                case 0xFBE:
                    msg = sPickupNameText_GateKey;
                    break;
                default:
                    msg = sPickupNameText_Key;
                    break;
            }
        } else if ((u32) (id - 0x1004) < 0x20) {
            msg = sPickupNameText_SatchelCharge;
        } else if ((u32) (id - 0x1068) < 0x20) {
            switch (id) {
                case 0x1084:
                    msg = NULL;
                    break;
                case 0x1085:
                    msg = sPickupNameText_RedLaserCell;
                    break;
                case 0x1086:
                    msg = sPickupNameText_BlueLaserCell;
                    break;
                case 0x1087:
                    msg = sPickupNameText_IonCapacitor;
                    effect = 0x17D;
                    break;
                default:
                    msg = sPickupNameText_PowerCell;
                    break;
            }
        } else if ((u32) (id - 0x10CC) < 51) {
            switch (id) {
                case 0x10CC:
                    msg = sPickupNameText_Level1Key;
                    break;
                case 0x10D6:
                    msg = sPickupNameText_Level2Key;
                    break;
                case 0x10E0:
                    msg = sPickupNameText_Level3Key;
                    break;
                case 0x10EA:
                    msg = sPickupNameText_Level4Key;
                    break;
                case 0x10F4:
                    msg = sPickupNameText_Level5Key;
                    break;
                case 0x10FE:
                    msg = sPickupNameText_Level6Key;
                    key = Level_GetHub(gScene_m_LevelID) - 1;
                    if (key < 6) {
                        player->m_Data.m_KeySet6LevelFlags[key]++;
                    }
                    break;
                default:
                    msg = NULL;
                    break;
            }
        } else if ((u32) (id - 0x1108) < 6) {
            sound = 0x13A9;
            effect = 0x180;
            switch (id) {
                case 0x1108:
                    msg = sPickupNameText_PrimagenKey1;
                    break;
                case 0x1109:
                    msg = sPickupNameText_PrimagenKey2;
                    break;
                case 0x110A:
                    msg = sPickupNameText_PrimagenKey3;
                    break;
                case 0x110B:
                    msg = sPickupNameText_PrimagenKey4;
                    break;
                case 0x110C:
                    msg = sPickupNameText_PrimagenKey5;
                    break;
                case 0x110D:
                    msg = sPickupNameText_PrimagenKey6;
                    break;
            }
        } else {
            msg = NULL;
        }
        CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
        if (msg != NULL) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, msg);
        }
        if (sound != 0) {
            Sound__Play(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
        }
        if (effect != 0) {
            Sound__PlaySpeech(effect);
        }
        result = 1;
    }
    return result;
}

#include "structs/weapon.h"
#include "structs/pickup.h"
#include "globals/game.h"

s32 CPlayer__AddAmmo(CPlayer *player, s32 index, s32 argument);

/* Weapon pickup: gives the weapon, selects it when it is better than the
 * current one, and plays the descriptor's feedback. */
s32 Pickup_GiveWeapon(CPlayer *player, CPickupEntry *entry) {
    WeaponInfo *info;
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 had;
    CPlayer *p;

    result = 0;
    if (entry->m_Params.m_Weapon.m_AmmoType != -1 && player->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon] != 0 &&
        (result = CPlayer__AddAmmo(player, entry->m_Params.m_Weapon.m_AmmoType, entry->m_Params.m_Weapon.m_AmmoAmount)) == 0 &&
        (!gEngineApp_m_Settings.multiplayer || !player->m_pOptions->m_TagMode)) {
        return 0;
    }
    if (gEngineApp_m_Settings.multiplayer) {
        if (!(gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon]->flags & 8)) {
            return 0;
        }
    } else {
        if (gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon]->flags & 8) {
            return 0;
        }
    }
    had = player->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon];
    p = player; /* a second name for the player: needed for register allocation */
    p->m_Data.m_WeaponList[entry->m_Params.m_Weapon.m_Weapon] = 1;
    info = gWeaponInfoTable[entry->m_Params.m_Weapon.m_Weapon];
    if (p->m_Data.m_CurrentWeapon < entry->m_Params.m_Weapon.m_Weapon) {
        if ((p->m_CharacterActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1)) {
            if (!gEngineApp_m_Settings.multiplayer && had == 0) {
                p->m_RequestedWeapon = entry->m_Params.m_Weapon.m_Weapon;
            }
        }
    }
    sound = entry->m_Sound;
    callback = entry->m_Callback;
    resource = entry->m_pMessage;
    CCamera__StartFlash(((void *)p->m_pCamera), 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, ((void *)p->m_pCamera), resource);
    }
    if (sound != 0) {
        Sound__Play(sound, p->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
    if (result == 0) {
        CPlayer__AddAmmo(p, entry->m_Params.m_Weapon.m_AmmoType, entry->m_Params.m_Weapon.m_AmmoAmount);
    }
    return 1;
}

#ifdef TARGET_PORT
/* Host body: the rows are CPickupRow18 (globals/pickup.h), 0x18 bytes on the N64 only */
void *Pickup_FindWeaponEntry(s32 arg0) {
    CPickupRow18 *row = sWeaponPickups;
    s32 i = 0x1E;

    do {
        i -= 1;
        if (row->m_Id == arg0) {
            return row;
        }
        row++;
    } while (i != -1);
    return 0;
}
#else
typedef struct Shared__struct_D_800D34C0_0x18 Shared__struct_D_800D34C0_0x18;
struct Shared__struct_D_800D34C0_0x18 {
    char pad0[4];
    s16 id;
    char pad6[6];
    s16 unkC;
    s16 unkE;
    s16 unk10;
    char pad12[0x6];
};
void *Pickup_FindWeaponEntry(s32 arg0) {
    char *v1;
    s32 i;
    Shared__struct_D_800D34C0_0x18 *record;
    v1 = (char *) sWeaponPickups;
    i = 0x1E;
    do {
        record = (Shared__struct_D_800D34C0_0x18 *)v1;
        i -= 1;
        if (record->id != arg0) {
            v1 += 0x18;
        } else {
            return v1;
        }
    } while (i != -1);
    return 0;
}
#endif

CPickupRow14 *Pickup_FindAmmoEntry(s32 id) {
    s32 i;
    CPickupRow14 *p = sAmmoPickups;
    for (i = 0x23; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}

CPickupRow18 *Pickup_FindHealthEntry(s32 id) {
    s32 i;
    CPickupRow18 *p = (CPickupRow18 *) sHealthPickups; /* typed for gendata (structs/pickup.h) */
    for (i = 6; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802AADCC_de_9.c, func_802AC9B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void *Pickup_FindLifeForceEntry(s32 arg0) {
    char *v1;
    s32 i;
    v1 = (char *) sLifeForcePickups;
    i = 1;
    do {
        i -= 1;
        if (((struct EntryRecAtE *)(v1))->id != arg0) {
            v1 += 0x14;
        } else {
            return v1;
        }
    } while (i != -1);
    return 0;
}

CPickupRow10 *Pickup_FindItemEntry(s32 id) {
    s32 i;
    CPickupRow10 *p = sItemPickups;
    for (i = 14; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return ((void *)0);
}

#include "structs/game_object.h"

/* Nonzero for an object (kind 1) with flags 0x300000 set. */
s32 Sweep_IsSpecialObject(sptr arg0, CGameObjectInstance *arg1) {
    s32 r = 0;
    if (arg1->header.m_Type == 1) {
        r = (u32) r < (u32) (arg1->m_dwFlags & 0x300000);
    }
    return r;
}

#include "structs/level_actor.h"
#include "protos/scene.h"

s32 CPooledLevelActor__Release(sptr);
s32 GameObject_GetTypeId(sptr);
s32 CGameRegion__FireTriggers(sptr, s32, sptr);           /* extern */

void CPlayer__TouchPickup(sptr arg0, CPooledLevelActor *arg1) {
    u16 temp_v0;
    u16 temp_v1;

    temp_v1 = arg1->m_Actor.m_wFlags;
    if (temp_v1 & 8) {
        if (arg1->m_PoolFlags & 1) {
            goto block_4;
        }
    } else if (!(temp_v1 & 1)) {
block_4:
#ifdef TARGET_PORT
        /* the port: DEBUG-ONLY T2_PICKUP_LOG=1 logs each touch (port/game/t2debug.c) */
        s32 id = GameObject_GetTypeId((sptr) arg1);
        s32 got = CPlayer__CollectPickup(arg0, (sptr) arg1, id);
        extern void T2Port_LogPickup(sptr player, void *actor, s32 id, s32 got);
        T2Port_LogPickup(arg0, arg1, id, got);
        if (got != 0) {
#else
        if (CPlayer__CollectPickup(arg0, (sptr) arg1, GameObject_GetTypeId((sptr) arg1)) != 0) {
#endif
            temp_v0 = arg1->m_Actor.m_wFlags | 1;
            arg1->m_Actor.m_wFlags = temp_v0;
            if (temp_v0 & 8) {
                CPooledLevelActor__Release((sptr) arg1);
                return;
            }
            CScene__SetLevelActorKilled(&gScene, arg1, 1);
            CGameRegion__FireTriggers(arg1->m_Actor.header.m_pCurrentRegion, 0x400, arg0);
        }
    }
}

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

s32 Pickup_GiveAmmo(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;

    result = 0;
    if (entry->m_Params.m_Ammo.m_Type != -1) {
        result = CPlayer__AddAmmo(player, entry->m_Params.m_Ammo.m_Type, entry->m_Params.m_Ammo.m_Amount);
    }
    if (result != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        resource = entry->m_pMessage;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}

s32 Pickup_GivePistolClip(CPlayer *player, CPickupEntry *entry) {
    s32 sound;
    s32 callback;

    if (CPlayer__AddAmmo(player, 2, entry->m_Params.m_Amount) != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        pickup_feedback(player, sPickupMessageText_10PistolBulletsClip, sound, callback);
        return 1;
    }
    return 0;
}

s32 Pickup_GivePistolAmmo(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;

    result = CPlayer__AddAmmo(player, 2, entry->m_Params.m_Amount);
    if (result != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        resource = entry->m_pMessage;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}

/* Health pickup: add the entry's amount, up to its cap (in multiplayer, up
 * to the player's own maximum). */
s32 Pickup_GiveHealth(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 max;
    u32 limit;

    max = entry->m_Params.m_Health.m_Max << 8;
    result = 0;
    if (player->m_Data.m_Health < max) {
        player->m_Data.m_Health += entry->m_Params.m_Health.m_Amount << 8;
        { /* a block boundary keeps the flag load after the store */
            if (gEngineApp_m_Settings.multiplayer) {
                limit = player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
                player->m_CharacterActor.m_AI.m_Health = player->m_Data.m_Health = (u32)player->m_Data.m_Health < limit ? (u32)player->m_Data.m_Health : limit;
            } else if (player->m_Data.m_Health > max) {
                player->m_Data.m_Health = max;
            }
        }
        result = 1;
    }
    if (result != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        resource = entry->m_pMessage;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}

void Sound__Play2D(s32 sound);

/* Life force pickup: every 100 gives an extra life, up to 9. */
s32 Pickup_GiveLifeForce(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;

    player->m_Data.m_LifeForce += entry->m_Params.m_Amount;
    if (player->m_Data.m_LifeForce >= 100) {
        player->m_Data.m_LifeForce -= 100;
        if (player->m_Data.m_Lives < 9) {
            player->m_Data.m_Lives++;
            Sound__Play2D(0x3A9D);
        }
    }
    sound = entry->m_Sound;
    callback = entry->m_Callback;
    resource = entry->m_pMessage;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}

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

/* Flashes the player's screen and plays a pickup's optional resource, sound and callback. */
void Pickup_Feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, resource);
    }
    if (sound != 0) {
        Sound__Play((s16) sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
}
