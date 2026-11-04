#include "common.h"

typedef struct {
    /* 0x00 */ u16 vals[4];
    /* 0x08 */ u8 pad8[0x30 - 0x8];
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 pad31[2];
    /* 0x33 */ u8 bytes[8];
} Settings;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ Settings *settings;
} Menu;

extern s32 D_80130970;          /* preceded by 4 u16s at D_80130968 */
extern u8 D_80130974[];

/* Copies the saved values into the settings record of the menu. */
void func_004214C4(Menu *arg0)
{
  s32 var_a0;
  u8 temp_v0;
  Settings *temp_a1;
  Settings *var_v1;
  temp_a1 = arg0->settings;
  temp_a1->unk30 = *(u8 *) &D_80130970;
  temp_a1->vals[0] = ((u16 *) &D_80130970)[-4];
  temp_a1->vals[1] = ((u16 *) &D_80130970)[-3];
  temp_a1->vals[2] = ((u16 *) &D_80130970)[-2];
  var_a0 = 0;
  temp_a1->vals[3] = ((u16 *) &D_80130970)[-1];
  var_v1 = temp_a1;
  do
  {
    temp_v0 = D_80130974[var_a0];
    var_v1 = (Settings *) ((u8 *) temp_a1 + var_a0);
    var_a0 += 1;
    var_v1->bytes[0] = temp_v0;
  }
  while (var_a0 < 8);
}
