/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_5.CPP, func_8022E0B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct Player {
    char pad0[0xA08];
    s16 state;                 /* 0xA08 (Rage Wars 0x650) */
    char padA0A[0xAC8 - 0xA0A];
    f32 unkAC8;                /* 0xAC8 (Rage Wars 0x724) */
} Player;

typedef struct LevelActor {
    char pad0[0x1A8];
    Player *controller;        /* 0x1A8 (Rage Wars 0x1D8) */
} LevelActor;

typedef struct SoundRange {
    char pad0[0x18];
    s8 min;                    /* 0x18 */
    s8 max;                    /* 0x19 */
    s8 divisor;                /* 0x1A */
} SoundRange;

typedef struct SoundEvent {
    LevelActor *object;        /* 0x00 (Rage Wars 0x08) */
    char pad4[0x14 - 0x4];
    SoundRange *range;         /* 0x14 (Rage Wars 0x1C) */
    char pad18[0x90 - 0x18];
    s32 value;                 /* 0x90 (Rage Wars 0x04) */
} SoundEvent;

extern void func_00210790(void *arg0, f32 arg1);
extern float D_800A71FC;

void func_00256738(void *arg0, void *arg1) {
    SoundRange *range;
    Player *actor;
    s32 value;
    f32 amount;
    SoundEvent *event = arg1;
    LevelActor *object;
    range = event->range;
    value = event->value;
    if (value < range->min) {
        return;
    }
    if (range->max < value) {
        return;
    }
    object = event->object;
    actor = object->controller;
    amount = -actor->unkAC8;
    if (actor->state == 0xE) {
        amount += D_800A71FC;
    }
    func_00210790(arg0, amount / (f32)range->divisor);
}
