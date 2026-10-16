/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025C95C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
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
extern int D_800B2A80;
struct SoundSourceNode;
typedef struct SoundSourceNode SoundSourceNode;
extern s32 CList__AddTail(IntrusiveList *list, void *node);
extern s32 func_00275544(s16 arg0, Vec3 position, s32 arg4, s32 arg5);
void *func_00275C6C(void *arg0, s32 arg1, Vec3 *arg2, void *arg3, s32 arg4) {
    extern void CList__Remove(void *arg0, void *arg1);
    SoundSourceNode *node;
    s32 result;
    node = ((SoundVoicePool *)arg0)->available.head;
    if (node != 0) {
        CList__Remove(arg0, node);
        CList__AddTail(&((SoundVoicePool *)arg0)->playing, node);
        node->state = -1;
        node->value = -1;
        result = func_00275544((s16)arg1, *arg2,
                                (s32)arg3, arg4);
        node->value = result;
        D_800B2A80 = result;
        node->state = arg1;
        node->position = *arg2;
        node->owner = arg3;
    }
    return node;
}
