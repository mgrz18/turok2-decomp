/* Types of the functions taken from thejunkman9000/Turok-Rage-Wars-Decomp (tools/import_ref.py),
 * gathered in one place by tools/centralize_types.py. Their names are that
 * decomp's own. */

#ifndef RW_JUNK_TYPES_H
#define RW_JUNK_TYPES_H

/* The shared types come from the shared headers: libultra's from
 * ultra_types.h, CVector3 (Rage Wars' Vec3f, M04Vec3, B19Vec3, O01Vec3) from
 * structs/vector.h, libaudio's ALHeap/ALFilter from structs/libaudio.h. The
 * sound manager types (Rage Wars' SndMgr, SndVoice, SndSeq, SndLock and the
 * M04Emitter/M04VP/M04Ctx views of them) were the same layouts as
 * structs/sound.h's SndMgr, SndVoice, SndVoiceParams, SndSeq and CMutex: the
 * files that used them include structs/sound.h now. */
#include "common.h"
#include "ultra_types.h"
#include "structs/vector.h"
#include "structs/libaudio.h"
#include "structs/list.h" /* CList (Rage Wars' B12List) */

typedef struct B12Node B12Node;
struct B12Node {
               u32 key;
               s32 unk4;
               s32 refs;
               u32 flags;
               u32 unk10;
               B12Node *link;
               B12Node *link18;
               u8 pad1C[4];
               B12Node *prev;
               B12Node *next;
};
typedef struct B12Bucket B12Bucket;
struct B12Bucket {
              u32 unk0;
              u32 unk4;
              s32 id;
              B12Bucket *next;
};
typedef struct B12Mgr {
                u8 pad0[0x5F0];
                B12Node * nodes;
                B12Node ** stk;
                B12Node * cur;
                u8 * pool2;
                CList lists[2];
                s32 tbl628[2];
                s32 tbl630[2];
                s32 f638;
                u8 pad63C[0x4];
                OSThread thread;
                u8 pad7F0[0x80];
                OSMesgQueue q870;
                OSMesg buf870[0x40];
                u8 stack[0x800];
                 CList lists3[3];
                 s32 tbl11C4[2];
                 s32 cnt;
                 CMutex lock;          /* 0x11D0 (ultra_types.h) */
                 OSMesgQueue q11F0;
                 OSMesg buf11F0;
                 s32 f120C;
                 u32 stamp;
                 u8 pad1214[0xC];
                 u32 hashMask;
                 B12Bucket * buckets;
} B12Mgr;
typedef struct B12Msg {
    B12Node *node;
    u8 pad4[0xC];
    u32 flags;
} B12Msg;
typedef struct R05Anim {
               f32 time;
               s16 f4;
               s16 f6;
               s16 count;
               u8 padA[2];
               u32 loopStart;
               void **data;
} R05Anim;
typedef struct R05CurveCfg {
               u8 pad0[0x18];
               f32 n;
               f32 scale;
} R05CurveCfg;
typedef struct R05Vec3i { s32 x, y, z; } R05Vec3i;
typedef struct B14Trk {
               s32 bits;
               f32 base;
               f32 range;
               u32 data;
               f32 c0;
               f32 c1;
               f32 c2;
               f32 c3;
               u32 span;
               u32 start;
               s32 cur;
               s32 f2C;
} B14Trk;
typedef struct { f32 c0, c1, c2, c3; u32 span; } B14Cubic;
typedef struct { u32 bits; f32 min; f32 range; } P3QInfo;
typedef struct { u32 pos; P3QInfo q; } P3QArr;
typedef struct B14FRange { u32 n; f32 min; f32 range; } B14FRange;
typedef struct B14FieldRd { u32 addr; B14FRange r; } B14FieldRd;
typedef struct { u32 pad; B14FRange r; } B14Tmp;
typedef struct { s32 f0; s32 f4; s32 f8; f32 fC; s32 f10; s32 f14; } B14Hdr;
typedef struct DLNode {
                struct DLNode *prev;
                struct DLNode *next;
} DLNode;
typedef struct DLList {
                DLNode *head;
                DLNode *tail;
                s32 count;
} DLList;
typedef struct NodePool {
                DLList freel;
                DLList usedl;
} NodePool;
typedef struct B19Spawn B19Spawn;
typedef struct B19PNode B19PNode;
typedef struct B19Pool B19Pool;
struct B19PNode {
              u8 pad0[0x4];
              B19PNode * next;
};
struct B19Pool {
              u8 pad0[0x14];
               B19PNode * head;
               u8 pad18[0x10];
               s32 count;
};
struct B19Spawn {
              u8 pad0[0x4];
              B19Spawn * next;
              void * src;
              CVector3 pos;
               f32 f18;
               f32 f1C;
               s32 h20;
               u8 pad24[0x8];
               s32 h2C;
               u8 pad30[0x8];
               B19Spawn ** link;
};
typedef struct B21Plane {
    f32 x, y, z, w;
} B21Plane;
typedef u32 OSEvent;
typedef struct B12AiCfg {
    u32 unk0;
    u32 unk4;
    s32 unk8;
} B12AiCfg;
typedef struct B12AiTask {
               s32 *cmd;
               u8 pad4[0x14];
               s32 f18;
               s32 f1C;
               u8 *f20;
               s32 f24;
               u8 *f28;
               s32 f2C;
               u8 *f30;
               s32 f34;
               s32 f38;
               s32 f3C;
               s32 f40;
               s32 f44;
               u8 pad48[8];
               s32 f50;
               s32 f54;
               OSMesgQueue *f58;
               struct B12AiTask *f5C;
               u8 pad60[0x10];
} B12AiTask;
typedef struct B12DbEnt2 {
    ALLink link;
    s32 f8;
    s32 fC;
    void *data;
} B12DbEnt2;
typedef struct M04SortTab {
               u8 pad0[0xE];
               s16 n;
               u32 v[1];
} M04SortTab;
/* The sound manager seen from its bank pointer (SndMgr.bank, 0x7C): snd is
 * SndMgr.handles. Kept as a view because addressing the handles through it
 * is what the ROM does (SndVoicePool_FindSlot, SndVoicePool_ReleaseVoice). */
