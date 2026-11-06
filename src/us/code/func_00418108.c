#include "common.h"

extern s32 D_8011AB8B;

s32 PlayerSetupMenu_NameLabel(void *arg0, void *arg1)
{
  *((void **) (((s8 *) arg0) + 0x14)) = (void *) (((*((s8 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x20)))) + 4))) << 6) + ((s8 *) &D_8011AB8B));
  return 0;
}
