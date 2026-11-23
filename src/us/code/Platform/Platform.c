/* Boundary fixed in versions/cuts.us.txt: splat's func_00263A14 starts with 3 orphan words (addu a0,s0; addu a1,s0;
 * jal CVector3__Add) whose delay slot is this function's first instruction. The real
 * function is Platform_UpdateSpecial (called from Platform_Step); this file matches it from
 * 0x263A20 to the end of splat's func_00263A14. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/engine.h"

f32 Random_Blend(f32 lo, f32 hi);
f32 SmoothStep(f32 x);

static __inline__ s32 canClose(CPlatformIntelligence *params, CAI *ai) {
    if ((params->m_dwPlatformFlags & 0x8000000) && !(ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    if ((params->m_dwPlatformFlags & 0x10000000) && (ai->m_dwFlags & 0x20000)) {
        return 0;
    }
    return 1;
}

void Platform_UpdateSpecial(CGameObjectInstance *self, CAI *ai) {
    CPlatformIntelligence *params;
    s32 oldState;
    s32 flags;

    params = &self->header.m_pIntelligence->m_Platform;
    oldState = ai->m_Action;
    switch (oldState) {
        case 0:
            flags = params->m_dwPlatformFlags;
            if (!(flags & 0xC00000)) {
                break;
            }
            if (flags & 0x4000000) {
                if (ai->m_dwFlags & 0x20000) {
                    ai->m_Platform.m_SpecialTimer += gFrameIncrement;
                } else {
                    ai->m_Platform.m_SpecialTimer = 0.0f;
                    break;
                }
            } else if (ai->m_Platform.m_SpecialTimer > 0.0f || (ai->m_dwFlags & 0x10000)) {
                ai->m_Platform.m_SpecialTimer += gFrameIncrement;
            }
            if (ai->m_Platform.m_SpecialTimer >= params->m_SpecialDelay) {
                ai->m_ActionTime = 0.0f;
                flags = params->m_dwPlatformFlags;
                if (flags & 0x1000000) {
                    ai->m_Action = 3;
                } else if (flags & 0x400000) {
                    ai->m_Action = 1;
                } else if (flags & 0x800000) {
                    ai->m_Action = 4;
                }
            }
            break;
        case 3:
            ai->m_Platform.m_vSpecialOffset.x = Random_Blend(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.y = Random_Blend(-0.512f, 0.512f);
            ai->m_Platform.m_vSpecialOffset.z = Random_Blend(-0.512f, 0.512f);
            ai->m_ActionTime += gFrameIncrement;
            if (ai->m_ActionTime > 45.0f) {
                ai->m_ActionTime = 0.0f;
                flags = params->m_dwPlatformFlags;
                if (flags & 0x400000) {
                    ai->m_Action = 1;
                } else if (flags & 0x800000) {
                    ai->m_Action = 4;
                }
            }
            break;
        case 1:
            if (ai->m_ActionTime < params->m_SpecialSinkTime) {
                ai->m_ActionTime += gFrameIncrement;
            } else if (params->m_dwPlatformFlags & 0x2000000) {
                self->m_dwFlags &= ~0x2000;
            } else {
                ai->m_ActionTime = params->m_SpecialSinkTime;
                if (canClose(params, ai)) {
                    ai->m_Action = 2;
                }
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 2:
            if (ai->m_ActionTime > 0.0f) {
                ai->m_ActionTime -= gFrameIncrement;
            } else {
                ai->m_Action = 0;
                ai->m_Platform.m_SpecialTimer = 0.0f;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            break;
        case 4:
            if (ai->m_ActionTime < params->m_SpecialSinkTime) {
                ai->m_ActionTime += gFrameIncrement;
            }
            if (params->m_SpecialSinkTime > 0.0f) {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist * SmoothStep(ai->m_ActionTime / params->m_SpecialSinkTime);
            } else {
                ai->m_Platform.m_vSpecialOffset.y = -params->m_SpecialSinkDist;
            }
            if (params->m_dwPlatformFlags & 0x2000000) {
                self->m_dwFlags &= ~0x2000;
            }
            break;
    }
    if (oldState != ai->m_Action) {
        ai->m_ModeLastFrame = -1;
    }
}

/* FAKEMATCH (round 21 audit): an if/else whose two arms are the same code, kept
 * for the ROM's register allocation or layout; with one arm: Platform_UpdatePosition: 13 of 191 words. */