typedef struct SndCore {
                 u8 pad0[0x60];
                 s16 snd[16];
} SndCore;
typedef struct M04Bank2 {
    u8 pad0[0xC];
    M04SortTab *tab;
} M04Bank2;
typedef struct SndNodeRec { s32 w0; u16 h4; } SndNodeRec;
typedef struct AtlasCell {
    u32 w;
    u32 h;
} AtlasCell;
typedef struct ObjInit {
    s32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C;
    union { s32 i; f32 f; } unk20;
    CVector3 v;
    f32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
} ObjInit;
typedef struct DlCmd {
    u32 w0;
    u32 w1;
} DlCmd;
typedef struct B21Vp {
    s16 vscale[4];
    s16 vtrans[4];
} B21Vp;
typedef struct B21Hdr {
    u8 pad0[0x114];
    DlCmd *unk114;
} B21Hdr;
typedef struct B09LNode B09LNode;
struct B09LNode {
               B09LNode *next;
               B09LNode *prev;
               u16 slot;
               u8 id;
               u8 tlb;
               s32 unkC;
};
typedef struct B09SlotA {
            u16 key;
            u8 a;
            u8 b;
} B09SlotA;
typedef struct B09SlotCtl {
            u16 cur;
            u16 start;
} B09SlotCtl;
typedef struct {
    s64 quot;
    s64 rem;
} lldiv_t;
typedef struct O01SplRec {
               f32 x, y, z;
               f32 cx, cy, cz;
               f32 arc;
               f32 t;
               f32 c2;
} O01SplRec;
typedef struct O01Pad4 {
            u8 pad0[3];
            u8 unk3;
} O01Pad4;
typedef struct S7Light {
               u8 pad0[0x14];
               CVector3 dir;
               s32 enabled;
} S7Light;
typedef struct S5CtlEnt {
            u8 a;
            u8 b;
            u8 c;
            u8 d;
} S5CtlEnt;
typedef struct S5Ctl {
                s32 f0;
                s8 port;
                u8 pad5[3];
                s32 f8;
                s32 fC;
                s32 f10;
                s32 f14;
                s32 f18;
                s32 f1C;
                s32 f20;
                s32 f24;
                s32 f28;
                S5CtlEnt ent[32];
                u8 padAC[0xC4 - 0xAC];
                u8 fC4;
                u8 fC5;
                u8 fC6;
                u8 fC7;
                s32 fC8;
                s32 fCC;
                s32 fD0;
                s32 fD4;
                u8 padD8[0x140 - 0xD8];
                u8 blkA[0x2C];
                u8 blkB[0xB4];
                s32 f220;
} S5Ctl;
typedef struct S5PortFlags {
            u8 b[4];
} S5PortFlags;

#endif
