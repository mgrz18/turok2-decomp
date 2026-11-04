/* Boundary fixed in versions/cuts.us.txt: the function starts at 0x00267090. splat cut its first word,
 * `lw $2, 0x14($4)`, into a one-word func_00267090 (the name its callers
 * jal), and the rest into func_00267094. Written whole under its real name.
 * Its switch reads jtbl_800A7D30, which still lives in the asm .rodata. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include "structs/player.h"


typedef struct {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ u32 flags;
} Attachment;


extern u8 D_8011AAF1;

s32 func_00267090(CGameObjectInstance *actor) {
    CPlayer *owner;
    Attachment *attachment;
    u32 flags;
    u32 mask;

    switch ((u32) actor->header.m_pIntelligence->m_Type) {
        case 11:
            if (actor->header.m_Type != 1) {
                return 1;
            }
            if (actor->m_dwFlags & 0x300000) {
                owner = ((CPlayer *)actor->m_AI.m_pParent);
                if (D_8011AAF1 != 0 && owner->m_bEnteringDeathmatch != 0) {
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
            if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x2000) && (attachment = ((Attachment *)actor->m_AI.m_pModeCurrent)) != NULL) {
                mask = 0x10000;
                return (attachment->flags & mask) == 0;
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
