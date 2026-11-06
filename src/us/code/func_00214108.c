#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GAMECORE/80278900_2.CPP, func_80278ABC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
struct LevelActor;
struct StridedTable;
struct StridedTable {
    s32 stride;
    s32 count;
    char data[1];
};
struct Shape_typemap_13;
struct Shape_typemap_13 {
    int field_0;
    int field_4;
};
struct StridedTable;
typedef struct StridedTable StridedTable;
struct LevelActor;
struct TableRecAt8;
typedef struct TableRecAt8 TableRecAt8;
struct TableRecAt8;
struct TableRecAt8 {
    s32 unused;
    s32 count;
    u16 entries[1];
};
struct TableRecAt8;
typedef struct Shared_Placed Shared_Placed;
struct Shared_Placed {
    CVector3 pos;
    f32 unkC;
    s16 kind;
    s16 pad12;
};
struct WorldRecAt3C08;
struct EffectSlot {
    s8 active;
    char pad1[3];
    s16 model;
    char pad6[2];
    CVector3 position;
    s32 owner;
    struct CIntelligence *definition;
    CVector3 rotation;
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
    struct EffectSlot *prev;
    struct EffectSlot *next;
};
struct WorldRecAt3C08 {
    struct EffectSlot slots[32];
    struct EffectSlot *free;
    struct EffectSlot *head;
    struct EffectSlot *tail;
};
struct WorldRecAt3C08;
typedef struct WorldRecAt3C08 WorldRecAt3C08;
struct Shared_Pickup;
struct Record14;
struct LevelActor;
extern CWorld gScene;
struct ObjectLinks150C;
typedef struct ObjectLinks150C ObjectLinks150C;
struct IntegerState1504;
struct IntegerState1504 {
    char pad0[0x14A8];
    s32 unk_14A8;
};
struct IntegerState1504;
typedef struct IntegerState1504 IntegerState1504;
struct ObjectLinks150C;
struct ObjectLinks150C {
    unsigned char padding_0[0x14AC];
    s32 *unk_14AC;
};
struct Record14;
typedef struct Record14 Record14;
struct Record14;
struct Record14 {
    u32 coordinates[3];
    u16 lifetime;
    u8 flags;
    u8 kind;
    u8 state;
    u8 active;
    u8 mode;
    u8 reserved13;
};
struct LevelActor;
extern s32 func_00214108(void);
void *CISet__GetBlock(void *table, s32 index);
s32 func_00214108(void) {
    s32 total = 0;
    s32 i;
    char *base = (char *)&gScene;
    for (i = 0; i < ((IntegerState1504 *)(base))->unk_14A8; i++) {
        total += ((struct Shape_typemap_13 *)CISet__GetBlock(*((struct ObjectLinks150C *) (base + (i * 8)))->unk_14AC, 2))->field_4;
    }
    return total;
}
