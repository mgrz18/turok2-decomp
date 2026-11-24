#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/vector.h"
#include "structs/game_object.h"
#include "globals/engine.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80213ED4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* A mode's animation (CAIModeInfo.unk_10): a function that picks it, a table of actions, or an id. */
typedef void *(*StateCallback)(CGameObjectInstance *, CAI *);

s32 CGameObjectInstance__SetAnim(CGameObjectInstance *obj, sptr anim, s32 arg2);
s32 IsCodePointer(StateCallback);
s32 IsDataPointer(uptr);
void *CGameObjectInstance__PickAvailableAction(CGameObjectInstance *obj, void *table, s32 arg2);

/* Runs the current mode's animation: on a mode change picks the mode's animation (through its
 * picker or its action table) and its speed; counts the cycles the animation completes. Round 28:
 * on CGameObjectInstance and CAI (the N64 views and the port's host-offset ones joined: at the N64
 * offsets the host missed the animation speeds, and no animation advanced). */
void CAI__UpdateModeAnim(CGameObjectInstance *src, CAI *dst) {
    CAIModeInfo *entry;
    StateCallback callback;
    f32 factor;
    entry = dst->m_pModeCurrent;
    if (entry != 0) {
        if (dst->m_ModeLastFrame != dst->m_Mode) {
            if (entry->m_AnimSpeed >= 0.0f) {
                dst->m_AnimModeSpeed = entry->m_AnimSpeed;
            }
            dst->m_ModeLastFrameFlags = dst->m_ModeFlags;
            dst->m_ModeFlags = entry->unk_1C;
            src->m_CurrentAnimation.m_bCycleCompleted = 0;
            callback = (StateCallback) entry->unk_10;
            if (callback != 0) {
                if (IsCodePointer(callback) != 0) {
#ifdef TARGET_PORT
                    /* The pickers return a 16-bit animation action id (MotherBoss_PickPhaseAnim and
                     * the rest return s16; the action tables are BE16, CGameObjectInstance__PickAvailableAction). MIPS hands the
                     * caller a sign-extended v0; x86-64 and arm64 leave the register's upper bits
                     * unspecified (Linux: 0x42BF55FA for 0x55FA), and PickRandomAnim then found no
                     * animation: the Mother's attack anim never played on Linux. */
                    callback = (StateCallback)(sptr)(s16)(sptr)callback(src, dst);
#else
                    callback = callback(src, dst);
#endif
                } else if (IsDataPointer((uptr)callback) != 0) {
                    callback = (StateCallback)CGameObjectInstance__PickAvailableAction(src, callback, dst->m_AnimIndex);
                }
                if (callback != 0) {
                    if (callback != (StateCallback)-1) {
                        dst->m_AnimIndex = CGameObjectInstance__PickRandomAnim(src, (sptr)callback, dst->m_AnimIndex);
                        dst->m_AnimType = (s16)callback;
                        if (src->m_CurrentAnimation.m_nAnimIndex == dst->m_AnimIndex) {
                            src->m_CurrentAnimation.m_AnimationFrame = 0.0f;
                        }
                    }
                    dst->m_AnimRepeatedCount = 0;
                    dst->m_dwFlags &= ~1U;
                }
            }
            dst->m_ModeLastFrame = (u8)dst->m_Mode;
        } else if ((dst->m_AnimIndex == -1) || (dst->m_AnimType == -1)) {
            dst->m_AnimRepeatedCount++;
            dst->m_dwFlags |= 1;
        } else if (src->m_CurrentAnimation.m_nAnimIndex == dst->m_AnimIndex) {
            if (!(dst->m_dwFlags & 1)) {
                s32 difference = src->m_CurrentAnimation.m_nFrames - entry->unk_18;
                double converted = difference;
                if (difference < 0) {
                    converted += 4294967296.0;
                }
                if ((f32)converted <= src->m_CurrentAnimation.m_AnimationFrame) {
                    dst->m_AnimRepeatedCount++;
                    dst->m_dwFlags |= 1;
                }
            }
            if (src->m_CurrentAnimation.m_bCycleCompleted != 0) {
                src->m_CurrentAnimation.m_bCycleCompleted = 0;
                if (!(dst->m_dwFlags & 1)) {
                    dst->m_AnimRepeatedCount++;
                }
                dst->m_dwFlags &= ~1U;
            }
        }
        if (dst->m_ModeFlags & 0x10) {
            factor = dst->m_AnimModeSpeed;
        } else {
            factor = dst->m_AnimSpeed * dst->m_AnimModeSpeed;
        }
        gFrameIncrement *= factor;
        CGameObjectInstance__SetAnim(src, dst->m_AnimIndex, (dst->m_ModeFlags >> 5) & 1);
        if ((dst->m_AnimIndex != -1) && (src->m_CurrentAnimation.m_nRequestedAnimIndex != dst->m_AnimIndex)) {
            dst->m_AnimRepeatedCount = 0;
            dst->m_dwFlags &= ~1U;
            src->m_CurrentAnimation.m_bCycleCompleted = 0;
        }
        if (src->m_dwFlags & 0x08000000) {
            dst->m_AnimRepeatedCount++;
        }
    }
}
