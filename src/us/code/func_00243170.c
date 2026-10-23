/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80213ED4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern s32 func_0021E9F8(void *arg0, s32 arg1, s32 arg2);
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
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
struct Source110 {
    char pad0[0x1C];
    CVector3 velocity;
    char pad28[0xD4 - 0x28];
    u32 flags;          /* 0xD4 (0x100 in Rage Wars); following fields 0x2C earlier */
    f32 limit;
    s16 current_state;
    s16 previous_state;
    s16 counter;
    s8 state_changed;
    u8 animationChanged;
    u8 pad110[0x13];
    u8 stateChanged;
    u8 pad124[0x2E0 - 0x124];
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
    f32 height9C;       /* Turok 2 has 4 bytes less before here: scale at 0xBC (0xC0) */
    CVector3 positionA0;
    void *surfaceAC;
    CVector3 positionB0;
    void *surfaceBC;
    f32 scale;
    f32 entry_scale;
    s16 result;
    s8 state_result;
    u8 count;
    s8 secondaryLevel;
    u8 padCD[3];
    s32 fieldD0, fieldD4;
    u8 reservedD8[0x24];
    struct SoundSourceNode *sound;
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
struct StateEntry {
    s32 id;
    void (*enter)(struct Source110 *, struct Dest *);
    char pad8[8];
    StateCallback callback;
    f32 scale;
    s32 threshold;
    u32 flags;
};
extern double D_800A68B8;
extern f32 D_800B6D28;
extern s32 func_00267024(StateCallback);
extern s32 func_00266FC0(u32);
extern void *func_0021EA3C(void *, void *, s32);
extern s32 func_00219F74(void *, s32, s32);
void func_00243170(Source110 *src, Dest *dst) {
    StateEntry *entry;
    StateCallback callback;
    f32 factor;
    entry = dst->entry;
    if (entry != 0) {
        if (dst->old_state != dst->state) {
            if (entry->scale >= 0.0f) {
                dst->entry_scale = entry->scale;
            }
            dst->old_entry_flags = dst->entry_flags;
            dst->entry_flags = entry->flags;
            src->state_changed = 0;
            callback = entry->callback;
            if (callback != 0) {
                if (func_00267024(callback) != 0) {
                    callback = callback(src, dst);
                } else if (func_00266FC0((u32)callback) != 0) {
                    callback = (StateCallback)func_0021EA3C(src, callback, dst->state_result);
                }
                if (callback != 0) {
                    if (callback != (StateCallback)-1) {
                        dst->state_result = func_00219F74(src, (s32)callback, dst->state_result);
                        dst->result = (s16)callback;
                        if (src->current_state == dst->state_result) {
                            src->limit = 0.0f;
                        }
                    }
                    dst->count = 0;
                    dst->flags &= ~1U;
                }
            }
            dst->old_state = (u8)dst->state;
        } else if ((dst->state_result == -1) || (dst->result == -1)) {
            dst->count++;
            dst->flags |= 1;
        } else if (src->current_state == dst->state_result) {
            if (!(dst->flags & 1)) {
                s32 difference = src->counter - entry->threshold;
                double converted = difference;
                if (difference < 0) {
                    converted += D_800A68B8;
                }
                if ((f32)converted <= src->limit) {
                    dst->count++;
                    dst->flags |= 1;
                }
            }
            if (src->state_changed != 0) {
                src->state_changed = 0;
                if (!(dst->flags & 1)) {
                    dst->count++;
                }
                dst->flags &= ~1U;
            }
        }
        if (dst->entry_flags & 0x10) {
            factor = dst->entry_scale;
        } else {
            factor = dst->scale * dst->entry_scale;
        }
        D_800B6D28 *= factor;
        func_0021E9F8(src, dst->state_result, (dst->entry_flags >> 5) & 1);
        if ((dst->state_result != -1) && (src->previous_state != dst->state_result)) {
            dst->count = 0;
            dst->flags &= ~1U;
            src->state_changed = 0;
        }
        if (src->flags & 0x08000000) {
            dst->count++;
        }
    }
}
