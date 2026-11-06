#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern char D_800B40B0[];
extern char D_800B40C8[];
extern CPlayerOptions gEngineApp_m_Settings_defaultSlot;

/* The weapon select item's text: quick or normal. */
s32 OptionsMenu_WeaponSelectText(MenuItem *item, Menu *menu)
{
  char *quick;
  CPlayerOptions *settings;
  u8 *select;
  CPlayer *player;
  player = menu->player;
  quick = D_800B40B0;
  settings = &gEngineApp_m_Settings_defaultSlot;
  if (player != 0)
  {
    settings = player->m_pOptions;
  }
  select = &settings->m_WeaponSelect;
  if (*select == 1)
  {
    item->text = quick;
  }
  else
  {
    item->text = D_800B40C8;
  }
  return 0;
}
