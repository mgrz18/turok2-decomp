/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80279198_de.c, func_80279204_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct StridedTable;
struct StridedTable {
    s32 stride;
    s32 count;
    char data[1];
};
struct StridedTable;
typedef struct StridedTable StridedTable;
struct Trigger;
typedef struct Trigger Trigger;
struct Trigger;
struct Trigger {
    s32 mask;
    s32 unk4;
    s32 unk8;
    union {
        s32 word;
        struct {
            u16 unkC;
            u8 state;
            u8 id;
        } bytes;
    } flags;
    s32 unk10;
};
struct TableRecAt8;
typedef struct TableRecAt8 TableRecAt8;
struct Shared_World;
struct TableRecAt8;
struct TableRecAt8 {
    s32 unused;
    s32 count;
    u16 entries[1];
};
struct TableRecAt8;
struct SceneEffect;
typedef struct SceneEffect SceneEffect;
struct SceneEffect;
struct ActorDefinitionHeader;
struct SceneEffect {
    s8 active;
    char pad1[3];
    s16 model;
    char pad6[2];
    Vec3 position;
    s32 owner;
    struct ActorDefinitionHeader *definition;
    Vec3 rotation;
    char pad28[0x28];
    s32 resource;
    s32 animation;
    s32 frame;
    s32 frameState;
    char pad60[0x108];
    s32 collisionState;
    f32 collisionScale;
    s32 collisionFlags;
    s32 angle;
    s32 angleState;
    f32 minX;
    f32 minY;
    f32 minZ;
    f32 maxX;
    f32 maxY;
    f32 maxZ;
    s32 boundsState;
    char pad198[4];
    u16 flags;
    char pad19E[0x26];
    s32 effectState;
    f32 lifetime;
    f32 lifetimeState;
    s32 state;
    s32 *counter;
    struct SceneEffect *prev;
    struct SceneEffect *next;
    char pad1E0[0x110];
    signed int * ref;
    unsigned char padding2F4[4];
};
struct SceneEffect;
struct WorldRecAt3C08;
struct WorldRecAt3C08 {
    char pad0[0x3C00];
    struct SceneEffect *free;
    struct SceneEffect *head;
    struct SceneEffect *tail;
};
struct WorldRecAt3C08;
typedef struct WorldRecAt3C08 WorldRecAt3C08;
struct Shared_Pickup;
struct Record14;
struct Actor_func_8028E284_de;
typedef struct Shared_World {
    s32 mode;
    u8 unknown4[0x10];
    void *resourceHandle;
    s32 resourceKey;
    s32 resourceBank;
    u8 unknown20[4];
    s32 indexedBank;
    s32 lookupBank;
    u8 unknown2C[8];
    s32 trackLookup;
    u8 unknown38[0x14];
    s32 *resourceDirectory;
    u8 unknown50[4];
    s32 *indexedDirectory;
    s32 *lookupDirectory;
    u8 unknown5C[0x10];
    s32 *modelSections;
    s32 *objectSections;
    s32 *trackBundle;
    s32 *actorDefinitions;
    s32 *routeCount;
    void *bitSections;
    void *secondaryLevels;
    u8 unknown88[0xC];
    struct TableRecAt8 *modelIds;
    StridedTable *lookupTable;
    u8 unknown9C[4];
    StridedTable *records;
    StridedTable *stridedRecords;
    u8 unknownA8[4];
    StridedTable *actorRecords;
    u8 unknownB0[0x38];
    s32 resource;
    u8 unknownEC[0x34];
    s32 counter120;
    s32 counter124;
    u8 system128[0x10];
    struct Actor_func_8028E284_de *actors;
    s32 actorCapacity;
    s32 actorCount;
    u8 unknown144[0x800];
    s32 counter944;
    u8 unknown948[0x200];
    s32 counterB48;
    u8 unknownB4C[0x100];
    s32 counterC4C;
    u8 unknownC50[0x200];
    s32 counterE50;
    u8 unknownE54[0x100];
    s32 counterF54;
    u8 unknownF58[0x84];
    s32 counterFDC;
    u8 unknownFE0[0x40];
    s32 counter1020;
    u8 unknown1024[0x80];
    s32 counter10A4;
    u8 unknown10A8[0x10];
    s32 counter10B8;
    s32 counter10BC;
    u8 unknown10C0[0x100];
    s32 recordCount;
    s32 alternateRecordCount;
    s32 * recordIndex;
    s32 * alternateRecordIndex;
    struct Record14 * recordArray;
    struct Record14 * alternateRecordArray;
    u8 unknown11D8[0x32C];
    s32 counter1504;
    u8 unknown1508[0x600];
    u8 system1B08[0x11778 - 0x1B08];
    WorldRecAt3C08 system11778;
    u8 unknown15384[0x1B308 - 0x15384];
    f32 elapsed;
    u8 unknown1B30C[0x100];
    s32 level;
    s32 selectionState;
    s32 frozen;
    f32 selectionTimer;
    s32 selectionFlags;
    u8 unknown1B420[0x14];
    s32 selectionValues[4];
    s32 previewState;
    s32 previewTimer;
    s32 previewStart;
    u8 unknown1B450[0x1C4];
    Vec3 offset;
    struct Shared_Pickup *objects[16];
    s32 count;
    struct Actor_func_8028E284_de *specialActors[16];
    s32 specialActorCount;
} Shared_World;
extern Shared_World D_800F7078;
extern s32 func_00224B58(Shared_World *world, Trigger *trigger);
extern s32 func_00225210(Shared_World *world, s32 id);
extern void func_002252A0(Shared_World *world, s32 id);
extern void func_0026E294(Trigger *trigger);
extern s32 func_0026E6D4(Trigger *trigger, s32 arg);
extern void func_00224A68(Shared_World *world, Trigger *trigger, s32 arg);
extern void func_00224C98(Shared_World *world, s32 id);
extern s32 func_00224CF8(Shared_World *world, s32 id);
extern void func_0022515C(Shared_World *world, s32 id);
extern void func_0026E210(Trigger *trigger);
void func_0026E4B8(Trigger *triggers, s32 count, s32 require, s32 enter, s32 exclude, s32 group, s32 arg) {
    s32 i;
    s32 active;
    s32 usable;
    Shared_World *world;
    Trigger *trigger;
    for (trigger = triggers, i = 0; i < count; trigger++, i++) {
        world = &D_800F7078;
        usable = 1;
        if (world->mode != 4) {
            usable = func_00224B58(world, trigger) == 0;
        }
        if (!usable) {
            continue;
        }
        active = 1;
        if (!(trigger->flags.bytes.state & 0x20) && enter != 0 && (trigger->mask & enter)) {
            trigger->flags.bytes.state |= 0x40;
            if (trigger->flags.bytes.id != 0) {
                if (func_00225210(world, trigger->flags.bytes.id) != 0) {
                    func_002252A0(world, trigger->flags.bytes.id);
                }
            } else {
                func_0026E294(trigger);
            }
            active = 1;
        }
        if (require != 0) {
            active = 0;
            if (trigger->mask & require) {
                active = 1;
            }
        }
        if (enter != 0 && !(trigger->mask & enter)) {
            active = 0;
        }
        if ((trigger->mask & group) && !(trigger->mask & group & exclude)) {
            active = 0;
        }
        if ((trigger->flags.word & 0x1100) == 0x1000) {
            active = 0;
        }
        if (func_0026E6D4(trigger, arg) == 0) {
            active = 0;
        }
        if (!active) {
            continue;
        }
        if (trigger->flags.bytes.id != 0) {
            if (!(trigger->flags.bytes.state & 0x80)) {
                func_00224A68(world, trigger, 1);
                trigger->flags.bytes.state |= 0x80;
                func_00224C98(world, trigger->flags.bytes.id);
                if (func_00224CF8(world, trigger->flags.bytes.id) != 0) {
                    func_0022515C(world, trigger->flags.bytes.id);
                }
            }
        } else {
            func_0026E210(trigger);
        }
    }
}
