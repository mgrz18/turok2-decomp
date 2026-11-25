/* Boundary fixed in versions/cuts.us.txt: the function starts at 0x00267090. splat cut its first word,
 * `lw $2, 0x14($4)`, into a one-word GameObject_IsSolid (the name its callers
 * jal), and the rest into func_00267094. Written whole under its real name.
 * Its switch reads jtbl_800A7D30, which still lives in the asm .rodata. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "globals/game.h"


s32 GameObject_IsSolid(CGameObjectInstance *actor) {
    CPlayer *owner;
    CAIModeInfo *modeInfo;
    u32 flags;
    u32 mask;

#ifdef TARGET_PORT
    /* Port guard (STABILITY.md S17): T2_WARP_TO=3999 drops the player into the ending (engine
     * mode 16) with no body intelligence: not solid (as S8: no ladder jump without one). */
    if (actor->header.m_pIntelligence == NULL) {
        return 0;
    }
#endif
    switch ((u32) actor->header.m_pIntelligence->m_Type) {
        case 11:
            if (actor->header.m_Type != 1) {
                return 1;
            }
            if (actor->m_dwFlags & 0x300000) {
                owner = ((CPlayer *)actor->m_AI.m_pParent);
                if (gEngineApp_m_Settings.multiplayer != 0 && owner->m_bEnteringDeathmatch != 0) {
                    return 0;
                }
            }
            /* fallthrough */
        case 8:
            return 1;
        case 0:
            return actor->header.m_pIntelligence->m_dwTypeFlags & 2;
        case 2:
            if (actor->header.m_Type == 1) {
                return actor->m_dwFlags & 0x10000;
            }
            return 0;
        case 1:
            if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x2000) && (modeInfo = (CAIModeInfo *) actor->m_AI.m_pModeCurrent) != NULL) {
                mask = 0x10000;
                /* Round 16 merge (n64viewcheck.py's cast kind; round 30 typed the variable): m_pModeCurrent is a CAIModeInfo row
                 * (host layout: its sptr words are 8 bytes, unk_1C at 0x30); the old Attachment view's 0x1C read
                 * the high half of m_pData on the host (0: always solid). The member instead. */
                return (modeInfo->unk_1C & mask) == 0;
            }
            return 0;
        case 4:
        case 5:
        case 7:
        case 10:
            if (actor->header.m_Type == 1) {
                flags = actor->m_dwFlags;
                if (!(flags & 0x2000)) {
                    return 0;
                }
                return flags & 0x10000;
            }
            return 0;
        case 12:
            return actor->header.m_pIntelligence->m_dwTypeFlags & 1;
        case 3:
        case 6:
        case 9:
        default:
            return 0;
    }
}

#include "protos/game_object.h"

/* NOTE: Turok 2 differs from Rage Wars in case 11 (subtracts two owner values
 * and falls through to case 1/4) and has no cases 13/14.
 * The object's collision height, by the type of its intelligence (a ducking
 * player's lower). */
f32 GameObject_GetCollisionHeight(CGameObjectInstance *a) {
    CIntelligence *d;
    f32 r;
    if (a->header.m_Type == 1 && a->m_AI.m_Health <= 0) {
        return GameObject_GetDeadCollisionHeight(a);
    }
    switch (a->header.m_pIntelligence->m_Type) {
    case 11:
        if (a->header.m_Type == 1 && (a->m_dwFlags & 0x300000)) {
            return a->header.m_pIntelligence->m_Player.m_CollisionHeight
                   - ((CPlayer *) a->m_AI.m_pParent)->m_ActualCameraVertEyeOffset
                   - ((CPlayer *) a->m_AI.m_pParent)->m_DuckOffset;
        }
        /* fallthrough */
    case 1:
    case 4:
        return a->header.m_pIntelligence->m_Enemy.m_CollisionHeight;
    case 8:
        d = a->header.m_pIntelligence;
        r = d->m_Pickup.m_dwFlags & 1; /* dead store, needed for scheduling */
        r = 122.88f;
        if (!(d->m_Pickup.m_dwFlags & 1)) {
            r = d->m_Pickup.m_CollisionHeight;
        }
        return r;
    case 7:
        return a->header.m_pIntelligence->m_Destructible.m_CollisionHeight;
    case 6:
        return 102.399994f;
    case 0:
    case 5:
    case 10:
    case 12:
        return a->header.m_pIntelligence->m_Static.m_CollisionHeight;
    default:
        return 0.0f;
    }
}

/* Radius of an object, by the kind of its definition. */
f32 GameObject_GetCollisionRadius(CGameObjectInstance *obj) {
    CIntelligence *def;
    f32 r;
    f32 m;

    switch (obj->header.m_pIntelligence->m_Type) {
    case 11:
        if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000) && ((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy != NULL) {
            return 122.88f;
        }
        /* fallthrough */
    case 1:
    case 4:
        return obj->header.m_pIntelligence->m_Enemy.m_CollisionRadius;
    case 0:
    case 5:
    case 8:
    case 10:
    case 12:
        return obj->header.m_pIntelligence->m_Static.m_CollisionRadius;
    case 2:
        def = obj->header.m_pIntelligence;
        if (def->m_Platform.m_CollisionType == 2) {
            m = def->m_Platform.m_CollisionWidth;
            if (!(def->m_Platform.m_CollisionRadius <= m)) {
                m = def->m_Platform.m_CollisionRadius;
            }
            r = def->m_Platform.m_CollisionLength;
            if (!(m <= r)) {
                r = m;
            }
            return r * 1.42f;
        }
        return def->m_Platform.m_CollisionRadius;
    case 7:
        return obj->header.m_pIntelligence->m_Destructible.m_CollisionRadius;
    default:
        return 0.0f;
    }
}

