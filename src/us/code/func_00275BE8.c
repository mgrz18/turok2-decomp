/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025C8D8_de.
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
struct SoundSourceNode;
typedef struct SoundSourceNode SoundSourceNode;
extern void CList__Construct(IntrusiveList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(IntrusiveList *list, void *node);
void func_00275BE8(void *arg0, SoundSourceNode *arg1, s32 arg2) {
    s32 i;
    CList__Construct(arg0, 0, 4);
    CList__Construct(&((SoundVoicePool *)arg0)->playing, 0, 4);
    i = 0;
    if (arg2 > 0) {
        do {
            CList__AddTail(arg0, arg1);
            i += 1;
            arg1 = arg1 + 1;
        } while (i < arg2);
    }
    ((SoundVoicePool *)arg0)->state = 0;
}
