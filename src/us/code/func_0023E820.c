#include "common.h"
#include "globals/camera.h"
#include "globals/scene.h"
#include "globals/pickup.h"

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

extern u8 D_800B37BC[];
extern u8 D_800B37CC[];
extern u8 D_800B37D8[];
extern u8 D_800B37DC[];
extern u8 D_800B3800[];
extern u8 D_800B3810[];
extern u8 D_800B3820[];
extern u8 D_800B3830[];
extern u8 D_800B383C[];
extern u8 D_800B384C[];
extern u8 D_800B3858[];
extern u8 D_800B3864[];
extern u8 D_800B3870[];
extern u8 D_800B387C[];
extern u8 D_800B3888[];
extern u8 D_800B3894[];
extern u8 D_800B38A4[];
extern u8 D_800B38B4[];
extern u8 D_800B38C4[];
extern u8 D_800B38D4[];
extern u8 D_800B38E4[];

s32 CPlayer__IsSpecialPickup(Picker *picker, s32 id);
void CPlayer__CollectKeyPiece(Picker *picker, s32 id);
s32 Level_GetHub(s32 arg0);
void CCamera__StartFlash(s32 hud, s32 a, s32 r, s32 g, s32 b, s32 arg5, s32 arg6, s32 arg7);
void CCameraPool__AddText(u8 *arg0, s32 hud, u8 *msg);
void Sound__Play(s32 sound, s32 x, sptr y, s32 z, s32 arg4, s32 arg5);
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
    CPickupRow18 *p = sHealthPickups;

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
                    msg = D_800B37DC;
                    break;
                case 0xFB9:
                    msg = D_800B383C;
                    break;
                case 0xFBE:
                    msg = D_800B3830;
                    break;
                default:
                    msg = D_800B37D8;
                    break;
            }
        } else if ((u32) (id - 0x1004) < 0x20) {
            msg = D_800B37BC;
        } else if ((u32) (id - 0x1068) < 0x20) {
            switch (id) {
                case 0x1084:
                    msg = NULL;
                    break;
                case 0x1085:
                    msg = D_800B3810;
                    break;
                case 0x1086:
                    msg = D_800B3820;
                    break;
                case 0x1087:
                    msg = D_800B3800;
                    effect = 0x17D;
                    break;
                default:
                    msg = D_800B37CC;
                    break;
            }
        } else if ((u32) (id - 0x10CC) < 51) {
            switch (id) {
                case 0x10CC:
                    msg = D_800B384C;
                    break;
                case 0x10D6:
                    msg = D_800B3858;
                    break;
                case 0x10E0:
                    msg = D_800B3864;
                    break;
                case 0x10EA:
                    msg = D_800B3870;
                    break;
                case 0x10F4:
                    msg = D_800B387C;
                    break;
                case 0x10FE:
                    msg = D_800B3888;
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
                    msg = D_800B3894;
                    break;
                case 0x1109:
                    msg = D_800B38A4;
                    break;
                case 0x110A:
                    msg = D_800B38B4;
                    break;
                case 0x110B:
                    msg = D_800B38C4;
                    break;
                case 0x110C:
                    msg = D_800B38D4;
                    break;
                case 0x110D:
                    msg = D_800B38E4;
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