#include "structs/game.h"
#include "globals/math.h"
#include "protos/mtxf.h"

CQuaternion CGameObjectInstance__GetRotation(CGameObjectInstance *actor);
void CQuaternion__ToMatrix(f32 *q, f32 *m);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
CGameObjectInstance *CPlayerPool__FindNearest(void *pool, CVector3 p);

/* The rotation that turns an actor (or the world's z axis, without one) toward its sight target, or
 * else the nearest player, at three quarters of its collision height. Round 28: on
 * CGameObjectInstance (Rage Wars' LevelActor view read the sight target at 0x1C0 and the collision
 * y offset at 0x54, which on the host missed). */
CQuaternion GameObject_GetAimRotation(CGameObjectInstance *actor, CVector3 point) {
    CVector3 origin;
    CVector3 dir;
    CVector3 forward;
    CVector3 local;
    CVector3 axis;
    CQuaternion turn;
    CQuaternion aim;
    CQuaternion result;
    CMtxF matrix;
    CGameObjectInstance *target;
    CGameObjectInstance *found;
    f32 angle;
    f32 scale;

    target = NULL;
    if (actor != NULL && actor->header.m_Type == 1) {
        target = actor->m_AI.m_pSightTarget;
        aim = CGameObjectInstance__GetRotation(actor);
        CQuaternion__ToMatrix((f32 *) &aim, (f32 *) matrix);
        local.x = 0.0f;
        local.y = 0.0f;
        local.z = 1.0f;
        CMtxF__TransformPoint(matrix, &local, &forward);
    } else {
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
    }
    if (target == NULL) {
        found = CPlayerPool__FindNearest(&gEngineApp_m_Game.m_PlayerList, point);
        if (found != NULL) {
            target = found;
        }
        if (target == NULL) {
            result.x = result.y = result.z = 0.0f;
            result.w = 1.0f;
            return result;
        }
    }
    origin = target->header.m_vPos;
    origin.y += target->m_CollisionYOffset;
    origin.y += GameObject_GetCollisionHeight(target) * 0.75f;
    CVector3__Subtract(&dir, &origin, &point);
    CVector3__Normalize(&dir);
    CVector3__Cross(&axis, &forward, &dir);
    CVector3__Normalize(&axis);
    angle = Math_AcosLookup(forward.x * dir.x + forward.y * dir.y + forward.z * dir.z) * 0.5f;
    scale = func_002982F0(angle);
    turn.x = axis.x * scale;
    turn.y = axis.y * scale;
    turn.z = axis.z * scale;
    gQuaternionSin = scale;
    turn.w = func_002974C0(angle);
    if (actor != NULL && actor->header.m_Type == 1) {
        CQuaternion__Multiply(&result, &aim, &turn);
    } else {
        result = turn;
    }
    return result;
}

#define MAX(a, b) ((a) < (b) ? (b) : (a))

static __inline__ f32 nodeFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    void *node = actor->header.m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = actor->header.m_vPos.y;
    } else {
        result = CGameRegion__GetHeight(node, x, z);
    }
    return result;
}

/* GameObject_GetFloorHeightAt, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == 2) {
            switch (def->m_Platform.m_CollisionType) {
                case 0:
                    return nodeFloor(actor, x, z);
                case 1:
                case 2:
                    return Platform_GetTopHeight(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

static __inline__ CGameObjectInstance *standOn(CGameObjectInstance *actor) {
    CGameObjectInstance *result = NULL;

    if (actor->header.m_Type == 1) {
        result = actor->m_pInstanceBelow;
    }
    return result;
}

/* Ground height under an actor: its own region floor, or what it stands on if higher. */
f32 GameObject_GetGroundHeight(CGameObjectInstance *actor) {
    f32 ground;

    if (actor->header.m_pCurrentRegion == NULL) {
        return actor->header.m_vPos.y;
    }
    ground = CGameRegion__GetHeight(actor->header.m_pCurrentRegion, actor->header.m_vPos.x, actor->header.m_vPos.z);
    if (standOn(actor) == NULL) {
        return ground;
    }
    return MAX(ground, actorFloor(standOn(actor), actor->header.m_vPos.x, actor->header.m_vPos.z));
}