#include "structs/vector.h"
#include "common.h"
#include "structs/word.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00206DD4.c, func_00206DD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
typedef struct M1Info M1Info;
typedef struct M1Def M1Def;
typedef struct M1Actor M1Actor;
typedef struct M1State M1State;
struct M1Info {
                u32 flags;
                CWord unk4;
                s8 b8;
                u8 pad9[0x3];
                s32 unkC;
                u8 pad10[0x8];
                s32 unk18;
                s32 unk1C;
                CWord unk20;
                CWord unk24;
                CWord unk28;
                f32 f2C;
                f32 f30;
                f32 f34;
                u8 pad38[0x4];
                f32 f3C;
                CWord unk40;
                f32 f44;
                CWord unk48;
                CWord unk4C;
                u8 b50[4];
                f32 f54;
                f32 f58;
                f32 f5C;
                f32 f60;
                f32 f64;
                f32 f68;
                u8 pad6C[0x4];
                u8 b70;
                u8 b71;
};
struct M1Def {
                s32 kind;
                u32 unk4;
                u8 pad8[0xC];
                M1Info info;
};
/* Turok 2 LevelActor / state: only the fields used here. */
#ifdef TARGET_PORT
struct M1Actor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_M1Actor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_M1Actor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk10[T2_HOFF_M1Actor(header.m_pCurrentRegion)]; sptr unk10; }; /* pointer-wide */ /* N64 0x10 */
        struct { u8 _hp_def[T2_HOFF_M1Actor(header.m_pIntelligence)]; M1Def *def; }; /* N64 0x14 */
        struct { u8 _hp_f50[T2_HOFF_M1Actor(m_RotY)]; f32 f50; }; /* N64 0x50 */
    };
};
#else
struct M1Actor {
    u8 pad0[0x4];
    CVector3 pos;      /* 0x4 */
    s32 unk10;      /* 0x10 */
    M1Def *def;     /* 0x14 */
    u8 pad18[0x38];
    f32 f50;        /* 0x50 (0x6C in Rage Wars) */
};
#endif
#ifdef TARGET_PORT
struct M1State {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_M1State(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_flags3C[T2_HOFF_M1State(m_ModeFlags)]; u32 flags3C; }; /* N64 0x3C */
        struct { u8 _hp_v54[T2_HOFF_M1State(m_vDesiredPos)]; CVector3 v54; }; /* N64 0x54 */
        struct { u8 _hp_f60[T2_HOFF_M1State(m_RotYLastFrame)]; f32 f60; }; /* N64 0x60 */
        struct { u8 _hp_f64[T2_HOFF_M1State(m_ConditionTimer)]; f32 f64; }; /* N64 0x64 */
        struct { u8 _hp_f98[T2_HOFF_M1State(m_InitialRotY)]; f32 f98; }; /* N64 0x98 */
        struct { u8 _hp_v9C[T2_HOFF_M1State(m_vInitialPos)]; CVector3 v9C; }; /* N64 0x9C */
        /* m_VarBuffer (by hand: hostview does not map into the union) */
        struct { u8 _hp_unk110[T2_HOFF_M1State(m_VarS32[1])]; CWord unk110; }; /* N64 0x110 */
        struct { u8 _hp_unk114[T2_HOFF_M1State(m_VarS32[2])]; CWord unk114; }; /* N64 0x114 */
        struct { u8 _hp_vec118[T2_HOFF_M1State(m_VarF32[3])]; CVector3 vec118; }; /* N64 0x118 */
        struct { u8 _hp_f124[T2_HOFF_M1State(m_VarF32[6])]; f32 f124; }; /* N64 0x124 */
    };
};
#else
struct M1State {
    u8 pad0[0x3C];
    u32 flags3C;    /* 0x3C */
    u8 pad40[0x14];
    CVector3 v54;      /* 0x54 */
    f32 f60;
    f32 f64;        /* 0x64 */
    u8 pad68[0x30];
    f32 f98;        /* 0x98 */
    CVector3 v9C;      /* 0x9C */
    u8 padA8[0x68];
    CWord unk110;    /* 0x110 */
    CWord unk114;
    CVector3 vec118;
    f32 f124;       /* 0x124 */
};
#endif

