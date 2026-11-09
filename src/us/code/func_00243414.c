#include "structs/vector.h"
#include "globals/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80214178_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "protos/ai.h"
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct SoundSourceNode;
struct SoundSourceNode {
    s32 unk0;
    struct SoundSourceNode *next;
    s32 value;
    s32 state;
    CVector3 position;
    void *owner;
};
struct SoundSourceNode;
struct SoundSourceNode;
typedef struct SoundSourceNode SoundSourceNode;
struct Dest;
typedef struct Dest Dest;
struct Source110;
typedef struct Source110 Source110;
struct StateEntry;
typedef struct StateEntry StateEntry;
struct Source110;
struct Source110 { /* Turok 2 layout */
    char pad0[0x18];
    CVector3 velocity;
    char pad24[0xD4 - 0x24];
    u32 flags;
    f32 limit;
    s16 current_state;
    s16 previous_state;
    s16 counter;
    s8 state_changed;
    u8 animationChanged;
    u8 padE4[0x13];
    u8 stateChanged;
    u8 padF8[0x27C - 0xF8];
    u32 renderFlags;
};
struct Dest;
struct Source110;
struct StateEntry;
struct Dest {
    u32 flags;
    s32 health;
    s32 maxHealth;
    s32 handle;
    s8 level;
    u8 pad11;
    u8 state12, state13;
    s8 state14[4];
    f32 initialScale;
    u8 pad1C[0x10];
    void *table;
    StateEntry *entry;
    s8 state;
    s8 old_state;
    s8 requestedState;
    char pad37;
    u32 old_entry_flags;
    u32 entry_flags;
    s32 stateTimer;
    u8 pad44[4];
    CVector3 initialPosition;
    CVector3 previousPosition;
    f32 initialHeight;
    f32 cueTimer;
    void *field68;
    u8 reserved6C[0x18];
    u8 reserved84[8];
    f32 height8C;
    f32 height90;
    u8 state94, state95;
    s16 state96;
    u8 reserved98[4];
    f32 height9C;
    CVector3 positionA0;
    void *surfaceAC;
    CVector3 positionB0;
    void *surfaceBC;
    f32 scale; /* Turok 2: no entry_scale, count at 0xC7 */
    s16 result;
    s8 state_result;
    u8 count;
    s8 secondaryLevel;
    u8 padCD[3];
    s32 fieldD0, fieldD4;
    u8 reservedD4[0x18];
    struct SoundSourceNode *sound; /* 0xEC in Turok 2 */
    u8 pending;
    u8 pad101;
    s16 channel;
    u8 pad104[4];
    void *enter;
    void *draw;
    void *hit;
    u8 pad114[8];
    void *callback11C;
    u8 reserved120[4];
    s32 field124;
    f32 charge;
    f32 field12C;
    s32 field130;
    f32 restPitch;
    s32 field138, field13C;
    u8 pad140[4];
    s32 field144, field148, field14C, field150;
    u8 reserved154[0x1C];
};
typedef void *( *StateCallback)(struct Source110 *, struct Dest *);
struct StateEntry {          /* CAIModeInfo (structs/game_object.h) */
    sptr id;
    void (*enter)(struct Source110 *, struct Dest *);
#ifdef TARGET_PORT
    void *pad8[2];
#else
    char pad8[8];
#endif
    StateCallback callback;
    f32 scale;
    s32 threshold;
    u32 flags;
};
struct ActorStateTable {
    struct ActorStateTable *next;   /* the header row */
#ifdef TARGET_PORT
    u8 pad4[sizeof(StateEntry) - sizeof(void *)];
#else
    u8 pad4[0x1C];
#endif
    StateEntry entries[1];
};
extern void AI_StopSound(void *, void *);
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port: the same on CGameObjectInstance/CAI/CAIModeInfo (the Rage Wars Source110/Dest views
 * are the N64 layout): switches the AI to mode arg2, found in its chain of mode tables (each
 * table's header row links the next one in m_Mode, its rows end with m_Mode -1). */
s32 CAI__SetMode(void *arg0, void *arg1, s32 arg2) {
    CGameObjectInstance *obj = (CGameObjectInstance *) arg0;
    CAI *ai = (CAI *) arg1;
    CAIModeInfo *node;
    CAIModeInfo *entry;
    CAIModeInfo *scan;
    u32 flags;

    if (gScene.m_Mode == 2) {
        return 1;
    }
    if (gScene.m_Mode == 4) {
        obj->m_dwFlags |= 0x100;
    }
    ai->m_ModeBefore = ai->m_Mode;
    obj->m_CurrentAnimation.m_bAnimActive = 1;
    obj->m_BlendAnimation.m_bAnimActive = 1;
    if (ai->m_pModeCurrent != NULL && ai->m_Mode == arg2) {
        return 1;
    }
    node = ai->m_pModeTable;
    entry = NULL;
    ai->m_Mode = arg2;
    *(s32 *) &ai->m_ModeTime = 0;
    while (node != NULL) {
        for (scan = node + 1; scan->m_Mode != -1; scan++) {
            if (scan->m_Mode == arg2) {
                entry = scan;
                break;
            }
        }
        if (entry != NULL) {
            break;
        }
        node = (CAIModeInfo *) node->m_Mode;
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
        AI_StopSound(arg0, arg1);
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
    return ai->m_Mode == arg2;
}
#else
s32 CAI__SetMode(void *arg0, void *arg1, s32 arg2) {
    struct ActorStateTable *node;
    StateEntry *entry;
    StateEntry *scan;
    s32 flags;
    if (gScene.m_Mode == 2) {
        return 1;
    }
    if (gScene.m_Mode == 4) {
        ((Source110 *)arg0)->flags |= 0x100;
    }
    ((Dest *)arg1)->requestedState = ((Dest *)arg1)->state;
    ((Source110 *)arg0)->animationChanged = 1;
    ((Source110 *)arg0)->stateChanged = 1;
    if ((((Dest *)arg1)->entry != 0) && (((Dest *)arg1)->state == arg2)) {
        return 1;
    }
    node = ((Dest *)arg1)->table;
    entry = 0;
    ((Dest *)arg1)->state = arg2;
    ((Dest *)arg1)->stateTimer = 0;
    while (node != 0) {
        scan = node->entries;
        if (scan->id != -1) {
            while (scan->id != -1) {
                if (scan->id == arg2) {
                    entry = scan;
                    node = 0;
                    break;
                }
                scan = scan + 1;
            }
        }
        if (node != 0) {
            node = node->next;
        }
    }
    if (entry == 0) {
        return 0;
    }
    ((Dest *)arg1)->entry = entry;
    ((Source110 *)arg0)->renderFlags &= 0x1F80007F;
    if ((((Source110 *)arg0)->flags & 0x08000000) == 0) {
        ((Dest *)arg1)->count = 0;
        ((Dest *)arg1)->flags &= ~1U;
    }
    if (((Dest *)arg1)->sound != 0) {
        AI_StopSound(arg0, arg1);
    }
    if (entry->enter != 0) {
        entry->enter(arg0, arg1);
    }
    flags = entry->flags;
    ((Dest *)arg1)->entry_flags = flags;
    if (flags & 4) {
        ((Source110 *)arg0)->velocity.x = 0;
        ((Source110 *)arg0)->velocity.y = 0;
        ((Source110 *)arg0)->velocity.z = 0;
    }
    return ((Dest *)arg1)->state == arg2;
}
#endif