/* GameObject_GetFloorHeightAt, inlined: floor height under (x, z) for an actor. */
static __inline__ f32 actorFloor_267800(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == 2) {
            switch (def->m_Platform.m_CollisionType) {
                case 0:
                    if (actor->header.m_pCurrentRegion != NULL) {
                        return CGameRegion__GetHeight(actor->header.m_pCurrentRegion, x, z);
                    }
                    return actor->header.m_vPos.y;
                case 1:
                case 2:
                    return Platform_GetTopHeight(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

/* The platform an actor stands on, if it is still touching it. */
CGameObjectInstance *GameObject_GetPlatformBelow(CGameObjectInstance *actor) {
    CGameObjectInstance *platform;
    CVector3 velocity;
    f32 floor;
    f32 gap;

    platform = NULL;
    if (actor->header.m_Type == 1) {
        platform = actor->m_pInstanceBelow;
    }
    if (platform == NULL) {
        return NULL;
    }
    if (platform->header.m_pIntelligence->m_Type != 2) {
        return platform;
    }
    floor = actorFloor_267800(platform, actor->header.m_vPos.x, actor->header.m_vPos.z);
    GameObject_GetFrameDelta(platform, &velocity);
    gap = actor->header.m_vPos.y - floor + velocity.y;
    if ((gap < 0.0f) ? (-gap <= 5.12f) : (gap <= 5.12f)) {
        return platform;
    }
    return NULL;
}

#define MIN(a, b) ((a) > (b) ? (b) : (a))

static __inline__ s32 isHeld(CGameObjectInstance *obj) {
    return (obj->header.m_Type != 1) ? 0 : (obj->m_dwFlags & 1);
}

/* Tilt of an actor toward the floor normal under it. */
static __inline__ CQuaternion floorTilt(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion q;
    f32 angle;

    CGameRegion__GetNormal(&normal, obj->header.m_pCurrentRegion);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    CVector3__Cross(&axis, &up, &normal);
    CVector3__Normalize(&axis);
    angle = Math_AcosLookup(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    gQuaternionSin = func_002982F0(angle);
    q.x = axis.x * gQuaternionSin;
    q.y = axis.y * gQuaternionSin;
    q.z = axis.z * gQuaternionSin;
    q.w = func_002974C0(angle);
    return q;
}

/* An actor's body lean on the ground: players blend the floor tilt with a lean along their heading. */
CQuaternion GameObject_GetGroundRotation(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion tilt;
    CQuaternion lean;
    CQuaternion result;
    f32 slope;
    f32 sine;
    f32 cosine;
    f32 angle;
    f32 half;
    CGameObjectInstance *self;

    if (obj->header.m_pCurrentRegion != NULL && isHeld(obj) == 0) {
        if (obj->header.m_Type == 1) {
            self = obj;
            if (obj->m_AI.m_dwFlags & 0xC0000) {
                goto identity;
            }
            if (obj->m_dwFlags & 0x300000) {
                if (CGameRegion__IsSteep(obj->header.m_pCurrentRegion) != 0) {
                    goto identity;
                }
                CGameRegion__GetNormal(&normal, obj->header.m_pCurrentRegion);
                up.x = 0.0f;
                up.y = 1.0f;
                up.z = 0.0f;
                CVector3__Cross(&axis, &up, &normal);
                CVector3__Normalize(&axis);
                slope = Math_AcosLookup(up.x * normal.x + up.y * normal.y + up.z * normal.z);
                gQuaternionSin = func_002982F0(MIN(slope, 0.43633235f) * 0.375f);
                tilt.x = axis.x * gQuaternionSin;
                tilt.y = axis.y * gQuaternionSin;
                tilt.z = axis.z * gQuaternionSin;
                tilt.w = func_002974C0(MIN(slope, 0.43633235f) * 0.375f);
                sine = func_002982F0(self->m_RotY);
                cosine = func_002974C0(self->m_RotY);
                slope = CGameRegion__GetClimbAngle(obj->header.m_pCurrentRegion, obj->header.m_vPos.x, obj->header.m_vPos.z, obj->header.m_vPos.x - sine, obj->header.m_vPos.z - cosine);
                slope = MIN(slope, 25.0f) * 0.75f;
                half = 0.5f;
                angle = slope * half;
                gQuaternionSin = func_002982F0(angle);
                lean.y = 0.0f;
                lean.x = cosine * gQuaternionSin;
                lean.z = -sine * gQuaternionSin;
                lean.w = func_002974C0(angle);
                CQuaternion__SlerpFast(&result, half, &lean, &tilt);
                return result;
            }
        }
        return floorTilt(obj);
    }
identity:
    tilt.x = tilt.y = tilt.z = 0.0f;
    tilt.w = 1.0f;
    return tilt;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DD24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267D2C(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        do { return def->m_dwTypeFlags & 0x20; } while (0);
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DD58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267D60(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        if (def) {
            return def->m_dwTypeFlags & 0x40;
        } else {
            return def->m_dwTypeFlags & 0x40;
        }
    }
    return 0;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* An enemy definition's type flag 1 (0 for other definitions). */
s32 GameObject_IsLeader(CInstanceHeader *header) {
    CIntelligence *def = header->m_pIntelligence;

    if (def->m_Type == INTELLIGENCE_ENEMY) {
        return def->m_dwTypeFlags & 1;
    }
    return 0;
}

/* Enemies only: bit 2 of the definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_IsFollower(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_dwTypeFlags & 2;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DDDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267DE4(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        if (def) {
            return def->m_dwTypeFlags & 0x80;
        } else {
            return def->m_dwTypeFlags & 0x80;
        }
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DE10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 func_00267E18(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        do { return def->m_dwTypeFlags & 0x100; } while (0);
    }
    return 0;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* An enemy definition's type flag 0x200 (0 for other definitions). */
s32 func_00267E4C(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def->m_Type == INTELLIGENCE_ENEMY) {
        return def->m_dwTypeFlags & 0x200;
    }
    return 0;
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_4.CPP, func_8024DE6C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* An enemy or turret definition's type flag (0 for other definitions). */
s32 GameObject_IsLeashed(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;
    s32 kind = def->m_Type;

    if ((kind == 1) || (kind == 4)) {
        if (def) {
            return def->m_dwTypeFlags & 0x10;
        } else {
            return def->m_dwTypeFlags & 0x10;
        }
    }
    return 0;
}

s32 GameObject_CanClimbLedges(CGameObjectInstance *arg0) {
    CIntelligence *temp_a0;

    temp_a0 = arg0->header.m_pIntelligence;
    if (temp_a0->m_Type == 0xB) {
        if (gEngineApp_m_Settings.multiplayer != 0) {
            return temp_a0->m_Player.m_dwFlags & 1;
        }
        return 1;
    }
    return 0;
}

/* Enemies only: bit 1 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanClimbUp(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 1;
    }
    return 0;
}

s32 GameObject_CanDropDown(CGameObjectInstance *a) {
    CIntelligence *d = a->header.m_pIntelligence;
    s32 r;
    switch (d->m_Type) {
    case 11:
        return 1;
    case 1:
        r = 0;
        if (a->m_AI.m_Health <= 0 || (d->m_Enemy.m_dwEnemyFlags & 2)) {
            r = 1;
        }
        return r;
    default:
        return 0;
    }
}

s32 GameObject_CanEnterWater(CGameObjectInstance *arg0) {
    s32 var_v1;
    CIntelligence *temp_a1;

    temp_a1 = arg0->header.m_pIntelligence;
    if (temp_a1->m_Type == 1) {
        var_v1 = 0;
        if ((arg0->m_AI.m_Health <= 0) || (temp_a1->m_Enemy.m_dwEnemyFlags & 4)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}

/* Enemies only: bit 8 of the enemy definition's m_dwEnemyFlags. Round 22 rewrote the m2c
 * draft (identical arms, an empty do-while) as the getters round 21 de-faked: the test for the
 * case that returns the field first. */
s32 GameObject_CanLeaveWater(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 8;
    }
    return 0;
}

/* For type-1 objects: whether dead (or flagged 0x800). */
s32 GameObject_CanEnterInfoFlag52_80(CGameObjectInstance *arg0) {
    s32 var_v1;
    struct CIntelligence *temp_a1;

    temp_a1 = arg0->header.m_pIntelligence;
    if (temp_a1->m_Type == 1) {
        var_v1 = 0;
        if ((arg0->m_AI.m_Health <= 0) || (temp_a1->m_Enemy.m_dwEnemyFlags & 0x800)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}

/* Enemies only: bit 0x1000 of the enemy definition's m_dwEnemyFlags. Round 22 rewrote the m2c
 * draft (identical arms, an empty do-while) as the getters round 21 de-faked: the test for the
 * case that returns the field first. */
s32 GameObject_CanLeaveInfoFlag52_80(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x1000;
    }
    return 0;
}

s32 GameObject_CanEnterInfoFlag400000(CGameObjectInstance *obj) {
    s32 var_v1;
    CIntelligence *temp_a1;

    temp_a1 = obj->header.m_pIntelligence;
    if (temp_a1->m_Type == 1) {
        var_v1 = 0;
        if ((obj->m_AI.m_Health <= 0) || (temp_a1->m_Enemy.m_dwEnemyFlags & 0x10)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* An enemy definition's flag 0x20 (0 for other definitions). */
s32 GameObject_CanLeaveInfoFlag400000(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def->m_Type == INTELLIGENCE_ENEMY) {
        return def->m_Enemy.m_dwEnemyFlags & 0x20;
    }
    return 0;
}

/* Enemies only: bit 0x200 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanEnterRegionFlag1000(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x200;
    }
    return 0;
}

/* Enemies only: bit 0x400 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanLeaveRegionFlag1000(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x400;
    }
    return 0;
}

/* Enemies only: bit 0x2000 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanEnterRegionFlag80(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x2000;
    }
    return 0;
}

/* Enemies only: bit 0x4000 of the enemy definition flags. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
s32 GameObject_CanLeaveRegionFlag80(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_dwEnemyFlags & 0x4000;
    }
    return 0;
}

#include "m2c_macros.h"

s32 func_00268148(void) {
    return 0;
}

s32 func_00268150(void) {
    return 0;
}

s32 func_00268158(void) {
    return 0;
}

/* Host body: the instance's members instead of its N64 offsets (0xD4 m_dwFlags,
 * 0x14 header.m_pIntelligence, whose first word is the definition type). */
s32 GameObject_CastsShadow(u8 *arg0) {
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;

    if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)) {
        return 1;
    }
    switch (*(s32 *) obj->header.m_pIntelligence) {
        case 1:
        case 2:
        case 5:
        case 8:
        case 9:
            return 1;
    }
    return 0;
}

s32 GameObject_IsMover(CGameObjectInstance *a) {
    CAIModeInfo *p;
    if (a->header.m_Type != 1) {
        return 0;
    }
    if (!(a->m_dwFlags & 0x2000)) {
        return 0;
    }
    p = a->m_AI.m_pModeCurrent;
    if (p != 0) {
        if (p->unk_1C & 0x10000) {
            return 0;
        }
    }
    return 1;
}

s32 Cinema_PathPlaying(void);

/* Enemies and throwables always; a player's body only while its flags 0x300000 are set and its
 * owner (the CPlayer) rides an enemy or a cinema path plays. Round 28 joined the N64 view and the
 * port's (round 11 C26) host-offset view into one on CGameObjectInstance: the owner is a player,
 * its 0xBB0 m_pOnEnemy. */
s32 func_00268210(CGameObjectInstance *obj) {
    switch (obj->header.m_pIntelligence->m_Type) {
    case INTELLIGENCE_PLAYER:
        if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)
            && (((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy != NULL || Cinema_PathPlaying() != 0)) {
            return 1;
        }
        return 0;
    case INTELLIGENCE_ENEMY:
    case INTELLIGENCE_THROWABLE:
        return 1;
    default:
        return 0;
    }
}

s32 GameObject_TiltsToGround(CGameObjectInstance *obj) {
    return ((u32) obj->header.m_pIntelligence->m_dwFlags >> 2) & 1;
}

s32 GameObject_IsEnemy(CGameObjectInstance *obj) {
    return obj->header.m_pIntelligence->m_Type == 1;
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

s32 GameObject_IsInanimate(void *arg0) {
#ifdef TARGET_PORT
    /* the instance header's m_pIntelligence (N64 0x14) by member: its m_Type */
    switch (*(s32 *) ((CInstanceHeader *) arg0)->m_pIntelligence) {
#else
    switch (**(s32 **)((u8 *)arg0 + 0x14)) {
#endif
        case 0:
        case 12:
        default:
            return 1;
        case 1:
        case 4:
        case 9:
        case 11:
            return 0;
    }
}

f32 GameObject_GetWallRadius(CGameObjectInstance *a) {
    switch (a->header.m_pIntelligence->m_Type) {
    case 11:
        return a->header.m_pIntelligence->m_Player.m_CollisionRadius;
    case 1:
        return a->header.m_pIntelligence->m_Enemy.m_CollisionWallRadius;
    case 2:
        if (a->header.m_pIntelligence->m_Platform.m_CollisionType == 0) {
            return 256.0f;
        }
        return a->header.m_pIntelligence->m_Platform.m_CollisionRadius;
    case 5:
        return a->header.m_pIntelligence->m_Throwable.m_CollisionRadius;
    case 8:
        if (a->header.m_pIntelligence->m_Pickup.m_dwFlags & 1) {
            return 81.92f;
        }
        return a->header.m_pIntelligence->m_Pickup.m_CollisionRadius;
    default:
        return 61.44f;
    }
}

/* A dead object's collision height: the enemy block's (players and turrets share it), 0 for the
 * other definitions. Round 28 joined the N64 view and the port's (the definition by member; it is
 * asset data at its N64 byte offsets). */
f32 GameObject_GetDeadCollisionHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    switch (def->m_Type) {
        case INTELLIGENCE_ENEMY:
        case INTELLIGENCE_TURRET:
        case INTELLIGENCE_PLAYER:
            return def->m_Enemy.m_CollisionDeadHeight;
    }
    return 0.0f;
}

/* An object's collision y offset; scenery (definition type 0) keeps it in its definition.
 * Round 28 joined the N64 view and the port's host-offset one on CGameObjectInstance. */
f32 GameObject_GetCollisionYOffset(CGameObjectInstance *item) {
    CIntelligence *def;

    if (item->header.m_Type == 1) {
        return item->m_CollisionYOffset;
    }
    def = item->header.m_pIntelligence;
    if (def->m_Type == INTELLIGENCE_STATIC) {
        return def->m_Static.m_CollisionHeightOffset;
    }
    return 0.0f;
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"

/* The port: the same on host types (the N64 text reads the instance and the player at N64
 * offsets: 0x14 the intelligence, 0xD4 the flags, 0x1A8 the AI parent, CPlayer 0xBB0 the
 * mounted enemy). The wall radius of a collision sweep: the enemy/turret/player definition's
 * m_CollisionWallRadius (0x2C), the throwable's radius (0x18); a riding player uses its mount's. */
f32 GameObject_GetSweepRadius(u8 *actor) {
    CGameObjectInstance *obj = (CGameObjectInstance *) actor;
    CIntelligence *def = obj->header.m_pIntelligence;
    CGameObjectInstance *mount;

    switch (def->m_Type) {
        case 11:
            if (obj->header.m_Type == 1 && (obj->m_dwFlags & 0x300000)) {
                mount = ((CPlayer *) obj->m_AI.m_pParent)->m_pOnEnemy;
                if (mount != NULL) {
                    return GameObject_GetSweepRadius((u8 *) mount);
                }
            }
            return *(f32 *) ((u8 *) def + 0x2C);
        case 1:
        case 4:
            return *(f32 *) ((u8 *) def + 0x2C);
        case 5:
            return *(f32 *) ((u8 *) def + 0x18);
    }
    return 0.0f;
}
#else

f32 GameObject_GetSweepRadius(u8 *actor) {
    u8 *def = *(u8 **) (actor + 0x14);
    u8 *weapon;

    switch (*(s32 *) def) {
        case 11:
            if (actor[0] == 1 && (*(s32 *) (actor + 0xD4) & 0x300000)) {
                weapon = *(u8 **) (*(u8 **) (actor + 0x1A8) + 0xBB0);
                if (weapon != NULL) {
                    return GameObject_GetSweepRadius(weapon);
                }
            }
            return *(f32 *) (*(u8 **) (actor + 0x14) + 0x2C);
        case 1:
        case 4:
            return *(f32 *) (def + 0x2C);
        case 5:
            return *(f32 *) (def + 0x18);
    }
    return 0.0f;
}
#endif

/* Enemies only: the enemy definition's m_AttackCombatRadius. Round 22 rewrote the m2c draft (two equal arms or a dead
 * do-while, an unmarked fake) as the getters round 21 de-faked: the case that returns the
 * field is tested first. */
f32 GameObject_GetAttackCombatRadius(CGameObjectInstance *obj) {
    CIntelligence *def = obj->header.m_pIntelligence;

    if (def->m_Type == 1) {
        return def->m_Enemy.m_AttackCombatRadius;
    }
    return 0.0f;
}

/* De-faked in round 21 (j4): the matched draft wrapped the return in an if/else with two
 * identical arms; testing for the case that returns the field first gives the ROM's layout
 * (the field's path after the branch, the beq's delay slot empty). */

/* An enemy definition's leash radius (0 for other definitions). */
f32 GameObject_GetLeashRadius(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def->m_Type == INTELLIGENCE_ENEMY) {
        return def->m_Enemy.m_LeashRadius;
    }
    return 0.0f;
}

/* The floor point under point: traced from its x and z at height 0, then the height of the region
 * the trace ends in. Round 28: the position is a CVector3 (Rage Wars' TripleRecAt8 had an s32 y,
 * which on the host needed the GameObject_TraceMove_fif wrapper of port/game/t2abi.c). */
void GameObject_GetFloorPointAt(CInstanceHeader *obj, CVector3 *point, CVector3 *out) {
    CVector3 pos;
    CGameRegion *region;

    pos.x = point->x;
    pos.y = 0.0f;
    pos.z = point->z;
    GameObject_TraceMove(obj, pos, out, &region, 0, 0);
    out->y = CGameRegion__GetHeight(region, out->x, out->z);
}

s32 GameObject_GetInstanceBelow(u8 *arg0) {
    if (arg0[0] == 1) {
        return *(s32 *)(arg0 + 0x30);
    }
    return 0;
}

/* Whether the actor stands on (or within 0.512 above) the ground. The ROM passes a second
 * argument (0) that GameObject_GetGroundHeight ignores. */
s32 GameObject_IsOnGround(CGameObjectInstance *actor) {
    s32 state;

    if (actor->header.m_Type == 1) {
        if (actor->m_dwFlags & 0x1000) {
            return 1;
        }
        state = actor->m_AI.m_dwFlags;
        if ((state & 0xC0000) && !(state & 0x40000)) {
            return 1;
        }
    }
    return actor->header.m_vPos.y - ((f32 (*)(CGameObjectInstance *, s32)) GameObject_GetGroundHeight)(actor, 0) <= 0.512f;
}

static __inline__ f32 nodeFloor_268688(CGameObjectInstance *actor, f32 x, f32 z) {
    CGameRegion *node = actor->header.m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = actor->header.m_vPos.y;
    } else {
        result = CGameRegion__GetHeight(node, x, z);
    }
    return result;
}

/* Floor height under (x, z) for the actor: a platform's top for the platforms that collide by
 * shape (collision types 1 and 2), its region's floor for type 0, its own height otherwise. */
f32 GameObject_GetFloorHeightAt(CGameObjectInstance *actor, f32 x, f32 z) {
    CIntelligence *def;

    if (actor->header.m_Type == 1) {
        def = actor->header.m_pIntelligence;
        if (def->m_Type == INTELLIGENCE_PLATFORM) {
            switch (def->m_Platform.m_CollisionType) {
                case 0:
                    return nodeFloor_268688(actor, x, z);
                case 1:
                case 2:
                    return Platform_GetTopHeight(actor, x, z);
            }
        }
    }
    return actor->header.m_vPos.y;
}

/* The floor height under an object (offset by offset in x and z when given): its region's, or
 * its own height when it has no region. */
f32 GameObject_GetRegionFloorHeight(CInstanceHeader *obj, CVector3 *offset) {
    CGameRegion *node = obj->m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = obj->m_vPos.y;
    } else if (offset == NULL) {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x, obj->m_vPos.z);
    } else {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x + offset->x, obj->m_vPos.z + offset->z);
    }
    return result;
}

void AI_ComputeFrameMotion(CGameObjectInstance *a, CAI *b, CAIFrameMotion *o);

/* The distance a live object moved this frame (zero for the others). Round 28 joined the N64 view
 * (a 12-byte Triple that AI_ComputeFrameMotion fills to 0x3C bytes) and the port's (the whole
 * CAIFrameMotion: on the host the Triple overwrote the caller's frame). */
void GameObject_GetFrameDelta(CGameObjectInstance *obj, CVector3 *delta) {
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        *delta = motion.m_vDelta;
        return;
    }
    delta->x = 0.0f;
    delta->y = 0.0f;
    delta->z = 0.0f;
}

/* The turn a live object made this frame (0 for the others). Round 28 joined the N64 view and the
 * port's (the turn by member, N64 buffer + 0x38). */
f32 GameObject_GetFrameTurn(CGameObjectInstance *obj) {
    CAIFrameMotion motion;

    if (obj->header.m_Type == 1) {
        AI_ComputeFrameMotion(obj, &obj->m_AI, &motion);
        return motion.m_Turn;
    }
    return 0.0f;
}

/* GameObject_GetRegionFloorHeight, inlined here. */
static __inline__ f32 floorHeight(CInstanceHeader *obj, CVector3 *offset) {
    CGameRegion *node = obj->m_pCurrentRegion;
    f32 result;

    if (node == NULL) {
        result = obj->m_vPos.y;
    } else if (offset == NULL) {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x, obj->m_vPos.z);
    } else {
        result = CGameRegion__GetHeight(node, obj->m_vPos.x + offset->x, obj->m_vPos.z + offset->z);
    }
    return result;
}

/* Whether the actor is on (or within 0.4 above) the floor. Round 28 joined the N64 view and the
 * port's host-offset one (round 11 C26) on CGameObjectInstance. */
s32 GameObject_IsNearFloor(CGameObjectInstance *actor) {
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x1000)) {
        return 1;
    }
    return actor->header.m_vPos.y - floorHeight(&actor->header, NULL) <= 0.4f;
}