void CVector3__Lerp(CVector3 *dst, f32 t, CVector3 *src, CVector3 *v);
f32 CGameRegion__GetHeight(sptr id, f32 x, f32 z);
void CGameRegion__SetGroupHeight(sptr id, f32 y);

void Platform_UpdatePosition(M1Actor *actor, M1State *state) {
    M1Info *info;
    f32 t;
    f32 lo;
    f32 x;
    f32 fs0;
    f32 c;
    CVector3 v;
    t = state->f64;
    lo = 0.0f;
    info = &actor->def->info;
    if (t < lo || (lo = 1.0f, lo < t)) {
        t = lo;
    }
    if (info->unk28.b[1] == 0) {
        t = SmoothStep(t);
    }
    v = state->v9C;
    v.y += info->f2C;
    if (info->unk4.hu[0] != 0) {
        v.x += info->f30 * func_002982F0(info->f34);
        v.z += info->f30 * func_002974C0(info->f34);
    }
    CVector3__Lerp(&state->v54, t, &state->v9C, &v);
    switch (info->unk28.b[0]) {
    case 0:
        x = func_002982F0(state->f124);
        goto join;
    case 1:
        fs0 = state->f124;
        c = 0.25f;
        state->v54.y += info->f44 * (func_002982F0(fs0) * c);
        state->v54.y += info->f44 * (func_002982F0(fs0 * 1.1f) * c);
        state->v54.y += info->f44 * (func_002982F0(fs0 * 1.2f) * c);
        x = func_002982F0(fs0 * 1.3f) * c;
join:
        state->v54.y += info->f44 * x;
        break;
    }
    state->v54.x += state->unk110.f;
    state->v54.y += state->unk114.f;
    state->v54.z += state->vec118.x;
    switch (info->unk28.b[2]) {
    case 0:
        actor->f50 = state->f98 + t * ((state->f98 + info->f3C) - state->f98);
        break;
    case 1:
        if (!(state->flags3C & 0x400)) {
    case 2:
            if (info || (&actor->pos)) {
                actor->f50 = actor->f50 + info->f3C * 0.06666667f;
            } else {
                actor->f50 = actor->f50 + info->f3C * 0.06666667f;
            }
        }
        break;
    }
    if (info->unk4.hu[0] == 0) {
        CGameRegion__SetGroupHeight(actor->unk10, (CGameRegion__GetHeight(actor->unk10, actor->pos.x, actor->pos.z) + state->v54.y) - actor->pos.y);
    }
}

#include "m2c_macros.h"

void Platform_IdleEnter(void) {

}

/* The port: the AI's m_Action (N64 0x37) and m_Mode (0x34) by member. */
s32 Platform_GetModeAnim(sptr arg0, void *arg1) {
    CAI *ai = (CAI *) arg1;

    switch (ai->m_Action) {
        case 3:
            return 0x5280;
        case 1:
        case 2:
            return 0x5294;
        case 0:
            switch (ai->m_Mode) {
                case 0:
                case 2:
                    return 0x526C;
                case 1:
                case 3:
                    return 0x5276;
            }
            break;
        case 4:
            return 0x528A;
        default:
            return 0x526C;
    }
    return 0x526C;
}

#include "globals/scene.h"
#include "protos/scene.h"

void Platform_IsOn(s32 arg0) {
    CScene__GetInstanceSavedBit(&gScene, arg0);
}

#include "globals/ai.h"

void Platform_CalcOrientation(CGameObjectInstance *self, CAI *ai);
void Platform_Step(u8 *actor, u8 *state);

