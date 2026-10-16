/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B7520_2.c, func_802B2510_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct ALSound;
struct ALSound {
    void *envelope;
    void *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ALSound;
struct ALSound;
typedef struct ALSound ALSound;
struct PoolRecAt44;
typedef struct PoolRecAt44 PoolRecAt44;
struct SlotRecAt2F;
typedef struct SlotRecAt2F SlotRecAt2F;
struct SlotRecAt2F;
struct SlotRecAt2F {
    char pad0[0x1C];
    void *owner;
    s16 state;
    char pad22[2];
    f32 value;
    s32 timer;
    s16 amount;
    u8 flags;
    u8 extra;
};
struct PoolRecAt44;
struct SlotRecAt2F;
struct PoolRecAt44 {
    char pad0[0x40];
    struct SlotRecAt2F *slots;
    s32 count;
};
extern float D_800AA450;
s16 func_00293470(PoolRecAt44 *pool, void *owner) {
    s32 count = pool->count;
    SlotRecAt2F *slots = pool->slots;
    if (count > 0) {
        s16 i = 0;
        s16 state = 5;
        f32 value = D_800AA450;
        u8 flags = 0x40;
        do {
        s16 index = i;
        SlotRecAt2F *slot = (SlotRecAt2F *)(index * sizeof(*slots) + (u32)slots);
        if (slot->owner == 0) {
            slot->owner = owner;
            slot->state = state;
            slot->timer = 0;
            slot->value = value;
            slot->flags = flags;
            slot->extra = 0;
            slot->amount = ((((struct ALSound *)(owner))->sampleVolume * 0x7FFF) / 127);
            return index;
        }
        i++;
        } while ((s16)i < pool->count);
    }
    return -1;
}
