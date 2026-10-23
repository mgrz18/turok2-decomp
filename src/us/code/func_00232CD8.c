/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C.CPP, func_802846C8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
typedef struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
} IntrusiveList;
typedef struct EffectParams {
    char pad0[8];
    s8 listIndex;
} EffectParams;
typedef struct EffectEntry {
    char pad0[0x38];
    EffectParams *params;
} EffectEntry;
/* Turok 2 offsets: entry 0xF8, ownerId 0x104, next 0x1C8
 * (Rage Wars 0x118 / 0x124 / 0x1EC). */
typedef struct ParticleEffect {
    char pad0[0xF8];
    EffectEntry *entry;
    char padFC[8];
    s32 ownerId;
    char pad108[0xC0];
    struct ParticleEffect *next;
} ParticleEffect;
/* Turok 2: the lists sit 0x1000 earlier than in Rage Wars. */
typedef struct EffectSystem {
    char pad0[0xEC00];
    IntrusiveList free;
    IntrusiveList groups;
    IntrusiveList lists[3];
} EffectSystem;
/* Finds the effect of owner arg1 (and of entry arg2, when given). */
void *func_00232CD8(EffectSystem *arg0, s32 arg1, EffectEntry *arg2) {
    ParticleEffect *record;
    /* index[array] order matches the original addu operand order */
    record = arg2->params->listIndex[arg0->lists].head;
    if (arg2 != 0) {
        if (record != 0) {
            do {
                if (record->ownerId == arg1) {
                    if (record->entry == arg2) {
                        return record;
                    }
                }
                record = record->next;
            } while (record != 0);
        }
    } else if (record != 0) {
        do {
            if (record->ownerId == arg1) {
                return record;
            }
            record = record->next;
        } while (record != 0);
    }
    return 0;
}
