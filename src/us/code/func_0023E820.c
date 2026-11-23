#include "common.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/pickup.h"

#ifdef TARGET_PORT
/* the picker is the CPlayer: the view at its host offsets (round 7 v2); the camera (N64
 * 0x51C) is a pointer, pointer-wide here */
#include "structs/player.h"
#define T2_HOFF_Picker(m) __builtin_offsetof(CPlayer, m)
typedef struct Picker {
    union {
        struct { u8 _hp_x[T2_HOFF_Picker(m_CharacterActor.header.m_vPos)]; s32 x; s32 y; s32 z; }; /* N64 0x4 */
        struct { u8 _hp_hud[T2_HOFF_Picker(m_pCamera)]; sptr hud; }; /* N64 0x51C */
        struct { u8 _hp_keys[T2_HOFF_Picker(m_Data.m_KeySet6LevelFlags)]; u8 keys[6]; }; /* N64 0xA02 */
    };
} Picker;
#else
typedef struct Picker {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ s32 x;
    /* 0x008 */ s32 y;
    /* 0x00C */ s32 z;
    /* 0x010 */ u8 pad10[0x50C];
    /* 0x51C */ s32 hud;
    /* 0x520 */ u8 pad520[0x4E2];
    /* 0xA02 */ u8 keys[6];
} Picker;
#endif

extern u8 sPickupNameText_SatchelCharge[];
extern u8 sPickupNameText_PowerCell[];
extern u8 sPickupNameText_Key[];
extern u8 sPickupNameText_CaveDoorKey[];
extern u8 sPickupNameText_IonCapacitor[];
extern u8 sPickupNameText_RedLaserCell[];
extern u8 sPickupNameText_BlueLaserCell[];
extern u8 sPickupNameText_GateKey[];
extern u8 sPickupNameText_GraveyardKey[];
extern u8 sPickupNameText_Level1Key[];
extern u8 sPickupNameText_Level2Key[];
extern u8 sPickupNameText_Level3Key[];
extern u8 sPickupNameText_Level4Key[];
extern u8 sPickupNameText_Level5Key[];
extern u8 sPickupNameText_Level6Key[];
extern u8 sPickupNameText_PrimagenKey1[];
extern u8 sPickupNameText_PrimagenKey2[];
extern u8 sPickupNameText_PrimagenKey3[];
extern u8 sPickupNameText_PrimagenKey4[];
extern u8 sPickupNameText_PrimagenKey5[];
extern u8 sPickupNameText_PrimagenKey6[];

s32 CPlayer__IsSpecialPickup(Picker *picker, s32 id);
void CPlayer__CollectKeyPiece(Picker *picker, s32 id);
s32 Level_GetHub(s32 arg0);
void CCamera__StartFlash(sptr hud, s32 a, s32 r, s32 g, s32 b, s32 arg5, s32 arg6, s32 arg7);
void CCameraPool__AddText(u8 *arg0, sptr hud, u8 *msg);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play Sound__Play_i3
#endif
void Sound__Play(s32 sound, s32 x, sptr y, sptr z, s32 arg4, s32 arg5);
void Sound__PlaySpeech(s32 arg0);

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
#ifdef TARGET_PORT
    CPickupRow18 *p = (CPickupRow18 *) sHealthPickups; /* typed for gendata (structs/pickup.h) */
#else
    CPickupRow18 *p = sHealthPickups;
#endif

    for (i = 6; i != -1; i--) {
        if (p->m_Id == id) {
            return p;
        }
        p++;
    }
    return NULL;
}

/* func_0023EFCC */
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
s32 CPlayer__CollectPickup(Picker *picker, void *pickup, s32 id) {
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
        result = row18->m_pHandler(picker, row18);
    }
    if (result != 0) {
        return result;
    }
    row14 = FindRowB(id);
    if (row14 != NULL) {
        result = row14->m_pHandler(picker, row14);
    }
    if (result != 0) {
        return result;
    }
    row18 = FindRowC(id);
    if (row18 != NULL) {
        result = row18->m_pHandler(picker, row18);
    }
    if (result != 0) {
        return result;
    }
    row14 = FindRowD(id);
    if (row14 != NULL) {
        result = row14->m_pHandler(picker, row14);
    }
    if (result != 0) {
        return result;
    }
    row10 = FindRowE(id);
    if (row10 != NULL) {
        result = row10->m_pHandler(picker, row10, pickup);
    }
    if (result != 0) {
        return result;
    }
    if (CPlayer__IsSpecialPickup(picker, id)) {
        sound = 0x1388;
        CPlayer__CollectKeyPiece(picker, id);
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
                        picker->keys[key]++;
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
        CCamera__StartFlash(picker->hud, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
        if (msg != NULL) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, picker->hud, msg);
        }
        if (sound != 0) {
            Sound__Play(sound, picker->x, picker->y, picker->z, 0, -1);
        }
        if (effect != 0) {
            Sound__PlaySpeech(effect);
        }
        result = 1;
    }
    return result;
}
