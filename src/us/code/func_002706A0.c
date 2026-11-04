/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00257814.c, func_00257814.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef union { s32 i; f32 f; } IF32;
typedef struct SndEnv SndEnv;
typedef struct SndSeq SndSeq;
typedef struct SndFx SndFx;
typedef struct SndLink SndLink;
typedef struct SndMgr SndMgr;
typedef struct SndVoice SndVoice;
typedef struct SndNode SndNode;
typedef struct SndQueue SndQueue;
typedef struct SndVoices SndVoices;
typedef struct SndLock SndLock;
struct SndLink {
                 SndLink* prev;
                 SndLink* next;
};
struct SndEnv {
                 u16 w0;
                 u16 a;
                 u16 b;
                 s16 c;
                 u16 w8;
                 u16 wA;
};
struct SndVoice {
                 s32 idx;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 s32 unk10;
                 s32 unk14;
                 s32 unk18;
                 s32 unk1C;
                 s16 unk20;
                 u8 pad22[0x2];
                 s32 unk24;
                 s16 pan;
                 u8 pad2A[0x2];
                 f32 unk2C;
                 u8 pad30[0x4];
                 f32 unk34;
                 s16 unk38;
                 s16 unk3A;
                 s16 unk3C;
                 u8 pad3E[0x2];
                 s32 unk40;
                 Vec3f pos;
                 s32 unk50;
                 u8 unk54;
                 u8 pad55[0x3];
                 s32 unk58;
                 s32 unk5C;
                 u8 pad60[0x28];
                 s32 unk88;
                 SndEnv env;
                 f32 unk98;
                 f32 unk9C;
                 s32 unkA0;
                 s32 unkA4;
                 s32 tag;
                 s32 unkAC;
                 SndMgr* mgr;
                 SndMgr* unkB4;
                 f32 unkB8;
                 s32 unkBC;
                 s32 unkC0;
                 s32 unkC4;
                 u8 padC8[0x4];
};
struct SndNode {
                 SndLink link;
                 SndVoice body;
};
struct SndQueue {
                 SndMgr* owner;
                 SndNode active;
                 SndNode free;
                 SndNode nodes[32];
};
struct SndVoices {
                 SndMgr* owner;
                 SndVoice v[17];
};
struct SndLock {
                 OSMesgQueue q;
                 u8 pad18[0x4];
                 s32 count;
};
struct SndSeq {
                 SndMgr* owner;
                 s32 unk4;
                 s32* unk8;
                 s32 unkC;
                 void* unk10;
                 void* csp;
                 s32 unk18;
                 s32 unk1C;
                 s32 unk20;
                 s32 unk24;
                 s32 unk28;
                 f32 vol;
                 f32 unk30;
                 f32 unk34;
                 s32 unk38;
                 f32 unk3C;
                 IF32 unk40;
};
struct SndFx {
                 SndMgr* owner;
                 s32 unk4;
                 s32 unk8;
                 s32 unkC;
                 u8 pad10[0x4];
                 s32 unk14;
                 s32 unk18;
                 s32 sndId;
                 u8 pad20[0x20];
};
struct SndMgr {
                 u8 pad0[0x7C];
                 s32* unk7C;
                 u8 pad80[0x4];
                 u8 player[84];
                 u8 padD8[0x4];
                 s16 snd[16];
                 u8 padFC[0x6];
                 s16 unk102;
                 s32 unk104;
                 s32 unk108;
                 s32 unk10C;
                 SndLock lock;
                 s32 unk130;
                 s32 unk134;
                 SndQueue q;
                 SndSeq seq;
                 u8 heap[16];
                 SndVoices pool;
                 void* root2B48;
                 void* tree2B4C;
                 void* node2B50;
                 s32 unk2B54;
                 void* unk2B58;
                 s32* unk2B5C;
                 void* unk2B60;
                 s32 unk2B64;
                 u8 pad2B68[0x8];
                 u8* tab70;
                 u8* tab74;
                 u8* tab78;
                 s32 unk2B7C;
                 s32 unk2B80;
                 s32 unk2B84;
                 void* unk2B88;
                 s16 unk2B8C;
                 u8 pad2B8E[0x2];
                 s32 unk2B90;
                 u8 unk2B94;
                 u8 pad2B95[0x3];
                 s32 unk2B98;
                 s32 unk2B9C;
                 f32 unk2BA0;
                 f32 unk2BA4;
                 f32 unk2BA8;
                 s32 unk2BAC;
                 s32 unk2BB0;
                 s32 unk2BB4;
                 s32 unk2BB8;
                 u8 pad2BBC[0x4];
                 SndFx obj2BC0;
};
typedef struct M04Vec3 {
    f32 x, y, z;
} M04Vec3;
typedef union M04IF {
    s32 i;
    f32 f;
} M04IF;
typedef struct M04VP {
               s32 w0;
               s32 w4;
               s32 vol;
               s32 wC;
               s16 s10;
               u8 pad12[2];
               M04IF v14;
               s16 pan;
               u8 pad1A[2];
               f32 scale;
               f32 f20;
               f32 f24;
               s16 s28;
               s16 s2A;
               s16 s2C;
               u8 pad2E[6];
               M04Vec3 pos;
               s32 f40;
               s8 mode;
               u8 pad45[3];
               f32 f48;
               f32 f4C;
} M04VP;
extern f32 D_800A80C8;
void func_00293DE0(void *p, s32 pitch);
void func_00293D40(void *p, s32 mix);
void func_00293D90(void *p, u8 pan);
void func_00293E70(void *p, s16 vol);
void func_002764C4(void *q, s32 a, s32 id);
s32 func_00273B90(void *pool, s16 idx, s32 id);
void func_00273BF8(void *pool, s16 idx);
static __inline__ s32 find17(SndMgr *m, s32 id) {
    s32 i;
    SndVoice *vv;
    for (vv = m->pool.v, i = 0; i < 17; i++, vv++) {
        if (vv->unkC == id) {
            return i;
        }
    }
    return -1;
}
s32 func_002706A0(SndMgr *m, s32 id)
{
  s16 idx;
  M04VP *p;
  SndVoice *vs;
  u8 *pl;
  s32 mix;
  f32 f;
  u8 *new_var;
  if (id == (-1))
  {
    return -1;
  }
  idx = find17(m, id);
  if (((s16) idx) == (-1))
  {
    return -1;
  }
  if (((s16) idx) == 16)
  {
    func_002764C4(&m->q, 16, id);
    return -1;
  }
  p = (M04VP *) ((u8 *)((((s16) idx) * 0xCC) + (u32) m) + 0x1DCC);
  pl = m->player;
  if (p->vol == 0)
  {
    if (!((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60))) & 4))
    {
      *((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60)) |= 8;
      m->unk102 = -1;
      return 0;
    }
  }
  if (func_00273B90(&m->pool, idx, id) == 0)
  {
    func_00293DE0(pl, p->v14.i);
    mix = ((p->s10 + m->unk2B9C) < 0x80) ? ((((u8 *) p)[0x11] + (*(((u8 *) m) + 0x2B9F))) & 0xFF) : (0x7F);
    func_00293D40(pl, mix);
    func_00293D90(pl, ((u8 *) (&p->pan))[1]);
    if ((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E60))) & 0x200)
    {
      f = m->unk2BA8;
    }
    else
    {
      f = m->unk2BA0;
      if (((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E80))) != 0) && (m->unk2BB8 != 0))
      {
        if (m->unk2BA0)
        {
          f *= D_800A80C8;
        }
        else
        {
          f *= D_800A80C8;
        }
      }
    }
    func_00293E70(pl, (s16) ((s32) ((((f32) p->vol) * f) * p->scale)));
    if ((*((u32 *) ((((u8 *) m) - (-(((s16) idx) * 0xCC))) + 0x1E5C))) == 0)
    {
      func_00273BF8(&m->pool, idx);
    }
  }
  m->unk102 = -1;
  return 0;
}
