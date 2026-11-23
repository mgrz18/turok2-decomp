#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sOptionsMenuText_WeaponSelectQuick[];
extern char sOptionsMenuText_WeaponSelectNorm[];

/* The weapon select item's text: quick or normal. */
s32 OptionsMenu_WeaponSelectText(MenuItem *item, Menu *menu)
{
  char *quick;
  CPlayerOptions *settings;
  u8 *select;
  CPlayer *player;
  player = menu->player;
  quick = sOptionsMenuText_WeaponSelectQuick;
  settings = &gEngineApp_m_Settings.defaultSlot;
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
    item->text = sOptionsMenuText_WeaponSelectNorm;
  }
  return 0;
}
