/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80214178_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "protos/game_object.h"
#include "protos/cai.h"
#include "structs/game_object.h"
#include "globals/scene.h"


#ifdef TARGET_PORT
#include "globals/ai.h"
/* gendata types a data symbol by the declarations of the C that uses it. The shared base mode
 * table is reached only through the header rows' m_Mode words (integers), so nothing used it:
 * it came out as N64 bytes and the scan below read 0x20-byte rows as host ones. This reference
 * makes gendata lay it out as host CAIModeInfo rows. */
CAIModeInfo *const gAIBaseModeTable_port = gAIBaseModeTable;
#endif

/* Switches the AI to mode, found in its chain of mode tables (each table's header row links the
 * next one in m_Mode, its rows end with m_Mode -1); runs the mode's enter function. Returns
 * whether the AI is in that mode. Round 28: the N64 view (Rage Wars' Source110/Dest/StateEntry)
 * and the port's version on CGameObjectInstance/CAI/CAIModeInfo joined. */
s32 CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode) {
    CAIModeInfo *node;
    CAIModeInfo *entry;
    CAIModeInfo *scan;
    s32 flags;

    if (gScene.m_Mode == 2) {
        return 1;
    }
    if (gScene.m_Mode == 4) {
        obj->m_dwFlags |= 0x100;
    }
    ai->m_ModeBefore = ai->m_Mode;
    obj->m_CurrentAnimation.m_bAnimActive = 1;
    obj->m_BlendAnimation.m_bAnimActive = 1;
    if ((ai->m_pModeCurrent != NULL) && (ai->m_Mode == mode)) {
        return 1;
    }
    node = ai->m_pModeTable;
    entry = NULL;
    ai->m_Mode = mode;
    *(s32 *) &ai->m_ModeTime = 0;
    while (node != NULL) {
        scan = node + 1;
        if (scan->m_Mode != -1) {
            while (scan->m_Mode != -1) {
                if (scan->m_Mode == mode) {
                    entry = scan;
                    node = NULL;
                    break;
                }
                scan = scan + 1;
            }
        }
        if (node != NULL) {
            node = (CAIModeInfo *) node->m_Mode;
        }
    }
    if (entry == NULL) {
        return 0;
    }
    ai->m_pModeCurrent = entry;
    obj->m_dwHotPointFlags &= 0x1F80007F;
    if ((obj->m_dwFlags & 0x08000000) == 0) {
        ai->m_AnimRepeatedCount = 0;
        ai->m_dwFlags &= ~1U;
    }
    if (ai->m_pAttachedLoopingSound != NULL) {
        AI_StopSound(obj, ai);
    }
    if (entry->unk_04 != 0) {
        ((void (*)(CGameObjectInstance *, CAI *)) entry->unk_04)(obj, ai);
    }
    flags = entry->unk_1C;
    ai->m_ModeFlags = flags;
    if (flags & 4) {
        obj->m_vVelocity.x = 0;
        obj->m_vVelocity.y = 0;
        obj->m_vVelocity.z = 0;
    }
    return ai->m_Mode == mode;
}