#include "structs/vector.h"
#include "structs/collision.h"
#include "globals/collision.h"

/* Moves an actor to pos with collision, without moving it: returns where it would end up and in which region. */
void GameObject_TraceMove(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, CGameRegion **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    CGameRegion *region;
    s8 *flags;

    saved = *actor;
    oldHits = gCollisionResultsP;
    gCollisionResultsP = &hits;
    flags = &gCollisionInfoTrace.m_GroundBehavior;
    oldA = flags[0];
    oldB = flags[-2];
    flags[0] = flagA;
    flags[-2] = flagB;
    Collision_MoveInstance((CGameObjectInstance *) actor, pos, (CCollisionInfo *) (flags - 6));
    gCollisionResultsP = oldHits;
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    flags[0] = oldA;
    flags[-2] = oldB;
}

#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/region.h"

/* The CRegionInfo of the actor's current region (0 without one). */
sptr GameObject_GetRegionInfo(CInstanceHeader *arg0) {
    CGameRegion *r = arg0->m_pCurrentRegion;
    return r != NULL ? (sptr) CScene__GetRegionInfo(&gScene, &r->m_nInfo) : 0;
}

f32 GameObject_GetWaterHeight(CInstanceHeader *arg0) {
    CGameRegion *temp_a1;
    CRegionInfo *temp_v0;

    temp_a1 = arg0->m_pCurrentRegion;
    if ((temp_a1 != NULL) && (temp_a1->m_wFlags & 1)) {
        temp_v0 = CScene__GetRegionInfo(&gScene, &temp_a1->m_nInfo);
        if (temp_v0 != NULL) {
            return temp_v0->m_WaterHeight;
        }
    }
    return 0.0f;
}

