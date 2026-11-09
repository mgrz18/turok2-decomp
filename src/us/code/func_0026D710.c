/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/FX/8026E5E0.CPP, func_8026E5E0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/scene.h"
#include "protos/game.h"

/* Animation attachment events: for each attachment whose event bit is enabled
 * on the object, moves its offset through the bone matrix to world space and
 * plays the event: a footstep/impact sound (louder when the bone's surface is
 * water), the splash effects, or a dust puff of 5/10/15/20 units. */

typedef struct {
    /* 0x00 */ CVector3 offset;
    /* 0x0C */ u32 event;
    /* 0x10 */ u16 bone;
    /* 0x12 */ u8 pad12[0x14 - 0x12];
} Attachment;

typedef struct {
    s16 lifeBase;
    s16 lifeRange;
    s8 chance;
    s8 modifier;
    s8 countBase;
    s8 countRange;
} __attribute__((aligned(4))) PuffArgs;


extern u8 *CISet__GetBlock(void *list, s32 index);
extern void CMtxF__TransformPoint(CMtxF *mtx, CVector3 *in, CVector3 *out);
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
extern void Actor_SpawnParticle(CGameObjectInstance *obj, s32 sound, CVector3 pos, s32 flags);
extern void GameObject_TraceMove(CGameObjectInstance *obj, CVector3 pos, CVector3 *out, s32 *region, s32 a, s32 b);
extern f32 func_0021F2B0(CGameObjectInstance *obj);
extern void ImpactEvent_Send(CGameObjectInstance *obj, CGameObjectInstance *owner, s32 type, s32 id, CVector3 pos, PuffArgs args);

void GameObject_EmitHotPoints(Attachment *ents, s32 n, CMtxF *mtxs, CGameObjectInstance *obj, void *bones) {
    CVector3 pos;
    CVector3 mapped;
    CVector3 zero;
    CQuaternion rot;
    PuffArgs puff;
    s32 region;
    f32 len;
    s32 kind;
    s32 flag;
    s32 size;

    for (n = n - 1; n != -1; n--) {
        if (ents->event & obj->m_dwHotPointFlags) {
            flag = (CISet__GetBlock(CISet__GetBlock(bones, ents->bone), 1)[0x66] == 5) * 2;
            size = 0;
            CMtxF__TransformPoint(&mtxs[ents->bone], &ents->offset, &pos);
            CVector3__Scale(&pos, &pos, 1.0f / 65536.0f);
            switch (ents->event) {
            case 0x2:
                Actor_SpawnParticle(obj, 0x51, pos, flag);
                break;
            case 0x8:
                Actor_SpawnParticle(obj, 0x5C, pos, flag);
                break;
            case 0x1:
            case 0x4:
            case 0x10:
            case 0x20:
                break;
            case 0x40:
                obj->m_AI.m_vBoreTarget = pos;
                kind = obj->header.m_pIntelligence->m_Type;
                if ((kind == 1 && obj->m_AI.m_Mode == 0x21) || (kind == 0xB && ((CPlayer *) obj->m_AI.m_pParent)->m_Mode == 0x23)) {
                    GameObject_TraceMove(obj, pos, &mapped, &region, 0, 1);
                    CQuaternion__FromDirection(&rot, &obj->m_AI.m_vBoreCollNormal);
                    zero.x = 0;
                    zero.y = 0;
                    zero.z = 0;
                    if (obj->m_AI.m_ModeTime < 15.0f) {
                        CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, 0, 0, 0xE6, zero, rot, mapped, region, -1, flag | 1);
                    }
                    CParticleSystem__Spawn(&gScene.m_ParticleSystem, obj, obj, 0, 0, 0x8A, zero, rot, mapped, region, -1, flag | 1);
                }
                break;
            case 0x80:
                Actor_SpawnParticle(obj, 0xAC, pos, flag);
                break;
            case 0x100:
                Actor_SpawnParticle(obj, 0xAD, pos, flag);
                break;
            case 0x200:
                Actor_SpawnParticle(obj, 0xAE, pos, flag);
                break;
            case 0x400:
                Actor_SpawnParticle(obj, 0xAF, pos, flag);
                break;
            case 0x800:
                Actor_SpawnParticle(obj, 0xB0, pos, flag);
                break;
            case 0x1000:
                Actor_SpawnParticle(obj, 0xB1, pos, flag);
                break;
            case 0x2000:
                Actor_SpawnParticle(obj, 0xB2, pos, flag);
                break;
            case 0x4000:
                Actor_SpawnParticle(obj, 0xB3, pos, flag);
                break;
            case 0x8000:
                Actor_SpawnParticle(obj, 0xB4, pos, flag);
                break;
            case 0x10000:
                Actor_SpawnParticle(obj, 0xB5, pos, flag);
                break;
            case 0x20000:
                Actor_SpawnParticle(obj, 0xB6, pos, flag);
                break;
            case 0x40000:
                Actor_SpawnParticle(obj, 0xB7, pos, flag);
                break;
            case 0x80000:
                Actor_SpawnParticle(obj, 0xB8, pos, flag);
                break;
            case 0x100000:
                Actor_SpawnParticle(obj, 0xB9, pos, flag);
                break;
            case 0x200000:
                Actor_SpawnParticle(obj, 0xBA, pos, flag);
                break;
            case 0x400000:
                Actor_SpawnParticle(obj, 0xBB, pos, flag);
                break;
            case 0x20000000:
                Actor_SpawnParticle(obj, 0x1E9, pos, flag);
                break;
            case 0x40000000:
                Actor_SpawnParticle(obj, 0x1EA, pos, flag);
                break;
            case 0x80000000:
                Actor_SpawnParticle(obj, 0x1EB, pos, flag);
                break;
            case 0x8000000:
            case 0x10000000:
                Actor_SpawnParticle(obj, 0x11C, pos, flag);
                break;
            case 0x800000:
                size = 5;
                break;
            case 0x2000000:
                size = 0xF;
                break;
            case 0x1000000:
                size = 0xA;
                break;
            case 0x4000000:
                size = 0x14;
                break;
            }
            if (size != 0) {
                len = func_0021F2B0(obj);
                puff.lifeRange = size << 8;
                puff.chance = puff.modifier = puff.countBase = puff.countRange = 0;
                {
                    f32 t = 5.0f;
                    t = t / len;
                    puff.lifeBase = t * 256.0f;
                }
                ImpactEvent_Send(obj, obj, 3, -1, pos, puff);
            }
        }
        ents++;
    }
}
