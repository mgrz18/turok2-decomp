/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025CA24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
typedef struct SoundVoicePool {
    IntrusiveList available;
    IntrusiveList playing;
    s32 state;
} SoundVoicePool;
struct SoundSourceNode;
struct SoundSourceNode {
    s32 unk0;
    struct SoundSourceNode *next;
    s32 value;
    s32 state;
    Vec3 position;
    void *owner;
};
struct SoundSourceNode;
extern int D_800B2A84;
struct SoundSourceNode;
typedef struct SoundSourceNode SoundSourceNode;
extern s32 CList__AddTail(IntrusiveList *list, void *node);
extern void func_00275864(s32 arg0);
s32 func_00275D34(void *arg0, void *arg1) {
    extern void CList__Remove(void *, s32);
    void *var_s0;
    if (arg1 == 0) {
        return 0;
    }
    var_s0 = ((SoundVoicePool *)arg0)->playing.head;
    if (var_s0 != 0) {
        do {
            if (var_s0 == arg1) {
                func_00275864(((SoundSourceNode *)var_s0)->value);
                D_800B2A84 = ((SoundSourceNode *)var_s0)->value;
                CList__Remove(&((SoundVoicePool *)arg0)->playing, (s32)var_s0);
                CList__AddTail(arg0, var_s0);
                return 1;
            }
            var_s0 = ((SoundSourceNode *)var_s0)->next;
        } while (var_s0 != 0);
    }
    return 0;
}
