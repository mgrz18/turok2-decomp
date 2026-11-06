#include "common.h"

void Controller_UnwatchPak(s32);
s32 CEngineApp__SetupFadeTo(s32, s32);

extern s32 gEngineApp;
extern s32 D_801308F8;

s32 SingleMenu_GoBack_2(s32 arg0, void *arg1)
{
  s8 *new_var2;
  int new_var;
  CEngineApp__SetupFadeTo((s32) (&gEngineApp), 8);
  new_var2 = (s8 *) (*((void **) (((s8 *) arg1) + 0x20)));
  D_801308F8 = 0;
  new_var = 1;
  Controller_UnwatchPak((s32) (*((s8 *) (new_var2 + 4))));
  return new_var;
}
