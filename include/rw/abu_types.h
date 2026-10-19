/* Types of the functions taken from AbuN64/ragewars-decomp (tools/import_ref.py),
 * gathered in one place by tools/centralize_types.py. Their names are that
 * decomp's own. */

#ifndef RW_ABU_TYPES_H
#define RW_ABU_TYPES_H

#ifndef COMMON_H
typedef signed int s32;
#endif
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
#ifndef COMMON_H
typedef unsigned char u8;
#endif
#ifndef COMMON_H
typedef unsigned int u32;
#endif
#ifndef COMMON_H
typedef signed char s8;
#endif
struct Access_u8_3;
struct Access_u8_3 {
    char pad[0x3];
    u8 field;
};
typedef struct Access_u8_3 Access_u8_3;
struct ObjRecAt12;
typedef struct ObjRecAt12 ObjRecAt12;
struct ObjRecAt4V2;
typedef struct ObjRecAt4V2 ObjRecAt4V2;
struct ObjRecAt4V2 {
    void * unk0;
    s32 unk4;
};
struct ObjRecAt12 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    char pad8[0x6];
    s8 unk12;
};
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct Chunk;
struct CartridgeReadWorker;
struct CartridgeReadWorker {
    u8 reserved[0x230];
    u8 requestQueueStorage;
    u8 reserved231[0xA48 - 0x231];
    u8 completionQueueStorage;
    u8 reservedA49[0x5068 - 0xA49];
    IntrusiveList available;
    IntrusiveList pending;
};
struct Chunk {
    u32 source;
    void *destination;
    s32 length;
    void *queue;
    s32 value;
};
typedef struct Chunk Chunk;
typedef struct CartridgeReadWorker CartridgeReadWorker;
struct Queue;
struct Queue {
    char pad[0x14];
    s32 items[4];
};
typedef struct Queue Queue;
struct CartridgeReadRequest;
struct CartridgeReadRequest {
    u32 devAddr;
    void *buffer;
    s32 size;
    void *replyQueue;
    void *replyMsg;
    s32 active;
    void *next;
    void *prev;
};
typedef struct CartridgeReadRequest CartridgeReadRequest;
struct Pool80255720;
typedef struct Pool80255720 Pool80255720;
struct Block80255720;
struct Block80255720 {
    struct Block80255720 *prev;
    struct Block80255720 *next;
    struct Block80255720 *prev_phys;
    struct Block80255720 *next_phys;
    s32 offset;
    s32 size;
};
typedef struct Block80255720 Block80255720;
struct Pool80255720 {
    s32 unk0;
    s32 unk4;
    struct Block80255720 *head;
    struct Block80255720 *tail;
    struct Block80255720 *anchor;
};
struct HeapRecAtC;
typedef struct HeapRecAtC HeapRecAtC;
struct BlockRecAt20;
struct BlockRecAt20 {
    struct BlockRecAt20 *prevFree;
    struct BlockRecAt20 *nextFree;
    struct BlockRecAt20 *prev;
    struct BlockRecAt20 *next;
    u32 size;
    u32 free;
    u8 reserved18[8];
    u8 data[1];
};
struct HeapRecAtC {
    char pad0[8];
    struct BlockRecAt20 *head;
    struct BlockRecAt20 *tail;
};
typedef struct BlockRecAt20 BlockRecAt20;
struct Pool80255ACC;
struct Pool80255ACC {
    s32 unk0;
    s32 unk4;
    Block80255720 *head;
    Block80255720 *tail;
};
typedef struct Pool80255ACC Pool80255ACC;
struct EntryRecAtCV1;
typedef struct EntryRecAtCV1 EntryRecAtCV1;
struct EntryRecAtCV1 {
    u32 key;
    s32 value;
    s32 index;
    struct EntryRecAtCV1 *next;
};
typedef struct Shared_HashLink Shared_HashLink;
struct Shared_HashLink {
    u32 unknown0[3];
    Shared_HashLink *next;
};
struct Node80254C10;
typedef struct Node80254C10 Node80254C10;
struct Node80254C10 {
    u32 start;
    u32 end;
    s32 unk_8;
    u32 flags;
    s32 stamp;
    char pad14[0x10];
    struct Node80254C10 *next;
};
struct Shape_func_8021A2D4_de_2;
struct Shape_func_8021A2D4_de_2 {
    int field_0;
};
struct HashNode;
struct HashNode {
    s32 key;
    void *value;
    s32 unk08;
    struct HashNode *next;
};
typedef struct HashNode HashNode;
struct CachedResource;
typedef struct CachedResource CachedResource;
struct ResourceLoadRequest;
struct CachedResource {
    void *data;
    s32 unk04;
    s32 count;
    s32 flags;
    s32 stamp;
    struct ResourceLoadRequest *owner;
};
struct ResourceLoadRequest {
    struct CachedResource *node;
    struct CachedResource *extra;
    s32 key;
    s32 priority;
    s32 flags;
    void *tag;
    s32 owner;
    s32 mode;
    void *replyQueue;
    struct ResourceLoadRequest *next;
};
typedef struct ResourceLoadRequest ResourceLoadRequest;
struct Pool;
struct Pool {
    s32 count;
    char pad[0x40];
    s32 field44;
};
typedef struct Pool Pool;
struct ResourceCacheEntry;
struct ResourceCacheEntry {
    s32 resource;
    s32 size;
    s32 references;
    s32 flags;
    s32 lastUsed;
    s32 owner;
    struct ResourceCacheEntry *next;
    s32 key;
    s32 pending;
    s32 callback;
};
typedef struct ResourceCacheEntry ResourceCacheEntry;
struct EntryRecAt18V3;
struct EntryRecAt18V3 {
    char pad0[0xC];
    s32 flags;
    s32 stamp;
    s32 pad14;
    struct EntryRecAt18V3 *next;
};
struct EntryList;
struct EntryList {
    s32 pad0;
    struct EntryRecAt18V3 *first;
};
typedef struct EntryList EntryList;
typedef struct EntryRecAt18V3 EntryRecAt18V3;
#ifndef COMMON_H
typedef unsigned long long u64;
#endif
typedef union Gfx {
    struct { u32 w0; u32 w1; } words;
    u64 force_structure_alignment;
} Gfx;
struct Node8026E4F8;
typedef struct Node8026E4F8 Node8026E4F8;
struct Node8026E4F8 {
    u32 key;
    char pad4[0x14];
    struct Node8026E4F8 *left;
    struct Node8026E4F8 *right;
};
struct TreeNode;
struct TreeNode {
    unsigned int key;
    unsigned char padding[12];
    struct TreeNode *link10;
    struct TreeNode *link14;
    struct TreeNode *link18;
    struct TreeNode *link1c;
};
typedef struct TreeNode TreeNode;
#ifndef COMMON_H
typedef float f32;
#endif
struct BoxRecAtC;
typedef struct BoxRecAtC BoxRecAtC;
struct BoxRecAtC {
    f32 min[3];
    f32 max[3];
};
struct PackedMatrixWords;
struct PackedMatrixWords {
    u32 upper[8];
    u32 lower[8];
};
typedef struct PackedMatrixWords PackedMatrixWords;
struct Vector4f;
typedef struct Vector4f Vector4f;
struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct Matrix;
typedef struct Matrix Matrix;
struct Matrix {
    f32 m[16];
};
struct Mtx;
struct Mtx {
    Vec3 right;
    f32 m03;
    Vec3 up;
    f32 m13;
    Vec3 forward;
    f32 m23;
    f32 m30;
    f32 m31;
    f32 m32;
    f32 m33;
};
typedef struct Mtx Mtx;
typedef struct GeometryEdge {
    Vec3 start;
    Vec3 end;
    Vec3 direction;
    Vec3 unitDirection;
    Vec3 normal;
} GeometryEdge;
struct ObjRecAt2CV1;
struct ObjRecAt2CV1 {
    char pad0[0x18];
    f32 unk18;
    char pad18[0x1C - 0x18 - sizeof(f32)];
    f32 unk1C;
    char pad1C[0x20 - 0x1C - sizeof(f32)];
    f32 unk20;
    char pad20[0x24 - 0x20 - sizeof(f32)];
    f32 unk24;
    char pad24[0x28 - 0x24 - sizeof(f32)];
    f32 unk28;
    char pad28[0x2C - 0x28 - sizeof(f32)];
    f32 unk2C;
};
typedef struct ObjRecAt2CV1 ObjRecAt2CV1;
struct ObjectRecAtBC;
struct ObjectRecAtBC {
    f32 falloff;
    f32 value;
    Vec3 direction;
    Vec3 axis;
    char pad20[0xB8-0x20];
    u8 flagB8;
    u8 flagB9;
    char padBA;
    u8 shape;
    u8 animationSeed;
};
typedef struct ObjectRecAtBC ObjectRecAtBC;
#ifndef COMMON_H
typedef double f64;
#endif
struct Output8025EF54;
typedef struct Output8025EF54 Output8025EF54;
struct Output8025EF54 {
    f32 value;
    s32 pad04;
    s32 pad08;
    s32 pad0C;
};
struct Decode8025EF54;
struct Decode8025EF54 {
    s32 bits;
    f32 base;
    f32 range;
    s32 unused;
    u32 stream;
    s32 first_bits;
    s32 second_bits;
};
typedef struct Decode8025EF54 Decode8025EF54;
struct Curve;
typedef struct Curve Curve;
struct Curve {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    s32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
    u32 unk20;
    s32 unk24;
    s32 unk28;
};
struct Segment8025FFD0;
typedef struct Segment8025FFD0 Segment8025FFD0;
struct Segment8025FFD0 {
    s32 unk0;
    f32 unk4;
    f32 unk8;
    u32 unkC;
    u8 pad10[0x24 - 0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};
#ifndef COMMON_H
typedef signed short s16;
#endif
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
typedef struct Triple Triple;
struct ObjRecAt2V1;
typedef struct ObjRecAt2V1 ObjRecAt2V1;
struct Rec_func_8024C92C_de;
typedef struct Rec_func_8024C92C_de Rec_func_8024C92C_de;
struct Rec_func_8024C92C_de {
    s32 x;
    s32 y;
    s32 z;
    s32 pad0;
    s32 pad1;
};
struct ObjectLinks24_2;
typedef struct ObjectLinks24_2 ObjectLinks24_2;
struct ObjRecAt2V1 {
    s16 translation;
    short unk2;
};
struct ObjectLinks24_2 {
    char * unk_0;
    char pad0[0x4 - 0x0 - sizeof(char*)];
    char * unk_4;
    void * unk_8;
    void * unk_C;
    char padC[0x10 - 0xC - sizeof(void*)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
};
struct ObjectState14;
struct ObjectState14 {
    Vec3 position;
    s16 unk_C;
    char padC[0xE - 0xC - sizeof(s16)];
    s16 unk_E;
    char padE[0x10 - 0xE - sizeof(s16)];
    s16 unk_10;
    char pad10[0x12 - 0x10 - sizeof(s16)];
    s16 unk_12;
};
typedef struct ObjectState14 ObjectState14;
struct func_80203E78_S1;
struct func_80203E78_S1 {
    char pad0[0x4];
    s32 unk4;
};
typedef struct func_80203E78_S1 func_80203E78_S1;
struct Func802608ECResult;
typedef struct Func802608ECResult Func802608ECResult;
struct Func802608ECResult {
    u32 value;
    f32 start;
    f32 delta;
};
struct LocalDecodeRange;
struct LocalDecodeRange {
    u32 unused;
    Func802608ECResult range;
};
typedef struct LocalDecodeRange LocalDecodeRange;
struct BitFieldSource;
typedef struct BitFieldSource BitFieldSource;
struct BitFieldSource {
    int base;
    unsigned int width;
};
struct Pack;
struct Pack {
    s32 count;
    s32 offsets[1];
};
typedef struct Pack Pack;
struct Track_func_80261180_de;
struct Track_func_80261180_de {
    s32 keys;
    char pad4[0x2C - 4];
    s32 stride;
};
typedef struct Track_func_80261180_de Track_func_80261180_de;
struct Stream;
typedef struct Stream Stream;
struct Stream {
    s32 unk0;
    char pad4[8];
    s32 unkC;
    float coeff[4];
    u32 unk20;
    u32 unk24;
    s32 unk28;
};
#ifndef COMMON_H
typedef unsigned short u16;
#endif
typedef struct AnimRef {
               char pad0[4];
               u16 id;
               u16 frame;
               u16 count;
               u8 flags[2];
               u32 span;
               void **data;
} AnimRef;
typedef struct ResidentDataChunk {
    s32 width;
    s32 count;
    u8 data[1];
} ResidentDataChunk;
struct SineTable;
struct SineTable {
    f32 count;
    f32 value[120];
};
typedef struct SineTable SineTable;
typedef struct InputRecAt27 InputRecAt27;
struct InputRecAt27 {
    s32 unk0;
    f32 vec4[3];
    f32 vec10[3];
    u16 unk1C;
    u16 unk1E;
    u16 resource20;
    u16 resource22;
    s16 amount24;
    u8 unk26;
    u8 unk27;
};
struct StridedTable;
struct StridedTable {
    s32 stride;
    s32 count;
    char data[1];
};
typedef struct StridedTable StridedTable;
struct InputRecAt27;
typedef struct ActorDefinitionHeader {
    s32 kind;
    s32 gate;
    u8 pad8[4];
    s16 character;
    s8 first;
    u8 padF;
    s8 second;
    u8 pad11;
    s8 send;
    u8 pad13;
    s32 flags;
} ActorDefinitionHeader;
struct Def;
struct Def {
    s32 type;
    s32 flags;
    char pad8[0x20];
    s16 id;
};
struct Actor_func_8028E284_de;
struct Actor_func_8028E284_de {
    char pad0[0x18];
    struct Def *def;
    char pad1C[0xC8];
    u16 id;
    char padE6[0x202];
};
struct Shared_World;
typedef struct Actor_func_8028E284_de Actor_func_8028E284_de;
struct SceneEffect;
typedef struct SceneEffect SceneEffect;
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
struct WorldRecAt3C08;
struct WorldRecAt3C08 {
    char pad0[0x3C00];
    struct SceneEffect *free;
    struct SceneEffect *head;
    struct SceneEffect *tail;
};
typedef struct WorldRecAt3C08 WorldRecAt3C08;
typedef struct Def Def;
struct TableRecAt8;
typedef struct TableRecAt8 TableRecAt8;
struct TableRecAt8 {
    s32 unused;
    s32 count;
    u16 entries[1];
};
struct Shared_Pickup;
struct Record14;
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
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct ObjectLinks24;
typedef struct ObjectLinks24 ObjectLinks24;
struct ObjectLinks24 {
    char * unk_0;
    char pad0[0x4 - 0x0 - sizeof(char*)];
    char * unk_4;
    void *translationFrames;
    void * unk_C;
    char padC[0x10 - 0xC - sizeof(void*)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    s32 translationFirst;
    s32 translationNext;
    f32 unk_20;
};
typedef struct RenderSortRecord {
    s32 index;
    f32 depth;
} RenderSortRecord;
struct RangeNode;
typedef struct RangeNode RangeNode;
struct RangeNode {
    char pad0[4];
    struct RangeNode *next;
    u16 *radius;
    char padC[4];
    s16 x;
    s16 y;
    s16 z;
    s16 active;
};
union func_8020E674_S1_U8;
typedef union func_8020E674_S1_U8 func_8020E674_S1_U8;
union func_8020E674_S1_U8 {
    f32 v0;
    Vec3 v1;
};
union func_8022E280_S1_U744;
typedef union func_8022E280_S1_U744 func_8022E280_S1_U744;
union func_8022E280_S1_U744 {
    s32 v0;
    f32 v1;
};
struct ObjectLinks54;
struct ObjectLinks54 {
    char pad0[0x8];
    char * unk_8;
    char pad8[0x28 - 0x8 - sizeof(char*)];
    f32 unk_28;
    char pad28[0x3C - 0x28 - sizeof(f32)];
    s32 unk_3C;
    char pad3C[0x40 - 0x3C - sizeof(s32)];
    char * unk_40;
    char pad40[0x4C - 0x40 - sizeof(char*)];
    f32 unk_4C;
    char pad4C[0x50 - 0x4C - sizeof(f32)];
    s32 unk_50;
};
typedef struct ObjectLinks54 ObjectLinks54;
struct ObjectStateB0;
struct ObjectStateB0 {
    char pad0[0x10];
    Vec3 unk_10;
    char pad10[0xAC - 0x10 - sizeof(Vec3)];
    f32 unk_AC;
};
struct ObjectLinksB4;
struct ObjectLinksB4 {
    char pad0[0x4];
    char * unk_4;
    char pad4[0x8 - 0x4 - sizeof(char*)];
    f32 unk_8;
    char pad8[0x10 - 0x8 - sizeof(f32)];
    func_8020E674_S1_U8 unk_10;
    char pad10[0x1C - 0x10 - sizeof(func_8020E674_S1_U8)];
    f32 unk_1C;
    char pad1C[0x28 - 0x1C - sizeof(f32)];
    f32 unk_28;
    char pad28[0x68 - 0x28 - sizeof(f32)];
    f32 unk_68;
    char pad68[0xA8 - 0x68 - sizeof(f32)];
    f32 unk_A8;
    char padA8[0xAC - 0xA8 - sizeof(f32)];
    func_8022E280_S1_U744 unk_AC;
    char padAC[0xB0 - 0xAC - sizeof(func_8022E280_S1_U744)];
    s32 unk_B0;
};
typedef struct ObjectStateB0 ObjectStateB0;
struct ObjectState20;
struct ObjectState20 {
    unsigned char padding_0[8];
    f32 unk_8;
    unsigned char padding_C[16];
    u8 unk_1C;
};
typedef struct ObjectLinksB4 ObjectLinksB4;
typedef struct ObjectState20 ObjectState20;
struct Node802A697C;
struct Node802A697C {
    u32 unk0;
    f32 value;
    s32 childValue;
    s32 matrixIndex;
};
typedef struct Node802A697C Node802A697C;
struct State802A6DF8;
struct State802A6DF8 {
    struct State802A6DF8 *prev;
    struct State802A6DF8 *next;
    Node802A697C *node;
    u8 padC[0x10];
    void *object;
    s32 contextId;
    f32 value;
    u8 pad28[0xC];
    s32 renderMode;
    u8 pad38[4];
    u32 flags;
    struct Node438C *vertices;
    u8 pad44[4];
    s32 vertexCount;
};
typedef struct State802A6DF8 State802A6DF8;
struct Node438C;
typedef struct Node438C Node438C;
struct Node438C {
    u8 pad0[4];
    struct Node438C *next;
    u8 pad8[8];
    Vec3 position;
    f32 a;
    f32 b;
    f32 c;
    f32 matrices[2][16];
};
struct GameplayCamera;
struct ObjRecAt28V1;
typedef struct ObjRecAt28V1 ObjRecAt28V1;
struct ObjRecAt28V1 {
    struct ObjRecAt28V1 *next;
    s32 field4;
    f32 unk8;
    char padC[0x28 - 0xC];
    s32 matrix[2][16];
};
struct Key;
struct Key {
    char pad0[0x14];
};
typedef struct Key Key;
struct Shade;
typedef struct Shade Shade;
struct Actor_func_802A274C_de;
typedef struct Actor_func_802A274C_de Actor_func_802A274C_de;
struct Morph;
typedef struct Morph Morph;
struct Shade {
    u8 a;
    u8 b;
    u8 g;
    u8 r;
};
struct Blend;
struct Blend {
    Shade from;
    Shade to;
    u16 first;
    u16 second;
};
struct Track_func_802A274C_de;
struct Track_func_802A274C_de {
    char pad0[4];
    f32 span;
    f32 total;
    char padC[0x12 - 0xC];
    u8 closed;
    char pad13[0x21 - 0x13];
    u8 scaleS, scaleT, count;
    struct Blend blends[1];
};
struct Actor_func_802A274C_de {
    char pad0[8];
    struct Track_func_802A274C_de *track;
    struct Key *first;
    struct Key *second;
    char pad14[0x24 - 0x14];
    f32 time;
    char pad28[0x3C - 0x28];
    u32 flags;
    s32 field40;
    ObjRecAt28V1 *clock;
};
typedef struct Blend Blend;
struct func_802A66B4_S1;
typedef struct func_802A66B4_S1 func_802A66B4_S1;
struct Morph {
    s32 count;
    struct Blend *blends;
    struct Key *first;
    struct Key *second;
    union { void *firstFrame; f32 scaleS; };
    union { void *secondFrame; f32 scaleT; };
    f32 textureScaleS;
    f32 textureScaleT;
    f32 step;
};
struct Vtx;
typedef struct Vtx Vtx;
struct Vtx {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};
struct func_802A66B4_S1 {
    char pad0[0x2588];
    s32 unk2588;
    char pad2588[0x258C - 0x2588 - sizeof(s32)];
    s32 unk258C;
};
typedef struct Track_func_802A274C_de Track_func_802A274C_de;
struct Animator;
typedef struct Animator Animator;
struct Animator {
    s32 unk0;
    s32 mode;
    f32 time;
    f32 *loop;
    f32 *script;
    f32 value;
    f32 valueFrom;
    f32 valueTo;
    f32 valueDuration;
    f32 alpha;
    f32 alphaFrom;
    f32 alphaTo;
    f32 colourDuration;
    f32 rgb[3];
    char pad40[4];
    f32 rgbFrom[3];
    char pad50[4];
    f32 rgbTo[3];
};
struct func_802077F4_S2;
typedef struct func_802077F4_S2 func_802077F4_S2;
struct func_802077F4_S2 {
    char pad0[0x4];
    f32 unk4;
};
struct ImageHeader;
struct ImageHeader {
    char pad0[2];
    unsigned char widthShift;
    unsigned char heightShift;
    char pad4[0x1D - 4];
    unsigned char scale;
    char pad1E[0x21 - 0x1E];
    unsigned char width;
    unsigned char height;
    unsigned char format;
    char data[1];
};
struct Image;
struct Image {
    char pad0[8];
    struct ImageHeader *header;
    s32 fieldC;
    s32 field10;
    char pad14[0x3C - 0x14];
    s32 flags;
};
struct Sprite;
struct Sprite {
    s32 format;
    char *data;
    s32 field8;
    s32 fieldC;
    f32 width;
    f32 height;
    f32 extentX;
    f32 extentY;
    f32 step;
};
typedef struct Sprite Sprite;
typedef struct ImageHeader ImageHeader;
typedef struct Image Image;
struct ResourceManagerState;
struct ResourceManagerState {
    s32 active;
    s32 count;
};
typedef struct ResourceManagerState ResourceManagerState;
typedef void ( *func_80267198_de_Callback)(void *, void *, signed int, Vec3, struct ResourceManagerState);
struct CallbackEntryRecAt4V1;
struct CallbackEntryRecAt4V1 {
    func_80267198_de_Callback callback;
    s32 unused;
};
typedef struct CallbackEntryRecAt4V1 CallbackEntryRecAt4V1;
struct SevenBytes;
typedef struct SevenBytes SevenBytes;
struct Node_func_80267CE4_de;
struct Node_func_80267CE4_de {
    s32 unused;
    struct Node_func_80267CE4_de *next;
};
struct SevenBytes {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 g;
};
typedef struct Node_func_80267CE4_de Node_func_80267CE4_de;
struct Shared_Quad;
struct Shared_Quad {
    union { s32 w[4]; f32 f[4]; };
};
typedef struct Shared_Quad Shared_Quad;
struct MenuSettings;
typedef struct MenuSettings MenuSettings;
struct MenuSettings {
    u8 padding[0x581];
    u8 language;
};
struct Vec4s;
typedef struct Vec4s Vec4s;
struct Vec4s {
    s16 x;
    s16 y;
    s16 z;
    s16 w;
};
struct Node_func_804428F8_de;
typedef struct Node_func_804428F8_de Node_func_804428F8_de;
struct StateRecAtBCV1;
struct StateRecAtBCV1 {
    char pad00[0xB0];
    s32 field_B0;
    s32 field_B4;
    char padB8[0xBC - 0xB8];
    s32 field_BC;
};
struct MessageList;
typedef struct MessageList MessageList;
struct Table_func_804428F8_de;
struct Node_func_804428F8_de {
    char pad00[0x8];
    s32 handle;
    char pad0C[0x14 - 0xC];
    struct Table_func_804428F8_de *table;
    char pad18[0x20 - 0x18];
    struct StateRecAtBCV1 *state;
    char pad24[0x28 - 0x24];
    s16 kind;
    char pad2A[0x1D4 - 0x2A];
    struct Node_func_804428F8_de *next;
};
struct MessageList {
    struct Node_func_804428F8_de *head;
    char pad04[0x10 - 0x4];
    s32 flag;
    s16 rotation;
};
struct Table_func_804428F8_de {
    char pad00[0xC];
    void (*handler)(Node_func_804428F8_de *, MessageList *);
};
typedef struct StateRecAtBCV1 StateRecAtBCV1;
typedef struct Lights Lights;
struct Lights {
    s8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8 unk4;
    s8 unk5;
    s8 unk6;
    s8 unk7;
    s8 unk8;
    s8 unk9;
    s8 unkA;
    char padB[0x5];
    s8 unk10;
    u8 unk11;
    s8 unk12;
    s8 unk13;
    s8 unk14;
    u8 unk15;
    s8 unk16;
    s8 unk17;
    s8 unk18;
    s8 unk19;
    s8 unk1A;
    char pad1B[0x5];
};
typedef struct GameplayCamera GameplayCamera;
struct GameplayCamera {
    char pad0[4];
    GameplayCamera *next;
    char pad8[0xC];
    s16 unk14;
    char pad16[0x2];
    f32 unk18;
    s16 state;
    u8 pad1E[2];
    s32 stateCounter;
    s32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    Shared_Quad unk48;
    struct Shared_func_80234FDC_S8 *unk58;
    f32 unk5C;
    f32 unk60;
    s32 unk64;
    u16 unk68;
    char pad6A[0x2];
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    s32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    f32 unk94;
    f32 unk98;
    f32 unk9C;
    char padA0[0x18];
    char unkB8[20];
    char unkCC[20];
    char unkE0[20];
    f32 shakeHorizontal;
    f32 shakeVertical;
    char padFC[0x28];
    u16 unk124;
    char pad126[0x2];
    Vec3 position;
    char pad134[0x4];
    f32 unk138;
    char pad13C[4];
    Vector4f orientation;
    f32 unk150;
    char pad154[0xC];
    s8 unk160;
    char pad161[0x3F];
    f32 unk1A0;
    char pad1A4[0x3C];
    s8 unk1E0;
    char pad1E1[0x3F];
    f32 unk220;
    char pad224[0x3C];
    Vec3 corners[5];
    f32 unk29C;
    f32 unk2A0;
    f32 unk2A4;
    f32 unk2A8;
    char pad2AC[0x44];
    Vector4f clipPlanes[6];
    Vec3 clipBounds[2];
    Vec3 volumeBounds[2];
    char unk380[2][64];
    Lights lights[2];
    u16 unk440;
    char pad442[0x6];
    char unk448[3][64];
    char pad508[0x20];
    f32 unk528;
    f32 unk52C;
    f32 unk530;
    u8 pad534[4];
    s32 fadeCounter;
    u8 pad53C[8];
    s32 fadeEnabled;
    u8 pad548[2];
    s8 fadeMode;
    s8 fadeRed, fadeGreen, fadeBlue;
    s8 fadeStart, fadeEnd, fadeStep, fadeState;
    u8 pad552[2];
    MessageList messageList;
    u8 pad56C[4];
    s8 unk570;
    char pad571[0xE40 - 0x571];
    IntrusiveList objects;
    f32 unkE54;
    char padE58[0x3C];
    s8 unkE94;
    char padE95[0x3];
};
typedef struct Shared_func_80234FDC_S8 Shared_func_80234FDC_S8;
struct Shared_func_80234FDC_S8 {
    char pad0[0x1C];
    s32 unk1C;
};
typedef struct MenuMatchGlobals {
    u8 unobserved0[0x20];
    GameplayCamera *cameras;
    u8 unobserved24[0x40 - 0x24];
    GameplayCamera fallbackCamera;
    u8 unobservedED8[0x1200 - 0xED8];
    s32 useFallbackCamera;
    u8 unobserved1204[0x1240 - 0x1204];
    MenuSettings settings;
    u8 unobserved17C2[0x17F0 - 0x17C2];
    s32 status[8];
    u8 unobserved1810[0x186C - 0x1810];
    s32 regenPaused;
} MenuMatchGlobals;
struct EntryRecAtE;
typedef struct EntryRecAtE EntryRecAtE;
struct EntryRecAtE {
    void *resource;
    s16 id;
    s16 sound;
    s16 callback;
    s16 reservedA;
    s16 index;
    s16 argument;
};
struct Dst;
typedef struct Dst Dst;
struct Dst {
    s32 unk0;
    u8 unk4;
    u8 unk5;
    u8 unk6;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
};
struct Reusable;
typedef struct Reusable Reusable;
struct EntryRecAt24;
typedef struct EntryRecAt24 EntryRecAt24;
struct EntryRecAt24 {
    struct Dst *dst;
    s32 val0;
    u8 pad1[3];
    u8 flag0;
    u8 pad2[3];
    u8 flag1;
    u8 pad3[3];
    u8 flag2;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    f32 extra;
};
struct Reusable {
    s32 unk0;
    char pad4[0x10];
    s32 unk14;
    char pad18[0x70];
    s32 unk88;
    char pad8C[0x10];
    s32 unk9C;
    char padA0[0x10];
    s32 unkB0;
    s32 unkB4;
    char padB8[0xC];
    s32 unkC4;
};
struct WeightedTable;
struct WeightedTable {
    s16 count;
    s16 entries[25][2];
    s16 reserved;
};
typedef struct WeightedTable WeightedTable;
typedef struct ObjRecAtB8 {
    int unk0;
    int unk4;
    int unk8;
    char padC[0x90 - 0xC];
    float unk90;
    float unk94;
    float unk98;
    int unk9C;
    float unkA0;
    int unkA4;
    int unkA8;
    float unkAC;
    float unkB0;
    int unkB4;
    int unkB8;
} ObjRecAtB8;
typedef struct Shared_Func80243A80Collision Shared_Func80243A80Collision;
struct Shared_Func80243A80Collision {
    u16 id;
    u16 flags;
};
struct Node75;
typedef struct Node75 Node75;
struct Node75 {
    s32 pad0;
    Vec3 *prev;
    Vec3 *cur;
    Vec3 *next;
};
struct TripleRecAt8;
typedef struct TripleRecAt8 TripleRecAt8;
struct TripleRecAt8 {
    f32 a;
    s32 b;
    f32 c;
};
typedef struct CollisionBuildVertex {
    f32 x;
    f32 y;
    f32 z;
    f32 ceiling;
} CollisionBuildVertex;
struct Packed;
struct Packed {
    unsigned short unk0;
    unsigned short unk2;
    unsigned short first[3];
    unsigned short second[3];
    int unk10;
};
typedef struct Packed Packed;
struct HeaderRecAt1C;
struct HeaderRecAt1C {
    short unk0;
    short unk2;
    void *first[3];
    void *second[3];
    int unk1C;
};
typedef struct HeaderRecAt1C HeaderRecAt1C;
struct Triangle;
struct Triangle {
    char pad0[2];
    u16 flags;
    Vec3 *vertices[3];
};
typedef struct Triangle Triangle;
struct Shape_typemap_3;
struct Shape_typemap_3 {
    int field_0;
};
struct Field_void_4;
struct Field_void_4 {
    char pad[0x4];
    void * value;
};
struct func_80284AF4_G2;
typedef struct func_80284AF4_G2 func_80284AF4_G2;
struct func_80284AF4_G2 {
    void * unk0;
};
typedef struct Field_void_4 Field_void_4;
struct ListHeader;
typedef struct ListHeader ListHeader;
struct ListHeader {
    void *head;
    void *tail;
    s32 count;
};
typedef struct Record14 Record14;
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
struct func_80278EEC_S1;
typedef struct func_80278EEC_S1 func_80278EEC_S1;
struct func_80278EEC_S1 {
    char pad0[0xE];
    char unkE;
};
struct WorldRecAt1B40C;
struct Trigger;
typedef struct Trigger Trigger;
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
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    Vec3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
typedef struct NodeCode8028469C NodeCode8028469C;
struct PoolRecAt28V1;
struct PoolRecAt28V1 {
    IntrusiveList free;
    IntrusiveList active;
    s32 baseLevel;
};
typedef struct PoolRecAt28V1 PoolRecAt28V1;
struct PlaneEquation;
struct PlaneEquation {
    Vec3 normal;
    f32 distance;
};
typedef struct PlaneEquation PlaneEquation;
struct CacheEntry;
struct CacheEntry {
    struct CacheEntry *next;
    void *key;
    u16 state;
    u16 size;
    s32 padC;
    char data[0x80];
};
typedef struct CacheEntry CacheEntry;
struct Window;
typedef struct Window Window;
struct Window {
    struct Window *next;
    struct Window *prev;
    u32 base;
    s32 frame;
    s32 buffer;
};
struct ListLink;
struct ListLink {
    struct ListLink *next;
    struct ListLink *prev;
};
typedef struct ListLink ListLink;
struct MenuRules;
typedef struct MenuRules MenuRules;
struct MenuRules {
    char pad0[0x1C];
    s32 locked;
};
struct Header;
struct Header {
    u8 data[0xCC];
};
typedef struct Header Header;
struct Voice;
typedef struct Voice Voice;
struct Voice {
    s32 unk0;
    s32 sound;
    s32 unk8;
    s32 unkC;
    char pad10[0x28];
    s16 priority;
    s16 channel;
    char pad3C[0x64];
    s32 pending;
    s32 flags;
    char padA8[0x24];
};
struct Slots;
struct Slots {
    char pad0[0x60];
    s16 ids[16];
};
typedef struct Slots Slots;
struct ContextCode8025A3EC;
struct ContextCode8025A3EC {
    char pad0[0x7C];
    Slots slots;
    char padFC[0x104 - 0xFC];
    s32 frame;
    char pad108[0x134 - 0x108];
    s32 track;
    char pad138[0x2B98 - 0x138];
    s32 result;
};
typedef struct ContextCode8025A3EC ContextCode8025A3EC;
struct ContextRecAt2BC0;
typedef struct ContextRecAt2BC0 ContextRecAt2BC0;
struct Font_func_80257360_de;
typedef struct Font_func_80257360_de Font_func_80257360_de;
struct Font_func_80257360_de {
    char pad0[4];
    struct Font_func_80257360_de *next;
    char pad8[4];
    struct Font_func_80257360_de *child;
};
struct ContextRecAt2BC0 {
    char pad0[0x24];
    s32 field24;
    s32 field28;
    char *field2C;
    char pad30[0x7C - 0x30];
    struct Font_func_80257360_de *font;
    struct Font_func_80257360_de *resource;
    char list[0x104 - 0x84];
    s32 frame;
    u8 reserved108[8];
    MenuRules lock;
    u8 pad130[4];
    s32 track;
    char view[0x1D64 - 0x138];
    char panel[0x1D6C - 0x1D64];
    s32 field1D6C;
    char pad1D70[0x1DA8 - 0x1D70];
    char frames[0x10];
    char menu[4];
    Voice voices[17];
    char pad2B48[0x28];
    char *table2B70;
    char *table2B74;
    char *table2B78;
    char pad2B7C[0xC];
    s32 field2B88;
    s16 field2B8C;
    char pad2B8E[2];
    s32 field2B90;
    u8 field2B94;
    char pad2B95[3];
    s32 field2B98;
    s32 field2B9C;
    f32 red;
    f32 green;
    f32 blue;
    s32 field2BAC;
    s32 field2BB0;
    s32 field2BB4;
    s32 field2BB8;
    f32 alpha;
    char dialog[1];
};
struct VoiceRecAt8;
typedef struct VoiceRecAt8 VoiceRecAt8;
struct VoiceRecAt8 {
    ListLink link;
    Header header;
};
struct ChannelSet;
struct ChannelSet {
    s32 owner;
    ListLink free;
    Header freeHeader;
    ListLink active;
    Header activeHeader;
    VoiceRecAt8 voices[32];
};
typedef struct ChannelSet ChannelSet;
struct ObjRecAt2B9C;
typedef struct ObjRecAt2B9C ObjRecAt2B9C;
struct ObjRecAt2B9C {
    char pad[0x2B9C];
    int level;
};
struct SortTable;
typedef struct SortTable SortTable;
struct SortTable {
    char unknown00[0xE];
    s16 count;
    u32 entries[1];
};
struct SceneRecAt2B5C;
struct SceneRecAt2B5C {
    char pad0[0x130];
    s32 lastPick;
    char pad134[0x2B54 - 0x134];
    s32 count;
    s32 ids;
    s32 *weights;
};
typedef struct SceneRecAt2B5C SceneRecAt2B5C;
struct Angles;
typedef struct Angles Angles;
struct Bytes12;
typedef struct Bytes12 Bytes12;
struct ControllerCode802591C0;
typedef struct ControllerCode802591C0 ControllerCode802591C0;
struct Motion;
typedef struct Motion Motion;
struct ViewRecAtB0;
typedef struct ViewRecAtB0 ViewRecAtB0;
struct Angles {
    u16 heading;
    u16 pitch;
    u16 target;
    s16 steps;
};
struct Bytes12 {
    char bytes[12];
};
struct ControllerCode802591C0 {
    s16 group;
    s16 sample;
    s8 priority;
    s8 mode;
    u16 flags;
    s16 field8;
    s16 preset;
    s16 fieldC;
    s8 pitch;
    s8 fieldF;
    s16 yaw;
    s16 x;
    s16 y;
};
struct Motion {
    char pad0[0x14];
    f32 heading;
    char pad18[0x48 - 0x18];
    f32 rate1;
    f32 rate2;
};
struct ViewRecAtB0 {
    char pad0[0x10];
    Motion motion;
    Angles angles;
    char pad68[4];
    f32 pitch;
    f32 pitchStep;
    char pad74[0xB0 - 0x74];
    s32 presets;
};
struct SlotRecAtB0;
typedef struct SlotRecAtB0 SlotRecAtB0;
struct HeaderRecAt102;
typedef struct HeaderRecAt102 HeaderRecAt102;
struct HeaderRecAt102 {
    char pad[0x102];
    s16 local;
};
struct OwnerCode8025A3EC8025B5F0;
typedef struct OwnerCode8025A3EC8025B5F0 OwnerCode8025A3EC8025B5F0;
struct OwnerCode8025A3EC8025B5F0 {
    char pad[0x84];
    char sound[0x58];
    s16 samples[20];
    char pad2[(0x104 - 0xDC) - 40];
    s32 key;
};
struct SlotRecAtB0 {
    s32 index;
    s32 state;
    s32 used;
    char pad0[4];
    s32 key;
    char pad1[0x3C];
    s32 flag;
    char pad2[0x54];
    s32 value;
    s32 active;
    struct OwnerCode8025A3EC8025B5F0 *owner;
    char pad3[0x18];
};
struct RecordRecAt4V6;
struct RecordRecAt4V6 {
    struct HeaderRecAt102 *header;
    SlotRecAtB0 slots[17];
};
typedef struct RecordRecAt4V6 RecordRecAt4V6;
struct RecordCode8025A3EC;
typedef struct RecordCode8025A3EC RecordCode8025A3EC;
struct RecordCode8025A3EC {
    s32 index;
    char pad04[4];
    s32 f08;
    s32 f0C;
    char pad10[4];
    s32 f14;
    char pad18[0x14];
    f32 f2C;
    char pad30[4];
    f32 f34;
    s16 f38;
    s16 f3A;
    char pad3C[4];
    s32 f40;
    char pad44[0x14];
    s32 f58;
    s32 f5C;
    char pad60[0x44];
    s32 fA4;
    char padA8[4];
    s32 fAC;
    s32 owner;
    s32 fB4;
    f32 fB8;
    s32 fBC;
    s32 fC0;
    s32 fC4;
    f32 fC8;
};
struct ObjRecAt4V6;
struct ObjRecAt4V6 {
    char pad0[0x4];
    RecordCode8025A3EC unk4;
};
typedef struct ObjRecAt4V6 ObjRecAt4V6;
struct SlotRecAtB8;
struct SlotRecAtB8 {
    char pad0[0xA8];
    s32 flags;
    char padaC[8];
    char *owner;
    char end[0x14];
};
typedef struct SlotRecAtB8 SlotRecAtB8;
struct SlotRecAtA8;
struct SlotRecAtA8 {
    char pad0[8];
    s32 used;
    char pad[0x9C];
    s32 value;
    char pad2[0x20];
};
typedef struct SlotRecAtA8 SlotRecAtA8;
struct RecordRecAt4V3;
struct RecordRecAt4V3 {
    HeaderRecAt102 *header;
    SlotRecAtA8 slots[17];
};
typedef struct RecordRecAt4V3 RecordRecAt4V3;
struct SlotCode8025A3EC;
struct SlotCode8025A3EC {
    int id;
    char pad4[0x9C];
    int keyA;
    char padA4[8];
    int keyB;
    char padB0[0x1C];
};
typedef struct SlotCode8025A3EC SlotCode8025A3EC;
struct ObjRecAtCV4;
struct ObjRecAtCV4 {
    HeaderRecAt102 *owner;
    char pad4[8];
    SlotCode8025A3EC slots[16];
};
typedef struct ObjRecAtCV4 ObjRecAtCV4;
struct AudioHeapD844 {
    u8 *start;
    u8 *cursor;
    u32 size;
};
struct AudioRootD844 {
    char pad0[0x1DA8];
    struct AudioHeapD844 heap;
};
struct AudioParamsD844 {
    s32 count38, count1C;
    u8 kind;
    u8 pad9[3];
    struct AudioHeapD844 *context;
    s32 value10, value14, value18;
};
struct AudioStateD844 {
    struct AudioRootD844 *root;
    char pad4[0xC];
    void *allocation;
    void *player;
    s32 mode, flags, unused20, timer, index, unused2C;
    f32 first, second;
    s32 counter;
    f32 third, fourth;
};
struct InitParams;
struct InitParams {
    s32 count38;
    s32 count1C;
    u8 kind;
    u8 pad09[3];
    s32 context;
    s32 value10;
    s32 value14;
    s32 value18;
};
typedef struct InitParams InitParams;
typedef struct SequenceResource {
    char pad[0x2B50];
    void *sections;
    char pad2B54[0xC];
    void *lookup;
    s32 count;
} SequenceResource;
typedef struct SequenceState {
    SequenceResource *resource;
    char pad4[0x20];
    s32 command;
    char pad28[8];
    f32 duration;
} SequenceState;
typedef struct SequenceCommand { s32 command; u16 duration; } SequenceCommand;
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
typedef struct SoundSourceNode SoundSourceNode;
struct Vec3Words;
struct Vec3Words {
    s32 w[3];
};
typedef struct Vec3Words Vec3Words;
struct NodeRecAtC8;
struct NodeRecAtC8 {
    struct NodeRecAtC8 *prev;
    struct NodeRecAtC8 *next;
    char channel[4];
    s32 volume;
    char pad10[4];
    s32 handle;
    s32 frame;
    s32 sample;
    s32 gainA;
    s32 gainB;
    s16 pitch;
    char pad2A[2];
    s32 time;
    char pad30[0x10];
    s16 priority;
    s16 group;
    s16 volume16;
    char pad46[2];
    s32 voice;
    Vec3Words position;
    s32 pad58;
    u8 pan;
    char pad5D[0x4F];
    s32 flags;
    char padB0[4];
    s32 padB4;
    char padB8[0x10];
    s32 mix;
};
struct Link_func_80259260_de;
struct Link_func_80259260_de {
    struct NodeRecAtC8 *prev;
    struct NodeRecAtC8 *next;
};
typedef struct Link_func_80259260_de Link_func_80259260_de;
struct ManagerRecAtD8V1;
struct ManagerRecAtD8V1 {
    s32 *scene;
    Link_func_80259260_de active;
    char padC[0xCC];
    Link_func_80259260_de free;
};
typedef struct ManagerRecAtD8V1 ManagerRecAtD8V1;
typedef struct NodeRecAtC8 NodeRecAtC8;
struct ManagerRecAtD8V2;
typedef struct ManagerRecAtD8V2 ManagerRecAtD8V2;
struct Link_func_80259784_de;
typedef struct Link_func_80259784_de Link_func_80259784_de;
struct NodeRecAt4C;
struct NodeRecAt4C {
    struct NodeRecAt4C *prev;
    struct NodeRecAt4C *next;
    char pad8[0x14];
    s32 timer;
    char pad20[0x20];
    s16 priority;
    char pad42[6];
    s32 owner;
    Vec3 position;
};
struct Link_func_80259784_de {
    struct NodeRecAt4C *prev;
    struct NodeRecAt4C *next;
};
struct Entry_func_80259784_de;
typedef struct Entry_func_80259784_de Entry_func_80259784_de;
struct Entry_func_80259784_de {
    char pad0[0xAC];
    s32 busy;
    char padB0[0x1C];
};
struct SceneCode802591C0;
struct SceneCode802591C0 {
    char pad0[0x1DBC];
    Entry_func_80259784_de entries[1];
    char pad1E88[0x2A84 - 0x1E88];
    s32 selection;
    s32 previousSelection;
    char pad2A8C[0x28];
    s16 activeSelection;
    s16 pendingSelection;
};
struct ManagerRecAtD8V2 {
    struct SceneCode802591C0 *scene;
    Link_func_80259784_de active;
    char padC[0xCC];
    Link_func_80259784_de free;
};
typedef struct NodeRecAt4C NodeRecAt4C;
struct Node_func_802598B4_de;
struct Node_func_802598B4_de {
    struct Node_func_802598B4_de *prev;
    struct Node_func_802598B4_de *next;
    s32 unused;
    s32 value;
};
typedef struct Node_func_802598B4_de Node_func_802598B4_de;
struct Node_func_80259918_de;
struct Node_func_80259918_de {
    struct Node_func_80259918_de *prev;
    struct Node_func_80259918_de *next;
    char pad08[0xB0 - 0x8];
    s32 key;
};
struct OwnerRecAtD8;
struct OwnerRecAtD8 {
    s32 unk00;
    struct Node_func_80259918_de *activePrev;
    struct Node_func_80259918_de *activeNext;
    char pad0C[0xD8 - 0xC];
    struct Node_func_80259918_de *otherPrev;
};
typedef struct Node_func_80259918_de Node_func_80259918_de;
typedef struct OwnerRecAtD8 OwnerRecAtD8;
struct ListNode;
struct ListNode {
    struct ListNode *prev;
    struct ListNode *next;
    unsigned char pad08[0xB4];
    s32 valueBC;
};
typedef struct ListNode ListNode;
struct NodeRecAtAC;
typedef struct NodeRecAtAC NodeRecAtAC;
struct NodeRecAtAC {
    struct NodeRecAtAC *prev;
    struct NodeRecAtAC *next;
    char pad8[0xA4];
    s32 flags;
};
struct Lists;
struct Lists {
    char pad0[4];
    NodeRecAtAC first;
    char padB4[0x24];
    NodeRecAtAC second;
};
typedef struct Lists Lists;
struct SongHeader;
struct SongHeader {
    u32 tempo;
    u16 volume;
};
typedef struct SongHeader SongHeader;
struct Bank;
struct Bank {
    char pad0[0x2B50];
    void *songs;
};
struct PlayerRecAt24;
struct PlayerRecAt24 {
    struct Bank *bank;
    char pad4[4];
    u32 state;
    s32 loop;
    s32 song;
    s32 speed;
    char pad18[3];
    u8 priority;
    char pad1C[2];
    s16 id;
    f32 tempoScale;
    f32 volume;
};
struct QueueRec;
typedef struct QueueRec QueueRec;
struct QueueRec {
    void *sequence;
    s32 head;
    u8 kind;
    s32 tail;
};
typedef struct Bank Bank;
typedef struct PlayerRecAt24 PlayerRecAt24;
struct Shape_typemap_110;
struct Shape_typemap_110 {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
};
struct ContextRecAt2BB8;
typedef struct ContextRecAt2BB8 ContextRecAt2BB8;
struct ContextRecAt2BB8 {
    char pad[0x2B50];
    s32 unk2B50;
    char p54[12];
    s32 unk2B60;
    s32 unk2B64;
    char p68[0x3C];
    f32 unk2BA4;
    char pa8[16];
    s32 unk2BB8;
};
struct StateRecAt10;
typedef struct StateRecAt10 StateRecAt10;
struct StateRecAt10 {
    s32 mode;
    f32 value;
    f32 limit;
    f32 rate;
    f32 accumulator;
};
typedef StateRecAt10 AxisWave;
struct Effect33920;
typedef struct Effect33920 Effect33920;
struct Effect33920 {
    s32 unk00;
    Effect33920 *next;
    Vec3 origin;
    f32 radius;
    AxisWave x;
    AxisWave y;
    AxisWave z;
};
struct Wave;
struct Wave {
    s32 kind;
    char pad4[0xC - 4];
    f32 amplitude;
    f32 phase;
};
struct Channel;
struct Channel {
    s32 mode;
    f32 level;
    s32 state[3];
};
struct RecordRecAt18;
struct RecordRecAt18 {
    char pad[8];
    Triple triple;
    f32 value;
    struct Channel channels[3];
};
struct Effect;
struct Effect {
    char pad00[0xC];
    s16 *definition;
    s16 mask;
    char pad12[2];
    s32 resource;
    char pad18[4];
    s32 angle0;
    s32 angle1;
    char pad24[0x1E4];
    s32 timer;
    s32 velocity;
    char pad210[4];
    f32 scale;
    f32 offset0;
    f32 offset1;
    f32 offset2;
};
typedef struct Effect Effect;
typedef void (*Shared_ContextCallback)(void *);
typedef void (*Shared_MenuVoidCallback)(void);
struct SharedTrackObject;
struct EntryRecAt48;
typedef struct EntryRecAt48 EntryRecAt48;
struct EntryRecAt48 {
    s32 id0;
    s32 id4;
    s32 id8;
    s32 flags;
    char pad10[0x28];
    f32 scaleA0;
    f32 aspectRatio;
    u32 settings[2];
    u32 unknown48;
};
typedef struct Shared_MenuContext Shared_MenuContext;
struct Shared_MenuContext {
    void *owner;
    void *resource;
    u32 unknown08;
    Shared_MenuVoidCallback onClose;
    Shared_MenuVoidCallback callback10;
    u32 unknown14;
    struct SharedTrackObject *target;
    f32 time;
    f32 time20;
    f32 time24;
    f32 clipStart;
    f32 clipEnd;
    f32 start;
    f32 end;
    s32 active;
    s32 state3C;
    u32 unknown40;
    s32 initialized44;
    s32 closed;
    s32 field4C;
    s32 field50;
    s32 pending;
    s32 field58;
    u32 unknown5C;
    u32 unknown60;
    u32 unknown64[1];
    EntryRecAt48 spec;
    s32 fieldB4;
    s32 mode;
    u32 unknownBC[9];
    s32 requestE0;
    Shared_ContextCallback callbackE4;
    Shared_MenuVoidCallback callbackE8;
    u32 unknownEC;
    Vec3 position;
    f32 yaw;
    f32 response;
    u32 unknown104[5];
    char messages[5][0x28];
    s32 messageCount;
};
struct func_802457D0_S1;
typedef struct func_802457D0_S1 func_802457D0_S1;
struct func_802457D0_S1 {
    char pad0[0x1C];
    f32 unk1C;
    char pad1C[0x34 - 0x1C - sizeof(f32)];
    f32 unk34;
    char pad34[0x38 - 0x34 - sizeof(f32)];
    s32 unk38;
};
typedef void (*FuncPtr)(void);
typedef struct SharedTrackObjectPrefix {
    u32 unknown0[2];
    Vec3 position;
    u8 unknown14[0x3C];
} SharedTrackObjectPrefix;
typedef struct SharedTrackObject {
    SharedTrackObjectPrefix prefix;
    u8 unknown50[0x1C];
    f32 yaw;
} SharedTrackObject;
struct ObjectState30;
typedef struct ObjectState30 ObjectState30;
struct ObjectState224;
typedef struct ObjectState224 ObjectState224;
struct ObjectState30 {
    unsigned char padding_0[44];
    s8 unk_2C;
    s8 unk_2D;
    s8 unk_2E;
    s8 unk_2F;
};
struct ObjectState224 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s8 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s8)];
    s32 unk_8;
    char pad8[0xC - 0x8 - sizeof(s32)];
    s32 unk_C;
    char padC[0x10 - 0xC - sizeof(s32)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    char pad14[0x18 - 0x14 - sizeof(s32)];
    s32 unk_18;
    char pad18[0x1C - 0x18 - sizeof(s32)];
    s32 unk_1C;
    char pad1C[0x20 - 0x1C - sizeof(s32)];
    s32 unk_20;
    char pad20[0x24 - 0x20 - sizeof(s32)];
    s32 unk_24;
    char pad24[0x28 - 0x24 - sizeof(s32)];
    s32 unk_28;
    char pad28[0xC4 - 0x28 - sizeof(s32)];
    s8 unk_C4;
    char padC4[0xC5 - 0xC4 - sizeof(s8)];
    s8 unk_C5;
    char padC5[0xC6 - 0xC5 - sizeof(s8)];
    s8 unk_C6;
    char padC6[0xC7 - 0xC6 - sizeof(s8)];
    s8 unk_C7;
    char padC7[0xC8 - 0xC7 - sizeof(s8)];
    s32 unk_C8;
    char padC8[0xCC - 0xC8 - sizeof(s32)];
    s32 unk_CC;
    char padCC[0xD0 - 0xCC - sizeof(s32)];
    s32 unk_D0;
    char padD0[0xD4 - 0xD0 - sizeof(s32)];
    s32 unk_D4;
    char padD4[0x220 - 0xD4 - sizeof(s32)];
    s32 unk_220;
};
struct Obj80264124;
struct Obj80264124 {
    s32 flags;
    s8 enabled;
    char pad05[3];
    s32 x;
    s32 y;
    s32 z;
    char pad14[0xB4];
    s32 active;
    s32 state;
    f32 amount;
    f32 timer;
    char status[0x68];
    char object[1];
};
typedef struct Obj80264124 Obj80264124;
#ifndef COMMON_H
typedef signed long long s64;
#endif
struct OSSched;
struct View_func_8028FA60_de;
typedef struct View_func_8028FA60_de View_func_8028FA60_de;
struct Scene_func_8028FA60_de;
struct Scene_func_8028FA60_de {
    s32 pad0;
    s32 flags;
    s32 options;
    s32 padC;
    s32 mode;
    char pad14[0x38 - 0x14];
    s32 music;
    s64 *time;
};
struct OSScClientWork;
struct OSScClientWork {
    struct OSScClientWork *next;
    OSMesgQueue *queue;
};
struct View_func_8028FA60_de {
    char pad0[0x78];
    struct OSMesgQueue messages;
    char pad90[0x2F4 - 0x90];
    struct Scene_func_8028FA60_de *current;
    struct Scene_func_8028FA60_de *next;
    char pad2FC[4];
    s32 pending;
};
struct FrameSchedule;
struct FrameSchedule {
    char pad0[0x110];
    s32 task;
    char pad114[0x10];
    u32 duration;
    char pad128[0x18];
};
struct OSScTask_s;
struct OSScTask_s {
    struct OSScTask_s *next;
    u32 state;
    u32 flags;
    void *framebuffer;
    struct { s32 type; } list;
};
typedef struct FrameSchedule FrameSchedule;
typedef struct Scene_func_8028FA60_de Scene_func_8028FA60_de;
typedef struct OSScTask_s OSScTask_s;
struct OSSched {
    char pad0[0x20];
    s16 retraceMessage;
    char pad22[0x40 - 0x22];
    OSMesgQueue interruptQueue;
    char pad58[0x78 - 0x58];
    OSMesgQueue commandQueue;
    char pad90[0x2E0 - 0x90];
    struct OSScClientWork *clientList;
    struct OSScTask_s *audioListHead;
    struct OSScTask_s *gfxListHead;
    struct OSScTask_s *audioListTail;
    struct OSScTask_s *gfxListTail;
    struct OSScTask_s *curRSPTask;
    struct OSScTask_s *curRDPTask;
    u32 frameCount;
    s32 doAudio;
};
typedef struct OSScClientWork OSScClientWork;
typedef struct OSSched OSSched;
struct Ctx_func_8028F544_de;
struct Node_func_8028F544_de;
typedef struct Ctx_func_8028F544_de Ctx_func_8028F544_de;
typedef struct Node_func_8028F544_de Node_func_8028F544_de;
struct Node_func_8028F544_de {
    struct Node_func_8028F544_de *unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
};
struct Ctx_func_8028F544_de {
    char p0[0x2E4];
    struct Node_func_8028F544_de *unk2E4;
    struct Node_func_8028F544_de *unk2E8;
    s32 unk2EC;
    s32 unk2F0;
    char p1[0xC];
    s32 unk300;
};
struct Panel;
struct Panel {
    s16 active;
    char pad2[0x20 - 2];
    s16 mode;
    char pad22[0x40 - 0x22];
    char row0[0x18];
    char row0Slots[0x20];
    char row1[0x18];
    char row1Slots[0x20];
    char elements[0x2E0 - 0xB0];
    s32 field2E0;
    s32 field2E4;
    s32 field2E8;
    s32 field2EC;
    s32 field2F0;
    s32 field2F4;
    s32 field2F8;
    s32 field2FC;
    s32 field300;
};
typedef struct Panel Panel;
struct Node_func_8028FC10_de;
typedef struct Node_func_8028FC10_de Node_func_8028FC10_de;
struct Node_func_8028FC10_de {
    char pad0[8];
    u32 flags;
    char padC[4];
    s32 type;
};
struct EntryRecAt3;
struct EntryRecAt3 {
    u16 id;
    u8 team;
    u8 slot;
};
typedef struct EntryRecAt3 EntryRecAt3;
struct Mapping;
typedef struct Mapping Mapping;
struct Mapping {
    char pad0[8];
    u16 flags;
    char padA;
    u8 slot;
};
struct Draw;
typedef struct Draw Draw;
struct Draw {
    char pad0[8];
    s32 allocationMode;
    void *model;
    char reserved10[0x10];
    IntrusiveList recycled;
};
struct Block10;
typedef struct Block10 Block10;
struct Block10 {
    s16 f28;
    s16 pad;
    s32 f2C;
    void *f30;
    void **f34;
};
struct Block_func_8023C85C_de;
typedef struct Block_func_8023C85C_de Block_func_8023C85C_de;
struct Block_func_8023C85C_de {
    s16 type;
    s16 pad;
    s32 value;
    void *data;
};
struct Node_func_8023CC08_de;
typedef struct Node_func_8023CC08_de Node_func_8023CC08_de;
struct Link_func_8023CC08_de;
struct Link_func_8023CC08_de {
    struct Link_func_8023CC08_de *next;
    struct Link_func_8023CC08_de *prev;
    u16 id;
    union { u16 padA; struct { u8 index; u8 slot; }; };
    s32 fieldC;
};
typedef struct Link_func_8023CC08_de Link_func_8023CC08_de;
struct Node_func_8023CC08_de {
    struct Node_func_8023CC08_de *next;
    u16 start;
    u16 size;
    char pad8[8];
    u8 *data;
};
struct Node_func_8023CCD4_de;
typedef struct Node_func_8023CCD4_de Node_func_8023CCD4_de;
struct OwnerRecAt8V2;
struct OwnerRecAt8V2 {
    s32 unk0;
    u32 handle;
    s32 queue;
};
typedef struct OwnerRecAt8V2 OwnerRecAt8V2;
struct Node_func_8023CCD4_de {
    struct Node_func_8023CCD4_de *next;
    u16 id;
    u16 count;
    s32 unk8;
    s32 unkC;
    u8 *slots;
};
struct Slot_func_8023CCD4_de;
typedef struct Slot_func_8023CCD4_de Slot_func_8023CCD4_de;
struct Slot_func_8023CCD4_de {
    s32 unk0;
    u16 unk4;
    u16 unk6;
    u16 id;
    u16 unkA;
    s32 unkC;
};
struct ALCSeq_s;
typedef struct ALCSeq_s ALCSeq_s;
struct ALCMidiHdr;
struct ALCMidiHdr {
    u32 trackOffset[16];
    u32 division;
};
typedef struct ALCMidiHdr ALCMidiHdr;
struct ALCSeq_s {
    struct ALCMidiHdr *base;
    u32 validTracks;
    f32 qnpt;
    u32 lastTicks;
    u32 lastDeltaTicks;
    u32 deltaFlag;
    u8 *curLoc[16];
    u8 *curBUPtr[16];
    u8 curBULen[16];
    u8 lastStatus[16];
    u32 evtDeltaTicks[16];
};
struct ResourceTable;
typedef struct ResourceTable ResourceTable;
struct ResourceTable {
    u32 entries[17];
};
struct RuntimeStateRecAtB8;
struct RuntimeStateRecAtB8 {
    struct ResourceTable *resources;
    u32 active;
    f32 scale;
    u32 unkC;
    u32 unk10;
    u32 one14;
    void *objects[16];
    u32 unk58[16];
    s8 flags98[16];
    s8 flagsA8[16];
    u32 results[16];
};
typedef struct RuntimeStateRecAtB8 RuntimeStateRecAtB8;
struct DecodeResult_func_802AE6BC_de;
struct DecodeResult_func_802AE6BC_de {
    s16 type;
    u8 pad2[2];
    u32 value;
    u8 pad8[8];
};
struct RuntimeState;
typedef struct RuntimeState RuntimeState;
struct RuntimeState {
    void *resources;
    u32 active;
    u32 scale;
    u32 total;
    u32 previous;
    u32 first;
    u8 *objects[16];
    u32 values[16];
    u8 flags98[16];
    u8 flagsA8[16];
    u32 results[16];
};
struct DecodeResult;
struct DecodeResult {
    s16 type;
    u8 pad2[6];
    u8 status;
    u8 first_data;
    u8 second_data;
    u8 extra1;
    u32 extra2;
};
typedef struct DecodeResult_func_802AE6BC_de DecodeResult_func_802AE6BC_de;
typedef struct DecodeResult DecodeResult;
struct ObjRecAt40V1;
typedef struct ObjRecAt40V1 ObjRecAt40V1;
struct ObjRecAt40V1 {
    char p[0x40];
    s32 unk40;
};
struct CopyDest802B3998;
typedef struct CopyDest802B3998 CopyDest802B3998;
struct CopySource802B3998;
struct CopySource802B3998 {
    int word0;
    int word4;
    int word8;
    int wordsC[16];
    int words4C[16];
    unsigned char bytes8C[16];
    unsigned char bytes9C[16];
    int wordsAC[16];
};
typedef struct CopySource802B3998 CopySource802B3998;
struct CopyDest802B3998 {
    int pad0;
    int word4;
    int pad8;
    int wordC;
    int word10;
    int pad14;
    int words18[16];
    int words58[16];
    unsigned char bytes98[16];
    unsigned char bytesA8[16];
    int wordsB8[16];
};
struct ALEventQueue;
typedef struct ALEventQueue ALEventQueue;
struct ALNoteEvent;
struct ALOscEvent;
struct ALVolumeEvent;
struct ALEndEvent;
struct ALTempoEvent;
struct ALEvent;
typedef struct ALNoteEvent ALNoteEvent;
typedef struct ALOscEvent ALOscEvent;
typedef struct ALVolumeEvent ALVolumeEvent;
typedef struct ALEndEvent ALEndEvent;
typedef struct ALTempoEvent ALTempoEvent;
typedef struct ALEvent ALEvent;
struct ALEventQueue {
    ListLink freeList;
    ListLink allocList;
    s32 eventCount;
};
struct ALVoice_s;
typedef struct ALVoice_s ALVoice_s;
struct ALVoice_s {
    ListLink node;
    void *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALMIDIEvent;
typedef struct ALMIDIEvent ALMIDIEvent;
struct InventorySlot;
struct InventorySlot {
    u8 available;
    u8 slot;
};
typedef struct InventorySlot InventorySlot;
struct ALMIDIEvent {
    s32 ticks;
    u8 status;
    u8 byte1;
    u8 byte2;
    u32 duration;
};
struct ALPlayer_s;
typedef struct ALPlayer_s ALPlayer_s;
struct ALPlayer_s {
    struct ALPlayer_s *next;
    void *clientData;
    void *handler;
    s32 callTime;
    s32 samplesLeft;
};
struct Request;
struct Request {
    s16 type;
};
typedef struct Request Request;
struct ALEndEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
};
struct ALTempoEvent {
    s32 ticks;
    u8 status;
    u8 type;
    u8 len;
    u8 byte1;
    u8 byte2;
    u8 byte3;
};
struct ALNoteEvent {
    struct ALVoice_s *voice;
};
struct ALVolumeEvent {
    struct ALVoice_s *voice;
    s32 delta;
    u8 vol;
};
struct ALOscEvent {
    struct ALVoiceState *vs;
    void *oscState;
    u8 chan;
};
struct ALEvent {
    s16 type;
    union {
        ALMIDIEvent midi;
        ALTempoEvent tempo;
        ALEndEvent end;
        ALNoteEvent note;
        ALVolumeEvent vol;
        Request spvol;
        InventorySlot sppriority;
        func_80284AF4_G2 spseq;
        func_80284AF4_G2 spbank;
        ALOscEvent osc;
    } msg;
};
struct ALBank_s;
typedef struct ALBank_s ALBank_s;
struct ALKeyMap;
struct ALKeyMap {
    u8 velocityMin;
    u8 velocityMax;
    u8 keyMin;
    u8 keyMax;
    u8 keyBase;
    s8 detune;
};
struct ALVoiceState;
struct ALInstrument;
typedef struct ALInstrument ALInstrument;
struct ALSound_s;
typedef struct ALSound_s ALSound_s;
typedef struct ALKeyMap ALKeyMap;
struct ALEnvelope;
struct ALEnvelope {
    s32 attackTime;
    s32 decayTime;
    s32 releaseTime;
    u8 attackVolume;
    u8 decayVolume;
};
typedef struct ALEnvelope ALEnvelope;
struct ALSound_s {
    ALEnvelope *envelope;
    ALKeyMap *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ALInstrument {
    u8 volume;
    u8 pan;
    u8 priority;
    u8 flags;
    u8 tremType;
    u8 tremRate;
    u8 tremDepth;
    u8 tremDelay;
    u8 vibType;
    u8 vibRate;
    u8 vibDepth;
    u8 vibDelay;
    s16 bendRange;
    s16 soundCount;
    ALSound_s *soundArray[1];
};
struct ALChanState10;
struct ALChanState10 {
    ALInstrument *instrument;
    s16 bendRange;
    u8 fxId;
    u8 pan;
    u8 priority;
    u8 vol;
    u8 fxmix;
    u8 sustain;
    f32 pitchBend;
};
struct ALSeqPlayer;
typedef struct ALSeqPlayer ALSeqPlayer;
struct ALBank_s {
    s16 instCount;
    u8 flags;
    u8 pad;
    s32 sampleRate;
    ALInstrument *percussion;
    ALInstrument *instArray[1];
};
typedef struct ALVoiceState ALVoiceState;
typedef signed int ( *Lane_ALOscUpdate)(void *, float *);
struct ALVoiceState {
    struct ALVoiceState *next;
    ALVoice_s voice;
    ALSound_s *sound;
    s32 envEndTime;
    f32 pitch;
    f32 vibrato;
    u8 envGain;
    u8 channel;
    u8 key;
    u8 velocity;
    u8 envPhase;
    u8 phase;
    u8 tremelo;
    u8 flags;
};
typedef signed int ( *Lane_ALOscInit)(void * *, float *, unsigned char, unsigned char, unsigned char, unsigned char);
typedef void ( *Lane_ALOscStop)(void *);
typedef int ALMicroTime;
typedef struct ALChanState10 ALChanState10;
struct ALSeqPlayer {
    ALPlayer_s node;
    void *drvr;
    void *target;
    ALMicroTime curTime;
    ALBank_s *bank;
    s32 uspt;
    s32 nextDelta;
    s32 state;
    u16 chanMask;
    s16 vol;
    u8 maxChannels;
    u8 debugFlags;
    ALEvent nextEvent;
    ALEventQueue evtq;
    ALMicroTime frameTime;
    ALChanState10 *chanState;
    ALVoiceState *vAllocHead;
    ALVoiceState *vAllocTail;
    ALVoiceState *vFreeList;
    Lane_ALOscInit initOsc;
    Lane_ALOscUpdate updateOsc;
    Lane_ALOscStop stopOsc;
};
typedef int (*ALSetParam)(void *, int, void *);
struct ALSynth_func_802B3000_de;
typedef struct ALSynth_func_802B3000_de ALSynth_func_802B3000_de;
struct ALVoice_s_func_802B3000_de;
typedef struct ALVoice_s_func_802B3000_de ALVoice_s_func_802B3000_de;
struct ALSynth_func_802B3000_de {
    void *head;
    ListLink pFreeList;
    ListLink pAllocList;
    ListLink pLameList;
    s32 paramSamples;
};
struct ALVoiceConfig_s;
typedef struct ALVoiceConfig_s ALVoiceConfig_s;
struct ALFilter_s_func_802B3000_de;
struct ALFilter_s_func_802B3000_de {
    struct ALFilter_s_func_802B3000_de *source;
    void *handler;
    ALSetParam setParam;
};
struct PVoice_s_func_802B3000_de;
struct PVoice_s_func_802B3000_de {
    ListLink node;
    struct ALVoice_s_func_802B3000_de *vvoice;
    struct ALFilter_s_func_802B3000_de *channelKnob;
    char pad10[0xD8 - 0x10];
    s32 offset;
};
struct ALVoice_s_func_802B3000_de {
    ListLink node;
    struct PVoice_s_func_802B3000_de *pvoice;
    void *table;
    void *clientPrivate;
    s16 state;
    s16 priority;
    s16 fxBus;
    s16 unityPitch;
};
struct ALVoiceConfig_s {
    s16 priority;
    s16 fxBus;
    u8 unityPitch;
};
typedef struct ALFilter_s_func_802B3000_de ALFilter_s_func_802B3000_de;
struct ObjRecAt8V3;
typedef struct ObjRecAt8V3 ObjRecAt8V3;
struct Node_func_802AFD6C_de;
struct Node_func_802AFD6C_de {
    struct Node_func_802AFD6C_de *prev;
    struct Node_func_802AFD6C_de *next;
    s32 value;
    short type;
};
struct ObjRecAt8V3 {
    char pad0[0x8];
    struct Node_func_802AFD6C_de * unk8;
};
typedef struct Node_func_802AFD6C_de Node_func_802AFD6C_de;
struct Buf16c;
typedef struct Buf16c Buf16c;
struct Buf16c {
    s16 count;
    s16 _pad2;
    s32 zero;
    s8 f8;
    s8 f9;
    s8 fA;
    s8 _padB;
    s32 _padC;
};
struct ALEventListItem;
typedef struct ALEventListItem ALEventListItem;
struct ALEventListItem {
    ListLink node;
    s32 delta;
    ALEvent evt;
};
struct Node_func_80239AF4_de;
typedef struct Node_func_80239AF4_de Node_func_80239AF4_de;
struct Node_func_80239AF4_de {
    struct Node_func_80239AF4_de *next;
};
struct List802B663C;
typedef struct List802B663C List802B663C;
struct List802B663C {
    char pad[0x64];
    Node_func_80239AF4_de *head;
    Node_func_80239AF4_de *tail;
    Node_func_80239AF4_de *free;
};
struct func_80254D70_S2;
typedef struct func_80254D70_S2 func_80254D70_S2;
struct func_80254D70_S2 {
    char pad0[0x8];
    s32 unk8;
};
struct ObjectLinks88;
typedef struct ObjectLinks88 ObjectLinks88;
struct ObjectLinks88 {
    char pad0[0x18];
    void * unk_18;
    char pad18[0x24 - 0x18 - sizeof(void*)];
    s32 unk_24;
    char pad24[0x2C - 0x24 - sizeof(s32)];
    s32 unk_2C;
    char pad2C[0x48 - 0x2C - sizeof(s32)];
    char unk_48;
    char pad48[0x7C - 0x48 - sizeof(char)];
    void * unk_7C;
    char pad7C[0x80 - 0x7C - sizeof(void*)];
    char * unk_80;
    char pad80[0x84 - 0x80 - sizeof(char*)];
    s32 unk_84;
};
struct ObjRecAt18V1;
struct ObjRecAt18V1 {
    char pad18[0x18];
    s16 field18;
};
typedef struct ObjRecAt18V1 ObjRecAt18V1;
struct ALSeq_s;
typedef struct ALSeq_s ALSeq_s;
struct ALSeq_s {
    u8 *base;
    u8 *trackStart;
    u8 *curPtr;
    s32 lastTicks;
    s32 len;
    f32 qnpt;
    s16 division;
    s16 lastStatus;
};
struct ALSound;
struct ALSound {
    void *envelope;
    void *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
typedef struct ALSound ALSound;
struct PoolRecAt44;
typedef struct PoolRecAt44 PoolRecAt44;
struct SlotRecAt2F;
typedef struct SlotRecAt2F SlotRecAt2F;
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
struct PoolRecAt44 {
    char pad0[0x40];
    struct SlotRecAt2F *slots;
    s32 count;
};
typedef int (*ALVoiceHandler)(void *);
struct ALPlayer_s14;
struct ALPlayer_s14 {
    struct ALPlayer_s14 *next;
    void *clientData;
    ALVoiceHandler handler;
    s32 callTime;
    s32 samplesLeft;
};
typedef struct ALPlayer_s14 ALPlayer_s14;
struct ALHeap;
typedef struct ALHeap ALHeap;
struct ALHeap {
    u8 *base;
    u8 *cur;
    s32 len;
    s32 count;
};
struct ALSynth_func_802B2650_de;
struct ALSynth_func_802B2650_de {
    ALPlayer_s14 *head;
};
struct ALSndPlayer_func_802B2650_de;
struct ALSndPlayer_func_802B2650_de {
    ALPlayer_s14 node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    struct ALSynth_func_802B2650_de *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    s32 frameTime;
    s32 nextDelta;
    s32 curTime;
};
struct ALGlobals_func_802B2650_de;
typedef struct ALGlobals_func_802B2650_de ALGlobals_func_802B2650_de;
struct ALSndpConfig;
struct ALSndpConfig {
    s32 maxSounds;
    s32 maxEvents;
    ALHeap *heap;
};
struct ALSoundState;
typedef struct ALSoundState ALSoundState;
typedef struct ALSndPlayer_func_802B2650_de ALSndPlayer_func_802B2650_de;
typedef struct ALSndpConfig ALSndpConfig;
typedef struct ALSynth_func_802B2650_de ALSynth_func_802B2650_de;
struct ALGlobals_func_802B2650_de {
    ALSynth_func_802B2650_de drvr;
};
struct ALSoundState {
    ALVoice_s voice;
    void *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
};
union ALSndpEvent;
union ALSndpEvent {
    ALEvent msg;
    struct {
        s16 type;
        void *state;
    } common;
};
struct ALSndPlayer;
struct ALSndPlayer {
    ALPlayer_s node;
    ALEventQueue evtq;
    ALEvent nextEvent;
    void *drvr;
    s32 target;
    void *sndState;
    s32 maxSounds;
    s32 frameTime;
    s32 nextDelta;
    s32 curTime;
};
typedef struct ALSndPlayer ALSndPlayer;
typedef union ALSndpEvent ALSndpEvent;
union ALSndpEvent_func_802B2780_de;
typedef union ALSndpEvent_func_802B2780_de ALSndpEvent_func_802B2780_de;
struct ALSoundState_func_802B2780_de;
typedef struct ALSoundState_func_802B2780_de ALSoundState_func_802B2780_de;
struct ALSound_s_func_802B2780_de;
typedef struct ALSound_s_func_802B2780_de ALSound_s_func_802B2780_de;
struct ALSound_s_func_802B2780_de {
    ALEnvelope *envelope;
    void *keyMap;
    void *wavetable;
    u8 samplePan;
    u8 sampleVolume;
    u8 flags;
};
struct ALSoundState_func_802B2780_de {
    ALVoice_s voice;
    struct ALSound_s_func_802B2780_de *sound;
    s16 priority;
    f32 pitch;
    s32 state;
    s16 vol;
    u8 pan;
    u8 fxMix;
};
union ALSndpEvent_func_802B2780_de {
    ALEvent msg;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
    } common;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        s16 vol;
    } vol;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        f32 pitch;
    } pitch;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        u8 pan;
    } pan;
    struct {
        s16 type;
        ALSoundState_func_802B2780_de *state;
        u8 mix;
    } fx;
};
struct ParamsRecAt8V1;
typedef struct ParamsRecAt8V1 ParamsRecAt8V1;
struct ParamsRecAt8V1 {
    s16 f0;
    s32 f4;
    s16 f8;
};
struct ParamsRecAt8V2;
typedef struct ParamsRecAt8V2 ParamsRecAt8V2;
struct ParamsRecAt8V2 {
    s16 f0;
    s32 f4;
    s8 f8;
};
struct ObjRecAt40V2;
struct ObjRecAt40V2 {
    u8 padding000[0x14];
    ALEventQueue events;
    u8 padding028[0x14];
    s32 field3C;
    s32 field40;
};
typedef struct ObjRecAt40V2 ObjRecAt40V2;
struct ParamsRecAt8V3;
struct ParamsRecAt8V3 {
    s16 f0;
    s32 f4;
    s32 f8;
};
typedef struct ParamsRecAt8V3 ParamsRecAt8V3;
struct Buf16;
typedef struct Buf16 Buf16;
struct Buf16 {
    s16 count;
    s16 _pad2;
    s32 value;
    s32 _pad8;
    s32 _padC;
};
struct Limit;
struct Limit {
    char pad[0x16];
    s16 limit;
};
struct Node_func_802B3130_de;
typedef struct Node_func_802B3130_de Node_func_802B3130_de;
struct Node_func_802B3130_de {
    struct Node_func_802B3130_de *next;
    char pad4[4];
    struct Limit *limit;
    char padC[0xCC];
    s32 flags;
};
union ObjectLinks4_3;
typedef union ObjectLinks4_3 ObjectLinks4_3;
union ObjectLinks4_3 {
    void * v0;
    char * v1;
};
struct ObjectStateC_2;
struct ObjectStateC_2 {
    char pad0[0x8];
    ObjectLinks4_3 unk_8;
};
struct CallbackStateC_4;
typedef struct CallbackStateC_4 CallbackStateC_4;
struct Shape_typemap_71;
struct Shape_typemap_71 {
    unsigned char padding_0[28];
    int field_1C;
};
struct IntegerStateDC_2;
typedef struct IntegerStateDC_2 IntegerStateDC_2;
typedef struct ObjectStateC_2 ObjectStateC_2;
struct CallbackStateC_4 {
    char pad0[0x8];
    void (*callback)(void *, s32, void *);
};
struct IntegerStateDC_2 {
    unsigned char padding[216];
    s32 state;
};
struct ObjectLinks10_2;
typedef struct ObjectLinks10_2 ObjectLinks10_2;
struct ObjectLinks10_2 {
    char pad0[0x4];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
    char pad8[0xC - 0x8 - sizeof(s16)];
    void * unk_C;
};
typedef void ( *SharedRecV8)(void *, signed int, void *);
typedef void ( *SharedRecV4)(void *, signed int, void *);
struct ObjectState10_3;
typedef struct ObjectState10_3 ObjectState10_3;
struct ObjectState10_3 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
    char pad8[0xC - 0x8 - sizeof(s16)];
    s32 unk_C;
};
typedef void ( *SharedRecV5)(void *, signed int, void *);
struct ObjectState10_4;
typedef struct ObjectState10_4 ObjectState10_4;
typedef void ( *SharedRecV7)(void *, signed int, void *);
struct ObjectState10_4 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
    char pad8[0xC - 0x8 - sizeof(s16)];
    f32 unk_C;
};
struct ObjectState10_5;
typedef struct ObjectState10_5 ObjectState10_5;
typedef void ( *SharedRecV3)(void *, signed int, void *);
struct ObjectState1C;
typedef struct ObjectState1C ObjectState1C;
struct ObjectState1C {
    char pad0[0x8];
    ObjectLinks4_3 unk_8;
    char pad8[0x1A - 0x8 - sizeof(ObjectLinks4_3)];
    u16 unk_1A;
};
struct ObjectState10_5 {
    s32 unk_0;
    char pad0[0x4 - 0x0 - sizeof(s32)];
    s32 unk_4;
    char pad4[0x8 - 0x4 - sizeof(s32)];
    s16 unk_8;
    char pad8[0xA - 0x8 - sizeof(s16)];
    u16 unk_A;
    char padA[0xC - 0xA - sizeof(u16)];
    s32 unk_C;
};
struct ALStartParamAlt;
struct ALStartParamAlt {
    struct Node_func_80239AF4_de *next;
    s32 delta;
    s16 type;
    s16 unity;
    f32 pitch;
    s16 volume;
    u8 pan;
    u8 fxMix;
    s32 samples;
    void *wave;
};
typedef struct ALStartParamAlt ALStartParamAlt;
typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef union {
    Awords words;
    long long int force_union_align;
} Acmd;
typedef Acmd *(*ALCmdHandler)(void *, short *, int, int, Acmd *);
struct ALFilter_sC;
struct ALFilter_sC {
    struct ALFilter_sC *source;
    ALCmdHandler handler;
    ALSetParam setParam;
};
typedef struct ALFilter_sC ALFilter_sC;
struct ALSynth4C;
struct ALSynth4C {
    ALPlayer_s14 *head;
    ListLink pFreeList;
    ListLink pAllocList;
    ListLink pLameList;
    s32 paramSamples;
    s32 curSamples;
    void *dma;
    void *heap;
    void *paramList;
    void *mainBus;
    void *auxBus;
    ALFilter_sC *outputFilter;
    s32 numPVoices;
    s32 maxAuxBusses;
    s32 outputRate;
    s32 maxOutSamples;
};
typedef struct ALSynth4C ALSynth4C;
struct ALGlobals;
struct ALGlobals {
    ALSynth4C drvr;
};
typedef struct ALGlobals ALGlobals;
struct Obj_func_802B3EDC_de;
struct Obj_func_802B3EDC_de {
    char pad14[0x14];
    s32 count;
    char pad18[0x4];
    s32 *base;
};
typedef struct Obj_func_802B3EDC_de Obj_func_802B3EDC_de;
struct ALFilter_s14;
typedef struct ALFilter_s14 ALFilter_s14;
struct ALFilter_s14 {
    struct ALFilter_s14 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
typedef signed short AudioPoleFilterState[4];
struct AudioLowPassFilter;
struct AudioLowPassFilter {
    s16 cutoff;
    s16 gain;
    union {
        s16 taps[16];
        s64 alignment;
    } coefficients;
    AudioPoleFilterState *state;
    s32 first;
};
typedef struct AudioLowPassFilter AudioLowPassFilter;
struct ALDelay28;
typedef struct ALDelay28 ALDelay28;
struct ALResampler_s28;
struct ALResampler_s28 {
    ALFilter_s14 filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
};
struct ALFilter_s14_2;
typedef struct ALFilter_s14_2 ALFilter_s14_2;
struct ALFilter_s14_2 {
    struct ALFilter_s14_2 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
struct ALResampler_s;
struct ALResampler_s {
    ALFilter_s14_2 filter;
    void *state;
    f32 ratio;
    s32 upitch;
    f32 delta;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    s32 motion;
};
typedef struct ALResampler_s28 ALResampler_s28;
struct ALDelay28 {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    void *lp;
    ALResampler_s28 *rs;
};
typedef struct ALResampler_s ALResampler_s;
struct ALFx;
typedef struct ALFx ALFx;
struct ALFx {
    ALFilter_s14 filter;
    s16 *base;
    s16 *input;
    u32 length;
    ALDelay28 *delay;
    u8 section_count;
    void *paramHdl;
};
struct ALSynConfig;
typedef struct ALSynConfig ALSynConfig;
struct ALSynConfig {
    s32 maxVVoices;
    s32 maxPVoices;
    s32 maxUpdates;
    s32 maxFXbusses;
    void *dmaproc;
    ALHeap *heap;
    s32 outputRate;
    u8 fxType;
    s32 *params;
};
struct ALEnvMixer_s;
typedef struct ALEnvMixer_s ALEnvMixer_s;
struct ALEnvMixer_s {
    ALFilter_s14_2 filter;
    void *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    void *ctrlList;
    void *ctrlTail;
    ALFilter_s14_2 **sources;
    s32 motion;
};
struct LogTab;
typedef struct LogTab LogTab;
struct LogTab {
    f64 v[8];
};
struct ALParam_s;
typedef struct ALParam_s ALParam_s;
struct ALFilter_s;
typedef struct ALFilter_s ALFilter_s;
struct ALFilter_s {
    struct ALFilter_s *source;
    void *handler;
    s32 (*setParam)(void *, s32, void *);
    s16 inp;
    s16 outp;
    s32 type;
};
struct ALParam_s {
    struct ALParam_s *next;
    s32 delta;
    s32 type;
    union {
        s32 i;
        f32 f;
        void *p;
    } data;
};
struct ALEnvMixer;
struct ALEnvMixer {
    ALFilter_s filter;
    void *state;
    s16 pan;
    s16 volume;
    s16 cvolL;
    s16 cvolR;
    s16 dryamt;
    s16 wetamt;
    u16 lratl;
    s16 lratm;
    s16 ltgt;
    u16 rratl;
    s16 rratm;
    s16 rtgt;
    s32 delta;
    s32 segEnd;
    s32 first;
    struct ALParam_s *ctrlList;
    struct ALParam_s *ctrlTail;
    struct ALFilter_s **sources;
    s32 motion;
};
typedef struct ALEnvMixer ALEnvMixer;
struct CallbackStateC;
struct CallbackStateC {
    unsigned char padding_0[8];
    void (*callback)(void *, s32, s32);
};
struct IntegerState34;
typedef struct IntegerState34 IntegerState34;
struct IntegerState34 {
    char pad0[0x18];
    s32 unk_18;
    char pad18[0x1C - 0x18 - sizeof(s32)];
    s32 unk_1C;
    char pad1C[0x20 - 0x1C - sizeof(s32)];
    s32 unk_20;
    char pad20[0x24 - 0x20 - sizeof(s32)];
    s32 unk_24;
    char pad24[0x30 - 0x24 - sizeof(s32)];
    s32 unk_30;
};
typedef struct CallbackStateC CallbackStateC;
struct func_8022E694_S1;
typedef struct func_8022E694_S1 func_8022E694_S1;
struct func_8022E694_S1 {
    char pad0[0x44];
    s32 unk44;
};
struct ALDelay_func_802B5B94_de;
typedef struct ALDelay_func_802B5B94_de ALDelay_func_802B5B94_de;
struct ALFxCode802BAC64;
typedef struct ALFxCode802BAC64 ALFxCode802BAC64;
struct ALGlobals_func_802B5B94_de;
typedef struct ALGlobals_func_802B5B94_de ALGlobals_func_802B5B94_de;
struct ALDelay_func_802B5B94_de {
    u32 input;
    u32 output;
    s16 ffcoef;
    s16 fbcoef;
    s16 gain;
    f32 rsinc;
    f32 rsval;
    s32 rsdelta;
    f32 rsgain;
    Clip *lp;
    void *rs;
};
struct ALFxCode802BAC64 {
    ALFilter_s14 filter;
    s16 *base;
    s16 *input;
    u32 length;
    struct ALDelay_func_802B5B94_de *delay;
    u8 section_count;
    void *paramHdl;
};
struct ALGlobals_func_802B5B94_de {
    func_8022E694_S1 drvr;
};
struct ContReadFormat;
typedef struct ContReadFormat ContReadFormat;
struct ContReadFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u16 button;
    s8 stick_x;
    s8 stick_y;
};
struct ContPad;
struct ContPad {
    u16 button;
    s8 stick_x;
    s8 stick_y;
    u8 error;
};
typedef struct ContPad ContPad;
struct OSPifRam;
struct OSPifRam {
    u32 ramarray[15];
    u32 pifstatus;
};
typedef struct OSPifRam OSPifRam;
struct __OSContRequesFormat;
struct __OSContRequesFormat {
    u8 dummy;
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
    u8 dummy1;
};
typedef struct __OSContRequesFormat __OSContRequesFormat;
struct Block40;
struct Block40 {
    u8 bytes[0x28];
};
typedef struct Block40 Block40;
struct OSTask_t;
struct OSTask_t {
    u32 type;
    u32 flags;
    u64 *ucode_boot;
    u32 ucode_boot_size;
    u64 *ucode;
    u32 ucode_size;
    u64 *ucode_data;
    u32 ucode_data_size;
    u64 *dram_stack;
    u32 dram_stack_size;
    u64 *output_buff;
    u64 *output_buff_size;
    u64 *data_ptr;
    u32 data_size;
    u64 *yield_data_ptr;
    u32 yield_data_size;
};
typedef struct OSTask_t OSTask_t;
union OSTask;
union OSTask {
    OSTask_t t;
    long long force_structure_alignment;
};
typedef union OSTask OSTask;
struct __OSViScale;
struct __OSViScale {
    f32 factor;
    u16 offset;
    u32 scale;
};
struct OSViCommonRegs;
struct OSViCommonRegs {
    u32 ctrl;
    u32 width;
    u32 burst;
    u32 vSync;
    u32 hSync;
    u32 leap;
    u32 hStart;
    u32 xScale;
    u32 vCurrent;
};
struct OSViFieldRegs;
typedef struct OSViFieldRegs OSViFieldRegs;
typedef struct OSViCommonRegs OSViCommonRegs;
struct OSViFieldRegs {
    u32 origin;
    u32 yScale;
    u32 vStart;
    u32 vBurst;
    u32 vIntr;
};
struct Modep;
struct Modep {
    u8 type;
    OSViCommonRegs comRegs;
    OSViFieldRegs fldRegs[2];
};
typedef struct Modep Modep;
typedef struct __OSViScale __OSViScale;
struct __OSViContext_func_802BA910_de;
struct __OSViContext_func_802BA910_de {
    u16 state;
    u16 retraceCount;
    void *framep;
    struct Modep *modep;
    u32 control;
    void *msgq;
    void *msg;
    __OSViScale x;
    __OSViScale y;
};
typedef struct __OSViContext_func_802BA910_de __OSViContext_func_802BA910_de;
struct __OSViContext_func_802BA6B0_de;
typedef struct __OSViContext_func_802BA6B0_de __OSViContext_func_802BA6B0_de;
struct OSViMode;
struct OSViMode {
    u8 type;
    Awords comRegs;
};
struct __OSViContext_func_802BA6B0_de {
    u16 state;
    u16 retraceCount;
    void *framep;
    struct OSViMode *modep;
    u32 control;
};
typedef struct OSViMode OSViMode;
struct State_func_802BA700_de;
typedef struct State_func_802BA700_de State_func_802BA700_de;
struct State_func_802BA700_de {
    u16 status;
    u16 pad2;
    u32 word4;
    Awords *inner;
    u32 flags;
};
struct OSThread_s;
typedef struct OSThread_s OSThread_s;
struct OSThreadContext;
typedef struct OSThreadContext OSThreadContext;
struct OSThreadContext {
    u64 at;
    u64 v0;
    u64 v1;
    u64 a0;
    u64 a1;
    u64 a2;
    u64 a3;
    u64 t0;
    u64 t1;
    u64 t2;
    u64 t3;
    u64 t4;
    u64 t5;
    u64 t6;
    u64 t7;
    u64 s0;
    u64 s1;
    u64 s2;
    u64 s3;
    u64 s4;
    u64 s5;
    u64 s6;
    u64 s7;
    u64 t8;
    u64 t9;
    u64 gp;
    u64 sp;
    u64 s8;
    u64 ra;
    u64 lo;
    u64 hi;
    u32 sr;
    u32 pc;
    u32 cause;
    u32 badvaddr;
    u32 rcp;
    u32 fpcsr;
};
struct OSThread_s {
    struct OSThread_s *next;
    s32 priority;
    struct OSThread_s **queue;
    struct OSThread_s *tlnext;
    u16 state;
    u16 flags;
    s32 id;
    int fp;
    OSThreadContext context;
};
struct Quad_func_802A1BE0_de;
typedef struct Quad_func_802A1BE0_de Quad_func_802A1BE0_de;
struct Quad_func_802A1BE0_de {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
};
struct OSTimer_s;
typedef struct OSTimer_s OSTimer_s;
struct Opaque_OSMesgQueue;
typedef struct Opaque_OSMesgQueue Opaque_OSMesgQueue;
struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 value;
    Opaque_OSMesgQueue *mq;
    void * msg;
};
struct Node802C07B0;
typedef struct Node802C07B0 Node802C07B0;
struct Node802C07B0 {
    s32 field0;
    s32 field4;
    u64 field8;
    u64 field10;
    void *field18;
    void *field1C;
};
struct ThreadNode;
struct ThreadNode {
    struct ThreadNode *next;
    s32 priority;
    struct ThreadNode **queue;
    s32 unk0C;
    u16 state;
};
typedef struct ThreadNode ThreadNode;
struct NodeRecAt1C;
struct NodeRecAt1C {
    void *next;
    void *prev;
    u64 field8;
    u64 field10;
    s32 field18;
    s32 field1C;
};
typedef struct NodeRecAt1C NodeRecAt1C;
typedef struct DiskTransferInfo {
    u16 command;
    u16 mode;
    u32 block;
    s32 sector;
    u32 deviceAddress;
    u32 bmControl;
} DiskTransferInfo;
struct OSPiHandle_s;
typedef struct OSPiHandle_s OSPiHandle_s;
struct OSPiHandle_s {
    struct OSPiHandle_s *next;
    u8 type;
    u8 latency;
    u8 pageSize;
    u8 relDuration;
    u8 pulse;
    u8 domain;
    u32 baseAddress;
    u32 speed;
    u8 transferInfo[0x60];
};
struct Func802C1F10Node;
struct Func802C1F10Node {
    s32 field0;
    s32 field4;
    void *field8;
    struct Func802C1F10Node *next;
    u16 field10;
};
typedef struct Func802C1F10Node Func802C1F10Node;
typedef int (*ALDMAproc)(int addr, int len, void *state);
struct ALRawLoop;
typedef struct ALRawLoop ALRawLoop;
struct ALRawLoop {
    u32 start;
    u32 end;
    u32 count;
};
struct ALADPCMWaveInfo_func_802BE514_de;
typedef struct ALADPCMWaveInfo_func_802BE514_de ALADPCMWaveInfo_func_802BE514_de;
struct ALADPCMBook;
typedef struct ALADPCMBook ALADPCMBook;
struct ALADPCMBook {
    s32 order;
    s32 npredictors;
    s16 book[1];
};
struct ALADPCMWaveInfo_func_802BE514_de {
    void *loop;
    ALADPCMBook *book;
};
struct ALWaveTableSCode802BE0D0;
struct ALWaveTableSCode802BE0D0 {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo_func_802BE514_de adpcmWave;
    } waveInfo;
};
struct ALLoadFilter_func_802BE514_de;
struct ALLoadFilter_func_802BE514_de {
    ALFilter_s14 filter;
    void *state;
    void *lstate;
    ALRawLoop loop;
    struct ALWaveTableSCode802BE0D0 *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
typedef struct ALLoadFilter_func_802BE514_de ALLoadFilter_func_802BE514_de;
struct ALADPCMWaveInfo;
typedef struct ALADPCMWaveInfo ALADPCMWaveInfo;
struct ALADPCMloop;
typedef struct ALADPCMloop ALADPCMloop;
struct ALLoadFilter48;
typedef struct ALLoadFilter48 ALLoadFilter48;
struct ALRAWWaveInfo;
typedef struct ALRAWWaveInfo ALRAWWaveInfo;
struct ALWaveTable_s;
typedef struct ALWaveTable_s ALWaveTable_s;
typedef short ADPCM_STATE[16];
struct ALADPCMloop {
    u32 start;
    u32 end;
    u32 count;
    ADPCM_STATE state;
};
struct ALADPCMWaveInfo {
    ALADPCMloop *loop;
    ALADPCMBook *book;
};
struct ALRAWWaveInfo {
    ALRawLoop *loop;
};
struct ALWaveTable_s {
    u8 *base;
    s32 len;
    u8 type;
    u8 flags;
    union {
        ALADPCMWaveInfo adpcmWave;
        ALRAWWaveInfo rawWave;
    } waveInfo;
};
struct ALLoadFilter48 {
    ALFilter_s14 filter;
    ADPCM_STATE *state;
    ADPCM_STATE *lstate;
    ALRawLoop loop;
    ALWaveTable_s *table;
    s32 bookSize;
    void *dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
struct ALLoadFilter48_2;
typedef struct ALLoadFilter48_2 ALLoadFilter48_2;
struct ALLoadFilter48_2 {
    ALFilter_s14 filter;
    void *state;
    void *lstate;
    ALRawLoop loop;
    void *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
typedef struct DecodeNode {
    u16 first;
    u16 second;
} DecodeNode;
typedef struct DecodeSample {
    u16 tag;
    u16 value;
} DecodeSample;
typedef struct DecodePhrase {
    u8 pad0[0xA0];
    DecodeSample samples[64];
    s32 count;
    u8 pad1A4[0x14];
} DecodePhrase;
typedef struct DecodeTable {
    u8 pad0[0xE];
    u16 width;
    u8 pad10[0xC];
    DecodePhrase *phrases;
    DecodeNode *tree;
} DecodeTable;
typedef struct DecodeBlock {
    s32 kind;
    s32 start;
    s32 length;
    s32 offset;
    DecodeTable *table;
} DecodeBlock;
struct FftTables;
typedef struct FftTables FftTables;
struct FftTables {
    f32 *sines;
    f32 *cosines;
    s32 *order;
    s32 size;
};
typedef struct DecodeState {
    s32 first;
    s32 third;
    void *source;
    s32 second;
    s32 count;
    void *cursor;
} DecodeState;
struct OSIoMesg;
typedef struct OSIoMesg OSIoMesg;
struct OSIoMesgHdr;
typedef struct OSIoMesgHdr OSIoMesgHdr;
struct OSIoMesgHdr {
    u16 type;
    u8 pri;
    u8 status;
    Opaque_OSMesgQueue *retQueue;
};
struct OSIoMesg {
    OSIoMesgHdr hdr;
    void *dramAddr;
    u32 devAddr;
    u32 size;
    void *piHandle;
};
struct KeyRecAt1C;
typedef struct KeyRecAt1C KeyRecAt1C;
struct KeyRecAt1C {
    char pad0[0x1C];
    f32 time;
    char pad20[4];
};
struct PositionKey;
typedef struct PositionKey PositionKey;
struct PositionKey {
    f32 x;
    f32 y;
    f32 z;
    f32 pad;
    f32 time;
};
struct D_800C7470_Pair;
struct D_800C7470_Pair {
    f32 first;
    f32 second;
};
typedef struct D_800C7470_Pair D_800C7470_Pair;
struct State_func_8040385C_de;
typedef struct State_func_8040385C_de State_func_8040385C_de;
struct State_func_8040385C_de {
    char a[4];
    void *unk4;
    char b[0x30];
    s32 unk38;
};
struct State_func_804039C4_de;
typedef struct State_func_804039C4_de State_func_804039C4_de;
struct State_func_804039C4_de {
    s32 a;
    s32 *unk4;
};
struct EntryRecAt18V2;
struct EntryRecAt18V2 {
    char pad0[8];
    s32 flags;
    char pad0C[4];
    s8 back[4];
    char pad14[4];
    struct MenuRules *target;
    char pad1C[0xC];
};
struct MenuRecAt10;
struct MenuRecAt10 {
    s16 cursor;
    char pad2[0xA];
    struct EntryRecAt18V2 *entries;
    s32 count;
};
struct ControllerProfile;
typedef struct ControllerConnectionSample {
    u16 reserved;
    u8 connectionStatus;
    u8 flags;
} __attribute__((aligned(4))) ControllerConnectionSample;
struct ControllerProfile {
    u8 padding000[4];
    s8 port;
    u8 padding005[0x18 - 5];
    s32 inputButtons;
    ControllerConnectionSample inputConnection;
    s32 inputHistory[3];
    u8 padding02C[0xAC - 0x2C];
    s32 previousButtons;
    s32 buttons;
    ControllerConnectionSample connection;
    s32 history[2];
    u32 buttonState;
    s8 stickX;
    s8 stickY;
    s8 previousStickX;
    s8 previousStickY;
    u8 padding0C8[4];
    s32 mode;
    u8 padding0D0[0x224 - 0xD0];
};
typedef struct ControllerProfile ControllerProfile;
struct func_8022A404_S1;
struct func_8022A404_S1 {
    char pad0[0x20];
    int unk20;
};
typedef struct func_8022A404_S1 func_8022A404_S1;
struct ObjRecAt8V4;
struct ObjRecAt8V4 {
    char pad[8];
    u32 flags;
};
typedef struct ObjRecAt8V4 ObjRecAt8V4;
struct Binding;
typedef struct Binding Binding;
struct ItemRecAt14;
typedef struct ItemRecAt14 ItemRecAt14;
struct Binding {
    int pad0;
    int type;
    char pad8[16];
    void *(*storage)(void);
};
struct ItemRecAt14 {
    char pad[20];
    struct Binding *binding;
};
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
typedef struct PakMenuSlot PakMenuSlot;
struct OwnerCode8043DF84;
struct OwnerCode8043DF84 {
    char pad0[0x85C];
    s32 unk85C;
};
struct MenuRecAt20V2;
typedef struct MenuRecAt20V2 MenuRecAt20V2;
struct MenuRecAt20V2 {
    char pad0[0x1C];
    struct OwnerCode8043DF84 *owner;
    PakMenuSlot *widget;
};
struct GameSession;
struct Inner8043E56C;
struct Inner8043E56C {
    u8 pad0[4];
    s8 unk4;
};
struct Outer8043E56C;
struct Outer8043E56C {
    u8 pad0[0x20];
    struct Inner8043E56C *unk20;
};
typedef struct Outer8043E56C Outer8043E56C;
typedef struct Inner8043E56C Inner8043E56C;
struct ObjRecAtB0;
struct ObjRecAtB0 {
    char pad0[0xB0];
    s32 unkB0;
};
typedef struct ObjRecAtB0 ObjRecAtB0;
struct RecordRecAt20V1;
struct RecordRecAt20V1 {
    char pad[0x20];
    struct ObjRecAtB0 *inner;
};
struct Field_f32_10;
typedef struct Field_f32_10 Field_f32_10;
struct Field_f32_10 {
    char pad[0x10];
    f32 value;
};
struct MenuRecAt20V4;
struct MenuRecAt20V4 {
    s16 state;
    char pad2[0x1E];
    s32 input;
};
typedef struct MenuRecAt20V4 MenuRecAt20V4;
struct Triple_func_802683E0_de;
typedef struct Triple_func_802683E0_de Triple_func_802683E0_de;
struct Triple_func_802683E0_de {
    s32 x;
    f32 y;
    s32 z;
};
struct Shared_Entry;
typedef struct Shared_Entry Shared_Entry;
struct Shared_Entry {
    s32 value;
    s32 timer;
    s32 count;
    u8 text[24];
};
struct CheatCode;
struct CheatCode {
    u8 *code;
    s32 unk4;
    s32 flags;
    s32 sound;
};
typedef struct CheatCode CheatCode;
struct Func8043D458Arg;
struct Func8043D458Arg {
    s8 pad0[8];
    s32 flags;
    s8 padC[8];
    s32 *text;
};
struct StateRecAt14;
struct StateRecAt14 {
    char a[8];
    s32 unk8;
    char b[8];
    char *unk14;
};
typedef struct StateRecAt14 StateRecAt14;
struct MenuItemRecAt14;
struct MenuItemRecAt14 {
    char pad0[8];
    u32 flags;
    char padC[8];
    void *table;
};
typedef struct MenuItemRecAt14 MenuItemRecAt14;
struct FieldRecAt14;
struct FieldRecAt14 {
    char pad[0x14];
    char *text;
};
struct MenuRecAt20V1;
struct MenuRecAt20V1 {
    s16 state;
    char pad2[0x1A];
    s32 id;
    s32 input;
};
struct Spinner;
typedef struct Spinner Spinner;
struct Spinner {
    s32 value;
    f32 timer;
    s32 max;
    char padC[0xC];
};
typedef struct MenuRecAt20V1 MenuRecAt20V1;
struct ObjRecAt5DC;
typedef struct ObjRecAt5DC ObjRecAt5DC;
struct ObjRecAt5DC {
    char pad0[0x5DC];
    char * unk5DC;
};
struct Shared_PakMenu;
struct PakNotesMenuItem {
    char pad0[8];
    u32 flags;
    char padC[0x1C];
};
typedef struct Shared_PakMenu {
    s16 state;
    char pad02[0xA];
    struct PakNotesMenuItem *items;
    char pad10[4];
    char *dialogData;
    char pad18[4];
    ObjRecAt5DC *player;
    union {
        PakMenuSlot *owner;
        PakMenuSlot *slot;
    };
    void *unk24;
} Shared_PakMenu;
struct MenuRecAt24;
typedef struct MenuRecAt24 MenuRecAt24;
struct Messages;
struct Messages {
    char pad0[0x554];
    char text[1];
};
struct PlayerRecAt5EC;
struct PlayerRecAt5EC {
    char pad0[0x5DC];
    struct Messages *messages;
    char pad5E0[0xC];
    s32 menu;
};
struct MenuRecAt24 {
    char pad0[0x1C];
    struct PlayerRecAt5EC *player;
    PakMenuSlot *slot;
    char *prompt;
};
typedef struct PlayerRecAt5EC PlayerRecAt5EC;
struct PakNotesMenuItem;
typedef struct PakNotesMenuItem PakNotesMenuItem;
struct RecordRecAt24;
struct RecordRecAt24 {
    char pad[0x1C];
    s32 first;
    s32 second;
    void *resource;
};
struct Target_func_8040A77C_de;
struct Target_func_8040A77C_de {
    char pad0[0xA8];
    s32 first;
    char padAC[0xD0 - 0xAC];
    s32 second;
};
struct RecordRecAtCV1;
struct RecordRecAtCV1 {
    char pad[0xC];
    struct Target_func_8040A77C_de *target;
};
struct ItemRecAt24;
typedef struct ItemRecAt24 ItemRecAt24;
struct ItemRecAt24 {
    char pad[0x1C];
    int x;
    int y;
    char *label;
};
struct Shape_typemap_165;
struct Shape_typemap_165 {
    union { int field_0; struct Pack *pack; void *data; };
    int field_4;
    int field_8;
    int field_C;
};
struct Entry_func_804053E0_de;
struct Entry_func_804053E0_de {
    s32 size;
    char data[28];
};
struct RecordRecAt4V2;
struct RecordRecAt4V2 {
    s32 header;
    struct Entry_func_804053E0_de entries[16];
};
struct OSPfs;
struct OSPfs {
    int status;
    struct OSMesgQueue *queue;
    int channel;
    u8 id[32];
    u8 label[32];
    int version;
    int dir_size;
    int inode_table;
    int minode_table;
    int dir_table;
    int inode_start_page;
    u8 banks;
    u8 activebank;
};
typedef struct OSPfs OSPfs;
struct __OSInode;
typedef struct __OSInode __OSInode;
union __OSInodeUnit;
union __OSInodeUnit {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
};
typedef union __OSInodeUnit __OSInodeUnit;
struct __OSInode {
    __OSInodeUnit inode_page[128];
};
struct __OSDir;
typedef struct __OSDir __OSDir;
struct __OSDir {
    u32 game_code;
    u16 company_code;
    __OSInodeUnit start_page;
    u8 status;
    s8 reserved;
    u16 data_sum;
    u8 ext_name[4];
    u8 game_name[16];
};
struct NoteState;
struct NoteState {
    u32 file_size;
    u32 game_code;
    u16 company_code;
    char ext_name[4];
    char game_name[16];
    char pad[2];
};
typedef struct NoteState NoteState;
struct __OSContRequesFormatShort;
typedef struct __OSContRequesFormatShort __OSContRequesFormatShort;
struct __OSContRequesFormatShort {
    u8 txsize;
    u8 rxsize;
    u8 cmd;
    u8 typeh;
    u8 typel;
    u8 status;
};
struct __OSInodeCache;
struct __OSInodeCache {
    __OSInode inode;
    u8 bank;
    u8 map[256];
};
typedef struct __OSInodeCache __OSInodeCache;
struct GroupCode8044E2B8;
typedef struct GroupCode8044E2B8 GroupCode8044E2B8;
struct Piece;
typedef struct Piece Piece;
struct Piece {
    char pad0[0x14];
    s32 tag;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
};
struct GroupCode8044E2B8 {
    s32 unk0;
    s32 count;
    Piece pieces[1];
};
struct RecordRecAt1C;
struct RecordRecAt1C {
    s32 a;
    s32 b;
    s32 c;
    s32 values[3];
    s32 d;
    s32 e;
};

#endif