#include "structs/camera.h"

/* Height of the actor's region (0 if it has none), relative to the owner's
 * height for a kind-1 actor flagged 0x300000. */
f32 GameObject_GetViewWaterHeight(CGameObjectInstance *actor) {
    CGameRegion *region;
    CRegionInfo *info;
    f32 h;

    region = actor->header.m_pCurrentRegion;
    if (region == NULL || !(region->m_wFlags & 1)
        || (info = CScene__GetRegionInfo(&gScene, &region->m_nInfo)) == NULL) {
        return 0.0f;
    }
    h = info->m_WaterHeight;
    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        h -= ((CPlayer *)actor->m_AI.m_pParent)->m_pCamera->m_vTCorners[0].y;
    }
    return h;
}

/* How far the highest corner of the current region is above the object. */
f32 GameObject_GetRegionTopAbove(CGameObjectInstance *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    CGameRegion *temp_a1;

    temp_a1 = arg0->header.m_pCurrentRegion;
    var_f0 = temp_a1->m_pVertices[1]->y;
    temp_f1 = temp_a1->m_pVertices[0]->y;
    if (!(temp_f1 <= var_f0)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->m_pVertices[2]->y;
    if (!(var_f0 <= var_f1)) {
        var_f1 = var_f0;
    }
    return var_f1 - arg0->header.m_vPos.y;
}

/* Height of arg0 above the lowest vertex of its triangle. */
f32 GameObject_GetHeightAboveRegionBottom(CInstanceHeader *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    CGameRegion *temp_a1;

    temp_a1 = arg0->m_pCurrentRegion;
    var_f0 = temp_a1->m_pVertices[1]->y;
    temp_f1 = temp_a1->m_pVertices[0]->y;
    if (!(var_f0 <= temp_f1)) {
        var_f0 = temp_f1;
    }
    var_f1 = temp_a1->m_pVertices[2]->y;
    if (!(var_f1 <= var_f0)) {
        var_f1 = var_f0;
    }
    return arg0->m_vPos.y - var_f1;
}

/* De-faked in round 21 (j4): the draft returned 0 through three identical arms and read m_dwFlags by
 * offset (a host offset under TARGET_PORT); typed, and testing the kind-1 case first, it matches. */

/* Flag 1 of an instance of kind 1 (0 for other kinds). */
s32 GameObject_HasFlag1(CGameObjectInstance *obj) {
    if (obj->header.m_Type == 1) {
        return obj->m_dwFlags & 1;
    }
    return 0;
}

/* The type id of an object (kind 1) or of a particle. The ROM does not mask the u16 that
 * CScene__GetObjectTypeID returns (its caller's prototype returned a word); the host must
 * (docs/port/ABI.md: the upper bits of the return register are undefined). */
s32 GameObject_GetTypeId(CGameObjectInstance *arg0) {
    if (arg0->header.m_Type == 1) {
        return (u16) arg0->m_nTypeFlag;
    }
#ifdef TARGET_PORT
    return CScene__GetObjectTypeID(&gScene, arg0->header.m_nObjType);
#else
    return ((s32 (*)(struct CScene *, s32)) CScene__GetObjectTypeID)(&gScene, arg0->header.m_nObjType);
#endif
}

u8 *CLevelActorPool__Spawn(CLevelActorPool *table, void *slot, s32 kind, CVector3 velocity, CVector3 pos, CGameRegion *region, f32 arg);

/* GameObject_TraceMove's logic, inlined with the collision flags addressed directly. */
static __inline__ void tracePos(CInstanceHeader *actor, CVector3 pos, CVector3 *outPos, CGameRegion **outRegion, s32 flagA, s32 flagB) {
    CInstanceHeader saved;
    CVector3 end;
    CCollisionResults hits;
    CCollisionResults *oldHits;
    s32 oldA;
    s32 oldB;
    CGameRegion *region;

    saved = *actor;
    oldA = gCollisionInfoTrace.m_GroundBehavior;
    oldB = gCollisionInfoTrace.m_InstanceBehavior;
    oldHits = gCollisionResultsP;
    gCollisionInfoTrace.m_GroundBehavior = flagA;
    gCollisionInfoTrace.m_InstanceBehavior = flagB;
    gCollisionResultsP = &hits;
    Collision_MoveInstance((CGameObjectInstance *) actor, pos, &gCollisionInfoTrace);
    actor->m_vPos.y = pos.y;
    end.x = actor->m_vPos.x;
    end.y = pos.y;
    end.z = actor->m_vPos.z;
    region = actor->m_pCurrentRegion;
    gCollisionResultsP = oldHits;
    *actor = saved;
    *outPos = end;
    *outRegion = region;
    gCollisionInfoTrace.m_GroundBehavior = oldA;
    gCollisionInfoTrace.m_InstanceBehavior = oldB;
}

/* Spawns a particle of the given kind where an actor would end up at pos. */
void GameObject_DropPickupAt(CInstanceHeader *actor, CVector3 pos, s32 kind, f32 arg) {
    CVector3 up;
    CVector3 dir;
    CVector3 velocity;
    CVector3 at;
    CGameRegion *region;

    tracePos(actor, pos, &at, &region, 0, 0);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    dir = CVector3__RandomPerturb(&up, 0.25f);
    CVector3__Normalize(&dir);
    CVector3__Scale(&velocity, &dir, 409.59998f);
    CLevelActorPool__Spawn(&gScene.m_ActorPool, NULL, kind, velocity, at, region, arg);
}

s32 CGameObjectInstance__GetGeometryKind(void *arg);

/* The kind of floor an object stands on (footsteps, splashes): -1, 7 or 8 for the region flags
 * 0x4000, 0x400000 and 0x80 (second word), else the region's own kind; 1 while the AI's flag 0x400
 * is set; what the object below is made of while it rides one (AI flag 0x40000). */
s32 GameObject_GetFloorKind(CGameObjectInstance *obj) {
    CGameRegion *region = obj->header.m_pCurrentRegion;
    CRegionInfo *info;
    s32 result = 0;
    u32 objFlags;
    CGameObjectInstance *other;

    if (region != NULL && (info = CScene__GetRegionInfo(&gScene, &region->m_nInfo)) != NULL) {
        if (info->m_dwFlags & 0x4000) {
            result = -1;
        } else if (info->m_dwFlags & 0x400000) {
            result = 7;
        } else if (info->m_wFlags52 & 0x80) {
            result = 8;
        } else {
            result = (u8) info->unk_58;
        }
    }
    if (obj->header.m_Type == 1) {
        objFlags = obj->m_AI.m_dwFlags;
        if (objFlags & 0x400) {
            result = 1;
        }
        if (objFlags & 0x40000) {
            other = obj->m_pInstanceBelow;
            if (other != NULL && other->header.m_Type == obj->header.m_Type) {
                result = CGameObjectInstance__GetGeometryKind(other);
            }
        }
    }
    return result;
}

/* Tilt of an actor toward the floor normal under it. */
CQuaternion GameObject_GetFloorTilt(CGameObjectInstance *obj) {
    CVector3 normal;
    CVector3 up;
    CVector3 axis;
    CQuaternion q;
    f32 angle;

    CGameRegion__GetNormal(&normal, obj->header.m_pCurrentRegion);
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    CVector3__Cross(&axis, &up, &normal);
    CVector3__Normalize(&axis);
    angle = Math_AcosLookup(up.x * normal.x + up.y * normal.y + up.z * normal.z) * 0.5f;
    gQuaternionSin = func_002982F0(angle);
    q.x = axis.x * gQuaternionSin;
    q.y = axis.y * gQuaternionSin;
    q.z = axis.z * gQuaternionSin;
    q.w = func_002974C0(angle);
    return q;
}
