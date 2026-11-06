/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002917E8.c, func_002917E8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "structs/frame.h"
#include "globals/engine.h"

extern DlCmd *gDList;
extern u8 gRspInitDL[];
extern u8 gRdpInitDL[];
extern s32 gDepthBuffer;
extern B21Vp gScreenViewport[];
extern s32 gScreenWidth;
extern s32 gScreenHeight;
void CEngineApp__BeginDisplayList(void)
{
  f32 new_var;
  s32 new_var2;
  s32 new_var3;
  DlCmd *d1;
  DlCmd *d2;
  DlCmd *d3;
  DlCmd *d4;
  DlCmd *d5;
  DlCmd *d6;
  B21Vp *vp;
  gDList = gEngineApp_m_pCurrentFrameData->m_pDisplayList;
  d1 = gDList++;
  d1->w0 = 0xDB060000;
  d1->w1 = 0;
  d2 = gDList++;
  d2->w0 = 0xDE000000;
  d2->w1 = (u32) gRspInitDL;
  d3 = gDList++;
  d3->w0 = 0xDE000000;
  d3->w1 = (u32) gRdpInitDL;
  d4 = gDList++;
  d4->w0 = 0xFE000000;
  vp = &gScreenViewport[gEvenOdd];
  d4->w1 = gDepthBuffer;
  vp->vscale[1] = gScreenHeight * 2;
  vp->vscale[0] = gScreenWidth * 2;
  vp->vscale[2] = 0x3FF;
  vp->vtrans[1] = gScreenHeight * 2;
  vp->vscale[2] = 0x3FF;
  vp->vscale[3] = 0;
  vp->vtrans[0] = gScreenWidth * 2;
  vp->vtrans[2] = 0;
  vp->vtrans[3] = 0;
  new_var = (f32) (gScreenWidth - 1);
  d5 = gDList++;
  d5->w0 = 0xDC080008;
  d5->w1 = (u32) vp;
  d6 = gDList++;
  d6->w0 = 0xED000000;
  d6->w1 = ((((s32) (new_var * 4.0f)) & 0xFFF) << 12) | (((s32) (((f32) (gScreenHeight - 1)) * 4.0f)) & 0xFFF);
}