void Platform_Setup(CGameObjectInstance *arg0, CAI *arg1) {
    CTurretIntelligence *temp_v0;
    CIntelligence *temp_v1;

    temp_v1 = arg0->header.m_pIntelligence;
    arg1->m_pModeTable = gPlatformModeTable;
    arg1->m_pfStep = Platform_Step;
    arg1->m_pfCalculateOrientationMatrix = Platform_CalcOrientation;
    arg1->m_VarBuffer[4] = 0;
    arg1->m_VarBuffer[5] = 0;
    temp_v0 = &temp_v1->m_Turret;
    arg1->m_ConditionTimer = temp_v0->m_RotSpeed;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    arg1->m_VarBuffer[3] = 0;
    *(f32 *)&arg1->m_VarBuffer[6] = temp_v0->m_RotSpeed * 6.283186f;
    if (temp_v1->m_dwTypeFlags & 4) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
    }
}

#include "protos/ai.h"

void Platform_Advance(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_Action = 0;
    CAI__SetMode(arg0, arg1, 0);
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 0) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
    }
}

#include "globals/math.h"

void CMtxF__SetRotateZ(f32 mtx[4][4], f32 angle);
void CMtxF__RotateX(f32 mtx[4][4], f32 angle);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CQuaternion__ToMatrix(CQuaternion *q, f32 mtx[4][4]);
void CMtxF__MultiplyAffine(f32 out[4][4], f32 a[4][4], f32 b[4][4]);
void CMtxF__ScaleAxes(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CVector3__ClampLength(CVector3 *v, f32 limit);
void CMtxF__Translate(f32 mtx[4][4], f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 mtx[4][4]);

void Platform_CalcOrientation(CGameObjectInstance *self, CAI *ai) {
    f32 tilt[4][4];
    f32 spin[4][4];
    CQuaternion q;
    CQuaternion rot;
    f32 sinHalf;

    CMtxF__SetRotateZ(tilt, ai->m_Platform.m_Roll);
    CMtxF__RotateX(tilt, ai->m_Platform.m_Pitch);
    sinHalf = func_002982F0(self->m_RotY * 0.5f);
    q.x = 0.0f;
    q.y = sinHalf;
    q.z = 0.0f;
    gQuaternionSin = sinHalf;
    q.w = func_002974C0(self->m_RotY * 0.5f);
    CQuaternion__Multiply(&rot, &q, &self->m_qGround);
    CQuaternion__ToMatrix(&rot, spin);
    CMtxF__MultiplyAffine(self->m_mfOrient, tilt, spin);
    CMtxF__ScaleAxes(self->m_mfOrient, self->m_vScale.x, self->m_vScale.y, self->m_vScale.z);
    CVector3__ClampLength(&self->header.m_vPos, 20000.0f);
    CMtxF__Translate(self->m_mfOrient, self->header.m_vPos.x, self->header.m_vPos.y, self->header.m_vPos.z);
    CMtxF__SwapYZRows(self->m_mfOrient);
}

typedef struct {
    u8 pad0[0x8];
    void (*onEnter)(void *actor, void *state); /* 0x8 */
} StateHooks;
#ifdef TARGET_PORT
#include "structs/game_object.h"

/* The port: the same on CGameObjectInstance/CAI (N64 offsets: def 0x14, flags 0xD4, the AI's
 * current mode row 0x30 and its enter hook at row + 8, m_VarF32[6] at 0x124). The
 * intelligence body (+0x14, asset data) keeps its byte offset 0x48 (the spin speed). */
void Platform_Step(u8 *actor_, u8 *state_) {
    CGameObjectInstance *actor = (CGameObjectInstance *) actor_;
    CAI *state = (CAI *) state_;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    CAIModeInfo *hooks;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        actor->m_dwFlags &= ~0x10000;
        actor->m_dwFlags &= ~0x100;
        return;
    }
    actor->m_dwFlags |= 0x10000;
    hooks = state->m_pModeCurrent;
    if (hooks != NULL && hooks->m_pfCallback != NULL) {
        ((void (*)(void *, void *)) hooks->m_pfCallback)(actor, state);
    }
    state->m_VarF32[6] += *(f32 *) (info + 0x48) * gFrameIncrement;
    Platform_UpdateSpecial(actor, state);
    Platform_UpdatePosition(actor, state);
}
#else
void Platform_Step(u8 *actor, u8 *state) {
    u8 *info = *(u8 **) (actor + 0x14) + 0x14;
    StateHooks *hooks;

    if (CScene__GetInstanceSavedBit(&gScene, actor) == 0) {
        *(s32 *) (actor + 0xD4) &= ~0x10000;
        *(s32 *) (actor + 0xD4) &= ~0x100;
        return;
    }
    *(s32 *) (actor + 0xD4) |= 0x10000;
    hooks = *(StateHooks **) (state + 0x30);
    if (hooks != NULL && hooks->onEnter != NULL) {
        hooks->onEnter(actor, state);
    }
    *(f32 *) (state + 0x124) += *(f32 *) (info + 0x48) * gFrameIncrement;
    Platform_UpdateSpecial(actor, state);
    Platform_UpdatePosition(actor, state);
}
#endif

void Platform_AtStartEnter(CGameObjectInstance *obj, s32 *arg1) {
    if (!(obj->header.m_pIntelligence->m_Platform.m_dwPlatformFlags & 0x40)) {
        *arg1 &= 0xF7FFFFFF;
    }
}

/* The platform intelligence's flags and m_GoDelayTime; the AI's mode time. */
void Platform_AtStartUpdate(CGameObjectInstance *actor, CAI *state) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    s32 ok = 1;

    if (info->m_dwPlatformFlags & 0x10) {
        u32 f = state->m_dwFlags & 0x8000000;
        ok = f != 0;
    }
    if (info->m_dwPlatformFlags & 0x100) {
        if (!(state->m_dwFlags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->m_dwPlatformFlags & 0x400) {
        if (state->m_dwFlags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->m_ModeTime >= info->m_GoDelayTime) {
            CAI__SetMode(actor, state, 1);
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}

#include "structs/intelligence.h"
#include "protos/game.h"

void Platform_ForwardEnter(CGameObjectInstance *actor, u32 *flags) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    GameObject_FireTriggers(actor, 0x10000, actor);
    if (!(info->m_dwPlatformFlags & 0x40)) {
        *flags &= 0xF7FFFFFF;
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the intelligence from CGameObjectInstance (N64 0x14), the AI's m_dwFlags and
 * m_ConditionTimer (N64 0x64); the platform body (asset data) keeps its byte offsets. */
void Platform_ForwardUpdate(void *arg0, void *arg1) {
    CGameObjectInstance *actor = (CGameObjectInstance *) arg0;
    CAI *ai = (CAI *) arg1;
    u8 *info = (u8 *) actor->header.m_pIntelligence + 0x14;
    s32 flags = *(s32 *) (info + 0x24);
    s32 ok = 1;

    if (flags & 0x10000) {
        ok = (u32) (ai->m_dwFlags & 0x20000) < 1;
    }
    if ((flags & 0x4000) && !(ai->m_dwFlags & 0x20000)) {
        ok = 0;
    }
    if (ok != 0) {
        ai->m_ConditionTimer += gFrameIncrement / *(f32 *) (info + 0x38);
    }
    if (ai->m_ConditionTimer >= 1.0f) {
        CAI__SetMode(arg0, arg1, 2);
    }
}
#else
void Platform_ForwardUpdate(void *arg0, void *arg1)
{
  s32 temp_a3;
  s32 var_a2;
  void *temp_t0;
  unsigned int new_var;
  temp_t0 = (*((s32 *) (((s8 *) arg0) + 0x14))) + 0x14;
  temp_a3 = *((s32 *) (((s8 *) temp_t0) + 0x24));
  var_a2 = 1;
  if (temp_a3 & 0x10000)
  {
    new_var = (*((s32 *) (((s8 *) arg1) + 0))) & 0x20000;
    var_a2 = new_var < 1;
  }
  if ((temp_a3 & 0x4000) && (!((*((s32 *) (((s8 *) arg1) + 0))) & 0x20000)))
  {
    var_a2 = 0;
  }
  if (var_a2 != 0)
  {
    *((f32 *) (((s8 *) arg1) + 0x64)) = (*((f32 *) (((s8 *) arg1) + 0x64))) + (gFrameIncrement / (*((f32 *) (((s8 *) temp_t0) + 0x38))));
  }
  if ((*((f32 *) (((s8 *) arg1) + 0x64))) >= 1.0f)
  {
    CAI__SetMode(arg0, arg1, 2);
  }
}
#endif

void Platform_AtEndEnter(CGameObjectInstance *arg0, s32 *arg1) {
    if (!(arg0->header.m_pIntelligence->m_Platform.m_dwPlatformFlags & 0x80)) {
        *arg1 &= 0xEFFFFFFF;
    }
}

/* The platform intelligence's flags and m_ReturnDelayTime; the AI's mode time. */
void Platform_AtEndUpdate(CGameObjectInstance *actor, CAI *state) {
    CPlatformIntelligence *info = &actor->header.m_pIntelligence->m_Platform;
    s32 ok = 1;

    if (info->m_dwPlatformFlags & 0x20) {
        u32 f = state->m_dwFlags & 0x10000000;
        ok = f != 0;
    }
    if (info->m_dwPlatformFlags & 0x200) {
        if (!(state->m_dwFlags & 0x20000)) {
            ok = 0;
        }
    }
    if (info->m_dwPlatformFlags & 0x800) {
        if (state->m_dwFlags & 0x20000) {
            ok = 0;
        }
    }
    if (ok != 0) {
        if (state->m_ModeTime >= info->m_ReturnDelayTime) {
            CAI__SetMode(actor, state, 3);
        }
    } else {
        state->m_ModeTime = 0.0f;
    }
}

typedef struct B2Info {
    u32 flags;
    u8 pad4[0x20];
    u32 flags24;
} B2Info;
typedef struct B2Def {
    s32 type;
    u8 pad4[0x10];
    B2Info info;            /* 0x14 */
} B2Def;
typedef struct B2Actor {
    u8 pad0[0x14];
    B2Def *def;             /* 0x14 in Turok 2 (0x18 in Rage Wars) */
} B2Actor;
typedef struct B2State {
    u32 flags;
} B2State;

/* The port: the intelligence from CGameObjectInstance (the B2Actor view reads it at N64 0x14). */
void Platform_BackEnter(B2Actor *actor, B2State *state) {
    B2Info *info = (B2Info *) ((u8 *) ((CGameObjectInstance *) actor)->header.m_pIntelligence + 0x14);
    GameObject_FireTriggers(actor, 0x20000, actor);
    if ((info->flags24 & 0x80) == 0) {
        state->flags &= ~0x10000000;
    }
}

void Platform_BackUpdate(CGameObjectInstance *arg0, CAI *arg1) {
    s32 temp_a3;
    s32 var_v1;
    CPlatformIntelligence *temp_t0;

    temp_t0 = &(arg0->header.m_pIntelligence)->m_Platform;
    temp_a3 = temp_t0->m_dwPlatformFlags;
    var_v1 = 1;
    if (temp_a3 & 0x20000) {
        var_v1 = (u32)(arg1->m_dwFlags & 0x20000) < (u32)var_v1;
    }
    if ((temp_a3 & 0x8000) && !(arg1->m_dwFlags & 0x20000)) {
        var_v1 = 0;
    }
    if (var_v1 != 0) {
        arg1->m_ConditionTimer = arg1->m_ConditionTimer - (gFrameIncrement / temp_t0->m_MoveTime);
    }
    if (arg1->m_ConditionTimer <= 0.0f) {
        CAI__SetMode(arg0, arg1, 0);
    }
}

void Platform_EventForward(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    *arg1 |= 0x08000000;
    CAI__SetMode(arg0, arg1, 1);
}

void Platform_EventBack(CGameObjectInstance *arg0, s32 *arg1) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
    *arg1 |= 0x10000000;
    CAI__SetMode(arg0, arg1, 3);
}

void Platform_EventOn(CGameObjectInstance *arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x100);
}

#include "structs/event.h"

void Platform_EventOnRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

void Platform_EventOff(CGameObjectInstance *arg0) {
    CScene__SetInstanceSavedBit(&gScene, arg0, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x100);
}

void Platform_EventOffRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 0);
}

void Platform_EventShow(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}

void Platform_EventRequestForward(CGameObjectInstance *arg0, s32 *arg1) {
    *arg1 |= 0x08000000;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}

void Platform_EventRequestBack(CGameObjectInstance *arg0, s32 *arg1) {
    *arg1 |= 0x10000000;
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x2100);
}

void Platform_EventHide(CGameObjectInstance *arg0) {
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & ~0x2000);
}
