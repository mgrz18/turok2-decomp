/* FAKEMATCH: a dead per-mode switch before the remap test, and opt pinned to $s1 (see below). */
/* Builds the per-frame player input from the controller: optional button
 * remapping, then turn/look/strafe/move axes per control mode (Rage Wars has the
 * same code: func_802631B0_de in AbuN64/ragewars-decomp src/FX/802631D0.CPP).
 *
 * The ROM loads opt->m_ControlType once more than the remap test needs
 * (`lb $3, 0x2B($17)`, dead, before the live one). A switch on the mode whose
 * result the remap test overwrites leaves that load: its branches only go in
 * the delay-slot pass, after scheduling. It has to be a switch (beqz/beq on
 * the loaded value, so the load sits in $v1 beside `li $v0, 1` and right
 * before the deleted branch); a dead `if (mode < 2)` or a second call of the
 * remap test puts it in the wrong slot, and an empty if or a discarded call
 * leaves nothing. The ROM also keeps opt in a saved register though nothing
 * after the calls reads it; unpinned, opt lands in a temporary and every
 * access to it differs. */
#include "common.h"
#include "structs/player.h"
#include "globals/engine.h"
#include "globals/player.h"
#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "structs/camera.h"
#include "gfx.h"
#include "structs/vector.h"
#include "structs/game.h"
#include "globals/render.h"
#include "structs/weapon.h"
#include "globals/camera.h"
#include "globals/strings.h"
/* The cache calls of the module, as protos/cache.h declares them (the header itself is not included: its
 * CCache__Flush(void) against callers that pass the heap) */
void CCache__LockForDisplayList(s32, struct B12Node *);
struct CMemEntry *CCache__Request(s32, uptr, u32, u32, sptr, void *, char *, u32);
void CCache__SetFrameDelay(s32);
void CCache__Unlock(s32, struct CMemEntry *);
#include "structs/collision.h"
#include "structs/region.h"
#include "structs/particle.h"
#include "protos/player.h"
#include "structs/game_object.h"
#include "structs/light.h"
#include "protos/game.h"
/* CPlayer code: CPlayer__SetupActors, 0x0042DA60-0x0042DC18 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "structs/intelligence.h"
#include "protos/ai.h"
#include "structs/controller.h"
/* Player code: CPlayer__NewGame, 0x0042DC18-0x0042DE74 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "globals/menu_desc.h"
#include "protos/menu.h"
/* CPlayer code: CPlayer__LevelStart, 0x0042DE74-0x0042E14C (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "globals/particle.h"
/* CPlayer code: CPlayer__StartGame, 0x0042E14C-0x0042E29C (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayer__LoseLife, 0x0042E29C-0x0042E430 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "structs/world.h"
#include "gbi_t2.h"
/* Player_FindAutoAimTarget and Player_UpdateAim, one source file: the file's .rodata
 * opens with Player_FindAutoAimTarget's literals at 0x800A6AF0, and Player_UpdateAim's
 * jump table at 0x800A6B28 is 8-aligned from there (from its own start,
 * 0x800A6B0C, it would not be). */
#include "common.h"
#include "structs/player_options.h"
#include "globals/math.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217928_de, with Turok 2's layouts. */

#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217B3C_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_80217D74_de, with Turok 2's layouts. */

#include "common.h"
#include "globals/menu.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_802181FC_de, with Turok 2's layouts. */

#include "common.h"
/* One source file from CBarrel__OpenWheel to CPlayer__Update (at least: the file
 * starts earlier, on a 16-byte boundary in .text, but this is the shortest
 * run whose .rodata starts 8-aligned). CPlayer__Update's pool
 * starts at 0x800A6CFC, 4 past an 8-byte boundary, and its 2^32 double is
 * 8-aligned only counted from 0x800A6C70, where CBarrel__OpenWheel's pool opens;
 * CPlayer__DrawHud's (0x800A6C74) and CPlayer__UpdateOrbitCamera's (0x800A6CDC) are 4 past a
 * boundary too, so neither can open the file.
 *
 * The parts were written apart; the first three use structs/player.h's
 * CPlayer, CPlayer__Update keeps its own Player view (see there), and they
 * share one WeaponInfo. CPlayer__Update is after Rage Wars (func_80220ED4_de). */
#include "common.h"
#include "globals/collision.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_11.CPP, func_802233F0_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_12.CPP, func_802238E0_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_15.CPP, func_80224408_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_16.CPP, func_8022470C_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_17.CPP, func_80224C4C_de, with Turok 2's layouts. */
#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225D10.c, func_00225D10, with Turok 2's layouts (names from AbuN64/ragewars-decomp, WEAPONS/80225D10.CPP). */
/* A player travelling to a target: turn toward it and follow a parabola. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00225F20.c, func_00225F20, with Turok 2's layouts. */
#include "common.h"
/* CPlayer code: CPlayer__SetCharacter, 0x0042E430-0x0042E5AC (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayerPool__NewGame, 0x0042E5AC-0x0042E834 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
#include "protos/list.h"
#include "protos/iset.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_9.CPP, func_8022A170_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
/* The port: the same through CPlayerPool / CPlayer / CScene (host layouts). */
#include "structs/player.h"
#include "structs/player_list.h"
/* Player code: CPlayerPool__Construct, 0x0042E834-0x0042E894 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayerPool__LevelStart, 0x0042E894-0x0042E91C (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayer__Construct, 0x0042E91C-0x0042E9A4 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayer__Activate, 0x0042E9A4-0x0042EA00 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* CPlayer code: CPlayer__ResetHealth, 0x0042EA00-0x0042EB04 (splat had it inside the data at 0x0042D50C). */
#include "common.h"
/* The port: the view put the active list head at N64 0x20 and the player's controller and link at
 * 0xA50/0x1320; the typed CPlayerPool/CPlayer (structs/player_list.h, structs/player.h) at their host
 * offsets. GameNoteMenu_SelectYes (loading a save) crashed on the NULL it returned. */
#include "common.h"
/* Cut fixed in versions/cuts.us.txt: CPlayer__GetFiringAmmo really is 0x25398C-0x253A1C (0x90 bytes): splat cut it at 0x2539DC (.L002539DC), the loop that follows is its body. */
#include "common.h"
/* Cut fixed in versions/cuts.us.txt: CPlayer__GetFiringAmmoType really is 0x253A1C-0x253AC0 (0xA4 bytes): splat split off its tail (return *ammo) as func_00253AB4. */
#include "common.h"
/* Cut fixed in versions/cuts.us.txt: CPlayer__GetMaxAmmo really is 0x253AC0-0x253B54 (0x94 bytes, with the trailing nop): splat split it at its loop head as func_00253AD4. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/80225D10_10.CPP, func_8022ADBC_de, with Turok 2's layouts. */

#include "structs/game_object.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022ADF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/GFX/TEXT/8022BA90_3.CPP, func_8022C2B4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/render.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/TEAM/80217388_7.CPP, func_8021836C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/player.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022BAA0_de_4.c, func_8022C67C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894.CPP, func_8022CC34_de, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894.CPP, func_8022CE78_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894_2.CPP, func_8022D040_de, with Turok 2's layouts. */
#include "common.h"
/* Menu state: the current player pointer is the word at 0x20 (gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead). */
#include "globals/menu_desc.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_4.CPP, func_8022DB04_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_5.CPP, func_8022E0B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E120.c, func_0022E120.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* Cut fixed in versions/cuts.us.txt: PickupCounter_Dec really is 0x256AF0-0x256B34 (0x44 bytes, like its twin PickupCounter_Inc): splat cut its low-nibble return off as func_00256B24 (whose C in src/ is a fake for that tail). */
#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E77C.c, func_0022E77C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* The port: on the scene's CDLightSet and CDLight members (the N64 offsets 0xC..0x16 of the
 * light are 0x18..0x22 on the host); the ROM light is a pointer. */
#include "structs/particle.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022E948_de.c, func_8022EA3C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8022E948_de.c, func_8022EF04_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* CPlayer code: CPlayer__StopLoopingSounds, 0x0042EB04-0x0042EB4C (splat had it inside the data at 0x0042D50C). */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xB0];
    /* 0xB0 */ u32 buttons[5];
    /* 0xC4 */ u8 padC4[2];
    /* 0xC6 */ s8 stickX;
    /* 0xC7 */ s8 stickY;
} Controller;
typedef struct {
    /* 0x00 */ f32 turn;
    /* 0x04 */ f32 look;
    /* 0x08 */ f32 strafe;
    /* 0x0C */ f32 move;
    /* 0x10 */ Controller *ctrl;
    /* 0x14 */ f32 outTurn;
    /* 0x18 */ f32 outLook;
    /* 0x1C */ f32 outStrafe;
    /* 0x20 */ f32 outMove;
    /* 0x24 */ u32 buttons[5];
} PlayerInput;
f32 Math_DecayToZero(f32 value, f32 rate);
#define ABS(x) ((x) < 0.0f ? -(x) : (x))

/* The remap table applies to control modes 0 and 1 with remapping on. */
void CPlayer__ReadInput(PlayerInput *in, CPlayer *pl) {
    register CPlayerOptions *opt asm("$17");
    Controller *ctrl;
    ButtonMap *map;
    f32 damp;
    f32 rate;
    f32 d;
    f32 x;
    u32 b;
    u32 mask;
    u32 bits;

    opt = pl->m_pOptions;
    damp = 1.0f - pl->m_ZoomBlend * 0.99f;
    rate = opt->m_SettingY * (1.0f / 128) - 1.0f;
    rate = rate * 0.75f + 1.0f;
    rate = rate * (gFrameIncrement * (damp * 0.1f));
    ctrl = in->ctrl;
    switch (opt->m_ControlType) {
    case 0:
        break;
    case 1:
        map = NULL;
        break;
    }
    map = NULL;
    if (opt->m_ControlType < 2) {
        if (opt->m_ControlType >= 0) {
            if (opt->m_ControlSide == 1) {
                map = sControlSide1ButtonMap;
            }
        }
    }
    if (map != NULL) {
        in->buttons[0] = 0;
        in->buttons[1] = 0;
        in->buttons[2] = 0;
        in->buttons[3] = 0;
        in->buttons[4] = 0;
        while (map->mask != 0) {
            mask = map->mask;
            bits = map->bits;
            if (ctrl->buttons[0] & mask) in->buttons[0] |= bits;
            if (ctrl->buttons[1] & mask) in->buttons[1] |= bits;
            if (ctrl->buttons[2] & mask) in->buttons[2] |= bits;
            if (ctrl->buttons[3] & mask) in->buttons[3] |= bits;
            if (ctrl->buttons[4] & mask) in->buttons[4] |= bits;
            map++;
        }
    } else {
        in->buttons[0] = ctrl->buttons[0];
        in->buttons[1] = ctrl->buttons[1];
        in->buttons[2] = ctrl->buttons[2];
        in->buttons[3] = ctrl->buttons[3];
        in->buttons[4] = ctrl->buttons[4];
    }

    switch (opt->m_ControlType) {
    case 0:
        b = in->buttons[0];
        if (b & 1) {
            in->strafe = 1.0f;
        } else if (b & 2) {
            in->strafe = -1.0f;
        } else {
            in->strafe = 0.0f;
        }
        in->turn = ctrl->stickX * 0.0125f;
        b = in->buttons[0];
        if (b & 8) {
            in->move = 1.0f;
        } else if (b & 4) {
            in->move = -1.0f;
        } else {
            in->move = 0.0f;
        }
        if (opt->m_bLookSpring) {
            in->look = ctrl->stickY * 0.0125f;
            break;
        }
        if (pl->m_bHeadMovementDisabled != 0) {
            break;
        }
        if (ctrl->stickY != 0) {
            in->look += rate * 0.0125f * ctrl->stickY;
        } else if (in->move != 0.0f && pl->m_SniperMode == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = Math_DecayToZero(in->look, d);
        }
        if (in->look > 1.0f) {
            in->look = 1.0f;
        } else if (in->look < -1.0f) {
            in->look = -1.0f;
        }
        break;
    case 1:
        in->turn = ctrl->stickX * 0.0125f;
        b = in->buttons[0];
        if (b & 1) {
            in->strafe = 1.0f;
        } else if (b & 2) {
            in->strafe = -1.0f;
        } else {
            in->strafe = 0.0f;
        }
        if (pl->m_bHeadMovementDisabled == 0) {
            in->move = ctrl->stickY * 0.0125f;
        } else {
            in->move = 0.0f;
        }
        b = in->buttons[0];
        if (b & 0x10) {
            break;
        }
        if (b & 8) {
            x = in->look + rate * 0.75f;
            if (x > 1.0f) {
                x = 1.0f;
            }
            in->look = x;
        } else if (b & 4) {
            x = in->look - rate * 0.75f;
            if (x < -1.0f) {
                x = -1.0f;
            }
            in->look = x;
        } else if (opt->m_bLookSpring && in->move != 0.0f && pl->m_SniperMode == 0) {
            d = in->move * (damp * 0.1f);
            if (d < 0.0f) {
                d = -d;
            }
            in->look = Math_DecayToZero(in->look, d);
        }
        break;
    }
    in->outStrafe = in->strafe * ABS(in->strafe * in->strafe);
    if (in->turn * in->turn < 0.0f) {
        in->outTurn = in->turn * -(in->turn * in->turn);
    } else {
        in->outTurn = in->turn * (in->turn * in->turn);
    }
    in->outMove = in->move;
    in->outLook = in->look;
}

#ifdef TARGET_PORT
#include "structs/player.h"
#include <string.h>

/* The host: through CPlayerContState (m_pContState is a pointer at 0x10 of a 0x38-byte
 * N64 record, so the word view below would truncate it and miss the host layout). */
void CPlayerContState__Init(void *arg0_, void *arg1_) {
    CPlayerContState *arg0 = arg0_;
    struct ControllerProfile *arg1 = arg1_;

    memset(arg0, 0, sizeof(*arg0));
    arg0->m_pContState = arg1;
}
#else

/* A record of fourteen words: all cleared but the one at 0x10, set to arg1. */
typedef struct {
    /* 0x00 */ s32 unk0[4];
    /* 0x10 */ s32 value;
    /* 0x14 */ s32 unk14[9];
} Record14_2470E8;

void CPlayerContState__Init(void *arg0_, void *arg1_) {
    Record14_2470E8 *arg0 = arg0_;
    sptr arg1 = (sptr) arg1_;

    arg0->unk0[0] = 0;
    arg0->unk0[1] = 0;
    arg0->unk0[2] = 0;
    arg0->unk0[3] = 0;
    arg0->value = arg1;
    arg0->unk14[0] = 0;
    arg0->unk14[1] = 0;
    arg0->unk14[2] = 0;
    arg0->unk14[3] = 0;
    arg0->unk14[4] = 0;
    arg0->unk14[5] = 0;
    arg0->unk14[6] = 0;
    arg0->unk14[7] = 0;
    arg0->unk14[8] = 0;
}
#endif

s32 Player_SwapStrafeTurnBits(s32 arg0)
{
  int new_var;
  s32 var_v1;
  new_var = 0x80000;
  var_v1 = arg0 & 0xFFF6FFF3;
  if (arg0 & 0x10000)
  {
    var_v1 |= 8;
  }
  if (arg0 & 8)
  {
    var_v1 |= 0x10000;
  }
  if (arg0 & new_var)
  {
    var_v1 |= 4;
  }
  if (arg0 & 4)
  {
    var_v1 |= 0x80000;
  }
  return var_v1;
}

/* The scope model raised to the view. */


/* Places the scope model and animates it up (state 1) or down (state 3)
 * over 3.75 frames: the sniper scope (0xC83) swings in from the side, the
 * others tip up from below. */
void CPlayer__AnimateScope(CPlayerOverlay *m, CPlayer *owner) {
    s32 index;

    if (m->m_ObjectType == 0xC83) {
        m->m_vPos.x = -550.0f;
        m->m_vPos.y = 520.0f;
        m->m_vPos.z = -1125.0f;
        m->m_vScale.x = 0.28f;
        m->m_vScale.y = 0.33f;
        m->m_vScale.z = 0.25f;
    } else {
        m->m_vPos.x = 0.0f;
        m->m_vPos.y = 451.0f;
        m->m_vPos.z = -675.0f;
        m->m_vScale.x = 0.2f;
        m->m_vScale.y = 0.189f;
        m->m_vScale.z = 0.25f;
        m->m_vRot.y = 3.141593f;
    }
    if (m->m_DesiredObjectType != m->m_ObjectType && m->m_Mode == 0) {
        index = CScene__FindObjectType(&gScene, m->m_DesiredObjectType);
        if (index != -1) {
            m->m_rpObject = CScene__GetObject(&gScene, index);
            m->m_Mode = 1;
            m->m_ObjectType = m->m_DesiredObjectType;
        } else {
            m->m_rpObject = NULL;
        }
    }
    switch (m->m_Mode) {
        case 0:
            break;
        case 1:
            m->m_ModeTime += gFrameIncrement;
            if (m->m_ModeTime >= 3.75f) {
                m->m_Mode = 2;
                m->m_ModeTime = 3.75f;
            }
            if (m->m_ObjectType == 0xC83) {
                if (gEngineApp_m_Settings.multiplayer) {
                    m->m_vRot.y = 0.0f;
                    m->m_vRot.x = 0.0f;
                } else {
                    m->m_vRot.x = 0.0f;
                    m->m_vRot.y = (360.0f - m->m_ModeTime * 48.000004f) * 0.017453294f;
                }
            } else {
                m->m_vRot.x = (1.0f - m->m_ModeTime * 0.26666668f) * 1.5707965f;
            }
            break;
        case 2:
            break;
        case 3:
            if (owner->m_ZoomBlend == 0.0f) {
                m->m_ModeTime -= gFrameIncrement;
                if (m->m_ModeTime <= 0.0f) {
                    m->m_Mode = 0;
                    m->m_ModeTime = 0.0f;
                }
            }
            if (m->m_ObjectType == 0xC83) {
                if (gEngineApp_m_Settings.multiplayer) {
                    m->m_vRot.y = 0.0f;
                } else {
                    m->m_vRot.y = ((1.0f - m->m_ModeTime * 0.26666668f) * 180.0f + 180.0f) * 0.017453294f;
                }
            } else {
                m->m_vRot.x = (1.0f - m->m_ModeTime * 0.26666668f) * 1.5707965f;
            }
            break;
    }
}

typedef struct { s32 m[4][4]; } Mtx64;   /* a packed N64 Mtx, 0x40 bytes */
/* A model shown in the HUD (the weapon held up to the view). */


void Render_InvalidateModeCache(void);
void Render_ResetBatches(void);
void Render_FlushBatches(void);
void CEngineApp__ClearDepthRect(void *vp, s32 left, s32 right, s32 top, s32 bottom);
void CMtxF__SetScale(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__RotateY(f32 *mtx, f32 angle);
void CMtxF__RotateX(void *, f32);
void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__SwapYZRows(f32 *mtx);
void CMtxF__Copy(f32 *dst, f32 *src);
void CMtxF__ScaleColumns(f32 *mtx, f32 x, f32 y, f32 z);
void CMtxF__ToMtxAffine(f32 *src, Mtx64 *dst);
void CMtxF__TransformPoint(CMtxF, CVector3 *, CVector3 *);
void Render_QueueModelByKey(sptr key, void *matrix, void *lights, s32 lod, s32 pass);

void CPlayer__DrawScope(CPlayerOverlay *m, CCamera *view) {
    f32 mf[4][4];
    f32 mfDraw[4][4];
    CVector3 out;

    Render_InvalidateModeCache();
    if (m->m_rpObject == 0 || m->m_Mode == 0) {
        return;
    }
    gPrimAlphaOverride = 1;
    gPrimAlphaOverrideValue = 0x20;
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001801;
        g->w1 = 0xC0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000C00;
        g->w1 = 0x80000;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    Render_ResetBatches();
    CEngineApp__ClearDepthRect(&gEngineApp, view->m_Viewport.m_ScreenPosX, view->m_Viewport.m_Width + view->m_Viewport.m_ScreenPosX, view->m_Viewport.m_ScreenPosY, view->m_Viewport.m_Height + view->m_Viewport.m_ScreenPosY);
    CMtxF__SetScale(&mf[0][0], m->m_vScale.x, m->m_vScale.y, m->m_vScale.z);
    CMtxF__RotateY(&mf[0][0], m->m_vRot.y);
    CMtxF__RotateX(&mf[0][0], m->m_vRot.x);
    CMtxF__Translate(&mf[0][0], m->m_vPos.x, m->m_vPos.y, m->m_vPos.z);
    CMtxF__SwapYZRows(&mf[0][0]);
    CMtxF__Copy(&mfDraw[0][0], &mf[0][0]);
    if (gResolution == 2) {
        CMtxF__ScaleColumns(&mfDraw[0][0], sHiResViewScaleX, 1.0f, 1.0f);
    }
    CMtxF__ToMtxAffine(&mfDraw[0][0], &m->m_mOrient[gEvenOdd]);
    CMtxF__TransformPoint(&mf[0][0], &m->m_vPos, &out);
    Render_QueueModelByKey(m->m_rpObject, &m->m_mOrient[gEvenOdd], &gSceneLights, 0, -1);
    Render_FlushBatches();
    gPrimAlphaOverride = 0;
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3001801;
        g->w1 = 0;
    }
}

/* The scope model raised to the view (see CPlayer__AnimateScope). */

#define DEBUG_LINES_ON (*(s32 *) &gEngineApp_m_Game_m_DebugLines) /* the file's view of gEngineApp_m_Game_m_DebugLines, which globals/game.h declares: this type is what keeps the ROM's code */

void Math_EaseToward(f32 *value, f32 target, f32 rate);

/* Starts lowering the scope model if it is up or being raised. */
static __inline__ void LowerScope(CPlayerOverlay *m) {
    if (m->m_Mode != 0 && m->m_Mode != 3) {
        m->m_Mode = 3;
    }
}

/* Shows `model` as the scope, raising it again if it is already that one. */
static __inline__ void RaiseScope(CPlayerOverlay *m, s32 model) {
    if (m->m_DesiredObjectType == model) {
        m->m_Mode = 1;
    }
    m->m_DesiredObjectType = model;
}

/* Sniper zoom: toggled by button 0x100 on a weapon with a scope, then
 * adjusted by 8 (in) and 4 (out) while 0x10 is held. */
void CPlayer__UpdateZoom(CPlayer *p) {
    f32 target;
    f32 t;
    u16 model;

    target = 0.0f;
    if ((s16) gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel <= 0 || p->m_SniperMode == 0) {
        p->m_SniperMode = 0;
        LowerScope(&p->m_Overlay);
    }
    if (p->m_Data.m_Health != 0) {
        if ((p->m_ContState.m_dwButtonsSingle & 0x100) && (p->m_ContState.m_dwButtonsSingle & 0x100) == (p->m_ContState.m_dwButtons & 0x100)) {
            if (p->m_SniperMode != 0) {
                p->m_SniperMode = 0;
                LowerScope(&p->m_Overlay);
            } else if (gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel != 0 && (DEBUG_LINES_ON == 0 || !(p->m_ContState.m_dwButtons & 0x20))) {
                p->m_SniperMode = 1;
                if (p->m_Data.m_CurrentWeapon == 0x1B) {
                    p->m_TargetZoomBlend = 0.7f;
                } else {
                    p->m_TargetZoomBlend = 0.2f;
                }
                model = gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->zoomModel;
                RaiseScope(&p->m_Overlay, model);
            }
        }
        if (p->m_SniperMode != 0) {
            target = p->m_TargetZoomBlend;
            p->m_SniperMode = 1;
            if (p->m_ContState.m_dwButtons & 0x10) {
                if (p->m_ContState.m_dwButtons & 8) {
                    t = target + 0.025f;
                    if (t > 1.0f) {
                        t = 1.0f;
                    }
                    target = t;
                    p->m_SniperMode = 2;
                }
                if (p->m_ContState.m_dwButtons & 4) {
                    t = target - 0.025f;
                    if (t < 0.2f) {
                        t = 0.2f;
                    }
                    target = t;
                    p->m_SniperMode = 2;
                }
            }
        }
    }
    p->m_TargetZoomBlend = target;
    CPlayer__AnimateScope(&p->m_Overlay, p);
    if (p->m_Overlay.m_Mode != 1) {
        Math_EaseToward(&p->m_ZoomBlend, target, 0.5f);
        if (target == 0.0f && p->m_ZoomBlend < 0.01f) {
            p->m_ZoomBlend = 0.0f;
        }
    }
}

#undef DEBUG_LINES_ON
#define CAMERA_POOL_GAME (*(CGameFromCameraPool *) &gEngineApp_m_Game_m_CameraPool) /* the file's view of gEngineApp_m_Game_m_CameraPool, which globals/camera.h declares: this type is what keeps the ROM's code */
/* The game the settings belong to, recovered from the settings pointer. Its address is
   gEngineApp_m_Settings - 0x1228 for the compiler, not gEngineApp_m_Game_m_CameraPool: that is what keeps the tagged and
   head-shot messages from being cross-jumped into the "you killed" one. */
#define GAME_OF(settings) ((CGameFromCameraPool *)((u8 *)(settings) - T2_OFFSETOF(CGameFromCameraPool, m_Settings)))

/* The messages live in the data after .rodata ("head shot!" in the hit-region names), not in this file's pool. */

char *Util_StrCpy(char *dst, char *src); /* strcpy */
void CCameraPool__AddTimedText(CGameFromCameraPool *game, struct CCamera *camera, char *text, f32 time);
/* Shows a player's newest message (ring of four) on its camera. */
#define SHOW_MESSAGE(game, player, time)                                                         \
    CCameraPool__AddTimedText(game, (player)->m_pCamera,                                                     \
                  (player)->m_DeathMatchString[(player)->m_nDeathmatchString], time);            \
    (player)->m_nDeathmatchString++;                                                             \
    (player)->m_nDeathmatchString %= 4;

/* Deathmatch messages for a kill: "killed by" or "tagged by" on the victim's
   screen, "you killed", "you tagged" or "head shot!" on the killer's. */
void CPlayer__ShowKillMessages(CPlayer *victim, CPlayer *killer, DamageEvent *hit) {
    GameSettings *settings = &gEngineApp_m_Settings;
    HitFrame *target;

    if (settings->multiplayer != 0) {
        if (victim == killer) {
            Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouKilledYourself);
            SHOW_MESSAGE(&CAMERA_POOL_GAME, killer, 1.5f);
        } else if (victim->m_pOptions->m_TagMode != 0) {
            f32 time = 1.5f;
            CGameFromCameraPool *game;

            Util_StrCpy(victim->m_DeathMatchString[victim->m_nDeathmatchString], sKillMessageText_TaggedBy);
            Util_StrCpy(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->m_Name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouTagged);
            Util_StrCpy(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->m_Name);
            SHOW_MESSAGE(game, killer, 1.5f);
        } else {
            f32 time = 1.5f;
            CGameFromCameraPool *game;

            Util_StrCpy(victim->m_DeathMatchString[victim->m_nDeathmatchString], sKillMessageText_KilledBy);
            Util_StrCpy(&victim->m_DeathMatchString[victim->m_nDeathmatchString][10], killer->m_pOptions->m_Name);
            game = GAME_OF(settings);
            SHOW_MESSAGE(game, victim, time);
            target = hit->frame;
            if (target != NULL && (target->flags & 3) == 1) {
                Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_HeadShot);
                SHOW_MESSAGE(game, killer, 1.5f);
            } else {
                Util_StrCpy(killer->m_DeathMatchString[killer->m_nDeathmatchString], sKillMessageText_YouKilled);
                Util_StrCpy(&killer->m_DeathMatchString[killer->m_nDeathmatchString][11], victim->m_pOptions->m_Name);
                SHOW_MESSAGE(&CAMERA_POOL_GAME, killer, 1.5f);
            }
        }
    }
}

#undef CAMERA_POOL_GAME

typedef struct TeamGame {
    /* 0x00 */ s32 active;
} TeamGame;
s32 Cinema_PathPlaying(void);
s32 Cinema_IsActive(void);

static __inline__ s32 inSpecialRegion(CPlayer *player) {
    CRegionInfo *info;

    if (player->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = CScene__GetRegionInfo(&gScene, player->m_CharacterActor.header.m_pCurrentRegion);
        if (info != NULL) {
            if (info->m_wFlags52 & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
} HitSource;
void CPlayer__Hurt(CPlayer *p, HitSource *src, s32 damage, s32 arg3, s32 type);

/* A player's actor took a lethal hit in multiplayer: works out the killer and
   updates the kill, death, tag and team counts. */
void Player_OnMultiplayerKill(CGameObjectInstance *actor, sptr arg1, DamageEvent *hit) {
    CPlayer *player;
    CPlayer *killer = NULL;
    s32 arg4 = -1;
    CRegionInfo *info;
    s32 special;
    CGameObjectInstance *source;
    CGameObjectInstance *owner;
    CPlayerOptions *options;
    sptr killerIndex;
    sptr victimIndex;
    s32 *score;
    s32 *teamScore;

    if (Cinema_PathPlaying() != 0) {
        return;
    }
    if (gEngineApp_m_Settings.cheatFlags & 1) {
        return;
    }
    if (Cinema_IsActive() != 0) {
        return;
    }
    player = (CPlayer *) actor->m_AI.m_pParent;
    special = inSpecialRegion(player);
    if (special) {
        arg4 = 20;
    }
    source = hit->source;
    if (source != NULL && source->header.m_Type == 2 && ((CParticle *) source)->m_pOwner == (CGameObjectInstance *) player) {
        hit->damage = hit->damage * 0.25f;
    }
#ifdef TARGET_PORT
    {
        void T2Port_OnPlayerHit(CPlayer *player, DamageEvent *hit); /* DEBUG-ONLY (T2_HURT_LOG, port/game/t2debug.c) */
        T2Port_OnPlayerHit(player, hit);
    }
#endif
    CPlayer__Hurt(player, actor, hit->damage, hit->flags, arg4);

    source = hit->source;
    if (source != NULL) {
        switch (source->header.m_Type) {
            case 2:
                owner = ((CParticle *) source)->m_pOwner;
                if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
                    killer = (CPlayer *) owner;
                }
                break;
            case 1:
                if (source->m_dwFlags & 0x300000) {
                    killer = (CPlayer *) source->m_AI.m_pParent;
                }
                break;
        }
    } else {
        killer = player;
    }

    if (killer == NULL) {
        return;
    }
    if (player->m_Data.m_Health != 0) {
        return;
    }
    if (player->m_pOptions->m_bDead != 0) {
        return;
    }
    player->m_pOptions->m_bDead = 1;
    killerIndex = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, killer);
    options = player->m_pOptions;
    if (options->m_TagMode == 1) {
        options->m_TaggedBy[killerIndex]++;
    } else {
        options->m_KilledBy[killerIndex]++;
    }
    victimIndex = CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player);
    if (player->m_pOptions->m_TagMode == 1) {
        killer->m_pOptions->m_Tagged[victimIndex]++;
    } else {
        killer->m_pOptions->m_Killed[victimIndex]++;
    }
    CPlayer__ShowKillMessages(player, killer, hit);

    if (killerIndex == victimIndex) {
        player->m_pOptions->m_Suicides++;
        if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                    teamScore = &gEngineApp_m_Settings.match.teamScore;
                    break;
                case 1:
                    teamScore = &gEngineApp_m_Settings.match.teamScore[1];
                    break;
                default:
                    return;
            }
            (*teamScore)--;
        }
    } else {
        player->m_pOptions->m_Deaths++;
        if (player->m_pOptions->m_TagMode == 1) {
            killer->m_pOptions->m_Tags++;
            if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &gEngineApp_m_Settings.match.teamScore;
                        break;
                    case 1:
                        score = &gEngineApp_m_Settings.match.teamScore[1];
                        break;
                    default:
                        return;
                }
                if (player->m_nDeathmatchTeamSelected != killer->m_nDeathmatchTeamSelected) {
                    (*score)++;
                }
            }
        } else {
            killer->m_pOptions->m_Kills++;
            if (((TeamGame *)&gEngineApp_m_Settings_match_teams)->active != 0) {
                switch (killer->m_nDeathmatchTeamSelected) {
                    case 0:
                        score = &gEngineApp_m_Settings.match.teamScore;
                        break;
                    case 1:
                        score = &gEngineApp_m_Settings.match.teamScore[1];
                        break;
                    default:
                        return;
                }
                if (player->m_nDeathmatchTeamSelected != killer->m_nDeathmatchTeamSelected) {
                    (*score)++;
                }
            }
        }
    }
}

#ifdef TARGET_PORT
#include "structs/model.h"
/* the draw args (CDrawArgs) at their host offsets (soak S13: with --high-memory the model handle's
 * data pointer was read as an s32 and the player's draw crashed in the attract loop, ~27,000);
 * unkC is m_hModel (N64 0x0C), amount is m_nModel (N64 0x18) */
typedef struct Hit {
    union {
        struct { u8 _hp_unkC[__builtin_offsetof(CDrawArgs, m_hModel)]; s32 *unkC; };
        struct { u8 _hp_amount[__builtin_offsetof(CDrawArgs, m_nModel)]; s32 amount; };
        CDrawArgs _hp_whole;
    };
} Hit;
#else
typedef struct Hit {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 *unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 amount;
} Hit;
#endif

void *CISet__GetBlock(void *, s32);
s32 CPlayer__IsInMatchEndState(CPlayer *arg0);
void CGameObjectInstance__DrawModelImmediate(CGameObjectInstance *actor, sptr arg1, void *hit);
void CGameObjectInstance__DrawModel(CGameObjectInstance *actor, sptr arg1, void *hit);
void Render_DrawModelShared(s32 *arg0, sptr arg1, s32 arg2, void *arg3, sptr arg4, s32 arg5, s32 arg6);
void Render_QueueModelInstance(s32 *arg0, sptr arg1, s32 arg2, void *arg3, sptr arg4, s32 arg5, s32 arg6);
#define HIT_LIMIT(hit) (*(s32 *) CISet__GetBlock(*(void **) (hit)->unkC, 2)) /* the handle holds a pointer */
#define WEAPON_INDEX(player) (gWeaponInfoTable[(player)->m_Data.m_CurrentWeapon]->model - 0x5E0)

void Player_DrawNodeCallback(CGameObjectInstance *actor, sptr arg1, Hit *hit) {
    CPlayer *player = ((CPlayer *)actor->m_AI.m_pParent);
    u8 *flags;
    f32 *color;
    s32 index;

    if (hit->amount >= HIT_LIMIT(hit)) {
        return;
    }
    flags = &gEngineApp_m_Settings.multiplayer;
    if (*flags != 0) {
        if (player->m_pOptions->m_TagMode != 0 && player->m_Data.m_Health == 0) {
            return;
        }
        if (*(s32 *) (flags + (0x1FC - 0x19)) != 0) {
            gBlendState = 3;
            switch (player->m_nDeathmatchTeamSelected) {
                case 0:
                default:
                    color = gBlendState_m_Color;
                    color[0] = 0.0f;
                    color[1] = 0.0f;
                    color[2] = 192.0f;
                    break;
                case 1:
                    color = gBlendState_m_Color;
                    color[1] = 0.0f;
                    color[2] = 0.0f;
                    color[0] = 192.0f;
                    break;
            }
            gBlendState_m_Color[3] = 70.0f;
            gBlendState_m_Color[4] = 255.0f;
            CGameObjectInstance__DrawModelImmediate(actor, arg1, hit);
            if (player->m_ShockedTimer > 0.0f && CPlayer__IsInMatchEndState(player) == 0) {
                Render_DrawModelShared(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, (s8) hit->amount, -1);
            }
            if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_Data.m_Health != 0) {
                index = WEAPON_INDEX(player);
                if (index >= 0 && index < HIT_LIMIT(hit)) {
                    Render_DrawModel(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, index, actor->m_AI.m_nTexture);
                }
            }
            return;
        }
    }
    if (player->m_ShockedTimer > 0.0f && CPlayer__IsInMatchEndState(player) == 0) {
        CGameObjectInstance__DrawModelImmediate(actor, arg1, hit);
        Render_DrawModelShared(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, (s8) hit->amount, -1);
    } else {
        CGameObjectInstance__DrawModel(actor, arg1, hit);
    }
    if (player->m_Data.m_nDeathmatchCharacter != 10 && player->m_pOptions->m_TagMode == 0 && player->m_Data.m_Health != 0 && player->m_CharacterActor.m_nTypeFlag != gTagModeModel) {
        index = WEAPON_INDEX(player);
        if (index >= 0 && index < HIT_LIMIT(hit)) {
            Render_QueueModelInstance(hit->unkC, ((sptr)actor->m_pmtDrawMtxs), 1, &actor->m_Lights[gEvenOdd], 0, WEAPON_INDEX(player), actor->m_AI.m_nTexture);
        }
    }
}

extern void CGameObjectInstance__ConstructFromRecord(void *obj, void *rec, s32 mode, s32 index);
extern void CGameObjectInstance__CalculateOrientationMatrix();
void Actor_PlayRangedSound(void *arg0, void *arg1);
void CPlayer__OnParticleHitCallback(CGameObjectInstance *arg0, sptr arg1, sptr arg2);
extern void WeaponActor_BuildMatrix();
extern void WeaponActor_SwitchToRequested(void *, void *);
void CPlayer__UpdateOrbitCamera(CPlayer *player, CGameObjectInstance *target);
extern void Cinema_UpdateCameraIfActive(sptr);

VM_TEXT void CPlayer__SetupActors(CPlayer *p, void *rec, s32 mode) {
    f32 half;

    p->m_CharacterActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(p, rec, mode, 0);
    p->m_CharacterActor.m_AI.m_pParent = p;
    p->m_CharacterActor.m_dwFlags |= 0x100000;
    p->m_CharacterActor.m_dwFlags |= 0x10000;
    p->m_CharacterActor.m_dwFlags |= 0x1000000;
    p->m_CharacterActor.m_dwFlags |= 0x2000000;
    if (!gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_dwFlags |= 0x10000000;
    }
    p->m_CharacterActor.m_AI.m_pfCalculateOrientationMatrix = CGameObjectInstance__CalculateOrientationMatrix;
    p->m_CharacterActor.m_AI.m_pfEffects = Actor_PlayRangedSound;
    p->m_CharacterActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_CharacterActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    if (gEngineApp_m_Settings.multiplayer) {
        p->m_CharacterActor.m_AI.m_pfDraw = Player_DrawNodeCallback;
    }

    p->m_WeaponActor.m_AI.m_pParent = p;
    CGameObjectInstance__ConstructFromRecord(&p->m_WeaponActor, rec, mode, 0);
    p->m_WeaponActor.m_AI.m_pfCalculateOrientationMatrix = WeaponActor_BuildMatrix;
    p->m_WeaponActor.m_AI.m_Mode = -1;
    p->m_WeaponActor.m_AI.m_pParent = p;
    p->m_WeaponActor.m_AI.m_pfHurt = Player_OnMultiplayerKill;
    p->m_WeaponActor.m_AI.m_pfHit = CPlayer__OnParticleHitCallback;
    p->m_WeaponActor.m_dwFlags |= 0x200000;
    p->m_Data.m_CurrentWeapon = -1;
    half = 0.5f;
    p->m_WeaponActor.m_vScale.x = half;
    p->m_WeaponActor.m_vScale.y = half;
    p->m_WeaponActor.m_vScale.z = half;
    p->m_WeaponActor.m_dwFlags &= ~0x10000;
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 0);
    WeaponActor_SwitchToRequested(&p->m_WeaponActor, &p->m_WeaponActor.m_AI);
    CPlayer__UpdateOrbitCamera(p, p);
    Cinema_UpdateCameraIfActive(p->m_pCamera);
    p->m_CharacterActor.m_vScale.x = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_ScaleZ;
}

void CPlayerContState__Init(void *, void *);
f32 Actor_GetEyeHeight(CGameObjectInstance *actor);
/* The ROM clears the float fields with integer stores (sw $zero): a 0.0f
 * store keeps the constant in a register instead. */
#define CLEAR_WORD(field) (*(s32 *) &(field) = 0)

static __inline__ void setVec3(CVector3 *v, f32 x, f32 y, f32 z) {
    v->z = z;
    v->y = y;
    v->x = x;
}

/* Resets a player's movement, camera and crosshair state. */
void CPlayer__Reset(CPlayer *p) {
    f32 zero;

    CPlayerContState__Init(&p->m_ContState, &gControllers[p->m_nController]);
    CLEAR_WORD(p->m_Speed);
    CLEAR_WORD(p->m_SideSpeed);
    CLEAR_WORD(p->m_TotalSpeed);
    CLEAR_WORD(p->m_YVelLastFrame);
    p->m_bOnGround = 1;
    CLEAR_WORD(p->m_BurstSpeed);
    CLEAR_WORD(p->m_BurstSideSpeed);
    CLEAR_WORD(p->m_BurstTimer);
    CLEAR_WORD(p->m_TimeOffGround);
    CLEAR_WORD(p->m_BreathAmp);
    CLEAR_WORD(p->m_BreathSpeed);
    p->m_bHeadMovementDisabled = 0;
    CLEAR_WORD(p->m_JumpAngle);
    CLEAR_WORD(p->m_StartJumpAngle);
    CLEAR_WORD(p->m_DesiredJumpAngle);
    CLEAR_WORD(p->m_JumpU);
    CLEAR_WORD(p->m_vHeadRotOffset.x);
    CLEAR_WORD(p->m_vHeadRotOffset.y);
    CLEAR_WORD(p->m_vHeadRotOffset.z);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.x);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.y);
    CLEAR_WORD(p->m_vHeadWobbleRotOffset.z);
    CLEAR_WORD(p->m_vEyeOffset.x);
    p->m_vEyeOffset.y = Actor_GetEyeHeight(p);
    CLEAR_WORD(p->m_vEyeOffset.z);
    setVec3((CVector3 *) &p->m_qGround, 0.0f, 0.0f, 0.0f);
    CLEAR_WORD(p->m_ActualCameraVertEyeOffset);
    p->m_qGround.w = 1.0f;
    p->m_CameraMovementScaler = 1.0f;
    p->m_vDesiredPos = p->m_CharacterActor.header.m_vPos;
    CLEAR_WORD(p->m_vCrossHairVel.z);
    zero = p->m_vCrossHairVel.z;   /* read back as a float: the ROM reloads it */
    CLEAR_WORD(p->m_ModeTime);
    p->m_InSun = 0;
    CLEAR_WORD(p->m_FrameIncrementScaled);
    CLEAR_WORD(p->m_DuckOffset);
    p->m_bDuckMode = 0;
    CLEAR_WORD(p->m_ZoomBlend);
    CLEAR_WORD(p->m_TargetZoomBlend);
    p->m_SniperMode = 0;
    p->m_CrossHairMode = 0;
    CLEAR_WORD(p->m_CrossHairTimer);
    CLEAR_WORD(p->m_CrossHairAlpha);
    p->m_pCrossHairTarget = NULL;
    p->m_CrossHairIcon = -1;
    CLEAR_WORD(p->m_vCrossHairPos.x);
    CLEAR_WORD(p->m_vCrossHairPos.y);
    CLEAR_WORD(p->m_vCrossHairPos.z);
    CLEAR_WORD(p->m_vCrossHairVel.x);
    CLEAR_WORD(p->m_vCrossHairVel.y);
    setVec3((CVector3 *) &p->m_qAutoAim, zero, zero, zero);
    p->m_qAutoAim.w = 1.0f;
    p->m_pSightTarget = NULL;
    p->m_SightIcon = -1;
    p->m_vSightPos.x = zero;
    p->m_vSightPos.y = zero;
    p->m_vSightPos.z = zero;
    p->m_vSightVel.x = zero;
    p->m_vSightVel.y = zero;
    p->m_vSightVel.z = zero;
    CPlayer__SetState(p, p, 2);
    p->m_pOnEnemy = NULL;
    p->m_RideEnemyTime = zero;
    p->m_OnEnemyStompTime = zero;
    p->m_Oxygen = zero;
    p->m_bOxygenOut = 0;
    p->m_pOptions->m_bDead = 0;
    p->m_FleeTime = zero;
}

extern s32 Demo_GetPlayerCount(void);
extern s32 SavedPlayers_IsActive(void);
extern u8 *CCameraPool__AllocCamera(void *);
void CBarrel__Reset(void *menu_);
void CPlayer__GiveStartingWeapons(CPlayer *p);
void CPlayer__ResetHealth(CPlayer *p);
/* The port: the three window descs are data (MenuDesc), not functions: declared as such so
 * tools/port/gendata.py lays them out (as func_* they were zero .bss placeholders). */
#include "structs/menu.h"
#define func_00438808 (&gCharacterMenuDesc)

VM_TEXT void CPlayer__NewGame(CPlayer *p, s32 character, s32 arg2) {
    GameSettings *flags;
    s32 i;

    if (Demo_GetPlayerCount()) {
        arg2 = 0;
    }
    if (SavedPlayers_IsActive()) {
        arg2 = 0;
    }
    p->m_bActive = 1;
    p->m_Data.m_nDeathmatchCharacter = character;
    p->m_pCamera = (struct CCamera *) CCameraPool__AllocCamera(&gEngineApp_m_Game_m_CameraPool);
    CBarrel__Reset(&p->m_Barrel);
    for (i = 0; i < 0x400; i++) {
        p->m_Data.m_SpecialPickupCount[i] = 0;
    }
    p->m_Data.m_LifeForce = 0;
    for (i = 0; i < 6; i++) {
        p->m_Data.m_NukePartLevelFlags[i] = 0;
    }
    for (i = 0; i < 6; i++) {
        p->m_Data.m_KeySet6LevelFlags[i] = 0;
    }
    if (!gEngineApp_m_Settings.multiplayer) {
        p->m_Data.m_Lives = 3;
    } else {
        p->m_Data.m_Lives = 1;
    }
    CPlayer__GiveStartingWeapons(p);
    p->m_InvincibilityTime = 0;
    p->m_PlaneWalkerTime = 0;
    p->m_CharAnim = -1;
    p->m_LastCharAnim = -1;
    for (i = 0; i < 4; i++) {
        p->m_pOptions->m_KilledBy[i] = 0;
        p->m_pOptions->m_Killed[i] = 0;
        p->m_pOptions->m_TaggedBy[i] = 0;
        p->m_pOptions->m_Tagged[i] = 0;
    }
    p->m_pOptions->m_Suicides = 0;
    p->m_pOptions->m_Deaths = 0;
    p->m_pOptions->m_Kills = 0;
    p->m_pOptions->m_Tags = 0;
    p->m_pOptions->m_bDead = 0;
    p->m_pOptions->unk_0x3D = 0;
    p->m_pOptions->m_TagMode = 0;
    p->m_pOptions->m_LastTagMode = 0;
    for (i = 0; i < 6; i++) {
        p->m_Data.m_AdonAmmoFlagHelped[i] = 0;
        p->m_Data.m_AdonHealthFlagHelped[i] = 0;
    }
    p->m_Data.m_bLevel1TotemFlags = 0;
    p->m_Data.m_bLevel2TotemFlags = 0;
    p->m_Data.m_bLevel3TotemFlags = 0;
    p->m_Data.m_bLevel4TotemFlags = 0;
    p->m_Data.m_bLevel5TotemFlags = 0;
    p->m_Data.m_bBlindBossComplete = 0;
    p->m_Data.m_bQueenBossComplete = 0;
    p->m_Data.m_bMotherBossComplete = 0;
    p->m_Data.m_bPrimagenBossComplete = 0;
    p->m_Data.m_LevelIntroPlayedFlags = 0;
    p->m_Data.m_dwLevelFirstTimeFlags |= 1;
    CPlayer__ResetHealth(p);
    flags = &gEngineApp_m_Settings;
    if (flags->multiplayer) {
        p->m_nDeathmatchString = 0;
    }
    if (arg2) {
        if (flags->multiplayer) {
            p->m_pOptions->m_nCharacter = 0;
            p->m_Data.m_nDeathmatchCharacter = 0;
            p->m_bEnteringDeathmatch = 1;
            if (flags->match.teams) {
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, &gTeamMenuDesc, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            } else {
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, func_00438808, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
            }
        } else {
            MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, &gSingleMenuDesc, p, (sptr) p->m_ContState.m_pContState, p->m_nController);
        }
    }
}
void CPlayer__PickSpawnPoint(CPlayer *arg0);
void CPlayer__LoadCharacterModel(CPlayer *player);
extern void COverlay__Construct(COverlay *item, s32 x, s32 y);
extern void COverlay__StartScript(COverlay *item, s32 *script);
extern s32 Cinema_IsActive(void);
AmmoInfo *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon);
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
void CGameRegion__FireTriggers(void *, s32, void *);

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__PickSpawnPoint receives CPlayer__LevelStart's own argument
 * registers (pass-through); the host passes them. */
VM_TEXT void CPlayer__LevelStart(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;
    CPlayerOverlay *c;
    s32 warp;

    if (flags->multiplayer) {
        CPlayer__PickSpawnPoint(p);
        CPlayer__LoadCharacterModel(p);
    }
    c = &p->m_Overlay;
    c->m_Mode = 0;
    c->m_rpObject = NULL;
    c->m_ModeTime = 0.0f;
    c->m_DesiredObjectType = -1;
    c->m_ObjectType = -1;
    c->m_vPos.x = 0.0f;
    c->m_vRot.x = 0.0f;
    c->m_vRot.z = 0.0f;
    c->m_vPos.y = 437.5f;
    c->m_vPos.z = -675.0f;
    c->m_vScale.x = 0.25f;
    c->m_vScale.y = 0.1875f;
    c->m_vScale.z = 0.25f;
    c->m_vRot.y = 1.5707965f;
    p->m_pRumble = NULL;
    if (!flags->multiplayer) {
        Controller_SetRumbleOption(p->m_ContState.m_pContState, 1);
    }
    p->m_FlashlightTime = 0.0f;
    p->m_pDLFlashlight1 = NULL;
    p->m_pDLFlashlight2 = NULL;
    p->m_pDLDarkCityLight = NULL;
    COverlay__Construct(&p->m_AmmoOverlay, 120, 96);
    COverlay__StartScript(&p->m_AmmoOverlay, gAmmoOverlayScript);
    COverlay__Construct(&p->m_HealthOverlay, 48, 96);
    COverlay__StartScript(&p->m_HealthOverlay, gHealthOverlayScript);
    COverlay__Construct(&p->m_LivesOverlay, -64, 16);
    COverlay__StartScript(&p->m_LivesOverlay, gLivesOverlayScript);
    COverlay__Construct(&p->m_TokensOverlay, 40, 16);
    COverlay__StartScript(&p->m_TokensOverlay, gTokensOverlayScript);
    COverlay__Construct(&p->m_TotalKillsOverlay, 40, 42);
    COverlay__StartScript(&p->m_TotalKillsOverlay, gTotalKillsOverlayScript);
    COverlay__Construct(&p->m_TotalTagsOverlay, 40, 68);
    COverlay__StartScript(&p->m_TotalTagsOverlay, gTotalTagsOverlayScript);
    COverlay__Construct(&p->m_AirOverlay, 24, (s16) (gScreenHeight + 96));
    warp = 0;
    COverlay__Construct(&p->m_BossOverlay, 24, (s16) (gScreenHeight + 96));
    p->m_SfxTimer = 0.0f;
    p->m_SightIconFrame = 0.0f;
    p->PreviousCameraQuatern = p->m_pCamera->m_qViewAlign;
    CPlayer__Reset(p);
    if (!Cinema_IsActive()) {
        warp = gScene_m_LevelID != 999;
    }
    if (gScene.m_bStreamAll) {
        warp = 1;
    }
    if (warp) {
        p->m_Data.m_CheckPointWarpID = gScene_m_LevelID;
    }
    if (!flags->multiplayer && p->m_Data.m_WeaponList[21]) {
        /* the ROM adds the scaled index first: written index + array */
        *(CPlayer__GetFiringAmmo(p, 21)->type + p->m_Data.m_AmmoList) = 1;
    }
    Cinema_Disable();
    if (p->m_CharacterActor.header.m_pCurrentRegion) {
        CGameRegion__FireTriggers((sptr) p->m_CharacterActor.header.m_pCurrentRegion, 1, p);
    }
    Cinema_Enable();
}

s32 Level_GetHub(s32 who);

/* A full refill from player who (1-6): kind 0 tops up the ammo of every
 * weapon the player owns, kind 1 the health to 100; returns whether
 * anything was given, and if so notes who gave it. */
s32 CPlayer__RefillFrom(CPlayer *p, s32 kind, s32 who) {
    s32 given = 0;
    s32 slot;
    s32 any;
    s32 i;
    s32 j;
    WeaponInfo *weapon;
    AmmoInfo *info;
    s16 *ammo;

    slot = Level_GetHub(who);
    if (slot < 1 || slot > 6) {
        return given;
    }
    slot--;
    switch (kind) {
        case 0:
            any = 0;
            for (i = 0; i < 0x23; i++) {
                weapon = gWeaponInfoTable[i];
                if (p->m_Data.m_WeaponList[i] == 0) {
                    continue;
                }
                for (j = 0; j < 3; j++) {
                    info = weapon->ammo[j];
                    if (info == NULL) {
                        break;
                    }
                    ammo = &p->m_Data.m_AmmoList[info->type];
                    if (*ammo < info->max) {
                        any = 1;
                    }
                    *ammo = (info->max < *ammo) ? *ammo : info->max;
                }
            }
            given = any;
            if (given) {
                p->m_Data.m_AdonAmmoFlagHelped[slot] = 1;
            }
            break;
        case 1:
            if (p->m_Data.m_Health < 0x6400) {
                p->m_Data.m_Health = 0x6400;
                given = 1;
            }
            if (given) {
                p->m_Data.m_AdonHealthFlagHelped[slot] = kind;
            }
            break;
        default:
            return given;
    }
    return given;
}

s32 Level_GetHub(s32 arg0);
#ifdef TARGET_PORT
#include "structs/player.h"
/* Host body (round 7 v2): the raw N64 offsets of CPlayer by member (0x52C + n
 * m_Data.m_SpecialPickupCount[n], 0x9FC m_Data.m_NukePartLevelFlags, 0x988 m_WeaponList[22],
 * 0x934 m_AmmoList, 0xB14 m_RequestedWeapon). */
void CPlayer__CollectKeyPiece(u8 *arg0, s32 id) {
    CPlayer *player = (CPlayer *) arg0;
    u8 *key;
    u32 level;
    AmmoInfo *info;
    s16 *ammo;
    s32 count;
    s32 max;

    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        key = &player->m_Data.m_SpecialPickupCount[id - 4000];
        key[0] += 0x11;
        if (id == 4500) {
            level = Level_GetHub(gScene_m_LevelID) - 1;
            if (level < 6) {
                player->m_Data.m_NukePartLevelFlags[level]++;
            }
            if ((s8) (key[0] & 0xF) >= 6) {
                player->m_Data.m_WeaponList[22] = 1;
                info = gWeaponInfoTable[22]->ammo[0];
                ammo = &player->m_Data.m_AmmoList[info->type];
                count = *ammo + 2;
                max = info->max;
                if (count < max) {
                    max = count;
                }
                *ammo = max;
                player->m_RequestedWeapon = 0x16;
            }
        }
    }
}
#else
/* Collects key piece (id - 4000); key 4500 also counts per level and, once the
   sixth piece is found, flags the player and gives two rounds of ammo. */
void CPlayer__CollectKeyPiece(u8 *player, s32 id) {
    u8 *key;
    u32 level;
    AmmoInfo *info;
    s16 *ammo;
    s32 count;
    s32 max;

    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        key = player + (id - 4000);
        key[0x52C] += 0x11;
        if (id == 4500) {
            level = Level_GetHub(gScene_m_LevelID) - 1;
            if (level < 6) {
                (player + level)[0x9FC]++;
            }
            if ((s8) (key[0x52C] & 0xF) >= 6) {
                player[0x988] = 1;
                info = gWeaponInfoTable[22]->ammo[0];
                ammo = (s16 *) (player + (info->type * 2 + 0x934));
                count = *ammo + 2;
                max = info->max;
                if (count < max) {
                    max = count;
                }
                *ammo = max;
                *(s16 *) (player + 0xB14) = 0x16;
            }
        }
    }
}
#endif

CQuaternion CGameObjectInstance__GetRotation(void *);
/* A debug print compiled out of the release build. Its empty loop notes stop
 * loop.c (find_and_verify_loops) from moving the found block out of the
 * projectile loop, as in the ROM. */
#define DEBUG_MSG() do { } while (0)
#define CHARGE(p) if ((p)->m_CharacterActor.m_CurrentAnimation.m_AnimationFrame >= 3.0f) { (p)->m_CharacterActor.m_CurrentAnimation.m_AnimationFrame -= 3.0f; }

/* Whether the weapon fires the given projectile. */
static __inline__ s32 usesProjectile(s32 weapon, s32 type) {
    WeaponInfo *def = gWeaponInfoTable[weapon];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (def->ammo[i] == NULL) {
            break;
        }
        if (def->ammo[i]->projectile == type) {
            DEBUG_MSG();
            return 1;
        }
    }
    return 0;
}

/* Fires the player's weapon: spends the ammo, advances the firing
 * animation, picks the projectile (sniper and charge variants) and
 * launches it from the camera. */
void CPlayer__FireWeapon(CPlayer *p, s32 type) {
    CVector3 pos;
    CVector3 vel;
    CQuaternion aim;
    CCamera *camera = p->m_pCamera;
    AmmoInfo *info;
    s16 *ammo;
    sptr target;
    f32 f;
    s32 found;

    if ((u16) p->m_Mode < 2) {
        return;
    }
    aim = CGameObjectInstance__GetRotation(p);
    info = CPlayer__GetFiringAmmo(p, p->m_Data.m_CurrentWeapon);
    if (info != NULL) {
        found = usesProjectile(p->m_Data.m_CurrentWeapon, type); /* a user variable: loop.c keeps found = 1 in the loop */
        if (found) {
            s32 readyAnim = 1; /* compared below; held in $a2 from here */
            aim = p->m_qAutoAim;
            ammo = &p->m_Data.m_AmmoList[info->type];
            *ammo -= info->cost;
            if (*ammo < 0) {
                *ammo = 0;
            }
            type = info->projectile;
            if (p->m_CharAnim == readyAnim) {
                p->m_CharAnim = 0x1130;
            } else if (p->m_CharAnim == 0x1130) {
                CHARGE(p);
            } else if (p->m_CharAnim == 0xA28) {
                p->m_CharAnim = 0x1135;
            } else if (p->m_CharAnim == 0x1135) {
                CHARGE(p);
            } else if (p->m_CharAnim == 0x14) {
                p->m_CharAnim = 0x1144;
            } else if (p->m_CharAnim == 0x1144) {
                CHARGE(p);
            }
            p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
        }
    }
    gProjectileSpeedScale = gWeaponChargeScale;
    target = 0;
    if (camera != NULL) {
        pos = camera->m_vPos;
    } else {
        pos = p->m_CharacterActor.header.m_vPos;
    }
    vel = p->m_CharacterActor.m_vVelocity;
    if (p->m_CrossHairMode == 2) {
        target = (sptr) p->m_pCrossHairTarget;
    }
    gProjectilePower = p->weapon_particle_damage_scaler;
    if (p->m_SniperMode) {
        switch (p->m_Data.m_CurrentWeapon) {
            case 10:
                type = 0x110;
                break;
            case 0x1B:
                type = 0x13A;
                break;
            case 3:
                if (type == 2) {
                    type = 0x111;
                } else if (type == 0x56) {
                    type = 0x126;
                }
                break;
        }
    }
    if (type == 4) {
        f = gProjectilePower * 6.0f;
        type = (s32)((f > 6.0f ? 6.0f : f) + 460.0f);
    }
    CParticleSystem__Spawn(&gScene.m_ParticleSystem, p, p, 0, target, type, vel, aim, pos, 0, -1, (p->m_CharacterActor.m_AI.m_dwFlags & 0x80) ? 5 : 1);
    gProjectilePower = p->weapon_particle_damage_scaler = gWeaponChargeScale = gProjectileSpeedScale = 1.0f;
}
void CPlayer__Respawn(CPlayer *p);
void Sound__PlaySpeech(s32);
/* Puts the player at a warp point: unlocks/redirects level hub warps, marks
 * visited hubs, then picks the warp record (the one farthest from the other
 * players in multiplayer) and moves the player there. */

typedef struct WarpPlayer WarpPlayer;
void CPlayer__Warp(WarpPlayer *pl, s32 warp, s32 arg2, s32 respawn);

VM_TEXT void CPlayer__StartGame(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) * 2;

    if (gEngineApp_m_Settings_multiplayer) {
        p->m_Data.m_Lives = 1;
    } else {
        p->m_Data.m_Lives = 3;
    }
    if (gEngineApp_m_Settings_multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), gEngineApp_m_Settings_multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings_multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
    p->m_bEnteringDeathmatch = 0;
    CPlayer__Warp(p, p->m_Data.m_CheckPointWarpID, mode, 1);
}

VM_TEXT void CPlayer__LoseLife(CPlayer *p) {
    s32 mode = (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) * 2;
    GameSettings *flags = &gEngineApp_m_Settings;

    if (!(flags->cheatFlags & 2) || flags->multiplayer != 0) {
        p->m_Data.m_Lives--;
    }
    if (p->m_Data.m_Lives <= 0) {
        if (gEngineApp_m_Settings_multiplayer == 0 || (s8)p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted != 0 || p->m_pOptions->m_TagMode != 0) {
            CPlayer__SetState(p, p, 15);
        }
        return;
    }
    CPlayer__Warp(p, p->m_Data.m_CheckPointWarpID, mode, 1);
    if (gEngineApp_m_Settings_multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), gEngineApp_m_Settings_multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings_multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}

/* Takes every weapon and all ammo away, then gives the starting kit: in
 * multiplayer by character (10 gets weapon 0x22), otherwise the
 * knife-and-bow set with 20 rounds. */
void CPlayer__GiveStartingWeapons(CPlayer *p) {
    s32 i;

    for (i = 0; i < 0x1F; i++) {
        p->m_Data.m_AmmoList[i] = 0;
    }
    for (i = 0; i < 0x23; i++) {
        p->m_Data.m_WeaponList[i] = 0;
    }
    if (gEngineApp_m_Settings.multiplayer) {
        if (p->m_Data.m_nDeathmatchCharacter == 10) {
            p->m_Data.m_WeaponList[0x22] = 1;
            p->m_Data.m_LastWeapon[0] = 0x22;
            p->m_Data.m_LastWeapon[1] = 0x22;
            p->m_Data.m_LastWeapon[2] = -1;
            p->m_RequestedWeapon = 0x22;
            p->m_Data.m_CurrentWeapon = 0x22;
        } else {
            p->m_Data.m_WeaponList[0x18] = 1;
            p->m_Data.m_WeaponList[0x20] = 1;
            p->m_Data.m_LastWeapon[0] = 0x18;
            p->m_Data.m_LastWeapon[1] = 0x20;
            p->m_Data.m_LastWeapon[2] = -1;
            p->m_RequestedWeapon = 0x18;
            p->m_Data.m_CurrentWeapon = 0x18;
        }
    } else {
        p->m_Data.m_WeaponList[0] = 1;
        p->m_Data.m_WeaponList[2] = 1;
        p->m_Data.m_WeaponList[0x17] = 1;
        p->m_Data.m_AmmoList[0] = 0x14;
        p->m_Data.m_AmmoList[0x15] = 1;
        p->m_Data.m_AmmoList[0x17] = 1;
        p->m_RequestedWeapon = 0;
        p->m_Data.m_CurrentWeapon = 0;
        p->m_Data.m_LastWeapon[0] = 0;
        p->m_Data.m_LastWeapon[1] = 0;
        p->m_Data.m_LastWeapon[2] = 0x13;
    }
    if (gEngineApp_m_Settings.multiplayer) {
        p->m_WeaponActor.m_AI.m_pModeTable = gWeaponInfoTable[p->m_RequestedWeapon]->modeTable;
        p->m_WeaponActor.m_AI.m_pfEffects = gWeaponInfoTable[p->m_Data.m_CurrentWeapon]->effects;
    }
}

#define FALSE 0
#define TRUE 1
#ifdef TARGET_PORT
struct WarpPlayer {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_WarpPlayer(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_WarpPlayer(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_room[T2_HOFF_WarpPlayer(m_CharacterActor.header.m_pCurrentRegion)]; sptr room; }; /* N64 0x10 */
        struct { u8 _hp_angle[T2_HOFF_WarpPlayer(m_CharacterActor.m_RotY)]; f32 angle; }; /* N64 0x50 */
        struct { u8 _hp_spawnPos[T2_HOFF_WarpPlayer(m_WeaponActor.header.m_vPos)]; CVector3 spawnPos; }; /* N64 0x28C */
        struct { u8 _hp_spawnRoom[T2_HOFF_WarpPlayer(m_WeaponActor.header.m_pCurrentRegion)]; sptr spawnRoom; }; /* N64 0x298 */
        struct { u8 _hp_spawnAngle[T2_HOFF_WarpPlayer(m_WeaponActor.m_RotY)]; f32 spawnAngle; }; /* N64 0x2D8 */
        struct { u8 _hp_active[T2_HOFF_WarpPlayer(m_pCamera)]; sptr active; }; /* N64 0x51C */
        struct { u8 _hp_keys[T2_HOFF_WarpPlayer(m_Data) + 1200]; s32 keys[8]; }; /* N64 0x9D0 */
        struct { u8 _hp_hubsVisited[T2_HOFF_WarpPlayer(m_Data.m_dwLevelFirstTimeFlags)]; u32 hubsVisited; }; /* N64 0x9F8 */
        struct { u8 _hp_controller[T2_HOFF_WarpPlayer(m_ContState.m_pContState)]; struct ControllerProfile *controller; }; /* N64 0xA50 */
        struct { u8 _hp_unkC00[T2_HOFF_WarpPlayer(m_bLevelNotCompleteExit)]; s32 unkC00; }; /* N64 0xC00 */
        struct { u8 _hp_next[T2_HOFF_WarpPlayer(m_pNext)]; WarpPlayer *next; }; /* N64 0x1320 */
    };
};
#else
struct WarpPlayer {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ s32 room;
    /* 0x0014 */ u8 pad14[0x50 - 0x14];
    /* 0x0050 */ f32 angle;
    /* 0x0054 */ u8 pad54[0x28C - 0x54];
    /* 0x028C */ CVector3 spawnPos;
    /* 0x0298 */ s32 spawnRoom;
    /* 0x029C */ u8 pad29C[0x2D8 - 0x29C];
    /* 0x02D8 */ f32 spawnAngle;
    /* 0x02DC */ u8 pad2DC[0x51C - 0x2DC];
    /* 0x051C */ s32 active;
    /* 0x0520 */ u8 pad520[0x9D0 - 0x520];
    /* 0x09D0 */ s32 keys[8];
    /* 0x09F0 */ u8 pad9F0[0x9F8 - 0x9F0];
    /* 0x09F8 */ u32 hubsVisited;
    /* 0x09FC */ u8 pad9FC[0xA50 - 0x9FC];
    /* 0x0A50 */ s32 controller;
    /* 0x0A54 */ u8 padA54[0xC00 - 0xA54];
    /* 0x0C00 */ s32 unkC00;
    /* 0x0C04 */ u8 padC04[0x1320 - 0xC04];
    /* 0x1320 */ WarpPlayer *next;
};
#endif

void CPlayer__StopLoopingSounds(CPlayer *p);
s32 Mission_LevelObjectivesDone(s32);
f32 CVector3__Distance(void *, void *);

/* Sum of the distances from a warp record to every active player. */
static __inline__ f32 sumDistance(CWarpPoint *r) {
    WarpPlayer *other;
    f32 dist;

    dist = 0.0f;
    for (other = (WarpPlayer *)gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; other != NULL; other = other->next) {
        if (other->active) {
            dist += CVector3__Distance(&r->m_vPos, &other->pos);
        }
    }
    return dist;
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__StopLoopingSounds receives CPlayer__Warp's own argument
 * registers (pass-through); the host passes them. */
void CPlayer__Warp(WarpPlayer *pl, s32 warp, s32 arg2, s32 respawn) {
    CWarpPoint rec;
    CWarpPoint recs[32];
    s32 skip;
    s32 count;
    s32 best;
    s32 i;
    f32 bestDist;
    f32 dist;

    skip = FALSE;
    CPlayer__StopLoopingSounds(pl);
    Controller_SetRumbleOption(pl->controller, 0);

    if (gScene.m_bStreamAll && respawn) {
        skip = TRUE;
    }
    if (!skip) {
        switch (warp) {
        case 1000:
            if (!Mission_LevelObjectivesDone(1)) {
                pl->unkC00 = 1;
            }
            break;
        case 2000:
            if (!Mission_LevelObjectivesDone(2)) {
                pl->unkC00 = 1;
            }
            break;
        case 3000:
            if (!Mission_LevelObjectivesDone(3)) {
                pl->unkC00 = 1;
            }
            break;
        case 4000:
            if (!Mission_LevelObjectivesDone(4)) {
                pl->unkC00 = 1;
            }
            break;
        case 5000:
            if (!Mission_LevelObjectivesDone(5)) {
                pl->unkC00 = 1;
            }
            break;
        case 6000:
            if (!Mission_LevelObjectivesDone(6)) {
                pl->unkC00 = 1;
            }
            break;
        case 7100:
            if (Mission_LevelObjectivesDone(1)) {
                if (pl->keys[0]) {
                    warp = 1999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 1000;
            }
            break;
        case 7200:
            if (Mission_LevelObjectivesDone(2)) {
                if (pl->keys[1]) {
                    warp = 2999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 2000;
            }
            break;
        case 7300:
            if (Mission_LevelObjectivesDone(3)) {
                if (pl->keys[2]) {
                    warp = 3999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 3000;
            }
            break;
        case 7400:
            if (Mission_LevelObjectivesDone(4)) {
                if (pl->keys[3]) {
                    if (pl->keys[5]) {
                        warp = 4999;
                    } else {
                        warp = 7401;
                    }
                }
            } else {
                pl->unkC00 = 1;
                warp = 4000;
            }
            break;
        case 7500:
            if (Mission_LevelObjectivesDone(5)) {
                if (pl->keys[4]) {
                    if (pl->keys[6]) {
                        warp = 5999;
                    } else {
                        warp = 7501;
                    }
                }
            } else {
                pl->unkC00 = 1;
                warp = 5000;
            }
            break;
        case 7601:
            if (Mission_LevelObjectivesDone(6)) {
                if (pl->keys[7]) {
                    warp = 6999;
                }
            } else {
                pl->unkC00 = 1;
                warp = 6000;
            }
            break;
        }
    }
    if (respawn) {
        pl->unkC00 = 0;
    }

    switch (warp) {
    case 1000:
        pl->hubsVisited |= 1;
        break;
    case 2000:
        pl->hubsVisited |= 2;
        break;
    case 3000:
        pl->hubsVisited |= 4;
        break;
    case 4000:
        pl->hubsVisited |= 8;
        break;
    case 5000:
        pl->hubsVisited |= 0x10;
        break;
    case 6000:
        pl->hubsVisited |= 0x20;
        break;
    }

    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        count = CScene__FindWarpDests(&gScene, -1, warp, recs, 32);
        best = -1;
        bestDist = 0.0f;
        for (i = 0; i < count; i++) {
            dist = sumDistance(&recs[i]);
            if (bestDist <= dist) {
                bestDist = dist;
                best = i;
            }
        }
        if (best != -1) {
            pl->spawnPos = recs[best].m_vPos;
            pl->pos = pl->spawnPos;
            pl->angle = pl->spawnAngle = recs[best].m_RotY;
            pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, recs[best].m_nID);
            CPlayer__LevelStart(pl);
            return;
        }
    } else {
        do { /* a block of its own: it decides which of the two addresses gets $s0 */
            if (CScene__FindWarpDests(&gScene, -1, warp, &rec, 1) && rec.m_nLevel == gScene.m_nLevel &&
                (gEngineApp_m_Game.m_Settings.multiplayer || gScene.m_bStreamAll)) {
                pl->spawnPos = rec.m_vPos;
                pl->pos = pl->spawnPos;
                pl->angle = pl->spawnAngle = rec.m_RotY;
                pl->room = pl->spawnRoom = CScene__GetRegion(&gScene, rec.m_nID);
                CPlayer__LevelStart(pl);
                return;
            }
        } while (0);
    }
    CScene__RequestWarp(&gScene, warp, arg2);
}
void CPlayer__Dismount(CPlayer *player, CGameObjectInstance *obj);
void GameObject_DropPickupAt(CPlayer *player, CVector3 pos, s32 id, f32 arg);
s32 Sound__Play2D(s32);

static __inline__ s32 fallingFast(CPlayer *player) {
    return player->m_DuckOffset > 5.12f;
}

static __inline__ s32 inWater(CPlayer *player) {
    return player->m_Mode == 14;
}

/* Kills the player: drops the weapon in multiplayer and plays a death
   animation picked from the cause (`flags`) unless `anim` is given. */
void CPlayer__Kill(CPlayer *player, sptr arg1, s32 flags, s32 anim) {
    s16 weapon;
    s16 mode;

    Controller_SetRumbleOption(player->m_ContState.m_pContState, 0);
    CPlayer__Dismount(player, arg1);
    GameObject_FireTriggers(arg1, 1, arg1);
    CPlayer__StopLoopingSounds(player);
    if (gEngineApp_m_Settings.multiplayer != 0) {
        weapon = player->m_Data.m_CurrentWeapon;
        if (weapon != 32 && weapon != 24 && weapon != 34 && player->m_pOptions->m_TagMode == 0) {
            GameObject_DropPickupAt(player, player->m_CharacterActor.header.m_vPos, gWeaponInfoTable[weapon]->pickupId, 150.0f);
        }
        player->m_ShockedTimer = 0.0f;
        player->m_CharacterActor.m_dwFlags &= ~0x800000;
        if (player->m_pOptions->m_TagMode == 1) {
            Sound__Play2D(0x27D9);
        }
    }
    if (anim != -1) {
        CPlayer__SetState(player, arg1, anim);
    } else if (fallingFast(player)) {
        CPlayer__SetState(player, arg1, 0x19);
    } else {
        mode = player->m_Mode;
        if (inWater(player)) {
            CPlayer__SetState(player, arg1, 0x1E);
        } else if (mode == 8) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 9) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 10) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (mode == 11) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (flags & 0x2000000) {
            CPlayer__SetState(player, arg1, 0x1C);
        } else if (flags & 0x1000000) {
            CPlayer__SetState(player, arg1, 0x18);
        } else if (flags & 0x400000) {
            CPlayer__SetState(player, arg1, 0x16);
        } else {
            CPlayer__SetState(player, arg1, 0x13);
        }
    }
}

/* The camera pool gEngineApp_m_Game_m_CameraPool (CCameraPool) seen to the settings gEngineApp_m_Settings
 * (+0x1228): the ROM reads them through this symbol, and through gEngineApp_m_Settings
 * the code no longer matches. */
extern CGameFromCameraPool gCameraPoolGameView __asm__("gEngineApp_m_Game_m_CameraPool"); /* gEngineApp_m_Game_m_CameraPool (declared by globals/camera.h) seen as this file's type: what keeps the ROM's code */
struct CCameraText *CCameraPool__AddText(void *, void *, u8 *);
void CCamera__SetHitFlash(void *camera, s32 alpha, s32 time);
s32 Sound__Play(s16, CVector3, sptr, sptr);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Hurts a player: damage (8.8) scaled by the reduced-damage state and the
 * multiplayer handicap, unless the player is protected; then the red flash,
 * the low-health warning, the rumble and the hurt sound. */
void CPlayer__Hurt(CPlayer *p, HitSource *src, s32 damage, s32 arg3, s32 type) {
    s32 old;
    f32 f;
    s32 check;
    CRegionInfo *info;
    s32 prev;
    s32 health;
    s32 dead;
    s32 wasDead;
    s32 alpha;
    s32 time;
    s32 amount;
    CVector3 zero;

    f = 1.0f;
    old = p->m_Data.m_Health;
    if (Cinema_IsActive()) {
        return;
    }
    if (CPlayer__IsInMatchEndState(p)) {
        return;
    }
    if (gScene.m_WarpState) {
        return;
    }
    if ((gCameraPoolGameView.m_Settings.cheatFlags & 1) && type != 0x1D) {
        return;
    }
    if ((u16)p->m_Mode < 2) {
        return;
    }
    if (p->m_pOnEnemy) {
        damage /= 5;
    }
    if (gCameraPoolGameView.m_Settings.multiplayer) {
        if (p->m_pOptions->m_Handicap) {
            damage = damage * (1.0f / 256) / ((p->m_pOptions->m_Handicap + f) * 0.3f) * 256.0f;
        }
    }
    if (gCameraPoolGameView.m_Settings.cheatFlags & 0x10) {
        check = 1;
    } else {
        check = p->m_Data.m_SpecialPickupCount[384] & 0xF;
    }
    if (check && p->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = CScene__GetRegionInfo(&gScene, ((void *)p->m_CharacterActor.header.m_pCurrentRegion));
        if (info != NULL && (info->m_dwFlags & 0x400000)) {
            return;
        }
    }
    if (p->m_InvincibilityTime > 0.0f && type != 0x1D) {
        return;
    }
    prev = p->m_Data.m_Health;
    wasDead = prev == 0;
    health = prev - damage;
    if (health < 0) {
        health = 0;
    }
    p->m_CharacterActor.m_AI.m_Health = (p->m_Data.m_Health = health);
    p->m_WeaponActor.m_AI.m_Health = p->m_Data.m_Health;
    dead = p->m_Data.m_Health == 0;
    if (dead) {
        if (!wasDead) {
            CPlayer__Kill(p, src, arg3, type);
        }
        alpha = 0xFF;
        time = 0xC;
    } else {
        alpha = ((damage >> 8) * 0xFF) / 20;
        alpha = (MIN(alpha, 0xFF) < 0x80) ? 0x80 : MIN(alpha, 0xFF);
        time = 0x1E;
        if (old > 0xF00 && p->m_Data.m_Health <= 0xF00 && !gCameraPoolGameView.m_Settings.multiplayer) {
            CCameraPool__AddText(&gCameraPoolGameView, ((void *)p->m_pCamera), sPlayerHurtText_LowHealth);
            Sound__PlaySpeech(0x156);
        }
        amount = damage >> 8;
        if (amount != 0) {
            f = MIN(amount * (1.0f / 15), 1.0f);
            zero.x = 0;
            zero.y = 0;
            zero.z = 0;
            CScene__StartRumbles(&gScene, p->m_ContState.m_pContState->rumblePool, 0x11, zero, 51.199997f, f);
        }
    }
    if (!dead && (s16) (u16)p->m_Mode != 0xF) {
        CCamera__SetHitFlash(((void *)p->m_pCamera), alpha & 0xFF, time);
    }
    if ((old >> 8) != (p->m_Data.m_Health >> 8)) {
        if (gCameraPoolGameView.m_Settings.multiplayer) {
            if (p->m_pOptions->m_TagMode == 1) {
                Sound__Play2D(0x27DA);
            }
        } else if (!(p->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
            Sound__Play(0x3AC0, src->pos, 0, -1);
        }
    }
}

void CGameObjectInstance__Draw(void *, void *);

/* Draws the player's model: picks it (single player, tag mode or the
   character), then sets up the render state before CGameObjectInstance__Draw. */
void CPlayer__DrawCharacter(CPlayer *player, sptr arg1) {
    s32 model;
    Gfx *g;

    if (gEngineApp_m_Settings.multiplayer == 0) {
        model = 0x66;
    } else if (gEngineApp_m_Settings_match_tag[0] != 0 && player->m_pOptions->m_TagMode != 0) {
        model = gTagModeModel;
    } else {
        model = gCharacterModels[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(&gScene, player, model, player->m_CharAnim);
    g = gDList++;
    g->w0 = 0xE7000000;
    g->w1 = 0;
    g = gDList++;
    g->w0 = 0xE3000A01;
    g->w1 = 0x100000;
    g = gDList++;
    g->w0 = 0xD9FFFFFF;
    g->w1 = 0x10000;
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 2;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 2;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFE;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFE;
    CGameObjectInstance__Draw(player, arg1);
}

void CEngineApp__ClearDepthRect(void *, s32, s32, s32, s32);

/* Draws the weapon's HUD overlay, scissored to its screen box. */
void CPlayer__DrawWeaponOverlay(CPlayer *player, CCamera *view) {
    WeaponInfo *def;
    CCameraViewport *vp;
    f32 sx, sy;

    if (player->m_Data.m_CurrentWeapon == -1 || player->m_Data.m_Lives == 0) {
        return;
    }
    GFX_CMD(0xE3001201, 0x2000)
    GFX_CMD(0xE3000C00, 0x80000)
    Render_InvalidateModeCache();
    def = gWeaponInfoTable[player->m_Data.m_CurrentWeapon];
    vp = &view->m_Viewport;
    sx = vp->m_Width;
    sy = vp->m_Height;
    CEngineApp__ClearDepthRect(&gEngineApp,
                  def->scopeLeft * sx + vp->m_ScreenPosX,
                  def->scopeRight * sx + vp->m_ScreenPosX,
                  def->scopeTop * sy + vp->m_ScreenPosY,
                  def->scopeBottom * sy + vp->m_ScreenPosY);
    GFX_CMD(0xD9FFFFFF, 0x10000)
    GFX_CMD(0xDB040004, 1)
    GFX_CMD(0xDB04000C, 1)
    GFX_CMD(0xDB040014, 0xFFFF)
    GFX_CMD(0xDB04001C, 0xFFFF)
    Render_ResetBatches();
    CGameObjectInstance__Draw(&player->m_WeaponActor, view);
    Render_FlushBatches();
}

s32 AI_IsLineOfSightBlocked(CPlayer *, void *, CGameObjectInstance *);

/* Whether the player's weapon may lock on to a target. */
s32 CPlayer__CanLockOn(CPlayer *player, CGameObjectInstance *t) {
    s32 weapon = player->m_Data.m_CurrentWeapon;
    s32 busy;
    s32 ok;

    if (weapon == 13 || weapon == 29) {
        if (t->m_dwFlags & 0x300000) {
            if (!(t->header.m_pIntelligence->m_dwTypeFlags & 0x20)) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0x23) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0xF) {
                return 0;
            }
            if (t->m_AI.m_Health == 0) {
                return 0;
            }
            busy = 0;
            if (((CPlayer *) t)->m_pCamera != NULL) {
                busy = ((CPlayer *) t)->m_pCamera->m_Menus.count != 0;
            }
            if (!busy) {
                goto visible;
            }
            return 0;
        }
        if (t->header.m_pIntelligence->m_Type == 1) {
            if (!(t->header.m_pIntelligence->m_dwTypeFlags & 0x2400)) {
                return 0;
            }
            if (t->m_AI.m_Mode == 0x21 || t->m_AI.m_Mode == 0x34 || t->m_AI.m_Mode == 0x3C || t->m_AI.m_Mode == 0x3D) {
                return 0;
            }
            if (t->m_AI.m_Health != 0) {
                return AI_IsLineOfSightBlocked(player, &player->m_CharacterActor.m_AI, t) == 0;
            }
            return 0;
        }
    }
    switch (t->header.m_pIntelligence->m_Type) {
    case 1:
        if (t->m_AI.m_Mode == 0x3C) {
            return 0;
        }
        if (weapon == 0x10) {
            goto visible;
        }
        return 1;
    case 4:
        if (t->m_AI.m_Health != 0) {
        visible:
            return AI_IsLineOfSightBlocked(player, &player->m_CharacterActor.m_AI, t) == 0;
        }
        return 0;
    case 7:
        return 0;
    case 11:
        ok = 0;
        if (t->m_AI.m_Mode != 0x23 && t->m_AI.m_Mode != 0xF && t->m_AI.m_Health != 0) {
            busy = 0;
            if (((CPlayer *) t)->m_pCamera != NULL) {
                busy = ((CPlayer *) t)->m_pCamera->m_Menus.count != 0;
            }
            if (!busy) {
                ok = 1;
            }
        }
        return ok;
    }
    return 0;
}

/* Weapon auto-aim: finds the target closest to the player's aim direction
 * within the weapon's cone and returns the rotation that turns the aim
 * toward it (identity when there is none). */



typedef struct AimActor AimActor;
#ifdef TARGET_PORT
struct AimActor {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_AimActor(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_AimActor(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_height[T2_HOFF_AimActor(m_CharacterActor.m_CollisionYOffset)]; f32 height; }; /* N64 0x54 */
        struct { u8 _hp_aimPos[T2_HOFF_AimActor(m_CharacterActor.m_AI.m_vBoreTarget)]; CVector3 aimPos; }; /* N64 0x214 */
        struct { u8 _hp_options[T2_HOFF_AimActor(m_pOptions)]; CPlayerOptions *options; }; /* N64 0x518 */
        struct { u8 _hp_view[T2_HOFF_AimActor(m_pCamera)]; CCamera *view; }; /* N64 0x51C */
        struct { u8 _hp_weapon[T2_HOFF_AimActor(m_Data.m_CurrentWeapon)]; s16 weapon; }; /* N64 0x996 */
        struct { u8 _hp_lockState[T2_HOFF_AimActor(m_CrossHairMode)]; s32 lockState; }; /* N64 0xB2C */
        struct { u8 _hp_lockTimer[T2_HOFF_AimActor(m_CrossHairTimer)]; f32 lockTimer; }; /* N64 0xB30 */
        struct { u8 _hp_lockTarget[T2_HOFF_AimActor(m_pCrossHairTarget)]; AimActor *lockTarget; }; /* N64 0xB38 */
        struct { u8 _hp_hold[T2_HOFF_AimActor(m_CrossHairIcon)]; s32 hold; }; /* N64 0xB3C */
        struct { u8 _hp_aimPoint[T2_HOFF_AimActor(m_vCrossHairPos)]; CVector3 aimPoint; }; /* N64 0xB40 */
        struct { u8 _hp_aimVel[T2_HOFF_AimActor(m_vCrossHairVel)]; CVector3 aimVel; }; /* N64 0xB4C */
        struct { u8 _hp_seenTarget[T2_HOFF_AimActor(m_pSightTarget)]; AimActor *seenTarget; }; /* N64 0xB58 */
        struct { u8 _hp_range[T2_HOFF_AimActor(m_SightIcon)]; s32 range; }; /* N64 0xB5C */
        struct { u8 _hp_viewPoint[T2_HOFF_AimActor(m_vSightPos)]; CVector3 viewPoint; }; /* N64 0xB64 */
        struct { u8 _hp_viewVel[T2_HOFF_AimActor(m_vSightVel)]; CVector3 viewVel; }; /* N64 0xB70 */
        struct { u8 _hp_aimRot[T2_HOFF_AimActor(m_qAutoAim)]; CQuaternion aimRot; }; /* N64 0xB7C */
        struct { u8 _hp_unkB8C[T2_HOFF_AimActor(m_SniperMode)]; s32 unkB8C; }; /* N64 0xB8C */
        struct { u8 _hp_unkCC4[T2_HOFF_AimActor(m_Overlay.m_Mode)]; s32 unkCC4; }; /* N64 0xCC4 */
        struct { u8 _hp_next[T2_HOFF_AimActor(m_pNext)]; AimActor *next; }; /* N64 0x1320 */
    };
};
#else
struct AimActor {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ u8 pad10[0x54 - 0x10];
    /* 0x0054 */ f32 height;
    /* 0x0058 */ u8 pad58[0x214 - 0x58];
    /* 0x0214 */ CVector3 aimPos;
    /* 0x0220 */ u8 pad220[0x518 - 0x220];
    /* 0x0518 */ CPlayerOptions *options;
    /* 0x051C */ CCamera *view;
    /* 0x0520 */ u8 pad520[0x996 - 0x520];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xB2C - 0x998];
    /* 0x0B2C */ s32 lockState;
    /* 0x0B30 */ f32 lockTimer;
    /* 0x0B34 */ u8 padB34[4];
    /* 0x0B38 */ AimActor *lockTarget;
    /* 0x0B3C */ s32 hold;
    /* 0x0B40 */ CVector3 aimPoint;
    /* 0x0B4C */ CVector3 aimVel;
    /* 0x0B58 */ AimActor *seenTarget;
    /* 0x0B5C */ s32 range;
    /* 0x0B60 */ u8 padB60[4];
    /* 0x0B64 */ CVector3 viewPoint;
    /* 0x0B70 */ CVector3 viewVel;
    /* 0x0B7C */ CQuaternion aimRot;
    /* 0x0B8C */ s32 unkB8C;
    /* 0x0B90 */ u8 padB90[0xCC4 - 0xB90];
    /* 0x0CC4 */ s32 unkCC4;
    /* 0x0CC8 */ u8 padCC8[0x1320 - 0xCC8];
    /* 0x1320 */ AimActor *next;
};
#endif

CVector3 *CGameObjectInstance__LocalToWorld(CVector3 *, void *, CVector3);
f32 func_002974C0(f32);
f32 func_002982F0(f32);
f32 func_00298470(f32);
f32 Math_AcosLookup(f32);
f32 GameObject_GetCollisionHeight(void *);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *out, CVector3 *a, CVector3 *b);
f32 CVector3__Normalize(CVector3 *);
void CQuaternion__Multiply(CQuaternion *out, CQuaternion *a, CQuaternion *b);
void CVector3__Scale(CVector3 *out, CVector3 *a, f32 s);
void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

CQuaternion Player_FindAutoAimTarget(AimActor *pl, AimActor **outTarget, CVector3 *outPos) {
    CVector3 tpos;
    CVector3 dir;
    CVector3 eye;
    CVector3 bestPos;
    CVector3 fwd;
    CVector3 off;
    CVector3 bestDir;
    CVector3 axis;
    CQuaternion turn;
    CQuaternion rot;
    CQuaternion result;
    AimActor *best;
    AimActor *t;
    f32 cone;
    f32 bestDot;
    f32 dot;
    f32 s;
    f32 angle;
    f32 theta;
    f32 half;
    s32 i;
    s32 n;

    off.x = 0.0f;
    off.y = 0.0f;
    off.z = 1.0f;
    CGameObjectInstance__LocalToWorld(&fwd, pl, off);
    eye = pl->pos;
    eye.y += Actor_GetEyeHeight(pl);
    cone = gWeaponInfoTable[pl->weapon]->aimCone;
    best = NULL;
    bestDot = func_002974C0(cone * 0.5f);
    bestPos.x = 0.0f;
    bestPos.y = 0.0f;
    bestPos.z = 0.0f;
    bestDir.x = 0.0f;
    bestDir.y = 0.0f;
    bestDir.z = 0.0f;

    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        for (t = (AimActor *)gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead; t != NULL; t = t->next) {
            if (CPlayer__CanLockOn(pl, t) && t != pl) {
                if (pl->weapon == 29) {
                    tpos = t->aimPos;
                } else {
                    tpos = t->pos;
                    tpos.y += t->height;
                    tpos.y += GameObject_GetCollisionHeight(t) * 0.75f;
                }
                CVector3__Subtract(&dir, &tpos, &eye);
                CVector3__Normalize(&dir);
                dot = fwd.x * dir.x + fwd.y * dir.y + fwd.z * dir.z;
                if (bestDot < dot) {
                    bestDot = dot;
                    bestPos = tpos;
                    best = t;
                }
            }
        }
    }

    n = gScene.m_nRegistered;
    for (i = 0; i < n; i++) {
        t = gScene.m_pRegistered[i];
        if (CPlayer__CanLockOn(pl, t)) {
            if (pl->weapon == 13) {
                tpos = t->aimPos;
            } else {
                tpos = t->pos;
                tpos.y += t->height;
                tpos.y += GameObject_GetCollisionHeight(t) * 0.75f;
            }
            CVector3__Subtract(&dir, &tpos, &eye);
            CVector3__Normalize(&dir);
            dot = fwd.x * dir.x + fwd.y * dir.y + fwd.z * dir.z;
            if (bestDot < dot) {
                bestDot = dot;
                best = t;
                bestPos = tpos;
                bestDir = dir;
            }
        }
    }

    *outTarget = best;
    *outPos = bestPos;
    if (best != NULL) {
        CVector3__Cross(&axis, &fwd, &bestDir);
        CVector3__Normalize(&axis);
        theta = Math_AcosLookup(bestDot);
        half = 0.5f;
        angle = theta * func_00298470(func_00298470(1.0f - theta / (cone * half))) * half;
        s = func_002982F0(angle);
        turn.x = axis.x * s;
        turn.y = axis.y * s;
        turn.z = axis.z * s;
        gQuaternionSin = s;
        turn.w = func_002974C0(angle);
        rot = CGameObjectInstance__GetRotation(pl);
        CQuaternion__Multiply(&result, &rot, &turn);
    } else {
        result.x = result.y = result.z = 0.0f;
        result.w = 1.0f;
    }
    return result;
}

/* Per-frame weapon aiming: the aim rotation, the auto-aim target lock
 * (acquire, hold, switch target) and the two spring-damped aim points. */
void Player_UpdateAim(AimActor *pl) {
    CVector3 tpos;
    CVector3 off;
    CVector3 delta;
    CVector3 aimDir;
    AimActor *target;
    CCamera *view;
    s32 canLock;
    s32 hold;
    s32 range;
    f32 stiff;
    f32 damp;
    f32 switchTime;
    f32 lockTime;

    view = pl->view;
    target = NULL;
    off.x = 0.0f;
    off.y = 0.0f;
    off.z = -1024.0f;
    CMtxF__TransformPoint(view->m_mfViewOrient, &off, &aimDir);
    canLock = TRUE;
    stiff = 0.080000006f;
    damp = 0.52f;
    hold = gWeaponInfoTable[pl->weapon]->aimHold;
    range = hold + 100;

    switch (pl->weapon) {
    case 13:
        lockTime = switchTime = 15.0f;
        break;
    case 16:
        lockTime = switchTime = 7.5f;
        break;
    case 29:
        lockTime = switchTime = 11.25f;
        break;
    default:
        lockTime = switchTime = 0.0f;
        break;
    }

    switch (pl->weapon) {
    case 10:
    case 27:
        if (pl->unkB8C == 0) {
            break;
        }
        if (pl->unkCC4 != 2) {
            break;
        }
        hold = 740;
        range = -1;
        break;
    case 3:
        if (pl->unkCC4 == 2) {
            break;
        }
        hold = 730;
        range = -1;
        break;
    case 13:
    case 16:
    case 29:
        Player_FindAutoAimTarget(pl, &target, &tpos);
        canLock = FALSE;
        break;
    default:
        range = -1;
        break;
    }

    pl->aimRot = CGameObjectInstance__GetRotation(pl);
    if (target == NULL) {
        tpos = aimDir;
        if (pl->options->m_bAutoAim && canLock) {
            CVector3 tpos2;
            CQuaternion rot2;
            AimActor *target2;

            rot2 = Player_FindAutoAimTarget(pl, &target2, &tpos2);
            if (target2 != NULL) {
                pl->aimRot = rot2;
            }
        }
    }

    if (hold == -1) {
        pl->lockState = 0;
    } else if (pl->lockState == 0) {
        pl->lockState = 1;
        pl->aimPoint = aimDir;
        pl->aimVel.x = 0.0f;
        pl->aimVel.y = 0.0f;
        pl->aimVel.z = 0.0f;
        pl->viewPoint = aimDir;
        pl->viewVel.x = 0.0f;
        pl->viewVel.y = 0.0f;
        pl->viewVel.z = 0.0f;
    }

    CVector3__Subtract(&delta, &tpos, &pl->viewPoint);
    CVector3__Scale(&delta, &delta, stiff * 1.25f);
    CVector3__Add(&pl->viewVel, &pl->viewVel, &delta);
    CVector3__Add(&pl->viewPoint, &pl->viewPoint, &pl->viewVel);
    CVector3__Scale(&pl->viewVel, &pl->viewVel, damp * 1.25f);

    if (target != NULL) {
        pl->range = gWeaponInfoTable[pl->weapon]->lockHold + 100;
    } else {
        pl->range = range;
    }

    switch (pl->lockState) {
    case 0:
        tpos = aimDir;
        break;
    case 1:
        if (target == NULL || pl->lockTarget != target) {
            pl->lockTarget = target;
            pl->lockTimer = 0.0f;
        }
        pl->lockTimer += gFrameIncrement;
        if (lockTime <= pl->lockTimer) {
            pl->lockState = 2;
            pl->lockTimer = 0.0f;
        }
        tpos = aimDir;
        break;
    case 2:
        if (pl->seenTarget != target) {
            pl->seenTarget = target;
            pl->lockTimer = 0.0f;
        } else {
            pl->lockTimer += gFrameIncrement;
        }
        if (target != NULL && pl->lockTarget != target && switchTime <= pl->lockTimer) {
            pl->lockTarget = target;
            pl->lockTimer = 0.0f;
        }
        target = pl->lockTarget;
        if (target != NULL) {
            if (!CPlayer__CanLockOn(pl, target)) {
                pl->lockState = 1;
                pl->lockTimer = 0.0f;
            }
            if (pl->weapon == 13 || pl->weapon == 29) {
                tpos = target->aimPos;
            } else {
                tpos = target->pos;
                tpos.y += target->height;
                tpos.y += GameObject_GetCollisionHeight(target) * 0.75f;
            }
            hold = gWeaponInfoTable[pl->weapon]->lockHold;
        } else {
            tpos = aimDir;
        }
        break;
    }

    CVector3__Subtract(&delta, &tpos, &pl->aimPoint);
    CVector3__Scale(&delta, &delta, stiff);
    CVector3__Add(&pl->aimVel, &pl->aimVel, &delta);
    CVector3__Add(&pl->aimPoint, &pl->aimPoint, &pl->aimVel);
    CVector3__Scale(&pl->aimVel, &pl->aimVel, damp);
    pl->hold = hold;
}

void CCamera__WorldToScreen(CCamera *, f32 *, f32 *, f32 *);
s32 Hud_DrawSprite(s32, s32, s16, s16, f32, f32, s32);

/* Draws a sprite at a world position projected to the screen (or centred
 * for the HUD sprites 0x2BC, 0x2DA and 0x2E4). */
void Hud_DrawWorldSprite(CCamera *view, f32 *pos, s32 sprite, f32 frame, f32 scale, f32 size) {
    f32 x;
    f32 y;
    f32 half;

    if (sprite == 0x2DA || sprite == 0x2E4 || sprite == 0x2BC) {
        x = view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width * 0.5f;
        y = view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height * 0.5f;
    } else {
        /* the last view-volume plane: points in front have dot <= dist */
        if (!(view->m_ViewVolume[5].m_vNormal.x * pos[0] + view->m_ViewVolume[5].m_vNormal.y * pos[1] + view->m_ViewVolume[5].m_vNormal.z * pos[2] <= view->m_ViewVolume[5].m_Dist)) {
            return;
        }
        CCamera__WorldToScreen(view, pos, &x, &y);
        if (x < -64.0f || (f32) (gScreenWidth + 64) < x || y < -64.0f || (f32) (gScreenHeight + 64) < y) {
            return;
        }
        if (gEngineApp_m_Settings.multiplayer != 0) {
            x = view->m_Viewport.m_ScreenPosX + x / gScreenWidth * view->m_Viewport.m_Width;
            y = view->m_Viewport.m_ScreenPosY + y / gScreenHeight * view->m_Viewport.m_Height;
        }
    }
    half = scale * size - 1.0f;
    Hud_DrawSprite(sprite, frame, x - half, y - half, size, size, 1);
}

void Hud_SetupTextureMode(s32);

/* Draws the player's two target markers. */
void CPlayer__DrawTargetMarkers(CPlayer *player, void *view) {
    s32 sprite;
    u8 show;
    f32 scale;

    player->m_SightIconFrame += gFrameIncrement;
    if (player->m_CrossHairMode == 0 || player->m_pOptions->m_TagMode == 1) {
        return;
    }
    Hud_SetupTextureMode(0xFF);

    sprite = player->m_SightIcon;
    if (sprite == 0x2DA && gEngineApp_m_Settings_multiplayer != 0) {
        if (gEngineApp_m_Settings_match_tag[0] == 0) {
            if (gEngineApp_m_Settings_match_teams != 0) {
                show = gEngineApp_m_Settings.teamCrosshairs;
            } else {
                show = gEngineApp_m_Settings.dmCrosshairs;
            }
        } else {
            show = gEngineApp_m_Settings.tagCrosshairs;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2E4:
        case 0x320:
        case 0x32A:
        case 0x32B:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            Hud_DrawWorldSprite(view, &player->m_vSightPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }

    sprite = player->m_CrossHairIcon;
    if (sprite == 0x2DA && gEngineApp_m_Settings_multiplayer != 0) {
        if (gEngineApp_m_Settings_match_tag[0] == 0) {
            if (gEngineApp_m_Settings_match_teams != 0) {
                show = gEngineApp_m_Settings.teamCrosshairs;
            } else {
                show = gEngineApp_m_Settings.dmCrosshairs;
            }
        } else {
            show = gEngineApp_m_Settings.tagCrosshairs;
        }
    } else {
        show = 1;
    }
    if (sprite != -1) {
        switch (sprite) {
        case 0x2DA:
            scale = 8.0f;
            break;
        case 0x2BC:
        case 0x2C6:
        case 0x2C7:
        case 0x2E4:
        case 0x320:
            scale = 32.0f;
            break;
        default:
            scale = 16.0f;
            break;
        }
        if (show) {
            Hud_DrawWorldSprite(view, &player->m_vCrossHairPos, sprite, player->m_SightIconFrame, scale, 1.0f);
        }
    }
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021E5D4.c, CGameObjectInstance__InitAIType.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2 layout (Rage Wars: pos 0x8, timer 0x678, fx 0x67C..0x684, flags 0x6B0). */
#ifdef TARGET_PORT
/* The port: the same views at the host offsets of CPlayer (m_CharacterActor.header.m_vPos,
 * m_FlashlightTime, m_pDLFlashlight1/2, m_pDLDarkCityLight, m_ContState.m_dwButtonsSingle) and
 * of CDLight (structs/particle.h). */
#include "structs/player.h"
#include "structs/particle.h"
#define B05_OFF(m) __builtin_offsetof(CPlayer, m)
typedef struct B05P14 {
    u8 pad0[B05_OFF(m_CharacterActor.header.m_vPos)];
    CVector3 pos;
    u8 pad10[B05_OFF(m_FlashlightTime) - B05_OFF(m_CharacterActor.header.m_vPos) - sizeof(CVector3)];
    f32 timer;
    u8 padT[B05_OFF(m_pDLFlashlight1) - B05_OFF(m_FlashlightTime) - sizeof(f32)];
    void *fx1;
    void *fx2;
    void *fx3;
    u8 padA40[B05_OFF(m_ContState.m_dwButtonsSingle) - B05_OFF(m_pDLDarkCityLight) - sizeof(void *)];
    u32 flagsA68;
} B05P14;
typedef struct B05Fx { void *pad0[2]; void *unk8; f32 unkC; s16 unk10; s16 unk12; s16 unk14; s16 unk16; } B05Fx;
#else
typedef struct B05P14 {
    u8 pad0[0x4];
    CVector3 pos;              /* 0x004 */
    u8 pad10[0xA30 - 0x10];
    f32 timer;              /* 0xA30 */
    void *fx1;              /* 0xA34 */
    void *fx2;              /* 0xA38 */
    void *fx3;              /* 0xA3C */
    u8 padA40[0xA68 - 0xA40];
    u32 flagsA68;           /* 0xA68 */
} B05P14;
typedef struct B05Fx { u8 pad0[8]; void *unk8; f32 unkC; s16 unk10; s16 unk12; s16 unk14; s16 unk16; } B05Fx;
#endif

void CDLightSet__Free(void *, void *);
s32 Random_Next();
void *CDLightSet__Alloc(void *, void *);

/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline void fxupd(B05P14 *p, void **slot, u8 *tab, f32 offz, f32 k1, f32 k2) {
    B05Fx *fx = *slot;
    CVector3 off;
    CVector3 v;
    if (fx == ((void *)0)) {
        fx = CDLightSet__Alloc((u8 *) &gScene.m_DLightSet, tab);
        if (fx == ((void *)0)) {
            return;
        }
    }
    *slot = fx;
    off.x = 0.0f;
    off.y = 0.0f;
    off.z = offz;
    CGameObjectInstance__LocalToWorld(&v, p, off);
    CVector3__Add(&v, &v, &p->pos);
    fx->unk16 = 1;
    fx->unk10 = v.x;
    fx->unk12 = v.y + GameObject_GetCollisionHeight(p) * k1;
    fx->unk14 = v.z;
    fx->unkC = k2;
}

void CPlayer__UpdateFlashlight(B05P14 *p, void *body) {
    s32 vis;
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        p->timer = 0.0f;
    } else {
        if (p->fx3 != ((void *)0)) {
            CDLightSet__Free((u8 *) &gScene.m_DLightSet, p->fx3);
            p->fx3 = ((void *)0);
        }
    }
    if (p->timer != 0.0f) {
        fxupd(p, &p->fx1, sFlashlightLight1, 204.79999f, 0.5f, 1.0f);
        fxupd(p, &p->fx2, sFlashlightLight2, 614.39996f, 0.5f, 1.0f);
        if (p->timer < 70.0f) {
            vis = 1;
            if (p->timer < 20.0f) {
                vis = ((Random_Next() % 100) < 0x32) ^ vis;
            } else if ((Random_Next() % 100) < 0xF) {
                vis = 0;
            }
            if (p->fx1 != ((void *)0)) {
                ((B05Fx *)p->fx1)->unk16 = vis;
            }
            if (p->fx2 != ((void *)0)) {
                ((B05Fx *)p->fx2)->unk16 = vis;
            }
        }
    } else {
        if (p->fx1 != ((void *)0)) {
            CDLightSet__Free((u8 *) &gScene.m_DLightSet, p->fx1);
            p->fx1 = ((void *)0);
        }
        if (p->fx2 != ((void *)0)) {
            CDLightSet__Free((u8 *) &gScene.m_DLightSet, p->fx2);
            p->fx2 = ((void *)0);
        }
    }
    if (gEngineApp_m_Settings.cheatFlags & 0x4000) {
        fxupd(p, &p->fx3, sDarkCityLight, 0.0f, 0.5f, 1.0f);
        if ((p->flagsA68 & 0x20) && p->fx3 != ((void *)0)) {
            if (((B05Fx *)p->fx3)->unk8 == sDarkCityLight) {
                ((B05Fx *)p->fx3)->unk8 = sDarkCityLight2;
            } else {
                ((B05Fx *)p->fx3)->unk8 = sDarkCityLight;
            }
        }
    }
}

/* Draws the weapon menu's icons: sorts the listed items, places each one in
 * the player's viewport (on the wheel, or in a vertical strip that fades out
 * away from the middle), tints it (pulsing under the cursor, dark when the
 * weapon is not owned, dimmer without ammo) and draws the sprite. */

#define MAX(a, b) ((a) < (b) ? (b) : (a))

void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
void Hud_GetSpriteSize(s32 sprite, s32 frame, s32 *width, s32 *height);
typedef struct {
    /* 0x0 */ u8 pad0[0xC];
    /* 0xC */ f32 key;
} Sortable;
s32 CBarrel__CompareIcons(Sortable *arg0, Sortable *arg1);
typedef struct {
    /* 0x00 */ s32 data[7];
} Record1C; /* size 0x1C */
void CBarrel__SwapIcons(Record1C *a, Record1C *b);

void CBarrel__DrawIcons(CBarrel *menu, CBarrelIcon *items, s32 count, CPlayer *player) {
    s32 width;
    s32 height;
    CBarrelIcon *item;
    CCamera *view;
    CCameraViewport *viewport;
    f32 fade;
    f32 scaleX;
    f32 scaleY;
    f32 x;
    f32 y;
    f32 alpha;
    f32 scale;
    s32 level;
    s32 half;
    s32 red;
    s32 green;
    s32 blue;

    Sort_Quick(items, count, sizeof(CBarrelIcon), CBarrel__CompareIcons, CBarrel__SwapIcons);
    level = gEngineApp_m_Settings.hudAlpha + 0x40;
    fade = menu->m_Open * (level < 0x100 ? level : 255.0f);
    item = items;
    while (--count != -1) {
        view = player->m_pCamera;
        scaleX = view->m_Viewport.m_Width * (1.0f / 320);
        scaleY = view->m_Viewport.m_Height * (1.0f / 240);
        viewport = &view->m_Viewport;
        if (menu->m_bStickWheel == 0) {
            half = viewport->m_Height * 0.5f;
            x = viewport->m_ScreenPosX + viewport->m_Width * 0.2f * menu->m_Open;
            y = viewport->m_ScreenPosY + half;
            y += item->m_vPos.y * 1.2f * viewport->m_Height;
            if (item->m_vPos.z < 0.0f) {
                alpha = 0.0f;
            } else {
                alpha = MAX(1.0f - ABS(y - (viewport->m_ScreenPosY + half)) / half, 0.0f);
            }
            scale = alpha * 0.4f + 0.6f;
            scaleX *= scale;
            scaleY *= scale;
        } else {
            x = viewport->m_ScreenPosX + viewport->m_Width * 0.5f + item->m_vPos.x * 0.5f * viewport->m_Width;
            y = viewport->m_ScreenPosY + viewport->m_Height * 0.5f + item->m_vPos.y * 0.5f * viewport->m_Height;
            alpha = item->m_Alpha;
        }
        Hud_GetSpriteSize(item->m_Icon, 0, &width, &height);
        if (width != 0) {
            if (menu->m_nCursor == item->m_Item) {
                red = func_002974C0(menu->m_Glow) * 63.0f + 192.0f;
                alpha = 1.0f;
                green = blue = red;
            } else {
                if (player->m_Data.m_WeaponList[menu->m_Items[item->m_Item].m_Weapon] == 0) {
                    blue = green = red = 8;
                    alpha *= 0.25f;
                } else if (menu->m_Items[item->m_Item].m_bAvailable == 0) {
                    blue = green = red = 0x40;
                    alpha *= 0.9f;
                } else {
                    blue = green = red = 200;
                }
            }
            {
                Gfx *_g = gDList++;
                _g->w0 = 0xFB000000;
                _g->w1 = (red << 24) | ((green & 0xFF) << 16) | ((blue & 0xFF) << 8) |
                         ((u32)(alpha * fade) & 0xFF);
            }
            Hud_DrawSprite(item->m_Icon, 0, x - width * item->m_Size * scaleX * 0.5f,
                          y - height * item->m_Size * scaleY * 0.5f, scaleX * item->m_Size,
                          scaleY * item->m_Size, 1);
        }
        item++;
    }
}

f32 Math_Approach(f32 value, f32 target, f32 rate);
void CMtxF__Identity(f32 *m);

/* Lays out the weapon menu's slots around the wheel and draws them. */
void CBarrel__DrawRing(CBarrel *menu, sptr arg1, sptr arg2) {
    f32 m[16];
    CVector3 position;
    CVector3 offset;
    CBarrelIcon items[36];
    CBarrelIcon *item;
    CBarrelItem *slot;
    s32 count;
    s32 i;
    f32 target;

    if (menu->m_State == 0) {
        return;
    }
    item = items;
    count = 0;
    for (i = 0; i < menu->m_nCount; i++) {
        slot = &menu->m_Items[i];
        if (i == menu->m_nCursor) {
            target = gBarrelSlotScale * 1.3f;
        } else {
            target = gBarrelSlotScale;
            slot->m_Lift = Math_Approach(slot->m_Lift, 0.0f, 0.35f);
        }
        slot->m_Scale = Math_Approach(slot->m_Scale, target, gBarrelSlotScale * 0.1f);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.75f;
        CMtxF__Identity(m);
        CMtxF__RotateX(m, menu->m_Spin - slot->m_Angle);
        CMtxF__TransformPoint(m, &offset, &position);
        count++;
        item->m_Item = i;
        item->m_vPos = position;
        item->m_Icon = gWeaponInfoTable[slot->m_Weapon]->icon;
        item->m_Alpha = 0;
        item->m_Size = 1.0f;
        item++;
    }
    CBarrel__DrawIcons(menu, items, count, arg2);
}
s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
s32 CBarrel__HasUnlistedWeapon(CBarrel *list, CPlayer *player);
void CBarrel__OpenWheel(CBarrel *menu, CPlayer *player);
void Angle_Wrap(f32 *);

/* Runs the weapon menu: marks the weapons the player can use, steps the
 * cursor with the next/previous weapon buttons and turns the wheel towards
 * the selected slot. */
void CBarrel__RunButtons(CBarrel *menu, CPlayer *player, u32 *input) {
    s32 step;
    s32 i;
    s32 dir;
    f32 from;
    f32 to;
    f32 diff;
    f32 turn;

    dir = 0;
    if (menu->m_State == 1 || menu->m_State == 2) {
        for (i = 0; i < menu->m_nCount; i++) {
            if (CPlayer__GetWeaponAmmo(player, menu->m_Items[i].m_Weapon) != 0) {
                menu->m_Items[i].m_bAvailable = 1;
            } else {
                menu->m_Items[i].m_bAvailable = 0;
            }
        }
    }
    if (CPlayer__IsInMatchEndState(player) != 0) {
        dir = 0;
    } else if (player->m_ContState.m_dwButtonsSingle & 0x4000) {
        dir = 1;
    } else if (player->m_ContState.m_dwButtonsSingle & 0x8000) {
        dir = -1;
    }
    if ((menu->m_State == 0 && dir != 0)
        || (CBarrel__HasUnlistedWeapon(menu, player) != 0 && menu->m_State != 0)
        || (menu->m_nCursor != -1 && menu->m_Items[menu->m_nCursor].m_Weapon != player->m_RequestedWeapon)) {
        CBarrel__OpenWheel(menu, player);
    }
    if (dir != 0) {
        do {
            menu->m_nCursor += dir;
            if (menu->m_nCursor >= menu->m_nCount) {
                menu->m_nCursor = 0;
            } else if (menu->m_nCursor < 0) {
                menu->m_nCursor = menu->m_nCount - 1;
            }
        } while (menu->m_Items[menu->m_nCursor].m_bAvailable == 0);
        player->m_RequestedWeapon = menu->m_Items[menu->m_nCursor].m_Weapon;
        menu->m_Step = -dir;
        menu->m_Turn = 45.0f;
        if (menu->m_State == 0 || menu->m_State == 3) {
            menu->m_State = 1;
        }
    }
    if (menu->m_State != 0) {
        from = menu->m_Spin;
        to = menu->m_Items[menu->m_nCursor].m_Angle;
        step = menu->m_Step;
        Angle_Wrap(&from);
        Angle_Wrap(&to);
        if (step > 0) {
            if (to < from) {
                to += 6.283186f;
            }
        } else if (to > from) {
            to -= 6.283186f;
        }
        do {
            diff = to - from;
            turn = diff * 0.25f * gFrameIncrement;
            if (ABS(turn) > ABS(diff)) {
                turn = diff;
            }
        } while (0);
        menu->m_Spin += turn;
    }
}

#define ABS(a) (((a) < 0.0f) ? -(a) : (a))

/* Returns the 8-way stick direction (0 = up, clockwise) or -1 inside the dead zone. */
s32 CBarrel__GetStickDirection(sptr unused, CPlayer *player) {
    ControllerProfile *pad = player->m_ContState.m_pContState;
    s8 x = pad->stickX;
    s8 y = pad->stickY;

    if (x * x + y * y < 3600) {
        return -1;
    }
    if (ABS(ABS(x) - ABS(y)) < 46 && x != 0 && y != 0 && !gEngineApp_m_Settings.multiplayer) {
        if (x * y > 0) {
            if (x > 0) {
                return 1;
            }
            /* Redundant re-read kept from the original (Rage Wars has it too): it shapes the branch layout. */
            if (pad->stickX != 0) {
                return 5;
            }
            return 5;
        }
        return y > 0 ? 7 : 3;
    }
    if (ABS(x) > ABS(y)) {
        return x <= 0 ? 6 : 2;
    }
    if (y > 0) {
        return 0;
    }
    return 4;
}

/* A slot seen from the menu's base: menu + i * 0x18 (as Rage Wars' Block24). */
typedef struct MenuRow {
    /* 0x00 */ u8 pad0[0x1C];
    /* 0x1C */ union {
        s32 id;
        struct {
            s16 hi;
            s16 lo;
        } half;
    } weapon;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ s32 available;
} MenuRow;
void CBarrel__LayoutEight(CBarrel *menu, s32 group, CPlayer *player);

/* Runs the player's weapon menu each frame: the menu buttons open or
 * rebuild it, and the slot under the stick becomes the hovered entry,
 * moving the cursor and choosing the slot's weapon (or its alternate the
 * player holds). */
void CBarrel__RunStick(CBarrel *menu, CPlayer *player, u32 *input) {
    s32 open;
    s32 next;
    s32 slot;
#ifndef TARGET_PORT
    MenuRow *entry;
#endif
    s16 weapon;
    s16 alternate;

    open = player->m_ContState.m_pContState->buttons & 0x8000;
    next = player->m_ContState.m_pContState->buttons & 0x4000;
    if (CPlayer__IsInMatchEndState(player) != 0) {
        open = next = 0;
    }
    if (menu->m_State == 0 || menu->m_State == 3) {
        if (!open && !next) {
            return;
        }
        CBarrel__LayoutEight(menu, open, player);
        player->m_bHeadMovementDisabled = 1;
        menu->m_State = 1;
    } else {
        if (menu->m_bUnderwater != 0) {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                goto toggle;
            }
        } else if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        toggle:
            CBarrel__LayoutEight(menu, open, player);
        }
        if (open && next && menu->m_bBothHeld == 0) {
            if (menu->m_Group != 0) {
                CBarrel__LayoutEight(menu, 0, player);
            } else {
                CBarrel__LayoutEight(menu, 1, player);
            }
            menu->m_bBothHeld = 1;
        } else if (!open || !next) {
            menu->m_bBothHeld = 0;
            if (menu->m_Group != 0) {
                if (!open) {
                    goto checkNext;
                }
            } else {
                if (!open) {
                    goto stick;
                }
                goto call;
            checkNext:
                if (next) {
                call:
                    CBarrel__LayoutEight(menu, open, player);
                }
            }
        }
    }
stick:
    slot = CBarrel__GetStickDirection(menu, player);
    if (slot == menu->m_Hover) {
        return;
    }
#ifdef TARGET_PORT
    if (slot != -1) {
        /* the slot as CBarrelItem: MenuRow puts m_Items at the N64 offset 0x1C (0x20 on the host,
         * after the pointer-wide m_pInfo the item is 0x20 bytes) and reads the weapon as the low
         * half of a big-endian word (half.lo): on the host that is the high half, 0 */
        CBarrelItem *item = &menu->m_Items[slot];

        if (item->m_bAvailable == 0) {
            Sound__Play2D(0x1455);
            return;
        }
        if (item->m_Weapon == -1) {
            menu->m_Hover = -1;
            return;
        }
        if (slot != menu->m_nCursor) {
            menu->m_nCursor = slot;
        }
        weapon = (s16) item->m_Weapon;
        if (weapon == player->m_Data.m_CurrentWeapon) {
            menu->m_Hover = slot;
            return;
        }
        player->m_RequestedWeapon = weapon;
        if (*input & 0x80) {
            menu->m_Hover = slot;
            return;
        }
        alternate = gWeaponInfoTable[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            item->m_Weapon = alternate;
        }
    }
#else
    if (slot != -1) {
        entry = (MenuRow *) ((u8 *) menu + slot * sizeof(CBarrelItem));
        if (entry->available == 0) {
            Sound__Play2D(0x1455);
            return;
        }
        if (entry->weapon.id == -1) {
            menu->m_Hover = -1;
            return;
        }
        if (slot != menu->m_nCursor) {
            menu->m_nCursor = slot;
        }
        weapon = entry->weapon.half.lo;
        if (weapon == player->m_Data.m_CurrentWeapon) {
            menu->m_Hover = slot;
            return;
        }
        player->m_RequestedWeapon = entry->weapon.half.lo;
        if (*input & 0x80) {
            menu->m_Hover = slot;
            return;
        }
        alternate = gWeaponInfoTable[weapon]->alternate;
        if (alternate != -1 && player->m_Data.m_WeaponList[alternate] != 0) {
            entry->weapon.id = alternate;
        }
    }
#endif
    menu->m_Hover = slot;
}
void CBarrel__Close(CBarrel *arg0);
void Rumble_SetLink(struct RumbleEffect *effect, struct RumbleEffect **owner);
s32 CBarrel__ShouldClose(void *, void *);

/* Updates the player's weapon menu: runs it (by stick or by buttons, from
 * the player's control style), then slides it open (state 1), keeps it
 * open (2) until CBarrel__ShouldClose closes it, and slides it shut (3). */
void CBarrel__Update(CBarrel *menu, CPlayer *player, u32 *input) {
    if (player->m_ModeFlags & 0x8000) {
        return;
    }
    if (player->m_Data.m_nDeathmatchCharacter == 10) {
        return;
    }
    if (player->m_pOptions->m_TagMode != 0) {
        return;
    }
    if (Cinema_IsActive() != 0) {
        return;
    }
    menu->m_bStickWheel = player->m_pOptions->m_WeaponSelect == 1;
    if (gEngineApp_m_Game_m_MenuPool.head != 0 && menu->m_State != 0) {
        CBarrel__Close(menu);
        return;
    }
    if (menu->m_bStickWheel) {
        CBarrel__RunStick(menu, player, input);
    } else {
        CBarrel__RunButtons(menu, player, input);
    }
    if (player->m_RequestedWeapon != player->m_Data.m_CurrentWeapon && player->m_pRumble != NULL) {
        Rumble_SetLink(player->m_pRumble, NULL);
        player->m_pRumble = NULL;
    }
    switch (menu->m_State) {
    case 1:
        menu->m_Open += gFrameIncrement * 0.25f;
        if (menu->m_Open >= 1.0f) {
            menu->m_Open = 1.0f;
            menu->m_State = 2;
        }
        break;
    case 2:
        if (CBarrel__ShouldClose(menu, player) == 0) {
            break;
        }
        menu->m_State = 3;
        player->m_bHeadMovementDisabled = 0;
        /* fall through */
    case 3:
        menu->m_Open -= gFrameIncrement * 0.25f;
        if (menu->m_Open <= 0.0f) {
            menu->m_Open = 0.0f;
            menu->m_State = 0;
        }
        break;
    }
    if (menu->m_State != 0) {
        menu->m_Glow += gFrameIncrement * 0.52359885f;
    } else {
        player->m_bHeadMovementDisabled = 0;
    }
}

/* Fills the weapon menu with the weapons flagged in `owned` (35 entries,
 * `rows` times over), spreading the slots evenly around the wheel. */
void CBarrel__Fill(CBarrel *menu, s32 rows, s32 *owned, s32 perRow, CPlayer *player) {
    s32 i;
    s32 n;
    s32 w;
    WeaponInfo *info;
    s32 ok;

    menu->m_nCount = 0;
    for (i = 0; i < perRow * rows; i++) {
#ifdef TARGET_PORT
        /* soak S9b: the slot layout loop overruns the same way (2 rows of 24: 48 slots; the
         * 37th..48th wrote angles and scales over the player's HUD overlays) */
        if (i >= (s32) (sizeof(menu->m_Items) / sizeof(menu->m_Items[0]))) {
            break;
        }
#endif
        menu->m_Items[i].m_Angle = -(i * (360.0f / (perRow * rows)) * 0.017453294f);
        menu->m_Items[i].m_Lift = 0;
        menu->m_Items[i].m_Scale = gBarrelSlotScale;
    }
    n = 0;
    for (i = 0; i < rows; i++) {
        for (w = 0; w < 35; w++) {
#ifdef TARGET_PORT
            /* soak S9: rows * count slots can pass the 36 of m_Items (CBarrel__Open picks 2 rows for
             * up to 26 weapons: 19+ weapons in the strip wheel, e.g. T2_GIVE_WEAPONS): the slots past
             * 36 overran the CBarrel into the player, and the items[36] of CBarrel__DrawRing (stack) */
            if (n >= (s32) (sizeof(menu->m_Items) / sizeof(menu->m_Items[0]))) {
                break;
            }
#endif
            if (owned[w] != 0) {
                info = gWeaponInfoTable[w];
                ok = CPlayer__GetWeaponAmmo(player, w) != 0;
                menu->m_Items[n].m_Weapon = w;
                menu->m_Items[n].m_bAvailable = ok;
                menu->m_Items[n].m_pInfo = info;
                if (menu->m_nCursor == -1 && w == player->m_RequestedWeapon) {
                    menu->m_nCursor = n;
                    menu->m_Spin = menu->m_Items[n].m_Angle;
                }
                n++;
                menu->m_nCount++;
            }
        }
    }
}

/* Opens the weapon menu: flags the weapons the player can pick (an owned
 * weapon usable in the current mode, unless it is the lower of a pair the
 * player owns both of), lays them out on one or two rings, then steps the
 * cursor by CREDITS.md	   Makefile	   docs		scripts  turok2.us.toml
Dockerfile	   README.md	   include	src	 us
Dockerfile.native  baserom.us.z64  nonmatching	test	 versions
LICENSE		   build	   references	tools until it rests on the queued weapon. */
void CBarrel__Open(CBarrel *menu, s32 dir, CPlayer *player) {
    s32 owned[35];
    s32 count;
    s32 i;
    s32 range;
    s32 rows;
    s32 ok;
    WeaponInfo *info;
    s32 alt;

    count = 0;
    range = gScene_m_PreviewTimer - gScene_m_PreviewState + 1;
    for (i = 0; i < 35; i++) {
        info = gWeaponInfoTable[i];
        if (player->m_Data.m_WeaponList[i] == 0) {
            ok = 0;
        } else if (!((player->m_WeaponActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            ok = 0;
        } else {
            alt = info->alternate;
            ok = 1;
            if (player->m_Data.m_WeaponList[alt] != 0) {
                ok = !(i < alt);
            }
        }
        owned[i] = ok;
        if (ok) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    CBarrel__Fill(menu, rows, owned, count, player);
    menu->m_Turn = 45.0f;
    if (menu->m_State == 0 || menu->m_State == 3) {
        menu->m_State = 1;
    }
    menu->m_Step = -dir;
    menu->m_Glow = 0;
    while (menu->m_Items[menu->m_nCursor].m_Weapon != player->m_RequestedWeapon) {
        menu->m_nCursor += dir;
        if (menu->m_nCursor < 0) {
            menu->m_nCursor = menu->m_nCount - 1;
        } else if (menu->m_nCursor >= menu->m_nCount) {
            menu->m_nCursor = 0;
        }
    }
}

/* Lays out the eight-slot weapon wheel from the layout table for :
 * each weapon becomes its alternate when the player owns that (and is not
 * holding it), the held weapon gets the cursor, and the slots are set
 * 45 degrees apart. */
void CBarrel__LayoutEight(CBarrel *menu, s32 group, CPlayer *player) {
    s32 *layout;
    s32 i;
    s32 w;
    s16 alt;
    s32 ok;

    if (group != 0) {
        layout = sBarrelLayoutGroup1;
        menu->m_bUnderwater = 0;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutGroup1MP;
        }
    } else {
        layout = sBarrelLayoutGroup0;
        menu->m_bUnderwater = 0;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutGroup0MP;
        }
    }
    if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        layout = sBarrelLayoutUnderwater;
        if (gEngineApp_m_Settings_multiplayer != 0) {
            layout = sBarrelLayoutUnderwaterMP;
        }
        menu->m_bUnderwater = 1;
    }
    menu->m_Group = group;
    menu->m_nCursor = -1;
    for (i = 0; i < 8; i++) {
        w = layout[i];
        if (w == -1) {
            menu->m_Items[i].m_Weapon = w;
            menu->m_Items[i].m_bAvailable = 0;
        } else {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                alt = gWeaponInfoTable[w]->alternate;
                if (alt != -1 && player->m_Data.m_WeaponList[alt] != 0 && player->m_Data.m_CurrentWeapon != alt) {
                    w = alt;
                }
            }
            menu->m_Items[i].m_Weapon = w;
            if (player->m_Data.m_CurrentWeapon == w) {
                menu->m_nCursor = i;
            }
            menu->m_Items[i].m_pInfo = gWeaponInfoTable[w];
            if (CPlayer__GetWeaponAmmo(player, w) == 0 || player->m_Data.m_WeaponList[w] == 0) {
                ok = 0;
            } else {
                ok = 1;
            }
            menu->m_Items[i].m_bAvailable = ok;
            menu->m_Items[i].m_Lift = 0;
            menu->m_Items[i].m_Scale = gBarrelSlotScale;
            menu->m_Items[i].m_Angle = i * 0.785398245f;
        }
    }
    menu->m_bStickWheel = 1;
}

#ifdef TARGET_PORT
#include "globals/menu.h"
#endif

/* Opens the weapon wheel: picks the weapons to show (owned, allowed here,
 * not superseded by an owned upgrade) and how many times round to repeat
 * them so that the slots are at most 20 degrees apart. */
void CBarrel__OpenWheel(CBarrel *menu, CPlayer *player) {
    s32 shown[0x23];
    s32 count = 0;
    s32 range = gScene_m_PreviewTimer - gScene_m_PreviewState + 1;
    s32 rows;
    s32 slots;
    s32 i;

    for (i = 0; i < 0x23; i++) {
        WeaponInfo *info = gWeaponInfoTable[i];
        s32 show;

        if (!player->m_Data.m_WeaponList[i]) {
            show = 0;
        } else if (!((player->m_WeaponActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            show = 0;
        } else {
            s16 upgrade = info->alternate;

            show = 1;
            if (player->m_Data.m_WeaponList[upgrade]) {
                show = !(i < upgrade);
            }
        }
        shown[i] = show;
        if (show) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    slots = rows * count;
    while (360 / slots > 20) {
        slots += count;
        rows++;
    }
    CBarrel__Fill(menu, rows, shown, count, player);
}

/* An animated HUD item (COverlay__Update ticks it, COverlay__StartScript restarts it). */

void COverlay__Update(COverlay *item);
void COverlay__StartScript(COverlay *item, s32 *script);
void CBarrel__DrawEight(CBarrel *menu, sptr arg1, sptr arg2);
s32 CPlayer__GetCurrentAmmoInfo(CPlayer *player);
void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32, s32 rightAlign, s32 minDigits);
void CPlayer__DrawSightIcon(void *, void *);

/* The next display list command. Through this helper, as in the ROM, the
 * head is loaded and stored by symbol; a post-increment written in place
 * keeps the address of the head in a register instead. */
static __inline__ Gfx *dl_next(void) {
    return gDList++;
}

/* Draw a player's HUD: tick the item animations, the crosshair and strokes,
 * then the ammo (with the charge gauge of weapon 0xDD), the health, the
 * lives, the keys in single player, and the score and frags in multiplayer. */
void CPlayer__DrawHud(CPlayer *player, CCamera *view) {
    f32 sx;
    f32 sy;
    f32 shift;
    f32 x;
    f32 y;
    s32 ammo;
    s32 icon;
    s32 frame;
    s32 lives;
    s32 showAmmo;
    s32 value;
    CBarrel *strokes;

    showAmmo = 1;
    if (gEngineApp_m_Settings.nextMode == 0xB || gEngineApp_m_Settings.nextMode == 8) {
        return;
    }
    COverlay__Update(&player->m_AmmoOverlay);
    COverlay__Update(&player->m_HealthOverlay);
    COverlay__Update(&player->m_LivesOverlay);
    COverlay__Update(&player->m_TokensOverlay);
    COverlay__Update(&player->m_TotalKillsOverlay);
    COverlay__Update(&player->m_TotalTagsOverlay);
    CPlayer__DrawTargetMarkers(player, view);
    if (view->m_ViewParams.m_nType == 0) {
        CPlayer__DrawScope(&player->m_Overlay, view);
    }
    strokes = &player->m_Barrel;
    Hud_SetupTextureMode(0xFF);
    {   /* gDPSetTextureFilter(G_TF_BILERP) */
        Gfx *g = dl_next();

        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    if (strokes->m_bStickWheel) {
        CBarrel__DrawEight(strokes, view, player);
    } else {
        CBarrel__DrawRing(strokes, view, player);
    }
    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    sx = view->m_Viewport.m_Width / (f32)gScreenWidth;
    shift = sx * 32.0f;
    sy = view->m_Viewport.m_Height / (f32)gScreenHeight;

    ammo = CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon);
    if (player->m_AmmoOverlay.m_Value != ammo) {
        COverlay__StartScript(&player->m_AmmoOverlay, gAmmoOverlayChangeScript);
        player->m_AmmoOverlay.m_Value = ammo;
    }
    icon = CPlayer__GetCurrentAmmoInfo(player);
    if (player->m_AmmoOverlay.m_bOnScreen && player->m_AmmoOverlay.m_Mode && icon != -1) {
        x = player->m_AmmoOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
        y = player->m_AmmoOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height;
        Hud_DrawSprite(0x12C, 0, x - shift + sx * 18.0f, y + sy, sx, sy, 1);
        frame = 0;
        if (icon == 0xDD) {
            frame = 8;
            if (!(player->m_WeaponActor.m_AI.m_Weapon.m_RefireDelay <= 0.0f)) {
                frame = (90.0f - player->m_WeaponActor.m_AI.m_Weapon.m_RefireDelay) * 0.08888889f;
            }
            showAmmo = 0;
        } else if (player->m_Data.m_CurrentWeapon == 0x15) {
            showAmmo = 0;
        }
        Hud_DrawSprite(icon, frame, x - shift, y - sy * 8.0f, sx, sy, 1);
        if (showAmmo) {
            Hud_DrawNumber(player->m_AmmoOverlay.m_Value, x - sx * 6.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);
        }
    }

    x = player->m_HealthOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
    y = player->m_HealthOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height;
    Hud_DrawSprite(0x12C, 0, x - shift + sx * 21.0f, y + sy, sx, sy, 1);
    Hud_DrawSprite(0x1F7, 0, x - shift + sx, y - sy * 9.0f, sx, sy, 1);
    Hud_DrawNumber((player->m_Data.m_Health + 0xFF) >> 8, x - sx * 3.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);

    lives = player->m_Data.m_LifeForce;
    if (player->m_TokensOverlay.m_Value != lives) {
        COverlay__StartScript(&player->m_TokensOverlay, gTokensOverlayChangeScript);
        player->m_TokensOverlay.m_Value = lives;
    }
    if (player->m_TokensOverlay.m_bOnScreen && player->m_TokensOverlay.m_Mode) {
        x = player->m_TokensOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
        y = player->m_TokensOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
        Hud_DrawSprite(0x12C, 1, x + sx * -42.0f, y + sy * 24.0f, sx, sy, 1);
        Hud_DrawSprite(0x1F5, 0, x, y + sy * 8.0f, sx, sy, 1);
        Hud_DrawNumber(player->m_Data.m_LifeForce, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
    }

    if (!gEngineApp_m_Settings.multiplayer) {
        value = player->m_Data.m_Lives;
        if (player->m_LivesOverlay.m_Value != value) {
            COverlay__StartScript(&player->m_LivesOverlay, gLivesOverlayChangeScript);
            player->m_LivesOverlay.m_Value = value;
        }
        if (player->m_LivesOverlay.m_bOnScreen && player->m_LivesOverlay.m_Mode) {
            x = player->m_LivesOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
            y = player->m_LivesOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
            Hud_DrawSprite(0x12C, 0, x + sx * 18.0f, y + sy * 28.0f, sx, sy, 1);
            Hud_DrawSprite(0x1F6, 0, x, y, sx, sy, 1);
            Hud_DrawNumber(player->m_Data.m_Lives, x + sx * 30.0f, y + sy * 24.0f, sx, sy, 1, 0, 0);
        }
    }
    if (gEngineApp_m_Settings.multiplayer) {
        value = player->m_pOptions->m_Kills;
        if (player->m_TotalKillsOverlay.m_Value != value) {
            COverlay__StartScript(&player->m_TotalKillsOverlay, gTotalKillsOverlayChangeScript);
            player->m_TotalKillsOverlay.m_Value = value;
        }
        if (player->m_TotalKillsOverlay.m_bOnScreen && player->m_TotalKillsOverlay.m_Mode) {
            x = player->m_TotalKillsOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
            y = player->m_TotalKillsOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
            Hud_DrawSprite(0x1FA, 0, x, y + sy * 8.0f, sx, sy, 1);
            Hud_DrawNumber(player->m_pOptions->m_Kills, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
        }
        if (gEngineApp_m_Settings_match_tag[0]) {
            value = player->m_pOptions->m_Tags;
            if (player->m_TotalTagsOverlay.m_Value != value) {
                COverlay__StartScript(&player->m_TotalTagsOverlay, gTotalTagsOverlayChangeScript);
                player->m_TotalTagsOverlay.m_Value = value;
            }
            if (player->m_TotalTagsOverlay.m_bOnScreen && player->m_TotalTagsOverlay.m_Mode) {
                x = player->m_TotalTagsOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
                y = player->m_TotalTagsOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
                Hud_DrawSprite(0x1FD, 0, x, y + sy * 8.0f, sx, sy, 1);
                Hud_DrawNumber(player->m_pOptions->m_Tags, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
            }
        }
    }
    if (player->m_pOptions->m_TagMode == 1) {
        CPlayer__DrawSightIcon(player, view);
    }
}

typedef struct Quad {
    s32 w[4];
} Quad;
void CCamera__SetViewParams(void *camera, s32 type, f32 a, f32 b, f32 x0, f32 y0, f32 x1, f32 y1, f32 z1,
                   f32 h, CQuaternion quad, struct CGameRegion *region, f32 c, f32 d);

/* Points the camera at : in mode 1 from the view's offsets, in
 * mode 2 orbiting it, the buttons turning the orbit and moving it in or out. */
void CPlayer__UpdateOrbitCamera(CPlayer *player, CGameObjectInstance *target) {
    void *camera;
    f32 angle;
    f32 x;

    camera = player->m_pCamera;
    if (camera == NULL) {
        return;
    }
    if (player->m_bActive == 1) {
        CCamera__SetViewParams(camera, 0, target->m_RotY, player->m_vHeadRotOffset.x + player->m_vHeadWobbleRotOffset.x - player->m_JumpAngle,
                      player->m_vHeadRotOffset.y + player->m_vHeadWobbleRotOffset.y, player->m_vHeadRotOffset.z + player->m_vHeadWobbleRotOffset.z,
                      target->header.m_vPos.x + player->m_vEyeOffset.x, target->header.m_vPos.y, target->header.m_vPos.z + player->m_vEyeOffset.z,
                      MAX(player->m_vEyeOffset.y + player->m_ActualCameraVertEyeOffset - player->m_DuckOffset, 40.96f), target->m_qGround, target->header.m_pCurrentRegion, player->m_ZoomBlend, 3.0f);
    } else if (player->m_bActive == 2) {
        if (player->m_ContState.m_dwButtons & 0x200) {
            sOrbitCameraAngle += 0.069813177f;
        } else if (player->m_ContState.m_dwButtons & 0x100) {
            sOrbitCameraAngle -= 0.069813177f;
        } else if (player->m_ContState.m_dwButtons & 0x800) {
            sOrbitCameraDistance -= 4.096f;
        } else if (player->m_ContState.m_dwButtons & 0x400) {
            sOrbitCameraDistance += 4.096f;
        }
        angle = target->m_RotY + sOrbitCameraAngle;
        x = target->header.m_vPos.x + sOrbitCameraDistance * func_002982F0(angle);
        CCamera__SetViewParams(camera, 1, angle, 0.0f, 0.0f, 0.0f, x, target->header.m_vPos.y + 81.92f,
                      target->header.m_vPos.z + sOrbitCameraDistance * func_002974C0(angle), 0.0f, target->m_qGround, target->header.m_pCurrentRegion, 0.0f, 3.0f);
    }
}

void WeaponActor_SwitchToRequested(void *a, void *b);

/* Picks the player's weapon set (0 on foot, 1 in states 10-11, 2 with
 * flag 0x8000); when it changes, remembers the weapon held in the old set
 * if it belongs there and queues the new set's weapon. */
void CPlayer__UpdateWeaponSet(CPlayer *player) {
    s32 keep;
    s16 weapon;

    player->m_LastFrameAllowedWeaponType = player->m_CurrentAllowedWeaponType;
    if ((u32) ((u16)player->m_Mode - 10) < 2) {
        player->m_CurrentAllowedWeaponType = 1;
    } else if (player->m_ModeFlags & 0x8000) {
        player->m_CurrentAllowedWeaponType = 2;
    } else {
        player->m_CurrentAllowedWeaponType = 0;
    }
    if (player->m_LastFrameAllowedWeaponType == player->m_CurrentAllowedWeaponType) {
        return;
    }
    weapon = player->m_Data.m_CurrentWeapon;
    switch (player->m_LastFrameAllowedWeaponType) {
    case 0:
        if (!(gWeaponInfoTable[weapon]->flags & 1)) {
            goto no;
        }
        keep = 1;
        break;
    case 1:
        if (!(gWeaponInfoTable[weapon]->flags & 2)) {
            goto no;
        }
        keep = 1;
        break;
    case 2:
        if (weapon != 19) {
            goto no;
        }
        keep = 1;
        break;
    default:
    no:
        keep = 0;
        break;
    }
    if (keep) {
        player->m_Data.m_LastWeapon[player->m_LastFrameAllowedWeaponType] = player->m_Data.m_CurrentWeapon;
    }
    player->m_RequestedWeapon = player->m_Data.m_LastWeapon[player->m_CurrentAllowedWeaponType];
    if (player->m_RequestedWeapon == 19) {
        WeaponActor_SwitchToRequested(&player->m_WeaponActor, &player->m_WeaponActor.m_AI);
    }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_8.CPP, func_80220ED4_de, with Turok 2's layouts. */

typedef struct Model {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
} Model;
/* What the actor stands in (GameObject_GetRegionInfo). */
typedef struct Surface {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ u16 item;
    /* 0x4E */ u8 pad4E[0x54 - 0x4E];
    /* 0x54 */ u16 unk54;
} Surface;
typedef struct Controller_24C47C {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ CVector3 pos;
} Controller_24C47C;
#ifdef TARGET_PORT
typedef struct Input {
/* the view at the host offsets of CPlayerContState (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_IN(m) __builtin_offsetof(CPlayerContState, m)
    union {
        struct { u8 _hp_controller[T2_HOFF_IN(m_pContState)]; Controller_24C47C *controller; }; /* N64 0x10 */
        struct { u8 _hp_held[T2_HOFF_IN(m_dwButtons)]; u32 held; }; /* N64 0x24 */
        struct { u8 _hp_pressed[T2_HOFF_IN(m_dwButtonsSingle)]; u32 pressed; }; /* N64 0x28 */
        struct { u8 _hp_unk2C[T2_HOFF_IN(m_dwButtonsSingleNoHold)]; s32 unk2C; }; /* N64 0x2C */
        struct { u8 _hp_unk30[T2_HOFF_IN(m_dwButtonsDouble)]; s32 unk30; }; /* N64 0x30 */
        struct { u8 _hp_unk34[T2_HOFF_IN(m_dwButtonsRepeat)]; s32 unk34; }; /* N64 0x34 */
    };
} Input;
#else
typedef struct Input {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Controller_24C47C *controller;
    /* 0x14 */ u8 pad14[0x24 - 0x14];
    /* 0x24 */ u32 held;
    /* 0x28 */ u32 pressed;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} Input;
#endif
#ifdef TARGET_PORT
typedef struct Voice_24C47C {
/* the view at the host offsets of CAIModeInfo (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_VO(m) __builtin_offsetof(CAIModeInfo, m)
    union {
        struct { u8 _hp_bank[T2_HOFF_VO(unk_10)]; s32 bank; }; /* N64 0x10 */
    };
} Voice_24C47C;
#else
typedef struct Voice_24C47C {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 bank;
} Voice_24C47C;
#endif
/* CPlayer (structs/player.h) as CPlayer__Update reads it, kept local: it uses
 * the player both as itself and as its m_CharacterActor (actor = p), and
 * several words are typed against the header (0x10 the region as a Model,
 * the controller's 0x08 as a position, the weapon actor's 0x3F4 as a voice,
 * u32 flags), which would take a cast at most uses. */
#ifdef TARGET_PORT
typedef struct Player {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PL(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_PL(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_model[T2_HOFF_PL(m_CharacterActor.header.m_pCurrentRegion)]; Model *model; }; /* N64 0x10 */
        struct { u8 _hp_def[T2_HOFF_PL(m_CharacterActor.header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_velY[T2_HOFF_PL(m_CharacterActor.m_vVelocity.y)]; f32 velY; }; /* N64 0x1C */
        struct { u8 _hp_unk40[T2_HOFF_PL(m_CharacterActor.m_qGround)]; Quad unk40; }; /* N64 0x40 */
        struct { u8 _hp_yaw[T2_HOFF_PL(m_CharacterActor.m_RotY)]; f32 yaw; }; /* N64 0x50 */
        struct { u8 _hp_flags[T2_HOFF_PL(m_CharacterActor.m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_anim[T2_HOFF_PL(m_CharacterActor.m_CurrentAnimation.m_nAnimIndex)]; s16 anim; }; /* N64 0xDC */
        struct { u8 _hp_animPending[T2_HOFF_PL(m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted)]; s8 animPending; }; /* N64 0xE2 */
        struct { u8 _hp_aiFlags[T2_HOFF_PL(m_CharacterActor.m_AI.m_dwFlags)]; u32 aiFlags; }; /* N64 0x140 */
        struct { u8 _hp_owner[T2_HOFF_PL(m_CharacterActor.m_AI.m_pParent)]; struct Player *owner; }; /* N64 0x1A8 */
        struct { u8 _hp_emitter[T2_HOFF_PL(m_WeaponActor.header)]; u8 emitter[4]; }; /* N64 0x288 */
        struct { u8 _hp_emitterPos[T2_HOFF_PL(m_WeaponActor.header.m_vPos)]; CVector3 emitterPos; }; /* N64 0x28C */
        struct { u8 _hp_emitterModel[T2_HOFF_PL(m_WeaponActor.header.m_pCurrentRegion)]; Model *emitterModel; }; /* N64 0x298 */
        struct { u8 _hp_emitter40[T2_HOFF_PL(m_WeaponActor.m_qGround)]; Quad emitter40; }; /* N64 0x2C8 */
        struct { u8 _hp_emitterYaw[T2_HOFF_PL(m_WeaponActor.m_RotY)]; f32 emitterYaw; }; /* N64 0x2D8 */
        struct { u8 _hp_unk3C8[T2_HOFF_PL(m_WeaponActor.m_AI)]; u8 unk3C8[0x3F4 - 0x3C8]; }; /* N64 0x3C8 */
        struct { u8 _hp_voice[T2_HOFF_PL(m_WeaponActor.m_AI.m_pModeTable)]; Voice_24C47C *voice; }; /* N64 0x3F4 */
        struct { u8 _hp_unk430[T2_HOFF_PL(m_WeaponActor.m_AI.m_pParent)]; struct Player *unk430; }; /* N64 0x430 */
        struct { u8 _hp_control[T2_HOFF_PL(m_bActive)]; s32 control; }; /* N64 0x510 */
        struct { u8 _hp_options[T2_HOFF_PL(m_pOptions)]; CPlayerOptions *options; }; /* N64 0x518 */
        struct { u8 _hp_character[T2_HOFF_PL(m_Data.m_nDeathmatchCharacter)]; s32 character; }; /* N64 0x520 */
        struct { u8 _hp_health[T2_HOFF_PL(m_Data.m_Health)]; s32 health; }; /* N64 0x524 */
        struct { u8 _hp_collide[T2_HOFF_PL(m_Data.m_Lives)]; s16 collide; }; /* N64 0x52A */
        struct { u8 _hp_unk6A9[T2_HOFF_PL(m_Data.m_SpecialPickupCount[381])]; u8 unk6A9; }; /* N64 0x6A9 */
        struct { u8 _hp_unk6AB[T2_HOFF_PL(m_Data.m_SpecialPickupCount[383])]; u8 unk6AB; }; /* N64 0x6AB */
        struct { u8 _hp_item[T2_HOFF_PL(m_Data.m_CheckPointWarpID)]; s32 item; }; /* N64 0x92C */
        struct { u8 _hp_lastItem[T2_HOFF_PL(m_Data.m_SavePointWarpID)]; s32 lastItem; }; /* N64 0x930 */
        struct { u8 _hp_ammo[T2_HOFF_PL(m_Data.m_AmmoList)]; s16 ammo[31]; }; /* N64 0x934 */
        struct { u8 _hp_owned[T2_HOFF_PL(m_Data) + 1106]; s8 owned[36]; }; /* N64 0x972 */
        struct { u8 _hp_weapon[T2_HOFF_PL(m_Data.m_CurrentWeapon)]; s16 weapon; }; /* N64 0x996 */
        struct { u8 _hp_state[T2_HOFF_PL(m_Mode)]; s16 state; }; /* N64 0xA08 */
        struct { u8 _hp_prevState[T2_HOFF_PL(m_ModeLastFrame)]; s16 prevState; }; /* N64 0xA0C */
        struct { u8 _hp_stateTime[T2_HOFF_PL(m_ModeTime)]; f32 stateTime; }; /* N64 0xA10 */
        struct { u8 _hp_prevStateFlags[T2_HOFF_PL(m_ModeLastFrameFlags)]; u32 prevStateFlags; }; /* N64 0xA14 */
        struct { u8 _hp_stateFlags[T2_HOFF_PL(m_ModeFlags)]; u32 stateFlags; }; /* N64 0xA1C */
        struct { u8 _hp_step[T2_HOFF_PL(m_FrameIncrementScaled)]; f32 step; }; /* N64 0xA24 */
        struct { u8 _hp_unkA28[T2_HOFF_PL(m_InvincibilityTime)]; f32 unkA28; }; /* N64 0xA28 */
        struct { u8 _hp_unkA30[T2_HOFF_PL(m_FlashlightTime)]; f32 unkA30; }; /* N64 0xA30 */
        struct { u8 _hp_input[T2_HOFF_PL(m_ContState)]; Input input; }; /* N64 0xA40 */
        struct { u8 _hp_velX[T2_HOFF_PL(m_Speed)]; f32 velX; }; /* N64 0xA78 */
        struct { u8 _hp_velZ[T2_HOFF_PL(m_SideSpeed)]; f32 velZ; }; /* N64 0xA7C */
        struct { u8 _hp_speed[T2_HOFF_PL(m_TotalSpeed)]; f32 speed; }; /* N64 0xA80 */
        struct { u8 _hp_lastVelY[T2_HOFF_PL(m_YVelLastFrame)]; f32 lastVelY; }; /* N64 0xA84 */
        struct { u8 _hp_onGround[T2_HOFF_PL(m_bOnGround)]; s32 onGround; }; /* N64 0xA88 */
        struct { u8 _hp_airTime[T2_HOFF_PL(m_TimeOffGround)]; f32 airTime; }; /* N64 0xA9C */
        struct { u8 _hp_goal[T2_HOFF_PL(m_vDesiredPos)]; CVector3 goal; }; /* N64 0xAA0 */
        struct { u8 _hp_unkAAC[T2_HOFF_PL(m_FleeTime)]; f32 unkAAC; }; /* N64 0xAAC */
        struct { u8 _hp_crouch[T2_HOFF_PL(m_DuckOffset)]; f32 crouch; }; /* N64 0xAC0 */
        struct { u8 _hp_crouchToggle[T2_HOFF_PL(m_bDuckMode)]; s32 crouchToggle; }; /* N64 0xAC4 */
        struct { u8 _hp_unkAC8[T2_HOFF_PL(m_vHeadRotOffset.x)]; f32 unkAC8; }; /* N64 0xAC8 */
        struct { u8 _hp_unkAFC[T2_HOFF_PL(m_BreathAmp)]; f32 unkAFC; }; /* N64 0xAFC */
        struct { u8 _hp_unkB00[T2_HOFF_PL(m_BreathSpeed)]; f32 unkB00; }; /* N64 0xB00 */
        struct { u8 _hp_nextWeapon[T2_HOFF_PL(m_RequestedWeapon)]; s16 nextWeapon; }; /* N64 0xB14 */
        struct { u8 _hp_lastPos[T2_HOFF_PL(m_vWeaponFirePos)]; CVector3 lastPos; }; /* N64 0xB18 */
        struct { u8 _hp_unkB28[T2_HOFF_PL(m_CameraMovementScaler)]; f32 unkB28; }; /* N64 0xB28 */
        struct { u8 _hp_action[T2_HOFF_PL(m_CharAnim)]; s32 action; }; /* N64 0xC04 */
        struct { u8 _hp_lastAction[T2_HOFF_PL(m_LastCharAnim)]; s32 lastAction; }; /* N64 0xC08 */
        struct { u8 _hp_unkCD0[T2_HOFF_PL(m_Barrel.m_State)]; s32 unkCD0; }; /* N64 0xCD0 */
        struct { u8 _hp_unk1050[T2_HOFF_PL(m_Barrel.m_bStickWheel)]; s32 unk1050; }; /* N64 0x1050 */
        struct { u8 _hp_shake[T2_HOFF_PL(m_ShockedTimer)]; f32 shake; }; /* N64 0x1298 */
        struct { u8 _hp_unk12A4[T2_HOFF_PL(m_UseLowerAmmo)]; s32 unk12A4; }; /* N64 0x12A4 */
        struct { u8 _hp_unk12AC[T2_HOFF_PL(m_pHitMeWithChargeDart)]; s32 unk12AC; }; /* N64 0x12AC */
    };
} Player;
#else
typedef struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ Model *model;
    /* 0x0014 */ CIntelligence *def;
    /* 0x0018 */ u8 pad18[4];
    /* 0x001C */ f32 velY;
    /* 0x0020 */ u8 pad20[0x40 - 0x20];
    /* 0x0040 */ Quad unk40;
    /* 0x0050 */ f32 yaw;
    /* 0x0054 */ u8 pad54[0xD4 - 0x54];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0xDC - 0xD8];
    /* 0x00DC */ s16 anim;
    /* 0x00DE */ u8 padDE[0xE2 - 0xDE];
    /* 0x00E2 */ s8 animPending;
    /* 0x00E3 */ u8 padE3[0x140 - 0xE3];
    /* 0x0140 */ u32 aiFlags;          /* CAI */
    /* 0x0144 */ u8 pad144[0x1A8 - 0x144];
    /* 0x01A8 */ struct Player *owner;
    /* 0x01AC */ u8 pad1AC[0x288 - 0x1AC];
    /* 0x0288 */ u8 emitter[4];
    /* 0x028C */ CVector3 emitterPos;
    /* 0x0298 */ Model *emitterModel;
    /* 0x029C */ u8 pad29C[0x2C8 - 0x29C];
    /* 0x02C8 */ Quad emitter40;
    /* 0x02D8 */ f32 emitterYaw;
    /* 0x02DC */ u8 pad2DC[0x3C8 - 0x2DC];
    /* 0x03C8 */ u8 unk3C8[0x3F4 - 0x3C8];
    /* 0x03F4 */ Voice_24C47C *voice;
    /* 0x03F8 */ u8 pad3F8[0x430 - 0x3F8];
    /* 0x0430 */ struct Player *unk430;
    /* 0x0434 */ u8 pad434[0x510 - 0x434];
    /* 0x0510 */ s32 control;
    /* 0x0514 */ u8 pad514[4];
    /* 0x0518 */ CPlayerOptions *options;
    /* 0x051C */ u8 pad51C[4];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ s32 health;               /* 8.8 */
    /* 0x0528 */ u8 pad528[2];
    /* 0x052A */ s16 collide;
    /* 0x052C */ u8 pad52C[0x6A9 - 0x52C];
    /* 0x06A9 */ u8 unk6A9;
    /* 0x06AA */ u8 pad6AA;
    /* 0x06AB */ u8 unk6AB;
    /* 0x06AC */ u8 pad6AC[0x92C - 0x6AC];
    /* 0x092C */ s32 item;
    /* 0x0930 */ s32 lastItem;
    /* 0x0934 */ s16 ammo[31];
    /* 0x0972 */ s8 owned[36];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ s16 state;
    /* 0x0A0A */ u8 padA0A[2];
    /* 0x0A0C */ s16 prevState;
    /* 0x0A0E */ u8 padA0E[2];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u32 prevStateFlags;
    /* 0x0A18 */ u8 padA18[4];
    /* 0x0A1C */ u32 stateFlags;
    /* 0x0A20 */ u8 padA20[4];
    /* 0x0A24 */ f32 step;
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[4];
    /* 0x0A30 */ f32 unkA30;
    /* 0x0A34 */ u8 padA34[0xA40 - 0xA34];
    /* 0x0A40 */ Input input;
    /* 0x0A78 */ f32 velX;
    /* 0x0A7C */ f32 velZ;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ f32 lastVelY;
    /* 0x0A88 */ s32 onGround;
    /* 0x0A8C */ u8 padA8C[0xA9C - 0xA8C];
    /* 0x0A9C */ f32 airTime;
    /* 0x0AA0 */ CVector3 goal;
    /* 0x0AAC */ f32 unkAAC;
    /* 0x0AB0 */ u8 padAB0[0xAC0 - 0xAB0];
    /* 0x0AC0 */ f32 crouch;
    /* 0x0AC4 */ s32 crouchToggle;
    /* 0x0AC8 */ f32 unkAC8;
    /* 0x0ACC */ u8 padACC[0xAFC - 0xACC];
    /* 0x0AFC */ f32 unkAFC;
    /* 0x0B00 */ f32 unkB00;
    /* 0x0B04 */ u8 padB04[0xB14 - 0xB04];
    /* 0x0B14 */ s16 nextWeapon;
    /* 0x0B16 */ u8 padB16[2];
    /* 0x0B18 */ CVector3 lastPos;
    /* 0x0B24 */ u8 padB24[4];
    /* 0x0B28 */ f32 unkB28;
    /* 0x0B2C */ u8 padB2C[0xC04 - 0xB2C];
    /* 0x0C04 */ s32 action;
    /* 0x0C08 */ s32 lastAction;
    /* 0x0C0C */ u8 padC0C[0xCD0 - 0xC0C];
    /* 0x0CD0 */ s32 unkCD0;
    /* 0x0CD4 */ u8 padCD4[0x1050 - 0xCD4];
    /* 0x1050 */ s32 unk1050;
    /* 0x1054 */ u8 pad1054[0x1298 - 0x1054];
    /* 0x1298 */ f32 shake;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 unk12A4;
    /* 0x12A8 */ u8 pad12A8[4];
    /* 0x12AC */ s32 unk12AC;
} Player;
#endif

void Sound__SetMuted(s32 enable);
void AI_UpdateRegionFlags(Player *, u32 *ai);
void CGameObjectInstance__UpdateAnimation(void *);
void AI_RideMovingPlatform(Player *, u32 *ai);
void *GameObject_GetRegionInfo(void *);
void CPlayer__UpdateSpeedScale(void *, void *, void *);
f32 CGameRegion__GetCeilingHeight(void *, f32, f32);
f32 CGameRegion__GetHeight(sptr, f32, f32);
void Sound__SetReverb(f32);
void CPlayer__EaseEyeOffset(CPlayer *arg0, sptr arg1);
f32 Random_Float(f32);
void DamageEvent_Set(DamageEvent *, void *, s32, f32, s32, void *);
s32 GameObject_IsOnGround(void *);
void GameObject_PlayLandSound(Player *);
s32 CPlayer__CheckJumpPad(CPlayer *player, void *actor);
s32 CPlayer__CheckWater(CPlayer *player, CGameObjectInstance *actor);
s32 CPlayer__CheckState12Hit(CPlayer *player, CGameObjectInstance *actor);
s32 CPlayer__CheckClimb(CPlayer *player, CGameObjectInstance *actor);
void GameObject_ApplyCurrent(Player *, Surface *, CVector3 *, s32);
void CPlayer__UpdateWaterRegion(CPlayer *arg0, CGameObjectInstance *arg1, CRegionInfo *arg2);
s32 Collision_MoveInstance(void *, CVector3, void *);
s32 CPlayer__RamWithMount(CPlayer *player, CGameObjectInstance *body, u32 *stateFlags);
s32 CPlayer__TryMount(CPlayer *p, CGameObjectInstance *body, u32 *stateFlags);
void CPlayer__TouchPickup(Player *, u8 *);
s32 CGameObjectInstance__SetAnimByAction(Player *, s32, s32);
sptr CGameObjectInstance__PickRandomAnim(Player *, sptr, sptr);
void CGameObjectInstance__SetAnim(Player *, sptr, s32);
void CGameObjectInstance__AlignToGround(Player *);

static __inline__ s32 link_enabled(u8 *bits) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return *bits & 0xF;
}

static __inline__ s32 not_flagged(u32 flags, u32 bit) {
    return (flags & bit) == 0;
}

/* Entering a surface that changes the player's state (flags 0x40000): through
 * a 0x200000 surface only from one without the flag, otherwise only from
 * above the floor. */
static __inline__ void enter_surface(Player *p, Player *actor, Surface *before, Surface *after, Model *model) {
    s32 ok = 0;     /* dead store, but it keeps the 2.5f of the shake out of the loop's hoisted invariants */

    if (p->stateFlags & 0x8000) {
        return;
    }
    if (not_flagged(after->flags, 0x40000)) {
        return;
    }
    if (after->flags & 0x200000) {
        ok = not_flagged(before->flags, 0x40000);
    } else if (CGameRegion__GetHeight(model, actor->pos.x, actor->pos.z) < actor->pos.y) {
        ok = 0;
    } else {
        ok = 1;
    }
    if (ok) {
        CPlayer__Warp(p, after->unk48, (after->unk54 & 1) ? 1 : 2, 0);
    }
}

/* Moves the player one frame: input toggles, health regeneration in
 * multiplayer, then the state machine and collision in a loop over the frame
 * time, the landscape ceiling, the animation for the current action, the
 * cheats (all weapons / ammo) and the weapon sounds. */
void CPlayer__Update(Player *p) {
    CVector3 prev;
    Input saved;
    DamageEvent damage;
    s32 soundFlags;
    s32 human;
    s32 linkFlags;
    f32 scale;
    f32 remaining;
    f32 rate;
    f32 regen;
    Player *actor;
    GameSettings *game;
    s32 enable;
    Model *oldModel;
    Model *model;
    Surface *material;
    Surface *before;
    Surface *under;
    u32 *stateFlags;
    s32 hit;
    s32 landed;
    s32 flag;
    sptr first;
    sptr second;
    sptr low;
    sptr high;
    s32 modelId;
    s32 i;
    s32 j;

    linkFlags = 0;
    scale = gFrameIncrement;
    actor = p;
    soundFlags = 0;
    human = p->control == 2;
    if (Cinema_PathPlaying()) {
        human = 1;
    }
    game = &gEngineApp_m_Settings;
    if (game->multiplayer) {
        human = 1;
        CCache__SetFrameDelay(8);
    }
    enable = 0;
    p->owner = p;
    p->unk430 = p;
    if ((p->aiFlags & 0x80) && game->multiplayer == 0) {
#ifdef TARGET_PORT
        /* the host's blocks are not N64-sized: name the menu pool (round 12 a5, n64viewcheck.py) */
        enable = *(u32 *) &gEngineApp_m_Game_m_MenuPool == 0;
#else
        enable = *(u32 *) ((u8 *) game - 0xCAC) == 0; /* gEngineApp_m_Game_m_MenuPool, the block before */
#endif
    }
    Sound__SetMuted(enable);
    if (gEngineApp_m_Settings.multiplayer == 0) {
        if (!(gEngineApp_m_Game_m_DebugLines.unk0 != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x800)) {
            if (p->unk12A4 != 0) {
                p->unk12A4 = 0;
            } else {
                p->unk12A4 = 1;
            }
        }
        if (gEngineApp_m_Settings.multiplayer == 0 && !(p->unk1050 != 0 && p->unkCD0 != 0)
            && !(gEngineApp_m_Game_m_DebugLines.unk0 != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x200)) {
            WeaponInfo *info = gWeaponInfoTable[p->nextWeapon];
            s16 next = info->alternate;

            if (next != -1 && p->owned[next] != 0) {
                p->nextWeapon = info->alternate;
            }
        }
    }
    regen = 0.0f;   /* also serves the comparisons with 0 (f2 in the ROM) */
    if (gEngineApp_m_Settings.multiplayer != 0 && (rate = gCharacterHealthRegen[p->character]) != 0.0f) {
        f32 health = p->health;

        if (health != 0.0f && p->options->m_TagMode == 0) {
            f32 max;

            regen = health + (f32) (s32) (rate * 256.0f) * (gFrameIncrement * 0.06666667f);
            max = (u32) (p->def->m_Player.m_StartHealth << 8);
            if (!(max <= regen)) {
                max = regen;
            }
            p->health = max;
        }
    }
    actor->flags &= ~0x20000;
    AI_UpdateRegionFlags(actor, &actor->aiFlags);
    if (human) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    gFrameIncrement = scale;
    AI_RideMovingPlatform(actor, &actor->aiFlags);
    if (link_enabled(&p->unk6A9)) {
        linkFlags |= 0x800;
    }
    if (link_enabled(&p->unk6AB)) {
        linkFlags |= 0x1000;
    }
    if (p->options->m_TagMode == 1) {
        linkFlags |= 0x2000;
        soundFlags |= 0x400000;
    }
    Cinema_PathPlaying();
    if (p->unkAAC > 0.0f) {
        f32 left = p->unkAAC - gFrameIncrement;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkAAC = left;
    }
    if (p->unkA28 > 0.0f) {
        f32 left = p->unkA28 - gFrameIncrement;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkA28 = left;
    }
    /* Run the frame in steps (one, here): the input's edges are seen once. */
    saved = p->input;
    for (remaining = gFrameIncrement; remaining > 0.0f;) {
        remaining = 0.0f;
        AI_UpdateRegionFlags(actor, &actor->aiFlags);
        p->prevState = (u16) p->state;
        p->prevStateFlags = p->stateFlags;
        model = actor->model;
        material = GameObject_GetRegionInfo(actor);
        CPlayer__UpdateSpeedScale(p, actor, material);
        if (gEngineApp_m_Settings.multiplayer == 0) {
            CVector3 *pos = &p->pos;

            if (model != NULL && pos != NULL && (model->flags & 0x40)) {
                f32 amount = (s32) (CGameRegion__GetCeilingHeight(model, p->pos.x, pos->z) - CGameRegion__GetHeight(model, p->pos.x, pos->z));

                if (amount < 1024.0f) {
                    Sound__SetReverb(1.0f - amount * 0.0009765625f);
                } else {
                    Sound__SetReverb(0.0f);
                }
            } else {
                Sound__SetReverb(0.0f);
            }
        }
        if (material != NULL) {
            p->step = gFrameIncrement * p->unkB28;
        } else {
            p->step = gFrameIncrement;
        }
        CPlayer__EaseEyeOffset(p, actor);
        p->speed = func_00298470(p->velX * p->velX + p->velZ * p->velZ);
        p->unkB00 += p->speed * 0.021000002f;
        p->unkAFC += p->speed * 1.00000005e-4f;
        p->goal = actor->pos;
        if (p->shake > 0.0f && p->health != 0 && !CPlayer__IsInMatchEndState(p)) {
            p->goal.x += (Random_Float(5.0f) - 2.5f) * 10.24f;
            p->goal.z += (Random_Float(5.0f) - 2.5f) * 10.24f;
            p->goal.y += Random_Float(1.0f) * 10.24f;
            p->yaw += (Random_Float(20.0f) - 10.0f) * 0.017453294f;
            p->unkAC8 += Random_Float(0.5f) - 0.25f;
            if (gEvenOdd != 0) {
                DamageEvent_Set(&damage, p->unk12AC, 0x19, 25.599998f, 0x100000, 0);
                Player_OnMultiplayerKill(p, &p->aiFlags, &damage);
            }
            Sound__Play(0x227, p->pos, 0, -1);
        }
        p->stateTime += gFrameIncrement;
        {
            s32 ground = GameObject_IsOnGround(actor);

            if (ground != 0 && p->onGround == 0 && p->lastVelY < -51.199997f) {
                GameObject_PlayLandSound(actor);
            }
            p->onGround = ground;
            p->lastVelY = actor->velY;
            if (ground == 0 && p->state != 14 && !(actor->aiFlags & 0x200)) {
                p->airTime += gFrameIncrement;
            } else {
                p->airTime = 0.0f;
            }
        }
        if ((u16) p->state >= 2 && p->health != 0) {
            CPlayer__CheckJumpPad(p, actor);
            CPlayer__CheckWater(p, actor);
            CPlayer__CheckState12Hit(p, actor);
            CPlayer__CheckClimb(p, actor);
            CPlayer__UpdateZoom(p);
        }
        if (!Cinema_PathPlaying() || (u32) ((u16) p->state - 19) < 12) {
            void (*update)(Player *, Player *) = (void (*)(Player *, Player *)) gPlayerStateTable[p->state].update;

            if (update != NULL) {
                update(p, actor);
            }
        }
        material = GameObject_GetRegionInfo(actor);
        if (material != NULL) {
            GameObject_ApplyCurrent(actor, material, &p->goal, 1);
            if ((material->flags & 0x4000) && actor->pos.y <= CGameRegion__GetHeight(actor->model, actor->pos.x, actor->pos.z)) {
                CPlayer__Hurt(p, actor, 0x3E700, 0, 0x1D);
            }
        }
        stateFlags = &gPlayerStateTable[p->state].physics->flags;
        if (p->state == 14) {
            stateFlags = &gCollisionInfoNoGravity.m_dwFlags;
            if (p->velX >= 0.0f) {
                stateFlags = &gCollisionInfoGravity.m_dwFlags;
            }
        }
        flag = 0;
        if (actor->aiFlags & 0x6000) {
            flag = 1;
        } else if ((u32) ((u16) p->state - 2) < 2) {
            if ((p->input.pressed & 0x400) && gEngineApp_m_Settings.multiplayer == 0 && !(gEngineApp_m_Game_m_DebugLines.unk0 != 0 && (p->input.held & 0x20))) {
                p->crouchToggle ^= 1;
            }
        } else {
            p->crouchToggle = 0;
        }
        if (p->crouchToggle != 0) {
            flag = 1;
        }
        {
            f32 change;

            if (flag) {
                change = (40.96f - p->crouch) * 0.25f;
            } else {
                change = -p->crouch * 0.25f;
            }
            p->crouch += change * gFrameIncrement;
            change = 40.96f - p->crouch;
            if (-10.24f <= change && change <= 10.24f) {
                *stateFlags |= 0x80;
            } else {
                *stateFlags &= ~0x80;
            }
        }
        CPlayer__UpdateWaterRegion(p, actor, material);
        if (p->unkA30 > 0.0f) {
            p->unkA30 -= gFrameIncrement;
            if (p->unkA30 < 0.0f) {
                p->unkA30 = 0.0f;
            }
        }
        CPlayer__UpdateFlashlight(p, actor);
        oldModel = actor->model;
        prev = actor->pos;
        before = GameObject_GetRegionInfo(actor);
        if (p->collide != 0) {
            actor->flags |= 0x10000;
            hit = Collision_MoveInstance(actor, p->goal, stateFlags);
            actor->flags &= ~0x10000;
        } else {
            hit = 0;
        }
        model = actor->model;
        material = GameObject_GetRegionInfo(actor);
        if (before != material) {
            CGameRegion__FireTriggers(oldModel, linkFlags | 2, actor);
            CGameRegion__FireTriggers(model, linkFlags | 1, actor);
        }
        CScene__CheckAutoActivation(&gScene, actor, &prev, &actor->pos);
        landed = 0;
        if (hit) {
            if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
                GameObject_FireTriggers((u8 *)gCollisionResults.m_pInstance, soundFlags | 2, actor);
            }
            if (gCollisionResults.m_pWallRegion != NULL) {
                CGameRegion__FireTriggers((Model *)gCollisionResults.m_pWallRegion, linkFlags | 4, actor);
            }
            CPlayer__RamWithMount(p, actor, stateFlags);
            landed = CPlayer__TryMount(p, actor, stateFlags);
            if (gCollisionResults.m_nFloorKind == 2 && (u32) ((u16) p->state - 4) < 2) {
                if (actor->velY > 0.0f) {
                    actor->velY = 0.0f;
                }
                p->state = 5;
                p->velZ *= 0.1f;
                p->velX *= 0.1f;
            }
        }
        if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 3) {
            CPlayer__TouchPickup(p, (u8 *)gCollisionResults.m_pInstance);
        }
        if (before != NULL && material != NULL) {
            if ((p->stateFlags & 0x8000) && !landed && !(before->unk54 & 8) && (material->unk54 & 8)) {
                CPlayer__Dismount(p, actor);
            }
            if (material->flags & 0x4000000) {
                if (material->item != p->item) {
                    p->item = material->item;
                }
            }
            if (material->flags & 0x8000000) {
                p->item = material->item;
                p->lastItem = material->item;
                gScene_m_PendingWarpID = material->item;
            }
            enter_surface(p, actor, before, material, model);
        }
        p->input.pressed = 0;
        p->input.unk2C = 0;
        p->input.unk30 = 0;
        p->input.unk34 = 0;
    }
    p->input = saved;
    gFrameIncrement = scale;
    under = GameObject_GetRegionInfo(actor);
    if (under != NULL && (under->flags & 0x4000)) {
        f32 x2 = actor->pos.x * actor->pos.x;
        f32 z2 = actor->pos.z * actor->pos.z;

        if (x2 + actor->pos.y * actor->pos.y + z2 > 4e8f) {
            f32 y = 4e8f - x2 - z2;

            if (y < 0.0f) {
                y = 0.0f;
            }
            y = func_00298470(y);
            if (actor->pos.y < 0.0f) {
                y = -y;
            }
            actor->pos.y = y;
            if (under->flags & 0x4000) {
                actor->pos.y = GameObject_GetRegionFloorHeight(actor, 0) - 1.0f;
            }
        }
    }
    if (human) {
        modelId = 102;
        if (gEngineApp_m_Settings.multiplayer != 0) {
            if (gEngineApp_m_Settings_match_tag[0] != 0 && p->options->m_TagMode != 0) {
                modelId = gTagModeModel;
            } else {
                modelId = gCharacterModels[p->def->m_Id];
            }
        }
        CScene__SetInstanceObjectType(&gScene, p, modelId, p->action);
        if (p->lastAction != p->action) {
            if (!CGameObjectInstance__SetAnimByAction(actor, p->action, 0) && Cinema_PathPlaying()) {
                CGameObjectInstance__SetAnimByAction(actor, 7000, 0);
            }
        }
        first = CGameObjectInstance__PickRandomAnim(actor, p->action, -1);
        second = CGameObjectInstance__PickRandomAnim(actor, p->action, first);
        low = second;
        if (second > first) {
            low = first;
        }
        high = second;
        if (high < first) {
            high = first;
        }
        if (actor->animPending) {
            if (actor->anim == low) {
                if (Random_Float(100.0f) < 0.4f) {
                    CGameObjectInstance__SetAnim(actor, high, 0);
                    actor->animPending = 0;
                }
            } else {
                CGameObjectInstance__SetAnim(actor, low, 0);
                actor->animPending = 0;
            }
        }
    }
    if (!human) {
        CGameObjectInstance__AlignToGround(actor);
    }
    p->emitterPos = p->pos;
    p->emitterYaw = p->yaw;
    p->emitterModel = p->model;
    p->emitter40 = p->unk40;
    p->lastPos = p->pos;
    if (gEngineApp_m_Settings.cheatFlags & 8) {
        for (i = 0; i < 31; i++) {
            if (i != 21) {
                p->ammo[i] = 0;
            }
        }
        for (i = 0; i < 35; i++) {
            WeaponInfo *info;

            if (i == 21) {
                continue;
            }
            info = gWeaponInfoTable[i];
            for (j = 0; j < 3; j++) {
                AmmoInfo *def = info->ammo[j];
                s16 *slot;

                if (def == NULL) {
                    break;
                }
                slot = &p->ammo[def->type];
                *slot = (def->max < *slot) ? *slot : def->max;
            }
        }
    }
    if (gEngineApp_m_Settings.cheatFlags & 4) {
        if (gEngineApp_m_Settings.multiplayer == 0 && p->owned[21] == 0) {
            *(p->ammo + CPlayer__GetFiringAmmo((CPlayer *) p, 21)->type) = 1;
        }
        for (i = 0; i < 35; i++) {
            if ((i >= 24) ^ !gEngineApp_m_Settings.multiplayer) {
                p->owned[i] = 1;
            }
        }
    }
    CPlayer__UpdateWeaponSet((CPlayer *) p);
    CBarrel__Update(&p->unkCD0, p, p->unk3C8);
    if (p->shake > 0.0f) {
        p->shake -= gFrameIncrement;
        if (p->shake > 0.0f) {
            p->flags |= 0x800000;
        } else {
            p->flags &= ~0x800000;
        }
    }
    Player_UpdateAim(p);
    p->input.controller->pos = p->pos;
    CScene__SetInstanceObjectType(&gScene, p->emitter, gWeaponInfoTable[p->weapon]->model, p->voice->bank);
    p->lastAction = p->action;
}

/* Switches the player to `state` (only to states 16-18 while
 * CPlayer__IsInMatchEndState holds in gEngineApp_m_Settings_multiplayer's mode): runs its enter hook and takes
 * its flags. Returns whether the player ends up in that state. */
s32 CPlayer__SetState(CPlayer *player, void *arg, s32 state) {
    s16 last;
    u32 lastFlags;

    if (gEngineApp_m_Settings.multiplayer != 0 && CPlayer__IsInMatchEndState(player) != 0 && state != 18 && state != 16 && state != 17) {
        return 0;
    }
    last = player->m_Mode;
    lastFlags = player->m_ModeFlags;
    player->m_Mode = state;
    player->m_ModeBefore = last;
    player->m_ModeBeforeFlags = lastFlags;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    if (gPlayerStateTable[state].enter != NULL) {
        gPlayerStateTable[state].enter(player, arg);
    }
    player->m_ModeFlags = gPlayerStateTable[player->m_Mode].flags;
    if (gPlayerStateTable[player->m_Mode].anim != 0) {
        player->m_CharAnim = gPlayerStateTable[player->m_Mode].anim;
    }
    if (player->m_Mode == state) {
        player->m_ModeTime = 0.0f;
        return 1;
    }
    return 0;
}

/* CPlayer__GetFiringAmmo, inlined. */
static __inline__ AmmoInfo *GetWeaponAmmo(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo;
    AmmoInfo *found;
    s32 i;
    WeaponInfo *def = gWeaponInfoTable[weapon];

    if (player->m_UseLowerAmmo && (ammo = def->ammo[1]) != NULL
        && player->m_Data.m_AmmoList[ammo->type] > 0) {
        return ammo;
    }
    found = NULL;
    for (i = 0; i < 3; i++) {
        ammo = def->ammo[i];
        if (ammo == NULL) {
            break;
        }
        found = ammo;
        if (player->m_Data.m_AmmoList[ammo->type] > 0) {
            return found;
        }
    }
    return found;
}

/* How many rounds of its ammo a weapon has, -1 if none. */
s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo;
    s32 type;
    s32 count;

    if (weapon == 0x15 && (s8)player->m_Data.m_WeaponList[0x15] != 0) {
        return -1;
    }
    ammo = GetWeaponAmmo(player, weapon);
    type = (ammo != NULL) ? ammo->type : -1;
    count = -1;
    if (type != -1) {
        count = player->m_Data.m_AmmoList[type];
    }
    return count;
}

/* gAmmoIcons entry of the current weapon ammo type, -1 if none. */
s32 CPlayer__GetCurrentAmmoInfo(CPlayer *player) {
    AmmoInfo *ammo;
    s32 type;
    s32 value;

    ammo = GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon);
    type = (ammo != NULL) ? ammo->type : -1;
    value = -1;
    if (type != -1) {
        value = gAmmoIcons[type];
    }
    return value;
}

static __inline__ s32 func_0024E88C_isSpecial(CPlayer *player) {
    if (player->m_Mode == 18 || player->m_Mode == 16 || player->m_Mode == 17) {
        return 1;
    }
    return 0;
}

/* Puts the player into the water states: state 11 when the actor is
 * flagged 0x80 and the player is not already in a water state (8-11, 35;
 * in states 16-18 the player is flagged instead), state 9 when the actor
 * is just under the surface. Returns whether the state changed. */
s32 CPlayer__CheckWater(CPlayer *player, CGameObjectInstance *actor) {
    if (actor->m_AI.m_dwFlags & 0x80) {
        /* Nested ifs, not &&: GCC would fold 10 || 11 into a range test. */
        if (player->m_Mode != 10) if (player->m_Mode != 11) if (player->m_Mode != 8)
        if (player->m_Mode != 9) if (player->m_Mode != 35) {
            if (func_0024E88C_isSpecial(player)) {
                player->m_CharacterActor.m_AI.m_dwFlags |= 0x80;
                return 0;
            }
            CPlayer__SetState(player, actor, 11);
            return 1;
        }
    }
    if (func_0024E88C_isSpecial(player)) {
        return 0;
    }
    if ((actor->m_AI.m_dwFlags & 0xC0200) != 0x200) {
        return 0;
    }
    if (!(actor->m_vVelocity.y <= 0.0f)) {
        return 0;
    }
    if (-20.48f < actor->m_vVelocity.y) {
        if (player->m_Mode != 10) if (player->m_Mode != 11) if (player->m_Mode != 8)
        if (player->m_Mode != 9) if (player->m_Mode != 35) {
            CPlayer__SetState(player, actor, 9);
            return 1;
        }
    }
    return 0;
}

static __inline__ s32 func_0024E9D0_isSpecial(CPlayer *player) {
    if (player->m_Mode == 18 || player->m_Mode == 16 || player->m_Mode == 17) {
        return 1;
    }
    return 0;
}

static __inline__ s32 func_0024E9D0_hasKey(CPlayer *player, s32 i) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return player->m_Data.m_SpecialPickupCount[382 + i] & 0xF;   /* the two key pickups of the region */
}

/* Enters state 32 when the actor stands in a region flagged 0x1000 whose
 * key requirements (0x800 on the flags, 0x10 on flags54) the player meets.
 * Returns whether it did. */
s32 CPlayer__CheckJumpPad(CPlayer *player, void *actor) {
    CRegionInfo *region;

    if (player->m_TimeOffGround > 0.0f) {
        return 0;
    }
    if (func_0024E9D0_isSpecial(player)) {
        return 0;
    }
    region = GameObject_GetRegionInfo(actor);
    if (region != NULL && (region->m_wFlags52 & 0x1000)) {
        if (!(region->m_dwFlags & 0x800)) {
            if (region->m_wFlags54 & 0x10) {
                goto second;
            }
            goto enter;
        } else {
            if (func_0024E9D0_hasKey(player, 0)) {
                goto enter;
            }
            if (!(region->m_wFlags54 & 0x10)) {
                return 0;
            }
        }
    second:
        if (func_0024E9D0_hasKey(player, 1)) {
        enter:
            CPlayer__SetState(player, actor, 32);
            return 1;
        }
    }
    return 0;
}

/* The creature to mount (an AI actor). */

f32 AI_AngleToPoint(CGameObjectInstance *body, CVector3 point);   /* angle to a point */
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 s);
void EnemyAI_Mount(CGameObjectInstance *mount, void *ai, CPlayer *player);

/* Mounts the creature of type 0x136 in front, if within 45 degrees of the
 * heading and the player is not holding 0x8000: the player takes its seat
 * and heading, and the movement state is reset. */
s32 CPlayer__TryMount(CPlayer *p, CGameObjectInstance *body, u32 *stateFlags) {
    CGameObjectInstance *mount;

    if (p->m_pOnEnemy == NULL && !(p->m_ModeBeforeFlags & 0x8000)) {
        if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
            mount = (CGameObjectInstance *)gCollisionResults.m_pInstance;
            if (mount->m_nTypeFlag == 0x136 && ABS(AI_AngleToPoint(body, mount->header.m_vPos)) < 0.785398245f) {
                p->m_pOnEnemy = mount;
                p->m_OnEnemyObjectType = 0x136;
                p->m_vOnEnemyScale = gRidingScale;
                CVector3__Scale(&p->m_vOnEnemyScale, &p->m_vOnEnemyScale, 0.03f);
                body->m_RotY = mount->m_RotY;
                CPlayerContState__Init(&p->m_ContState, &gControllers[p->m_nController]);
                p->m_Speed = 0;
                p->m_SideSpeed = 0;
                p->m_TotalSpeed = 0;
                p->m_YVelLastFrame = 0;
                p->m_bOnGround = 1;
                p->m_BurstSpeed = 0;
                p->m_BurstSideSpeed = 0;
                p->m_BurstTimer = 0;
                p->m_TimeOffGround = 0;
                p->m_BreathAmp = 0;
                p->m_BreathSpeed = 0;
                p->m_bHeadMovementDisabled = 0;
                p->m_JumpAngle = 0;
                p->m_StartJumpAngle = 0;
                p->m_DesiredJumpAngle = 0;
                p->m_JumpU = 0;
                p->m_vHeadRotOffset.x = 0;
                p->m_vHeadRotOffset.y = 0;
                p->m_vHeadRotOffset.z = 0;
                p->m_vHeadWobbleRotOffset.x = 0;
                p->m_vHeadWobbleRotOffset.y = 0;
                p->m_vHeadWobbleRotOffset.z = 0;
                p->m_vEyeOffset.x = 0;
                p->m_vEyeOffset.y = Actor_GetEyeHeight(p);
                p->m_vEyeOffset.z = 0;
                setVec3(&p->m_qGround, 0.0f, 0.0f, 0.0f);
                p->m_ActualCameraVertEyeOffset = 0;
                p->m_qGround.w = 1.0f;
                p->m_CameraMovementScaler = 1.0f;
                p->m_vDesiredPos = p->m_CharacterActor.header.m_vPos;
                CCache__SetFrameDelay(8);
                EnemyAI_Mount(mount, &mount->m_AI, p);
                return CPlayer__SetState(p, body, 0x21);
            }
        }
    }
    return 0;
}

void CCamera__SetShake(void *camera, f32 a, f32 b, f32 c, f32 d, s32 e, CVector3 pos);

/* While riding, button 0x8000 rams the creature in front: a live one is
 * thrown along the heading (knocked out of a state 0x2E with flag 0x400000)
 * and shakes the view hard, anything else just shakes it.
 * The single `return 1` after the if/else (not one per branch) is what gives
 * the ROMs plain bne at the 0x2E test: with two returns, reorgs liveness at
 * the fall-through keeps $v0 and annuls the slot (bnel). */
s32 CPlayer__RamWithMount(CPlayer *player, CGameObjectInstance *body, u32 *stateFlags) {
    CGameObjectInstance *aim = gCollisionResults.m_pInstance;
    CGameObjectInstance *target;
    CGameObjectInstance *mount;
    s32 type;
    if ((player->m_ModeFlags & 0x8000) && aim != NULL) {
        mount = player->m_pOnEnemy;
        if (aim != mount && aim->header.m_Type == 1) {
            type = aim->header.m_pIntelligence->m_Type;
            if (type == 1 && mount != NULL) {
                target = aim;
                if (target->m_AI.m_Health != 0) {
                    target->m_vVelocity.x = player->m_Speed * func_002982F0(body->m_RotY + 3.1415927f) * 20.0f;
                    target->m_vVelocity.y = player->m_Speed * 30.0f;
                    target->m_vVelocity.z = player->m_Speed * func_002974C0(body->m_RotY + 3.1415927f) * 20.0f;
                    CCamera__SetShake(player->m_pCamera, 50.0f, 100.0f, 50.0f, 1.0f, type, body->header.m_vPos);
                    if (mount->m_AI.m_Mode == 0x2E) {
                        target->m_AI.m_dwFlags |= 0x400000;
                        Sound__Play(0x278E, target->header.m_vPos, 0, -1);
                    } else {
                        Sound__Play(0x278D, target->header.m_vPos, 0, -1);
                    }
                    target->m_AI.m_ModeLastFrame = -1;
                    CAI__SetMode(target, &target->m_AI.m_dwFlags, 0x34);
                } else {
                CCamera__SetShake(player->m_pCamera, 30.0f, 60.0f, 30.0f, 1.0f, type, body->header.m_vPos);
                }
                return 1;
            }
        }
    }
    return 0;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x29];
    /* 0x29 */ u8 turnSpeed;        /* 0-255, 128 normal */
    /* 0x2A */ u8 lookSpeed;
    /* 0x2B */ s8 lookMode;
    /* 0x2C */ u8 pad2C;
    /* 0x2D */ u8 lookSpring;
} LookOptions;
typedef struct {
    /* 0x00 */ f32 turn;
    /* 0x04 */ f32 turnRate;
    /* 0x08 */ f32 lookRate;
    /* 0x0C */ f32 lookRateScale;
    /* 0x10 */ f32 lookDown;
    /* 0x14 */ f32 lookUp;
} LookTuning;
#ifdef TARGET_PORT
/* The N64 reads the u16 pair at 0x50 (m_DamageAmount, m_wFlags52) as one big-endian word and
 * tests its low half; the archive swaps them as two u16 (structs/region.h), so on the host the
 * word's low half is m_DamageAmount (0x2710 in level 51: & 0x6000 == 0x2000 blocked turning). */
typedef struct {
    /* 0x00 */ u8 pad0[0x52];
    /* 0x52 */ u16 flags;
} Floor;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x50];
    /* 0x50 */ u32 flags;
} Floor;
#endif

/* Turns the body and tilts the view from the stick, at the rates of the
 * tuning scaled by the player's speed settings and slowed while zoomed. */
void CPlayer__TurnAndLook(CPlayer *player, CGameObjectInstance *body, LookTuning *tuning) {
    LookOptions *options;
    Floor *floor;
    f32 zoomScale;
    f32 turnRate;
    f32 lookRate;
    f32 tilt;
    f32 speed;

    if (player->m_bHeadMovementDisabled != 0) {
        return;
    }
    options = player->m_pOptions;
    zoomScale = 1.0f - player->m_ZoomBlend * 0.99f;
    speed = options->turnSpeed * (1.0f / 128) - 1.0f;
    turnRate = speed * 0.5f + 0.875f;
    turnRate *= zoomScale * tuning->turnRate;
    speed = options->lookSpeed * (1.0f / 128) - 1.0f;
    lookRate = speed * 0.4f + 0.6f;
    lookRate *= zoomScale * tuning->lookRate * tuning->lookRateScale;
    switch (options->lookMode) {
        case 0:
            if (!options->lookSpring) {
                lookRate = 0.8f;
            }
            break;
        case 1:
            lookRate = 0.5f;
            break;
    }
    floor = GameObject_GetRegionInfo(body);
    if (floor == NULL || (floor->flags & 0x6000) != 0x2000) {
        if (tuning->turn != 0.0f) {
            Math_EaseToward(&body->m_RotY, body->m_RotY + player->m_ContState.m_FinalLookX * 80.0f * tuning->turn * 0.017453294f, turnRate);
        }
    }
    tilt = -player->m_ContState.m_FinalLookY * 80.0f;
    if (tilt < 0.0f) {
        tilt = tilt * -0.71619713f * tuning->lookDown;
    } else {
        tilt = tilt * 0.71619713f * tuning->lookUp;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.x, tilt * 0.017453294f, lookRate);
}

typedef struct {
    /* 0x00 */ f32 forwardAccel;
    /* 0x04 */ f32 forwardMax;
    /* 0x08 */ f32 forwardDecel;
    /* 0x0C */ f32 strafeAccel;
    /* 0x10 */ f32 strafeMax;
    /* 0x14 */ f32 strafeDecel;
    /* 0x18 */ f32 impulse;
    /* 0x1C */ f32 impulseDecel;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 impulseSound;
} Tuning;
f32 Math_StepClamped(f32 value, f32 delta, f32 limit);
f32 func_002982F0(f32 x);
f32 func_002974C0(f32 x);
void CVector3__LimitLength(CVector3 *v, f32 max);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);

/* Walking movement: forward and strafe speeds towards the stick, the dodge
 * impulses, turned into world space and added to the velocity. */
void CPlayer__MoveOnFoot(CPlayer *player, CGameObjectInstance *body, Tuning *tuning) {
    CVector3 motion;
    f32 forward;
    f32 strafe;
    f32 max;
    f32 time;
    s32 impulse;
    s32 ready;

    if (Cinema_IsActive() == 0) {
        impulse = 0;
        time = player->m_BurstTimer;
        ready = 1;
        if (!(time <= 0.0f)) {
            ready = 0;
            player->m_BurstTimer = time - gFrameIncrement;
        }
        forward = player->m_Speed;
        time = tuning->forwardMax;
        if (forward < 0.0f ? time < -forward : time < forward) {
            player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->forwardDecel * 0.5f);
        } else if (player->m_ContState.m_FinalMoveY != 0.0f && player->m_SniperMode != 2) {
            player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * tuning->forwardAccel,
                (player->m_ContState.m_FinalMoveY < 0.0f ? -player->m_ContState.m_FinalMoveY : player->m_ContState.m_FinalMoveY) * tuning->forwardMax);
        } else {
            player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->forwardDecel);
        }
        motion.x = player->m_Speed * func_002982F0(body->m_RotY + 3.1415927f);
        motion.y = 0;
        motion.z = player->m_Speed * func_002974C0(body->m_RotY + 3.1415927f);
        strafe = player->m_SideSpeed;
        max = tuning->strafeMax;
        if (strafe < 0.0f ? max < -strafe : max < strafe) {
            player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->strafeDecel * 0.5f);
        } else if (player->m_ContState.m_FinalMoveX != 0.0f) {
            player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * tuning->strafeAccel,
                (player->m_ContState.m_FinalMoveX < 0.0f ? -player->m_ContState.m_FinalMoveX : player->m_ContState.m_FinalMoveX) * tuning->strafeMax);
        } else {
            player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->strafeDecel);
        }
        motion.x += player->m_SideSpeed * func_002982F0(body->m_RotY - 1.5707964f);
        motion.z += player->m_SideSpeed * func_002974C0(body->m_RotY - 1.5707964f);
        strafe = player->m_SideSpeed;
        if (strafe < 0.0f) {
            strafe = -strafe;
        }
        forward = player->m_Speed;
        if (forward < 0.0f ? -forward < strafe : forward < strafe) {
            CVector3__LimitLength(&motion, tuning->strafeMax);
        } else {
            CVector3__LimitLength(&motion, tuning->forwardMax);
        }
        if (ready && (player->m_ContState.m_dwButtonsSingle & 8)) {
            player->m_BurstSpeed = tuning->impulse;
            impulse = 1;
        } else if (ready && (player->m_ContState.m_dwButtonsSingle & 4)) {
            player->m_BurstSpeed = -tuning->impulse * 0.5f;
            impulse = 1;
        } else {
            player->m_BurstSpeed = Math_DecayToZero(player->m_BurstSpeed, tuning->impulseDecel);
        }
        motion.x += player->m_BurstSpeed * func_002982F0(body->m_RotY + 3.1415927f);
        motion.z += player->m_BurstSpeed * func_002974C0(body->m_RotY + 3.1415927f);
        if (ready && (player->m_ContState.m_dwButtonsDouble & 1)) {
            player->m_BurstSideSpeed = tuning->impulse * 0.75f;
            impulse = 1;
        } else if (ready && (player->m_ContState.m_dwButtonsDouble & 2)) {
            player->m_BurstSideSpeed = -tuning->impulse * 0.75f;
            impulse = 1;
        } else {
            player->m_BurstSideSpeed = Math_DecayToZero(player->m_BurstSideSpeed, tuning->impulseDecel * 0.75f);
        }
        motion.x += player->m_BurstSideSpeed * func_002982F0(body->m_RotY - 1.5707964f);
        motion.z += player->m_BurstSideSpeed * func_002974C0(body->m_RotY - 1.5707964f);
        CVector3__Scale(&motion, &motion, player->m_FrameIncrementScaled);
        CVector3__Add(&player->m_vDesiredPos, &player->m_vDesiredPos, &motion);
        if (impulse) {
            player->m_BurstTimer += 7.5f;
            Sound__Play(tuning->impulseSound, body->header.m_vPos, 0, -1);
        }
    }
}

typedef struct {
    /* 0x00 */ f32 accel;
    /* 0x04 */ f32 max;
    /* 0x08 */ f32 decel;
    /* 0x0C */ f32 impulse;
    /* 0x10 */ f32 impulseDecel;
    /* 0x14 */ f32 climbAccel;
    /* 0x18 */ f32 climbMax;
    /* 0x1C */ f32 climbImpulse;
    /* 0x20 */ s16 pad20;
    /* 0x22 */ s16 impulseSound;
} Tuning_24F658;
f32 func_00298470(f32 x);
void CQuaternion__ToMatrix(CQuaternion *q, f32 *mtx);

/* Swimming movement: forward and strafe speeds, the dodge impulses and the
 * climb, turned by the body's facing and added to the velocity, with drag
 * on the way down and a roll from the strafe. */
void CPlayer__MoveInWater(CPlayer *player, CGameObjectInstance *body, Tuning_24F658 *tuning) {
    CVector3 forward;
    CVector3 strafe;
    CVector3 forwardOut;
    CVector3 strafeOut;
    CVector3 sum;
    f32 mtx[16];
    CQuaternion rot;
    f32 value;
    f32 max;
    f32 speed;
    f32 climb;
    f32 climbMax;
    s32 ready;
    s32 impulse;
    u32 pressed;

    impulse = 0;
    value = player->m_BurstTimer;
    ready = 1;
    if (!(value <= 0.0f)) {
        ready = 0;
        player->m_BurstTimer = value - gFrameIncrement;
    }
    if (player->m_Data.m_CurrentWeapon == 0x12 || player->m_Data.m_CurrentWeapon == 0x21) {
        ready = 0;
        if (player->m_ContState.m_dwButtons & 8) {
            player->m_Speed = Math_StepClamped(player->m_Speed, tuning->max * 0.75f, tuning->max * 3.0f);
        }
    }
    value = player->m_Speed;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->decel * 0.5f);
    } else if (player->m_ContState.m_FinalMoveY != 0.0f) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * tuning->accel,
            (player->m_ContState.m_FinalMoveY < 0.0f ? -player->m_ContState.m_FinalMoveY : player->m_ContState.m_FinalMoveY) * tuning->max);
    } else {
        player->m_Speed = Math_DecayToZero(player->m_Speed, tuning->decel);
    }
    value = player->m_SideSpeed;
    max = tuning->max;
    if (value < 0.0f ? max < -value : max < value) {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->decel * 0.5f);
    } else if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * tuning->accel,
            (player->m_ContState.m_FinalMoveX < 0.0f ? -player->m_ContState.m_FinalMoveX : player->m_ContState.m_FinalMoveX) * tuning->max * 0.75f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, tuning->decel);
    }
    if (ready) {
        pressed = player->m_ContState.m_dwButtonsSingle;
        if (pressed & 8) {
            speed = tuning->impulse;
            impulse = 1;
        } else if (pressed & 4) {
            speed = -tuning->impulse * 0.5f;
            impulse = 1;
        } else {
            goto decay;
        }
    } else {
    decay:
        speed = Math_DecayToZero(player->m_BurstSpeed, tuning->impulseDecel);
    }
    player->m_BurstSpeed = speed;
    if (ready && (player->m_ContState.m_dwButtonsDouble & 1)) {
        player->m_BurstSideSpeed = tuning->impulse * 0.75f;
        impulse = 1;
    } else if (ready && (player->m_ContState.m_dwButtonsDouble & 2)) {
        player->m_BurstSideSpeed = -tuning->impulse * 0.75f;
        impulse = 1;
    } else {
        player->m_BurstSideSpeed = Math_DecayToZero(player->m_BurstSideSpeed, tuning->impulseDecel * 0.75f);
    }
    if (ready && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        impulse = 1;
        body->m_vVelocity.y += tuning->climbImpulse;
    }
    if (player->m_ContState.m_dwButtons & 0x10) {
        climb = body->m_vVelocity.y;
        climbMax = tuning->climbMax;
        if (climb < climbMax) {
            body->m_vVelocity.y = Math_StepClamped(climb, tuning->climbAccel, climbMax);
        }
    }
    forward.x = 0.0f;
    forward.y = 0;
    forward.z = player->m_Speed + player->m_BurstSpeed;
    strafe.x = player->m_SideSpeed + player->m_BurstSideSpeed;
    strafe.y = 0;
    strafe.z = 0;
    rot = CGameObjectInstance__GetRotation(body);
    CQuaternion__ToMatrix(&rot, mtx);
    CMtxF__TransformPoint(mtx, &forward, &forwardOut);
    CMtxF__TransformPoint(mtx, &strafe, &strafeOut);
    player->m_vDesiredPos.x += (forwardOut.x + strafeOut.x) * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.y += (forwardOut.y + strafeOut.y) * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.z += (forwardOut.z + strafeOut.z) * player->m_FrameIncrementScaled;
    if (body->m_vVelocity.y < 0.0f) {
        CVector3__Add(&sum, &forwardOut, &strafeOut);
        body->m_vVelocity.y -= gPlayerStateTable[player->m_Mode].physics->drag
            * (func_00298470(sum.x * sum.x + sum.y * sum.y + sum.z * sum.z) / (tuning->max * 4.0f))
            * player->m_FrameIncrementScaled;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.z, (player->m_SideSpeed * 1.8f + player->m_ContState.m_FinalLookX * 16.0f) * 0.017453294f, 0.04f);
    if (impulse) {
        player->m_BurstTimer += 7.5f;
        Sound__Play(tuning->impulseSound, body->header.m_vPos, 0, -1);
    }
}

typedef struct {
#ifdef TARGET_PORT /* the body is a CGameObjectInstance: m_RotY at its host offset (at 0x50 the view read
                    * 0 on the host, so the side bob went along the wrong axis: demo 1's camera, round 11 z5) */
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_RotY)];
#else
    /* 0x00 */ u8 pad0[0x50];
#endif
    /* 0x50 */ f32 yaw;
} BobObject;

/* Settles the view tilt and kicks, then bobs the view with the walk. */
void CPlayer__BobView(CPlayer *p, BobObject *obj) {
    f32 phase;
    f32 base;
    f32 amount;

    Math_EaseToward(&p->m_vHeadRotOffset.z, 0.0f, 0.25f);
    Math_EaseToward(&p->m_vHeadRotOffset.y, 0.0f, 0.5f);
    p->m_vHeadWobbleRotOffset.x = Math_DecayToZero(p->m_vHeadWobbleRotOffset.x, 0.087266475f);
    p->m_vHeadWobbleRotOffset.y = Math_DecayToZero(p->m_vHeadWobbleRotOffset.y, 0.087266475f);
    p->m_vHeadWobbleRotOffset.z = Math_DecayToZero(p->m_vHeadWobbleRotOffset.z, 0.087266475f);

    if (p->m_BreathSpeed > 21.0f) {
        p->m_BreathSpeed = 21.0f;
    }
    if (p->m_BreathAmp > 0.1f) {
        p->m_BreathAmp = 0.1f;
    }
    p->m_BreathSpeed = Math_DecayToZero(p->m_BreathSpeed, 0.11666667f);
    p->m_BreathAmp = Math_DecayToZero(p->m_BreathAmp, 0.00055555557f);
    if (p->m_BreathAmp < 0.06f) {
        p->m_BreathAmp = 0.0f;
        p->m_BreathSpeed = 0.0f;
    }

    phase = p->m_ModeTime * p->m_BreathSpeed * 0.017453294f;
    if (p->m_CharacterActor.header.m_pIntelligence != NULL) {
        base = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_CollisionHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    Math_EaseToward(&p->m_vEyeOffset.y, base + p->m_BreathAmp * (func_002982F0(phase) * 10.24f), 0.5f);
    amount = p->m_BreathAmp * (func_002974C0(phase) * 8.192f);
    Math_EaseToward(&p->m_vEyeOffset.x, -func_002982F0(obj->yaw + 3.1415927f) * amount, 0.5f);
    Math_EaseToward(&p->m_vEyeOffset.z, -func_002974C0(obj->yaw + 3.1415927f) * amount, 0.5f);
}

/* Settles the view tilt and kicks, then bobs the view with the stride,
 * scaled by the speed. */
void CPlayer__BobViewStride(CPlayer *p, BobObject *obj) {
    f32 speed;
    f32 amount;
    f32 phase;
    f32 base;
    f32 y;
    f32 side;

    Math_EaseToward(&p->m_vHeadRotOffset.z, 0.0f, 0.25f);
    Math_EaseToward(&p->m_vHeadRotOffset.y, 0.0f, 0.5f);
    p->m_vHeadWobbleRotOffset.x = Math_DecayToZero(p->m_vHeadWobbleRotOffset.x, 0.087266475f);
    p->m_vHeadWobbleRotOffset.y = Math_DecayToZero(p->m_vHeadWobbleRotOffset.y, 0.087266475f);

    speed = p->m_Speed;
    if (speed < 0.0f) {
        amount = -speed * 0.032552086f;
    } else {
        amount = speed * 0.032552086f;
    }
    if (amount < 0.0f) {
        amount = 0.0f;
    } else if (amount > 1.0f) {
        amount = 1.0f;
    }

    phase = -(p->m_ModeTime * p->m_CameraMovementScaler * 360.0f) * 0.0014544412f;
    if (p->m_CharacterActor.header.m_pIntelligence != NULL) {
        base = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_CollisionHeight * 0.9f;
    } else {
        base = 82.94399f;
    }
    if (func_002982F0(phase) * 15.36f < 0.0f) {
        y = base - amount * -(func_002982F0(phase) * 15.36f);
    } else {
        y = base - amount * 15.36f * func_002982F0(phase);
    }
    Math_EaseToward(&p->m_vEyeOffset.y, y, 0.5f);
    side = amount * (func_002974C0(phase) * 7.68f);
    Math_EaseToward(&p->m_vEyeOffset.x, -func_002982F0(obj->yaw - 1.5707964f) * side, 0.5f);
    Math_EaseToward(&p->m_vEyeOffset.z, -func_002974C0(obj->yaw - 1.5707964f) * side, 0.5f);
    Math_EaseToward(&p->m_vHeadWobbleRotOffset.z, -(side * 0.0017044232f), 0.25f);
}

f32 Math_StepClamped(f32 value, f32 target, f32 rate);

/* Fly mode (cheat): turn and pitch with the stick, fly forward/sideways and
 * up/down with the buttons; holding all three 0xE000 buttons and pressing one
 * again leaves it, dropping onto the middle of the floor under the player. */
void CPlayer__UpdateFlyCheat(CPlayer *player, CGameObjectInstance *body) {
    CVector3 move;
    CGameRegion *floor;
    u32 held;

    if (player->m_Mode == 0 && !(player->m_ContState.m_dwButtons & 0xE000)) {
        player->m_Mode = 1;
    }
    player->m_vHeadRotOffset.z = 0.0f;
    Math_EaseToward(&body->m_RotY, body->m_RotY + player->m_ContState.m_pContState->stickX * 0.0043633236f, 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.x, player->m_vHeadRotOffset.x + -player->m_ContState.m_pContState->stickY * 0.0021816618f, 0.9f);
    if (player->m_ContState.m_FinalMoveY != 0.0f) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * 8.533333f,
                                        ABS(player->m_ContState.m_FinalMoveY) * 102.399994f);
    } else {
        player->m_Speed = Math_DecayToZero(player->m_Speed, 17.066666f);
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * 8.533333f,
                                       ABS(player->m_ContState.m_FinalMoveX) * 102.399994f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, 17.066666f);
    }
    held = player->m_ContState.m_dwButtons;
    if (held & 0x10) {
        body->m_vVelocity.y = Math_StepClamped(body->m_vVelocity.y, 5.9733334f, 71.68f);
    } else if (held & 0x2020) {
        body->m_vVelocity.y = Math_StepClamped(body->m_vVelocity.y, -5.9733334f, 71.68f);
    } else {
        body->m_vVelocity.y = Math_DecayToZero(body->m_vVelocity.y, 11.946667f);
    }
    move.x = player->m_Speed * func_002982F0(body->m_RotY + 3.1415927f);
    move.y = body->m_vVelocity.y;
    move.z = player->m_Speed * func_002974C0(body->m_RotY + 3.1415927f);
    move.x += player->m_SideSpeed * func_002982F0(body->m_RotY - 1.5707964f);
    move.z += player->m_SideSpeed * func_002974C0(body->m_RotY - 1.5707964f);
    CVector3__Scale(&move, &move, gFrameIncrement);
    CVector3__Add(&player->m_vDesiredPos, &player->m_vDesiredPos, &move);
    floor = CScene__FindRegionAt(&gScene, &body->header.m_vPos);
    if (floor != NULL) {
        body->header.m_pCurrentRegion = floor;
    }
    if (player->m_Mode == 1 && (player->m_ContState.m_dwButtons & 0xE000) == 0xE000 && (player->m_ContState.m_dwButtonsSingle & 0xE000)) {
        CPlayer__SetState(player, body, 2);
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, "Fly Mode Off");
        if (floor == NULL) {
            floor = body->header.m_pCurrentRegion;
            body->header.m_vPos = *floor->m_pVertices[0];
            CVector3__Add(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[1]);
            CVector3__Add(&body->header.m_vPos, &body->header.m_vPos, floor->m_pVertices[2]);
            CVector3__Scale(&body->header.m_vPos, &body->header.m_vPos, 0.33333334f);
        }
    }
}

static __inline__ s32 try_jump(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        CPlayer__SetState(player, body, 4);
        return 1;
    }
    return 0;
}

/* Standing and walking: bob the view, turn it with the crouching or standing
 * tuning, hand off to a jump or to moving, then pick the turning animation. */
void PlayerStand_Update(CPlayer *player, CGameObjectInstance *body) {
    f32 heading;
    s32 crouching;
    s32 animate;

    CPlayer__BobView(player, body);
    heading = body->m_RotY;
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        CPlayer__TurnAndLook(player, body, &gCrouchLookTuning);
    } else {
        CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    }
    heading = body->m_RotY - heading;
    if (try_jump(player, body)) {
        return;
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f || (player->m_SniperMode != 2 && player->m_ContState.m_FinalMoveY != 0.0f)) {
        CPlayer__SetState(player, body, 3);
    }
    animate = 1;
    if ((player->m_CharAnim == 0x1130 || player->m_CharAnim == 0x1135) && player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0) {
        animate = 0;
    }
    if (animate) {
        if (body->m_AI.m_dwFlags & 0x6000) {
            player->m_CharAnim = 0xA28;
        } else if (0.017453294f < heading) {
            player->m_CharAnim = 0xB5E;
        } else if (heading < -0.017453294f) {
            player->m_CharAnim = 0xB54;
        } else {
            player->m_CharAnim = 1;
        }
    }
}

/* Walking: bob the view, turn and move with the crouching or standing
 * tunings, hand off to a jump or to standing, play the wading sound in water,
 * then pick the walking animation from the direction. */
void PlayerWalk_Update(CPlayer *player, CGameObjectInstance *body) {
    f32 forward;
    f32 strafe;
    f32 time;
    s32 crouching;

    CPlayer__BobViewStride(player, body);
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        CPlayer__TurnAndLook(player, body, &gCrouchLookTuning);
    } else {
        CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    }
    crouching = 5.12f < player->m_DuckOffset;
    if (crouching) {
        CPlayer__MoveOnFoot(player, body, &gCrouchMoveTuning);
    } else {
        CPlayer__MoveOnFoot(player, body, &gStandMoveTuning);
    }
    if (try_jump(player, body)) {
        return;
    }
    if (player->m_Speed == 0.0f && player->m_SideSpeed == 0.0f) {
        CPlayer__SetState(player, body, 2);
    }
    if ((body->m_AI.m_dwFlags & 0x400) && player->m_bOnGround != 0) {
        time = player->m_SfxTimer + gFrameIncrement;
        player->m_SfxTimer = time;
        if (player->m_TotalSpeed > 5.12f && time >= 6.0f) {
            player->m_SfxTimer = time - 6.0f;
            Sound__Play(0x3AB, body->header.m_vPos, 0, -1);
        }
    }
    if (body->m_AI.m_dwFlags & 0x6000) {
        if (player->m_Speed >= 0.0f) {
            player->m_CharAnim = 0xA32;
        } else {
            player->m_CharAnim = 0xA37;
        }
        return;
    }
    forward = player->m_Speed;
    if (forward < 0.0f) {
        forward = -forward;
    }
    strafe = player->m_SideSpeed;
    if (forward < (strafe < 0.0f ? -strafe : strafe)) {
        if (player->m_SideSpeed < 0.0f) {
            player->m_CharAnim = 0x802;
        } else {
            player->m_CharAnim = 0x80C;
        }
    } else {
        if (player->m_Speed >= 0.0f) {
            player->m_CharAnim = 0x7DA;
        } else {
            player->m_CharAnim = 0x7DF;
        }
    }
}

f32 GameObject_GetWaterHeight(CGameObjectInstance *body);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

/* Wading: leave the state once out of the water, dive when looking down hard
 * while lifting, else follow the water surface, sway, and pick the animation
 * from the climb; moving plays the tuning's stroke sound now and then. */
void PlayerWade_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 8;
    } else {
        player->m_Mode = 9;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    CPlayer__MoveOnFoot(player, body, &gWadeMoveTuning);
    if (!(body->m_AI.m_dwFlags & 0x200)) {
        reset_strokes(&player->m_Barrel);
        CPlayer__SetState(player, body, 3);
        return;
    }
    if (player->m_vHeadRotOffset.x * 57.295776f < -40.0f && player->m_ContState.m_FinalMoveY > 0.0f) {
        player->m_vDesiredPos.y = GameObject_GetWaterHeight(body) - player->m_vEyeOffset.y - 51.199997f;
        CPlayer__SetState(player, body, 0xB);
        return;
    }
    if (!(body->m_AI.m_dwFlags & 0xC0000)) {
        body->m_vVelocity.y = 0.0f;
        Math_EaseToward(&player->m_vDesiredPos.y, GameObject_GetWaterHeight(body) - player->m_vEyeOffset.y * 0.6f, 0.5f);
    }
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 0.043633237f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 0.043633237f;
    if (try_jump(player, body)) {
        return;
    }
    idle = 0;
    if (player->m_CharAnim == 0x1144) {
        idle = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!idle) {
        if (1.024f <= player->m_Speed) {
            player->m_CharAnim = 0x8A2;
        } else if (player->m_Speed <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    player->m_SfxTimer += gFrameIncrement;
    if (5.12f < player->m_TotalSpeed && 11.25f < player->m_SfxTimer) {
        player->m_SfxTimer = 0.0f;
        Sound__Play(gWadeMoveTuning.impulseSound, body->header.m_vPos, 0, -1);
    }
}

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 pos;
} Ui;
f32 Random_Float(f32 scale);

/* Swimming: sway, move with the swimming tuning, and on surfacing (after two
 * time units) gasp for air and leave the state for falling, wading or
 * walking; then pick the animation from the climb and play strokes. */
void PlayerSwim_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 0xA;
    } else {
        player->m_Mode = 0xB;
    }
    CPlayer__TurnAndLook(player, body, &gSwimLookTuning);
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 2 * 0.017453294f;
    CPlayer__MoveInWater(player, body, &gSwimMoveTuning);
    if (!(body->m_AI.m_dwFlags & 0x80) && 2.0f < player->m_ModeTime) {
        reset_strokes(&player->m_Barrel);
        body->m_vVelocity.y = 0.0f;
        if (!gEngineApp_m_Settings.multiplayer) {
            if (player->m_bOxygenOut) {
                Sound__Play(0x3AC3, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
            } else if (52.5f < player->m_Oxygen) {
                Sound__Play(0x3AC2, player->m_pCamera->m_vPos, &player->m_pCamera->m_vPos, -1);
            }
        }
        if (body->m_AI.m_dwFlags & 0xC0000) {
            CPlayer__SetState(player, body, 2);
        } else if (body->m_AI.m_dwFlags & 0x200) {
            CPlayer__SetState(player, body, 9);
        } else {
            CPlayer__SetState(player, body, 3);
        }
    }
    idle = 0;
    if (player->m_CharAnim == 0x1144) {
        idle = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!idle) {
        if (1.024f <= player->m_Speed) {
            player->m_CharAnim = 0x8A2;
        } else if (player->m_Speed <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    player->m_SfxTimer += gFrameIncrement;
    if (5.12f < player->m_TotalSpeed && 45.0f < player->m_SfxTimer) {
        player->m_SfxTimer = Random_Float(15.0f);
        Sound__Play(gSwimMoveTuning.impulseSound, body->header.m_vPos, NULL, -1);
    }
}

s32 CGameRegion__IsSteep(void *region);
f32 CGameRegion__GetDownhillYaw(void *region);
f32 Angle_Difference(f32 from, f32 to);
f32 GameObject_GetRegionTopAbove(CGameObjectInstance *body);
f32 GameObject_GetHeightAboveRegionBottom(CGameObjectInstance *body);

static __inline__ s32 try_jump_250EC0(CPlayer *player, CGameObjectInstance *body) {
    if (player->m_SniperMode == 0 && !(11.25f < player->m_TimeOffGround) && !(body->m_AI.m_dwFlags & 0x6000)
#ifdef TARGET_PORT /* soak S8: a body with no intelligence (climbing after a debug warp) read NULL: no jump */
        && body->header.m_pIntelligence != NULL
#endif
        && (body->header.m_pIntelligence->m_dwTypeFlags & 2) && (player->m_ContState.m_dwButtonsSingle & 0x10)) {
        CPlayer__SetState(player, body, 4);
        return 1;
    }
    return 0;
}

/* Climbing: face the wall, look up or down with the stick (tilting the view
 * back near the top and the bottom of the climb), jump off, climb with the
 * stick and play a step sound every 11.25 time units. */
void PlayerClimb_Update(CPlayer *player, CGameObjectInstance *body) {
    f32 angle;
    f32 side;
    f32 sideRaw;
    f32 scale;
    f32 toTop;
    f32 toBottom;
    f32 lastForward;
    f32 s;
    f32 bob;
    f32 time;
    f32 c;
    s32 nearTop;
    s32 nearBottom;
    s32 up;
    s32 down;
    s32 anim;

    if (!CGameRegion__IsSteep(body->header.m_pCurrentRegion) || !(body->m_AI.m_dwFlags & 0x1000)) {
        player->m_vHeadRotOffset.x = 0.0f;
        CPlayer__SetState(player, body, 2);
        return;
    }
    CScene__GetRegionInfo(&gScene, body->header.m_pCurrentRegion);
    Math_EaseToward(&body->m_RotY, body->m_RotY - Angle_Difference(body->m_RotY, CGameRegion__GetDownhillYaw(body->header.m_pCurrentRegion)), 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.y, player->m_ContState.m_FinalLookX * 3.4906588f, 0.25f);
    angle = -player->m_ContState.m_FinalLookY * 80.0f;
    if (angle > 0.0f) {
        angle *= 0.1875f;
    } else {
        angle *= 2.25f;
    }
    /* |turn * 80| * 1.625, written as one product of two picked factors */
    side = player->m_ContState.m_FinalLookX;
    sideRaw = side * 80.0f;
    if (sideRaw < 0.0f) {
        side = 1.625f;
        scale = -sideRaw;
    } else {
        scale = 130.0f;
    }
    angle -= side * scale;
    angle += 90.0f;
    toTop = GameObject_GetRegionTopAbove(body);
    if (toTop < 0.0f) {
        toTop = 0.0f;
    }
    nearTop = 1;
    if (!(toTop <= 184.31999f)) {
        nearTop = 0;
    }
    toBottom = GameObject_GetHeightAboveRegionBottom(body);
    if (toBottom < 0.0f) {
        toBottom = 0.0f;
    }
    nearBottom = 1;
    if (!(toBottom <= 81.92f)) {
        nearBottom = 0;
    }
    if (nearTop && nearBottom) {
        if (toTop < toBottom) {
            nearBottom = 0;
        } else {
            nearTop = 0;
        }
    }
    if (nearTop) {
        angle -= (0.5f - func_002974C0((1.0f - toTop * 0.0054253475f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (nearBottom) {
        angle -= (0.5f - func_002974C0((1.0f - toBottom * 0.012207031f) * 3.1415927f) * 0.5f) * 90.0f;
    }
    if (angle < -90.0f) {
        angle = -90.0f;
    }
    Math_EaseToward(&player->m_vHeadRotOffset.x, angle * 0.017453294f, 0.5f);
    if (try_jump_250EC0(player, body)) {
        s = func_002982F0(body->m_RotY + 6.2831855f);
        c = func_002974C0(body->m_RotY + 6.2831855f);
        body->m_vVelocity.x += s * 384.0f;
        body->m_vVelocity.z += c * 384.0f;
        body->m_RotY += player->m_vHeadRotOffset.y;
        player->m_vHeadRotOffset.y = 0.0f;
        return;
    }
    bob = player->m_HandSide * player->m_Speed * func_002982F0(player->m_ModeTime * 0.13962635f);
    if (player->m_ContState.m_FinalMoveY > 0.0f || player->m_ContState.m_FinalMoveY < 0.0f) {
        Math_EaseToward(&player->m_vHeadRotOffset.z, bob, 0.25f);
    } else {
        Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.9f);
    }
    up = 1;
    if (!(player->m_ContState.m_FinalMoveY > 0.0f)) {
        up = 0;
    }
    down = 0;
    if (!gEngineApp_m_Settings.multiplayer && player->m_ContState.m_FinalMoveY < 0.0f) {
        down = 1;
    }
    lastForward = player->m_Speed;
    if (up || down) {
        player->m_Speed = Math_StepClamped(player->m_Speed, player->m_ContState.m_FinalMoveY * 2.048f,
                                        ABS(player->m_ContState.m_FinalMoveY) * 14.3359995f);
    } else {
        player->m_Speed = Math_DecayToZero(lastForward, 4.096f);
    }
    if (player->m_ContState.m_FinalMoveX != 0.0f) {
        player->m_SideSpeed = Math_StepClamped(player->m_SideSpeed, player->m_ContState.m_FinalMoveX * 2.048f,
                                       ABS(player->m_ContState.m_FinalMoveX) * 14.3359995f);
    } else {
        player->m_SideSpeed = Math_DecayToZero(player->m_SideSpeed, 4.096f);
    }
    if (player->m_Speed == 0.0f && player->m_SideSpeed == 0.0f) {
        player->m_ModeTime = 0.0f;
        if (lastForward != 0.0f) {
            player->m_HandSide = -player->m_HandSide;
        }
        CPlayer__BobView(player, body);
    } else if ((time = player->m_ModeTime) > 11.25f) {
        player->m_HandSide = -player->m_HandSide;
        player->m_ModeTime = time - 11.25f;
        player->m_Speed *= 0.1f;
        player->m_SideSpeed *= 0.1f;
        if (gEngineApp_m_Settings.multiplayer) {
            if (player->m_pOptions->m_TagMode == 1) {
                Sound__Play2D(0x27DB);
            } else {
                Sound__Play(sCharacterClimbSounds[player->m_Data.m_nDeathmatchCharacter].sound, body->header.m_vPos, 0, -1);
            }
        } else {
            Sound__Play(0x3AB7, body->header.m_vPos, 0, -1);
        }
    }
    s = func_002982F0(body->m_RotY - 1.5707964f);
    c = func_002974C0(body->m_RotY - 1.5707964f);
    player->m_vDesiredPos.x += s * player->m_SideSpeed * player->m_FrameIncrementScaled;
    player->m_vDesiredPos.z += c * player->m_SideSpeed * player->m_FrameIncrementScaled;
    if (player->m_Speed >= 0.0f) {
        s = func_002982F0(body->m_RotY + 3.1415927f);
        c = func_002974C0(body->m_RotY + 3.1415927f);
        player->m_vDesiredPos.x += s * player->m_Speed * player->m_FrameIncrementScaled;
        player->m_vDesiredPos.z += c * player->m_Speed * player->m_FrameIncrementScaled;
    } else {
        player->m_vDesiredPos.y += player->m_Speed * 0.5f;
    }
    anim = 0xA46;
    if (!(player->m_Speed > 0.0f)) {
        anim = 0xA3C;
    }
    player->m_CharAnim = anim;
}

/* A 0x64-byte record of the world model sections (CScene__GetRegionInfo finds it from an id). */

/* What CScene__FindIntelligence finds (id 0xC45): three floats at 0x38. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 unk38;
    /* 0x3C */ f32 unk3C;
    /* 0x40 */ f32 unk40;
} Found0xC45;
s32 Cinema_SetNextCallbacks(s32, sptr, s32 *);     /* extern */
void PlayerDeath_CinemaDone(void);

void CPlayer__StartDeathCinema(CPlayer *arg0, s32 arg1, s32 arg2) {
    s32 temp_a0;
    sptr temp_a1;
    CRegionInfo *temp_v0;
    Found0xC45 *temp_v0_2;
    s32 var_s0;
    s32 var_s1;
    s32 var_v1;

    var_s0 = 1;
    temp_a1 = arg0->m_CharacterActor.header.m_pCurrentRegion;
    var_s1 = 0;
    if (temp_a1 != 0) {
        temp_v0 = CScene__GetRegionInfo(&gScene, temp_a1);
        if (temp_v0 != 0) {
            var_s1 = temp_v0->m_dwFlags;
        }
    }
    if (gScene.m_bStreamAll != 0) {
        var_s0 = 0;
    }
    temp_a0 = arg0->m_ModeFlags & 0x8000;
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (gScene.m_bStreamAll != 0) {
        var_s0 = 0;
    }
    if (gEngineApp_m_Game.m_PlayerList.m_nPlayers >= 2) {
        var_s0 = 0;
    }
    if (arg2 == 0xC) {
        var_s0 = 0;
    }
    if (temp_a0 != 0) {
        var_s0 = 0;
    }
    if (var_s1 & 0x01000000) {
        var_s0 = 0;
    }
    if ((var_s1 & 0x80000) && (arg2 != 0x14) && (arg2 != 0x28) && (arg2 != 0x1E)) {
        var_s0 = 0;
    }
    arg0->m_DoDeathCin = var_s0;
    if (var_s0 != 0) {
        if (arg2 == 0x32) {
            gDeathTint = 1;
            var_v1 = 0;
            do {
                gDeathTintColor[var_v1] = arg0->m_pCamera->m_FogColor[var_v1];
                var_v1 += 1;
            } while (var_v1 < 4);
        }
        gCinemaHidesActors = 0;
        sDeathCinemaPlayer = arg0;
        Cinema_SetNextCallbacks(arg2, 0, &PlayerDeath_CinemaDone);
    }
    temp_v0_2 = CScene__FindIntelligence(&gScene, -1, 0xC45);
    if (temp_v0_2 != 0) {
        arg0->m_CharacterActor.m_vScale.x = temp_v0_2->unk38;
        arg0->m_CharacterActor.m_vScale.y = temp_v0_2->unk3C;
        arg0->m_CharacterActor.m_vScale.z = temp_v0_2->unk40;
    }
}

typedef CVector3 Position;
typedef struct TravelTarget {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ Position pos;
    /* 0x40 */ f32 height;
    /* 0x44 */ u8 pad44[0x1A];
    /* 0x5E */ u8 duration;
} TravelTarget;

void PlayerJumpPad_Update(CPlayer *player, void *body) {
    f32 yaw, targetYaw, alternate, duration, t;
    Position pos;

    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    player->m_JumpPadCurrentTime += gFrameIncrement;
    yaw = player->m_CharacterActor.m_RotY;
    targetYaw = player->m_JumpPadAngleDirection;
    if (targetYaw != yaw) {
        Angle_Wrap(&yaw);
        Angle_Wrap(&targetYaw);
        if (yaw > targetYaw) {
            alternate = targetYaw + 6.2831855f;
            if (alternate - yaw < yaw - targetYaw) {
                targetYaw = alternate;
            }
        } else {
            alternate = targetYaw - 6.2831855f;
            if (yaw - alternate < targetYaw - yaw) {
                targetYaw = alternate;
            }
        }
        if (targetYaw > yaw) {
            yaw += (targetYaw - yaw) * 0.2f;
            if (targetYaw < yaw) {
                yaw = targetYaw;
            }
        } else {
            yaw -= (yaw - targetYaw) * 0.2f;
            if (yaw < targetYaw) {
                yaw = targetYaw;
            }
        }
        Angle_Wrap(&yaw);
    }
    player->m_WeaponActor.m_RotY = yaw;
    player->m_CharacterActor.m_RotY = yaw;
    duration = (f32)(((TravelTarget *)player->m_pJumpPadRegionSet)->duration * 15);
    if (player->m_JumpPadCurrentTime < duration) {
        t = player->m_JumpPadCurrentTime / duration;
        pos.x = player->m_vJumpPadStart.x + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.x - player->m_vJumpPadStart.x) * t;
        pos.z = player->m_vJumpPadStart.z + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.z - player->m_vJumpPadStart.z) * t;
        pos.y = player->m_vJumpPadStart.y + (((TravelTarget *)player->m_pJumpPadRegionSet)->pos.y - player->m_vJumpPadStart.y) * t
              + ((TravelTarget *)player->m_pJumpPadRegionSet)->height * ((t - t * t) * 4.0f);
        player->m_vDesiredPos = pos;
        return;
    }
    CPlayer__SetState(player, body, 2);
}

/* The ridden creature (an AI actor). */
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The host view (STABILITY.md section 7): the mount's CAI and its mode at their host offsets
 * (the N64 view put them at 0x140 and 0x174). */
typedef union {
    struct { u8 _h_ai[__builtin_offsetof(CGameObjectInstance, m_AI)]; u8 ai[0x34]; };
    struct { u8 _h_state[__builtin_offsetof(CGameObjectInstance, m_AI.m_Mode)]; s8 aiState; };
} Mount;
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ u8 ai[0x34];
    /* 0x174 */ s8 aiState;
} Mount;
#endif

void CCamera__SetShake(void *ui, f32 a, f32 b, f32 c, f32 d, s32 e, CVector3 pos);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);

/* Riding: the mount sways the view as it turns, charges on button 8, shakes
 * the screen at each step and sets the rider's view offset. */
void CPlayer__UpdateRiding(CPlayer *player, CGameObjectInstance *body) {
    CMtxF m;
    Mount *mount;
    CVector3 *offset;
    f32 turn;
    f32 yaw;
    f32 fwd;
    f32 t;
    f32 step;
    f32 dist;
    f32 next;
    f32 shake;

    mount = ((Mount *)player->m_pOnEnemy);
    turn = player->m_ContState.m_FinalLookX * (((func_002974C0(player->m_RideEnemyTime * 0.2792527f) + 1.0f) * 0.01f + 0.06f) * 80.0f);
    player->m_RideEnemyTime += gFrameIncrement;
    turn *= 0.017453294f;
    yaw = body->m_RotY;
    Math_EaseToward(&body->m_RotY, yaw + turn, 0.9f);
    Math_EaseToward(&player->m_vHeadRotOffset.z, ((yaw - body->m_RotY) + player->m_ContState.m_FinalLookX * 8.0f) * 0.017453294f, 0.1f);
    CPlayer__TurnAndLook(player, body, &gRidingLookTuning);

    offset = NULL;
    if (player->m_ContState.m_dwButtonsDouble & 8) {
        CAI__SetMode(mount, mount->ai, 0x2E);
        player->m_FleeTime = 90.0f;
        player->m_Speed = gRidingForwardMoveTuning_forwardMax * 2;
    }

    fwd = player->m_Speed;
    dist = player->m_OnEnemyStompTime;
    if (fwd < 0.0f) {
        fwd = -fwd;
    }
    if ((player->m_SideSpeed * 2 < 0.0f ? -(player->m_SideSpeed * 2) : player->m_SideSpeed * 2) < fwd) {
        step = player->m_Speed;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    } else {
        step = player->m_SideSpeed * 2;
        if (step < 0.0f) {
            next = dist - step;
        } else {
            next = dist + step;
        }
    }
    player->m_OnEnemyStompTime = next;
    if (sMountStepLength < next) {
        player->m_OnEnemyStompTime = next - sMountStepLength;
        if (mount->aiState != 0x2E) {
            player->m_Speed *= 0.5f;
            player->m_SideSpeed *= 0.5f;
        }
        t = player->m_Speed;
        if (t < 0.0f) {
            shake = -t * 2.5f;
        } else {
            shake = t * 2.5f;
        }
        if (50.0f < shake) {
            shake = 50.0f;
        }
        CCamera__SetShake(player->m_pCamera, 0.0f, shake, 0.0f, 1.0f, 1, body->header.m_vPos);
    }

    if (player->m_Speed > 0.0f) {
        CPlayer__MoveOnFoot(player, body, &gRidingForwardMoveTuning);
    } else {
        CPlayer__MoveOnFoot(player, body, &gRidingBackwardMoveTuning);
    }
    t = player->m_Speed;
    if ((t < 0.0f ? -t : t) > 10.24f) {
        player->m_FleeTime = 15.0f;
    }

    if (player->m_OnEnemyObjectType == 0x136) {
        offset = &gRidingEyeOffset;
        player->m_vOnEnemyScale = gRidingScale;
    }
    if (offset != NULL) {
        CMtxF__SetRotateY(&m, body->m_RotY);
        CMtxF__TransformPoint(&m, offset, &player->m_vEyeOffset);
        CVector3__Scale(&player->m_vEyeOffset, &player->m_vEyeOffset, 10.24f);
    }
    player->m_RequestedWeapon = 0x13;
}

void CCamera__StartFlash(CCamera *cam, s32 maxOpacity, s32 red, s32 green, s32 blue, s32 attack, s32 sustain, s32 decay);
void CCameraPool__StartShake(void *pool, f32 ampX, f32 ampY, f32 ampZ, f32 radius, s32 type, CVector3 pos);

/* Being shocked: flash and shake the view at random; when the time runs out
 * (or the player dies) drop back to standing and take the shock's damage. */
void PlayerShock_Update(CPlayer *player) {
    DamageEvent event;
    CCamera *cam;

    if (Random_Float(100.0f) > 80.0f) {
        cam = player->m_pCamera;
        CCamera__StartFlash(cam, 0xFF, 0xFF, 0, 0, (u8) (u32) Random_Float(7.5f), 3, 7);
        CCameraPool__StartShake(&gEngineApp_m_Game_m_CameraPool, 0.0f, 204.79999f, 0.0f, 512.0f, 0, cam->m_vPos);
    }
    player->m_CharacterActor.m_AI.m_ConditionTimer -= gFrameIncrement;
    if (player->m_CharacterActor.m_AI.m_ConditionTimer <= 0.0f || player->m_Data.m_Health == 0) {
        player->m_CharacterActor.m_dwFlags &= ~0x800000;
        CPlayer__SetState(player, player, 2);
        DamageEvent_Set(&event, player->m_pHitMeWithCBore, 0x3200, 25.599998f, 0x100000, 0);
        player->m_pHitMeWithCBore = NULL;
        Player_OnMultiplayerKill(player, (u8 *) &player->m_CharacterActor.m_AI, &event);
    }
}

extern s32 Random_Next(void);

VM_TEXT static __inline__ CIntelligence *findDef(s32 kind, s32 character) {
    return CScene__FindIntelligence(&gScene, kind, character);
}

VM_TEXT void CPlayer__SetCharacter(CPlayer *p, s32 character) {
    CIntelligence *def;
    s32 modelId;

    p->m_Data.m_nDeathmatchCharacter = character;
    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    def = findDef(11, character);
    if (def == NULL) {
        def = findDef(11, -1);
        if (def == NULL) {
            def = findDef(-1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = def;
    p->m_CharacterActor.m_vScale.x = def->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = def->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = def->m_Player.m_ScaleZ;
    p->m_Data.m_Health = def->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    if (p->m_Data.m_nDeathmatchCharacter == 7) {
        p->m_CharacterActor.m_AI.m_nTexture = Random_Next() % 18;
    } else {
        p->m_CharacterActor.m_AI.m_nTexture = -1;
    }
    CPlayer__GiveStartingWeapons(p);
    modelId = 0x66;
    if (gEngineApp_m_Settings.multiplayer) {
        if (gEngineApp_m_Settings_match_tag[0] != 0 && p->m_pOptions->m_TagMode != 0) {
            modelId = gTagModeModel;
        } else {
            modelId = gCharacterModels[p->m_CharacterActor.header.m_pIntelligence->m_Id];
        }
    }
    CScene__SetInstanceObjectType(&gScene, p, modelId, p->m_CharAnim);
}

#define PLAYER_LAST T2_OFFSETOF(CPlayer, m_pLast)
#define PLAYER_NEXT T2_OFFSETOF(CPlayer, m_pNext)

typedef struct PlayerList_42E5AC {
    /* 0x00 */ CPlayer **handle;
    /* 0x04 */ CPlayer *players;
    /* 0x08 */ s32 count;
    CPlayerCList list0[1];
    CPlayerCList list1[1];
} PlayerList_42E5AC;
struct CMemEntry *CCache__Allocate(s32, s32, u32, char *);
void CCache__Deallocate(sptr, void *);
void CCache__Flush(s32);
void Controller_SetRumbleOptionAll(s32);
void *memset(void *, s32, uptr);

/* CPlayer__Construct, inlined. */
VM_TEXT static __inline__ void initPlayer(CPlayer *p, s32 index, CPlayerOptions *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}

VM_TEXT void CPlayerPool__NewGame(PlayerList_42E5AC *list, s32 count, s32 arg2) {
    GameSettings *flags;
    CPlayerOptions *options;
    CPlayer *p;
    s32 i;

    CList__Construct(list->list0, PLAYER_LAST, PLAYER_NEXT);
    CList__Construct(list->list1, PLAYER_LAST, PLAYER_NEXT);
    Controller_SetRumbleOptionAll(0);
    CCache__Flush(0);
    flags = &gEngineApp_m_Settings;
    if (list->handle != NULL) {
        CCache__Deallocate(0, list->handle);
        list->handle = NULL;
        list->players = NULL;
        list->count = 0;
    }
    if (count == 0) {
        return;
    }
    list->count = count;
    list->handle = CCache__Allocate(0, count * sizeof(CPlayer), 0x23, "Players");
    list->players = *list->handle;
    memset(list->players, 0, count * sizeof(CPlayer));
    for (i = 0; i < list->count; i++) {
        CList__AddTail(list->list0, &list->players[i]);
    }
    options = &flags->defaultSlot;
    if (flags->multiplayer) {
        for (i = 0; i < list->count; i++) {
            p = &list->players[i];
            options = &flags->slots[i];
            initPlayer(p, i, options);
            if (options->m_bActive) {
                CPlayer__NewGame(p, options->m_nCharacter, 1);
                CList__Remove(list->list0, p);
                CList__AddHead(list->list1, p);
            }
        }
    } else {
        p = list->players;
        initPlayer(p, options->m_nController, options);
        CPlayer__NewGame(p, options->m_nCharacter, arg2);
        CList__Remove(list->list0, p);
        CList__AddHead(list->list1, p);
    }
    gEngineApp_m_Settings.match.tagPhase = 1;
}

/* The port: the player pool at its host layout (CPlayerPool, structs/player_list.h: the two
 * leading pointers and the list links are pointer-wide). */
typedef struct {
    CPlayer *head;
    CPlayer *tail;
    s32 lastOffset;
    s32 nextOffset;
    u32 count;
} PlayerCList;
typedef struct {
    void *pad0[2];
    s32 nPlayers;
    PlayerCList idle;
    PlayerCList active;
    s32 cinema;
    s32 cinemaStarted;
} PlayerList;
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
} MatchState;
#ifdef TARGET_PORT
/* The port: the settings fields at their host distance from the menu pool (CGame.m_MenuPool to
 * m_Settings.multiplayer, .paused, .match). */
#define G_OFF(m) __builtin_offsetof(CGame, m)
typedef struct {
    u8 pad0[G_OFF(m_Settings.multiplayer) - G_OFF(m_MenuPool)];
    u8 timed;
    u8 padCC6[G_OFF(m_Settings.paused) - G_OFF(m_Settings.multiplayer) - 1];
    s32 paused;
    u8 padE7C[G_OFF(m_Settings.match) - G_OFF(m_Settings.paused) - sizeof(s32)];
    MatchState match;
} Globals;
#else
/* The globals from gEngineApp_m_Game_m_MenuPool: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* gEngineApp_m_Settings_multiplayer */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* gEngineApp_m_Settings_paused */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState match;       /* gEngineApp_m_Settings_match */
} Globals;
#endif

typedef struct {
    /* 0x00 */ s8 character;
    /* 0x01 */ u8 pad1[0x40 - 1];
} PortSetup;
extern Globals gMenuPoolGlobalsView __asm__("gEngineApp_m_Game_m_MenuPool"); /* gEngineApp_m_Game_m_MenuPool (declared by globals/menu.h) seen as this file's type: what keeps the ROM's code */
void Automap_SetPlayer(void *, sptr region, f32 yaw, CVector3 pos);
s32 Controller_StartPressedNoCinema(ControllerProfile *c);
void Cinema_TrySkip(void);
s32 MenuPool_CountOpen(void *);
void Music_DuckVolume(void);
void Sound__StopAllExcept(s32);
void Sound__DisableAmbientTrack(void);
s32 Level_GetHub(s32 level);
s32 Cinema_GetScriptId(void);
s32 Cinema_IsFinished(void);
void CCameraPool__FreeCamera(void *pool, CCamera *cam);
void CPlayerPool__UpdateMatchClock(void *);
void CPlayerPool__PickTagIt(CPlayerPool *list);

/* CPlayer__IsPausedOrInMenu: paused, or a menu other than the death screen open on the
 * player's camera. */
static __inline__ s32 in_menu(CPlayer *p) {
    s32 result;

    if (gEngineApp_m_Settings.paused) {
        return 1;
    }
    result = 0;
    if (p->m_pCamera != NULL && p->m_pCamera->m_Menus.head != NULL) {
        result = p->m_pCamera->m_Menus.head->desc != (MenuDesc *) &gPauseMenuDesc;
    }
    return result;
}

/* Update the players: move each one in the game (unless paused or in a menu:
 * CPlayer__IsPausedOrInMenu), handle pause (the death screen), the hub key screen and the level-end
 * cinema; let idle players join while the match allows it and send players
 * who left back to the idle list; then run the match clock and tag. */
void CPlayerPool__Update(PlayerList *list) {
    CPlayer *p;
    CPlayer *next;
    f32 frameTime;
    u32 hub;
    s32 character;

    if (gEngineApp_m_Settings.nextMode == 0xB || gEngineApp_m_Settings.nextMode == 8) {
        return;
    }
    frameTime = gFrameIncrement;
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        if (!in_menu(p)) {
            CPlayer__Update(p);
        }
        CPlayer__UpdateOrbitCamera(p, p);
        Automap_SetPlayer(&gEngineApp_m_Game.m_Automap, p->m_CharacterActor.header.m_pCurrentRegion, p->m_CharacterActor.m_RotY, p->m_CharacterActor.header.m_vPos);
    }
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        CPlayer__ReadInput(&p->m_ContState, p);
        if (Controller_StartPressedNoCinema(p->m_ContState.m_pContState)) {
            if (Cinema_IsActive()) {
                Cinema_TrySkip();
            } else if (!MenuPool_CountOpen(&gMenuPoolGlobalsView) && p->m_Data.m_Health != 0 && p->m_pCamera != NULL
                       && p->m_pCamera->m_Menus.count == 0 && (gMenuPoolGlobalsView.match.over == 0 || gMenuPoolGlobalsView.timed == 0)) {
                gMenuPoolGlobalsView.paused = 1;
                Music_DuckVolume();
                Sound__StopAllExcept(300);
                Sound__DisableAmbientTrack();
                reset_strokes(&p->m_Barrel);
                p->m_LivesOverlay.m_Value = -1;
                p->m_TokensOverlay.m_Value = -1;
                MenuPool_OpenMenu(&gMenuPoolGlobalsView, &gPauseMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
        }
        if (p->m_bLevelNotCompleteExit) {
            hub = Level_GetHub(gScene_m_LevelID) - 1;
            if (hub < 6 && (p->m_Data.m_dwLevelFirstTimeFlags & (1 << hub))) {
                MenuPool_OpenMenu(&gMenuPoolGlobalsView, &gMissionFailedMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
            p->m_bLevelNotCompleteExit = 0;
        }
        if (!MenuPool_CountOpen(&gMenuPoolGlobalsView) && Cinema_PathPlaying() && Cinema_GetScriptId() == 400 && !list->cinema) {
            if (p->m_pCamera != NULL && p->m_pCamera->m_Menus.count == 0) {
                list->cinema = 1;
                list->cinemaStarted = 0;
            }
        }
        if (list->cinema) {
            if (Cinema_IsFinished() && !list->cinemaStarted) {
                list->cinemaStarted = 1;
                MenuPool_OpenMenu(p->m_pCamera->m_MenuPool, &gSavePortalMenuDesc, p, (p->m_ContState.m_pContState), 0);
            }
        }
    }
    for (p = list->idle.head; p != NULL; p = next) {
        MatchState *match;

        CPlayer__ReadInput(&p->m_ContState, p);
        next = p->m_pNext;
        match = &gMenuPoolGlobalsView.match;
        if (p != NULL && !match->over && (match->timeLeft >= 900.0f || match->timeLeft < 0.0f || !gMenuPoolGlobalsView.timed)
            && ((p->m_ContState.m_pContState->buttonsSingle & 0x1000) || p->m_bActive)) {
            p->m_bActive = 1;
            character = gEngineApp_m_Settings.slots[p->m_nController].m_nCharacter;
            p->m_pOptions->m_bActive = 1;
            CPlayer__NewGame(p, character, 1);
            CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
            CPlayer__Warp(p, gEngineApp_m_Settings.matchMode, 0, 1);
            CList__Remove(&list->idle, p);
            CList__AddHead(&list->active, p);
        }
    }
    for (p = list->active.head; p != NULL; p = next) {
        next = p->m_pNext;
        if (p != NULL && !p->m_bActive && list->active.count >= 2) {
            if (p->m_pCamera != NULL) {
                CCameraPool__FreeCamera(&gEngineApp_m_Game_m_CameraPool, p->m_pCamera);
                p->m_pCamera = NULL;
            }
            CList__Remove(&list->active, p);
            CList__AddHead(&list->idle, p);
        }
    }
    CPlayerPool__UpdateMatchClock(list);
    CPlayerPool__PickTagIt(list);
    gFrameIncrement = frameTime;
}

/* Same as CPlayer__PickSpawnPoint. */
static __inline__ void place_at_spawn(CPlayer *p) {
    CIntelligence *sp;
    s32 character = p->m_Data.m_nDeathmatchCharacter;

    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    sp = CScene__FindIntelligence(&gScene, 11, character);
    if (sp == NULL) {
        sp = CScene__FindIntelligence(&gScene, 11, -1);
        if (sp == NULL) {
            sp = CScene__FindIntelligence(&gScene, -1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = sp;
    p->m_CharacterActor.m_vScale.x = sp->m_Player.m_ScaleX;
    p->m_CharacterActor.m_vScale.y = sp->m_Player.m_ScaleY;
    p->m_CharacterActor.m_vScale.z = sp->m_Player.m_ScaleZ;
}

/* Put the player into tag mode ("it"): a sound, the weapon wheel and the
 * weapon reset, then respawn with full health. */
void CPlayer__BecomeIt(CPlayer *p) {
    CBarrel *barrel;

    p->m_pOptions->m_TagMode = 1;
    Sound__Play2D(0x27D8);
    barrel = &p->m_Barrel;
    barrel->m_State = 0;
    barrel->m_Turn = 0;
    barrel->m_nCursor = -1;
    barrel->m_Open = 0;
    p->m_bHeadMovementDisabled = 0;
    CAI__SetMode((u8 *)p + T2_OFFSETOF(CPlayer, m_WeaponActor), (u8 *)p + T2_OFFSETOF(CPlayer, m_WeaponActor.m_AI), 1);
    place_at_spawn(p);
    p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : Random_Next() % 18;
}
void CPlayerPool__RespawnAll(CPlayerPool *list);
void CPlayerPool__EndMatch(CPlayerPool *arg0);
s32 CPlayerPool__IsScoreLimitReached(CPlayerPool *list);
CPlayer *CPlayerPool__GetViewPlayer(CPlayerPool *list, u32 index);
void CPlayerPool__ClearLastTagMode(CPlayerPool *arg0);
s32 CPlayerPool__CountPlaying(CPlayerPool *arg0);

/* Tag: pick who is "it" at the start of a round, flashing over the viewports
 * in turn until the pick lands. */
void CPlayerPool__PickTagIt(CPlayerPool *list) {
    s32 count;
    CPlayer *p;
    CCamera *cam;
    f32 w, h, cx, cy, sx, sy;
    GameMatch *s = &gEngineApp_m_Settings_match;

    if (!s->tag) {
        return;
    }
    if (CPlayerPool__IsScoreLimitReached(list) && s->scoreLimit > 0 && !s->over) {
        CPlayerPool__EndMatch(list);
        return;
    }
    count = CPlayerPool__CountPlaying(list);
    s = &gEngineApp_m_Settings_match;
    switch (s->tagPhase) {
        case 0:
        default:
            CPlayerPool__ClearLastTagMode(list);
            CPlayerPool__RespawnAll(list);
            s->tagPhase = 1;
            break;
        case 1:
            CPlayerPool__RespawnAll(list);
            if (count > 0) {
                s32 r = Random_Next() % count;
                s->tagPhase = 2;
                s->tagPickStart = r;
                s->tagPick = r;
            }
            break;
        case 2:
            p = CPlayerPool__GetViewPlayer(list, s->tagPick % count);
            cam = p->m_pCamera;
            if (cam != NULL && cam->m_Menus.count == 0) {
                Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
                w = cam->m_Viewport.m_Width;
                h = cam->m_Viewport.m_Height;
                cx = cam->m_Viewport.m_ScreenPosX + w * 0.5f;
                cy = cam->m_Viewport.m_ScreenPosY + h * 0.5f;
                sx = w / gScreenWidth;
                sy = h / gScreenHeight;
                Hud_DrawSprite(0x1FB, 0, cx - sx * 16.0f, cy - sy * 16.0f, sx, sy, 1);
            }
            s = &gEngineApp_m_Settings_match;
            if (++s->tagPick >= (5 - count) * 20 - s->tagPickStart) {
                CPlayerPool__ClearLastTagMode(list);
                s->tagPhase = 3;
                CPlayer__BecomeIt(p);
            }
            break;
        case 3:
            break;
    }
}

/* Respawn every player at a spawn point, with full health, out of tag mode.
 * A goto loop: as a for/do loop GCC hoists the constant 1 out of it. */
void CPlayerPool__RespawnAll(CPlayerPool *list) {
    CPlayer *p;

    p = list->m_ActiveList.m_pHead;
    if (p == NULL) return;
loop:
    {
        p->m_pOptions->m_TagMode = 0;
        place_at_spawn(p);
        p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
        p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
        p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : Random_Next() % 18;
        p = p->m_pNext;
        if (p != NULL) goto loop;
    }
}

/* Whether the frag limit is reached: by either team's total in team games,
 * otherwise by one player's frags (the slot CPlayerPool__GetIndex names left out). */
s32 CPlayerPool__IsFragLimitReached(CPlayerPool *list) {
    GameMatch *state = &gEngineApp_m_Settings.match;
    CPlayer *p;
    s32 i;

    if (state->teams) {
        s32 team1 = 0;
        s32 team0 = 0;
        sptr skip;

        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            skip = CPlayerPool__GetIndex(list, p);
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    switch (p->m_nDeathmatchTeamSelected) {
                        case 0:
                            team0 += p->m_pOptions->m_Killed[i];
                            break;
                        case 1:
                            team1 += p->m_pOptions->m_Killed[i];
                            break;
                    }
                }
            }
            if (team0 >= gEngineApp_m_Settings.match.scoreLimit) {
                return 1;
            }
            if (team1 >= gEngineApp_m_Settings.match.scoreLimit) {
                return 1;
            }
        }
    } else {
        GameMatch *gs = state;

        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            sptr skip;
            s32 sum;

            skip = CPlayerPool__GetIndex(list, p);
            sum = 0;
            for (i = 0; i < 4; i++) {
                if (i != skip) {
                    sum += p->m_pOptions->m_Killed[i];
                }
            }
            if (sum >= gs->scoreLimit) {
                return 1;
            }
        }
    }
    return 0;
}

#ifdef TARGET_PORT
#include "structs/camera.h"
#endif

extern s32 Cinema_PathPlaying(void);
s32 LoopSounds_Stop(void *, void *);
extern void CGameObjectInstance__UpdateAI(CGameObjectInstance *obj);

/* Whether the player's camera has a menu open. */
static __inline__ s32 menu_open(CPlayer *p) {
    void *cam = p->m_pCamera;
    s32 open = 0;

    if (cam != NULL) {
#ifdef TARGET_PORT
        open = ((CCamera *) cam)->m_Menus.count != 0; /* N64 0x54C: the menu pool's count */
#else
        open = *(s32 *)((u8 *)cam + 0x54C) != 0;
#endif
    }
    return open;
}

/* Animate each living player's weapon model on its own (frame time kept),
 * then put it back on the player; stop the weapon sounds otherwise. */
void CPlayerPool__AnimateWeapons(CPlayerPool *list) {
    CPlayer *p;

    if (gEngineApp_m_Settings.nextMode == 11 || gEngineApp_m_Settings.nextMode == 8) {
        return;
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if (Cinema_PathPlaying() || p->m_Data.m_Lives == 0) {
            LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponBackground);
            p->m_pLoopingSound_WeaponBackground = NULL;
            LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponFiring);
            p->m_pLoopingSound_WeaponFiring = NULL;
        } else if (gEngineApp_m_Settings.paused == 0) {
            if (!menu_open(p)) {
                CGameObjectInstance *weapon = &p->m_WeaponActor;
                s32 old = p->m_CharAnim;
                f32 frameTime = gFrameIncrement;

                p->m_WeaponActor.m_dwFlags |= 0x200;
                weapon->m_dwHotPointFlags = 16;
                weapon->m_dwFlags &= ~0x20000;
                CGameObjectInstance__UpdateAI(weapon);
                CGameObjectInstance__UpdateAnimation(weapon);
                gFrameIncrement = frameTime;
                if (old != p->m_CharAnim) {
                    p->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
                }
                p->m_WeaponActor.header.m_vPos = p->m_CharacterActor.header.m_vPos;
                p->m_WeaponActor.m_RotY = p->m_CharacterActor.m_RotY;
                p->m_WeaponActor.header.m_pCurrentRegion = p->m_CharacterActor.header.m_pCurrentRegion;
                p->m_WeaponActor.m_qGround = p->m_CharacterActor.m_qGround;
                p->m_WeaponActor.m_dwFlags &= ~0x200;
            }
        }
    }
}

/* The weapon actor's owner, at 0x1A8 (inside m_AI). */
#define OWNER(obj) ((*(CPlayer **)&((CGameObjectInstance *)(obj))->m_AI.m_pParent))

extern void CGameObjectInstance__UpdateBounds(CGameObjectInstance *obj, void *info);
extern void Shadow_DrawBlob(CGameObjectInstance *obj, CVector3 *offset, s32 arg2);

/* Reload the animation object info of every player's actor that has one. For
 * the player viewing through cam (while cam's word 0x24 is clear) it is the
 * weapon model instead, turned toward its owner's camera. */
void CPlayerPool__ReloadAnimInfo(CPlayerPool *list, CCamera *cam) {
    CPlayer *p;
    CGameObjectInstance *obj;
    void **res;
    CPlayer *owner;
    CVector3 d;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if (p->m_pCamera == cam && cam->m_ViewParams.m_nType == 0) { /* N64 0x24 (round 12 a5) */
            obj = &p->m_WeaponActor;
            if (obj->m_dwFlags & 0x40000) {
                res = CCache__Request(0, (sptr)obj->m_rpObject, obj->m_rpObjectHeader, 4, 0, 0, "player anim object info", 1);
                if (res != NULL) {
                    owner = OWNER(obj);
                    CGameObjectInstance__UpdateBounds(obj, CISet__GetBlock(*res, 0));
                    d.x = owner->m_pCamera->m_vPos.x - obj->header.m_vPos.x;
                    d.y = 0.0f;
                    d.z = owner->m_pCamera->m_vPos.z - obj->header.m_vPos.z;
                    Shadow_DrawBlob(obj, &d, 1);
                    CCache__Unlock(0, res);
                }
            }
        } else if (p->m_CharacterActor.m_dwFlags & 0x40000) {
            void **info = CCache__Request(0, (sptr)p->m_CharacterActor.m_rpObject, p->m_CharacterActor.m_rpObjectHeader, 4, 0, 0, "player anim object info", 1);

            if (info != NULL) {
                CGameObjectInstance__UpdateBounds(&p->m_CharacterActor, CISet__GetBlock(*info, 0));
                CCache__Unlock(0, info);
            }
        }
    }
}

extern void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32 arg5, s32 arg6, s32 arg7);

/* Draw the match clock (gEngineApp_m_Settings.match.timeLeft, in ticks of 1/15 s), minutes and seconds, centered at the top of the
 * screen (mid-screen when gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount != 1), scaled to cam's viewport; in the
 * last 30 seconds it blinks. */
void Hud_DrawMatchClock(s32 arg0, CCamera *cam) {
    f32 secs, mins;
    f32 sx, sy;
    f32 x, y;

    Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
    if (gEngineApp_m_Settings.match.timeLeft <= 0.0f) {
        return;
    }
    secs = gEngineApp_m_Settings.match.timeLeft * (1.0f / 15);
    mins = (s32)(secs * (1.0f / 60));
    secs -= mins * 60.0f;
    sx = cam->m_Viewport.m_Width / gScreenWidth;
    sy = cam->m_Viewport.m_Height / gScreenHeight;
    if (mins < 1.0f && secs < 30.0f && gFrameCountGameplay % 15 < 5) {
        return;
    }
    if (gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_nCount == 1) {
        x = gScreenWidth * 0.5f;
        y = 20.0f;
    } else {
        x = gScreenWidth * 0.5f;
        y = (gScreenHeight - 10) * 0.5f;
    }
    Hud_DrawNumber((s32)mins, x - sx * 6.0f, y, sx, sy, 1, 1, 0);
    Hud_DrawNumber((s32)secs, x + sx * 2.0f, y, sx, sy, 1, 0, 2);
}

#ifdef TARGET_PORT
struct ObjectPool {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ObjectPool(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_allocation[T2_HOFF_ObjectPool(m_pmePlayers)]; void *allocation; }; /* N64 0x0 */
        struct { u8 _hp_objects[T2_HOFF_ObjectPool(m_Players)]; char *objects; }; /* N64 0x4 */
        struct { u8 _hp_count[T2_HOFF_ObjectPool(m_nPlayers)]; s32 count; }; /* N64 0x8 */
        struct { u8 _hp_active_queue[T2_HOFF_ObjectPool(m_IdleList)]; CList active_queue; }; /* N64 0xC */
        struct { u8 _hp_object_queue[T2_HOFF_ObjectPool(m_ActiveList)]; CList object_queue; }; /* N64 0x20 */
    };
};
#else
struct ObjectPool {
    void *allocation;
    char *objects;
    s32 count;
    CList active_queue; /* 0x0C */
    CList object_queue; /* 0x20 */
};
#endif
/* Minimal Turok 2 view of a pooled actor (offsets differ from Rage Wars). */
#ifdef TARGET_PORT
struct Shared_ActorView {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Shared_ActorView(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_weapon[T2_HOFF_Shared_ActorView(m_WeaponActor)]; u8 weapon[sizeof(CGameObjectInstance)]; }; /* N64 0x288 */
        struct { u8 _hp_next[T2_HOFF_Shared_ActorView(m_pNext)]; struct Shared_ActorView *next; }; /* N64 0x1320 */
    };
};
#else
struct Shared_ActorView {
    u8 pad0[0x288];
    u8 weapon[0x1320 - 0x288]; /* 0x288 */
    struct Shared_ActorView *next; /* 0x1320 */
};
#endif

void Actor_ReloadAnimInfo(CGameObjectInstance *actor, s32 arg1);
extern s32 CGameObjectInstance__LoadBoneMatrices(void *arg0, s32 arg1);
extern void CGameObjectInstance__ClearDrawMatrices(void *arg0);

void CPlayerPool__PreUpdate(struct ObjectPool *arg0) {
    void *node;
    if (*(void **)arg0 != 0) {
        CCache__LockForDisplayList(0, *(void **)arg0);
    }
    node = arg0->object_queue.m_pHead;
    if (node != 0) {
        u8 *base = (u8 *)&gEngineApp_m_Settings;
        do {
            CGameObjectInstance__ClearDrawMatrices(node);
            CGameObjectInstance__ClearDrawMatrices(&((struct Shared_ActorView *)(node))->weapon);
            Actor_ReloadAnimInfo(node, 0);
            if (base[0x19] != 0) {
                CGameObjectInstance__LoadBoneMatrices(node, 1);
            }
            node = ((struct Shared_ActorView *)(node))->next;
        } while (node != 0);
    }
}

s32 GameObject_IsSolid(CGameObjectInstance *);

void CPlayerPool__RegisterActors(CPlayerPool *pool, CScene *w) {
    CPlayer *p;
    s32 n;

    for (p = pool->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        p->m_CharacterActor.m_CollisionYOffset = 0;
        p->m_WeaponActor.m_CollisionYOffset = 0;
        if (GameObject_IsSolid(&p->m_CharacterActor) != 0) {
            n = w->m_nDrawActors;
            if (n != 0x200) {
                w->m_pDrawActors[n] = &p->m_CharacterActor;
                w->m_nDrawActors = n + 1;
            }
            n = w->m_nMovers;
            if (n != 0x80) {
                w->m_pMovers[n] = &p->m_CharacterActor;
                w->m_nMovers = n + 1;
            }
        }
    }
}

s32 Cinema_FollowsPlayer(void);
void Render_ResetBatches();                               /* extern */

void CPlayerPool__DrawCharacters(CPlayerPool *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    Render_ResetBatches();
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera != arg1) || (arg1->m_ViewParams.m_nType == 1) || (Cinema_FollowsPlayer() != 0)) {
                CPlayer__DrawCharacter((sptr) var_s0, (sptr) arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    Render_FlushBatches();
}

void CPlayerPool__DrawWeaponOverlays(CPlayerPool *arg0, CCamera *arg1) {
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_pCamera == arg1) && (arg1->m_ViewParams.m_nType == 0)) {
                CPlayer__DrawWeaponOverlay(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}

s32 CCamera__LoadOnScreenProjection(sptr);                         /* extern */

void CPlayerPool__DrawHuds(CPlayerPool *arg0, sptr arg1) {
    CPlayer *var_s0;

    CCamera__LoadOnScreenProjection(arg1);
    var_s0 = arg0->m_ActiveList.m_pHead;
    if (var_s0 != NULL) {
        do {
            if ((sptr) var_s0->m_pCamera == arg1) {
                CPlayer__DrawHud(var_s0, arg1);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS Hud_DrawMatchClock receives CPlayerPool__DrawMatchClock's own argument
 * registers (pass-through); the host passes them. */
void CPlayerPool__DrawMatchClock(sptr pt0, void * pt1) {
    Hud_DrawMatchClock(pt0, pt1);
}

sptr CPlayerPool__GetFirstActive(void *pool) {
    return (sptr) ((CPlayerPool *) pool)->m_ActiveList.m_pHead;
}

/* The first player whose tag mode (options 0x3E) is 1. */
CPlayer *CPlayerPool__FindTagged(CPlayerPool *list) {
    CPlayer *p;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if (p->m_pOptions->m_TagMode == 1) {
            return p;
        }
    }
    return NULL;
}

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A470.c, func_0022A470.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2 layout: pos at 0x4, next at 0x1320 (Rage Wars: 0x8 / 0x16E0). */
typedef struct Plr Plr;
#ifdef TARGET_PORT
struct Plr {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Plr(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Plr(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_next[T2_HOFF_Plr(m_pNext)]; Plr *next; }; /* N64 0x1320 */
    };
};
#else
struct Plr {
    u8 pad0[0x4];
    CVector3 pos;          /* 0x0004 */
    u8 pad10[0x1310];
    Plr *next;          /* 0x1320 */
};
#endif

Plr *CPlayerPool__FindNearest(CPlayerPool *g, CVector3 p) {
    Plr *a;
    Plr *best = ((void *)0);
    f32 min = 3.4028235e38f;
    f32 dx, dy, dz, d;
    for (a = g->m_ActiveList.m_pHead; a != ((void *)0); a = a->next) {
        dx = a->pos.x - p.x;
        dy = a->pos.y - p.y;
        dz = a->pos.z - p.z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < min) {
            min = d;
            best = a;
        }
    }
    return best;
}

#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/player_list.h"
#endif

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A4F4.c, func_0022A4F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Plr_253370 Plr_253370;
typedef struct PlrGroup PlrGroup;
/* Turok 2 player: only the fields used here (Rage Wars: pos 0x8, unk5E4, next 0x16E0). */
#ifdef TARGET_PORT
struct Plr_253370 {
/* the view at the host offsets of CPlayer (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Plr(m) __builtin_offsetof(CPlayer, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Plr(m_CharacterActor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_unk524[T2_HOFF_Plr(m_Data.m_Health)]; s32 unk524; }; /* N64 0x524 */
        struct { u8 _hp_next[T2_HOFF_Plr(m_pNext)]; Plr_253370 *next; }; /* N64 0x1320 */
    };
};
#else
struct Plr_253370 {
    u8 pad0[0x4];
    CVector3 pos;      /* 0x4 */
    u8 pad10[0x514];
    s32 unk524;     /* 0x524 */
    u8 pad528[0xDF8];
    Plr_253370 *next;      /* 0x1320 */
};
#endif
#ifdef TARGET_PORT
struct PlrGroup {
/* the view at the host offsets of CPlayerPool (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_PlrGroup(m) __builtin_offsetof(CPlayerPool, m)
    union {
        struct { u8 _hp_unk0[T2_HOFF_PlrGroup(m_pmePlayers)]; s32* unk0; }; /* N64 0x0 */
        struct { u8 _hp_unk4[T2_HOFF_PlrGroup(m_Players)]; Plr_253370* unk4; }; /* N64 0x4 */
        struct { u8 _hp_unk8[T2_HOFF_PlrGroup(m_nPlayers)]; s32 unk8; }; /* N64 0x8 */
        struct { u8 _hp_head[T2_HOFF_PlrGroup(m_ActiveList.m_pHead)]; Plr_253370* head; }; /* N64 0x20 */
    };
};
#else
struct PlrGroup {
                 s32* unk0;
                 Plr_253370* unk4;
                 s32 unk8;
                 u8 padC[0x14];
                 Plr_253370* head;
                 u8 pad24[0x1C];
                 s32 unk40;
};
#endif

Plr_253370 *CPlayerPool__FindNearestOther(PlrGroup *g, CVector3 p, Plr_253370 *skip) {
    Plr_253370 *a;
    Plr_253370 *best = ((void *)0);
    f32 min = 3.4028235e38f;
    f32 dx, dy, dz, d;
    for (a = g->head; a != ((void *)0); a = a->next) {
        if (a->unk524 != 0 && a != skip) {
            dx = a->pos.x - p.x;
            dy = a->pos.y - p.y;
            dz = a->pos.z - p.z;
            d = dx * dx + dy * dy + dz * dz;
            if (d < min) {
                min = d;
                best = a;
            }
        }
    }
    return best;
}

VM_TEXT void CPlayerPool__Construct(CPlayerPool *pl) {
    memset(pl, 0, sizeof(CPlayerPool));
    CList__Construct(&pl->m_IdleList, PLAYER_LAST, PLAYER_NEXT);
    CList__Construct(&pl->m_ActiveList, PLAYER_LAST, PLAYER_NEXT);
    pl->m_pmePlayers = 0;
    pl->m_Players = 0;
    pl->m_nPlayers = 0;
    pl->m_bCinema = 0;
    pl->m_bCinemaStarted = 0;
}

extern void SavedPlayers_Deactivate(void);
extern void SavedPlayers_Restore(void);

VM_TEXT void CPlayerPool__LevelStart(CPlayerPool *list) {
    CPlayer *p;

    if (gEngineApp_m_Settings.mode == 13 && SavedPlayers_IsActive() != 0) {
        SavedPlayers_Restore();
        SavedPlayers_Deactivate();
    }
    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        CPlayer__LevelStart(p);
        CPlayer__UpdateOrbitCamera(p, p);
        Cinema_UpdateCameraIfActive(p->m_pCamera);
    }
}

VM_TEXT void CPlayer__Construct(CPlayer *p, s32 index, void *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}

VM_TEXT void CPlayer__Activate(CPlayer *p, s32 arg1) {
    p->m_pOptions->m_bActive = 1;
    CPlayer__NewGame(p, arg1, 1);
    CAI__SetMode(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
    CPlayer__Warp(p, gEngineApp_m_Settings.matchMode, 0, 1);
}

VM_TEXT void CPlayer__ResetHealth(CPlayer *p) {
    GameSettings *flags = &gEngineApp_m_Settings;

    if (flags->multiplayer != 0 || (Controller_SetRumbleOption(p->m_ContState.m_pContState, 1), flags->multiplayer != 0)) {
        CPlayer__Respawn(p);
    } else {
        p->m_Data.m_Health = 0x6400;
        p->m_CharacterActor.m_AI.m_Health = 0x6400;
    }
    p->m_InvincibilityTime = 60.0f;
    CPlayer__Reset(p);
    p->m_ShockedTimer = 0;
    p->m_pHitMeWithCBore = 0;
    p->m_pHitMeWithChargeDart = 0;
    p->m_nNumArrowsStuckInMe = 0;
    p->m_UseLowerAmmo = 0;
    if (gEngineApp_m_Settings.mode == 13 && gEngineApp_m_Settings.multiplayer == 0 && gFrameCountGameplay >= 11) {
        if (p->m_Data.m_Lives < 11) {
            Sound__PlaySpeech(gLivesSpeech[p->m_Data.m_Lives]);
        }
    }
    if (gEngineApp_m_Settings_multiplayer != 0) {
        CPlayer__GiveStartingWeapons(p);
    }
}

/* Pool of 0x1328-byte entries (4904 bytes each) starting at base. */
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ u32 base;
} Pool0025340C;

/* Host: the player array pointer-wide, and the host CPlayer's size (the N64 view read the array
 * as a u32 and divided by the N64 size 0x1328: a kill in high memory or a PIE indexed
 * m_KilledBy far out of range, STABILITY.md 7.2 S22). */
uptr CPlayerPool__GetIndex(void *arg0, uptr arg1) {
    return (arg1 - (uptr) ((CPlayerPool *) arg0)->m_Players) / sizeof(CPlayer);
}

void *CPlayerPool__FindByController(char *object, sptr arg1) {
    CPlayer *p;

    for (p = ((CPlayerPool *) object)->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        if ((sptr) p->m_ContState.m_pContState == arg1) {
            return p;
        }
    }
    return 0;
}

/* Once every player has options byte 0x3D set to 1, turns the pending flag
 * into the done flag and returns 1. */
s32 CPlayerPool__CheckAllReady(CPlayerPool *list) {
    GameMatch *state = &gEngineApp_m_Settings.match;
    CPlayer *p;
    s32 n;

    if (state->done) {
        return 0;
    }
    if (state->over) {
        n = 0;
        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            if (((CPlayerOptions *)p->m_pOptions)->unk_0x3D == 1) {
                n++;
            }
        }
        if (n == list->m_ActiveList.m_nCount) {
            GameMatch *s = &gEngineApp_m_Settings.match;

            s->done = 1;
            s->over = 0;
            return 1;
        }
    }
    return 0;
}

typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0;

/* 0x1328-byte list entries (see CPlayerPool__GetIndex) linked through 0x1320. */


void CPlayerPool__EndMatch(CPlayerPool *arg0) {
    CPlayer *var_a0;
    CPlayer *var_a0_2;
    CPlayer *var_s0;
    CPlayerPool *var_s1;

    var_s0 = (CPlayer *) &gEngineApp_m_Settings.match;
    var_s0 = (CPlayer *) &gEngineApp_m_Settings.match;
    var_s1 = arg0;
    if ((gEngineApp_m_Settings.match.over == 0) && (gEngineApp_m_Settings.match.done == 0)) {
        Controller_SetRumbleOptionAll(0);
        if (gEngineApp_m_Settings.match.teams != 0) {
            if (gEngineApp_m_Settings.match.teamOver == 0) {
                gEngineApp_m_Settings.match.teamOver = 1;
                var_s0 = var_s1->m_ActiveList.m_pHead;
                var_a0 = var_s0;
                if (var_s0 != NULL) {
                    do {
                        CPlayer__SetState(var_a0, var_s0, 0x12);
                        var_s0 = var_s0->m_pNext;
                        var_a0 = var_s0;
                    } while (var_s0 != NULL);
                }
            }
        } else {
            gEngineApp_m_Settings.match.over = 1;
            var_s0 = var_s1->m_ActiveList.m_pHead;
            var_a0_2 = var_s0;
            if (var_s0 != NULL) {
                do {
                    CPlayer__SetState(var_a0_2, var_s0, 0x12);
                    var_s0 = var_s0->m_pNext;
                    var_a0_2 = var_s0;
                } while (var_s0 != NULL);
            }
        }
    }
}

s32 RumblePool_FreeAll(sptr);                         /* extern */
#ifdef TARGET_PORT
#include "structs/game.h"
#include "structs/player.h"
#include "structs/controller.h"
/* The port (round 11 C26): the players and their controller profiles by member. The N64 views
 * (0x20 the list head, 0xA50 m_ContState.m_pContState, 0x1320 m_pNext; the profile 0xD0
 * rumbleLevel, 0x140 rumblePool) read other host members: the walk went off the list. */
void CPlayerPool__StopRumble(CPlayerPool *arg0) {
    CPlayer *p = arg0->m_ActiveList.m_pHead;

    while (p != NULL) {
        ControllerProfile *c = p->m_ContState.m_pContState;
        RumblePool_FreeAll((sptr) c->rumblePool);
        c->rumbleLevel = 0.0f;
        p = p->m_pNext;
    }
}
#else
typedef struct {
    /* 0x00 */ u8 pad0[0xD0];
    /* 0xD0 */ s32 unkD0;
    /* 0xD4 */ u8 padD4[0x140 - 0xD4];
    /* 0x140 */ u8 sub140[1];
} Attached002534E0_25359C;

/* 0x1328-byte list entries (see CPlayerPool__GetIndex) linked through 0x1320. */
typedef struct Node002534E0 {
    /* 0x0000 */ u8 pad0[0xA50];
    /* 0x0A50 */ Attached002534E0_25359C *attached;
    /* 0x0A54 */ u8 padA54[0x1320 - 0xA54];
    /* 0x1320 */ struct Node002534E0 *next;
} Node002534E0;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ Node002534E0 *head;
} List002534E0;

void CPlayerPool__StopRumble(List002534E0 *arg0) {
    Node002534E0 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            RumblePool_FreeAll((sptr) var_s0->attached->sub140);
            var_s0->attached->unkD0 = 0;
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
#endif

void CPlayerPool__SetInvincibility(CPlayerPool *arg0, f32 arg1) {
    CPlayer *var_v0;

    var_v0 = arg0->m_ActiveList.m_pHead;
    if (var_v0 != NULL) {
        do {
            var_v0->m_InvincibilityTime = arg1;
            var_v0 = var_v0->m_pNext;
        } while (var_v0 != NULL);
    }
}

/* Only what this reads and writes. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 value;
    /* 0x08 */ u8 pad8[0xB4 - 0x8];
    /* 0xB4 */ s32 repeated;
} Tracker;

/* Sets the value, noting when it is the same as before. */
void ValueTracker_Set(Tracker *arg0, s32 arg1) {
    if (arg0->value == arg1) {
        arg0->repeated = 1;
    }
    arg0->value = arg1;
}

/* A record with a state word at 0xB4 (0 and 3 are left alone). */
typedef struct {
    /* 0x00 */ u8 pad0[0xB4];
    /* 0xB4 */ s32 state;
} StateRecord;

void CPlayerOverlay__Lower(StateRecord *arg0) {
    s32 temp_v0;

    temp_v0 = arg0->state;
    if ((temp_v0 != 0) && (temp_v0 != 3)) {
        arg0->state = 3;
    }
}

extern s32 Level_GetHub(s32);

/* Whether Adon has not yet helped with ammo (kind 0) or health (kind 1) on
 * the hub level that Level_GetHub maps arg to (1-6). */
s32 CPlayer__CanGetRefill(CPlayer *player, s32 kind, s32 arg) {
    u32 hub = Level_GetHub(arg) - 1;

    if (hub >= 6) {
        return 0;
    }
    switch (kind) {
        case 0:
        default:
            return player->m_Data.m_AdonAmmoFlagHelped[hub] == 0;
        case 1:
            return player->m_Data.m_AdonHealthFlagHelped[hub] == 0;
    }
}

/* De-faked in round 21 (j4): the matched draft had a dead if/else with two identical
 * arms (a permuter find); the early return for the cheat (flag 0x10) matches, so the N64 and the host share it. */
/* Whether item is a special pickup (0xFA0-0x13A0); never with cheat flag 0x10. */
s32 CPlayer__IsSpecialPickup(CPlayer *player, s32 item) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 0;
    }
    return (u32) (item - 0xFA0) < 0x401U;
}

s32 CPlayer__GetSpecialPickupIndex(s32 arg0, s32 arg1) {
    return arg1 - 0xFA0;
}

void CPlayer__UseSpecialPickup(CPlayer *arg0, s32 arg1) {

    if (!(gEngineApp_m_Settings.cheatFlags & 0x10)) {
        if (arg1 < 0x1121) {
            if (arg1 < 0x111C) {
                goto block_3;
            }
        } else {
block_3:
            arg0->m_Data.m_SpecialPickupCount[arg1 - 0xFA0]--;
        }
    }
}

/* De-faked in round 21 (j4): the matched draft had a dead if/else with two identical
 * arms (a permuter find); the early return for the cheat (flag 0x10) matches, so the N64 and the host share it. */
/* The low nibble of the special pickup's count (1 with cheat flag 0x10). */
s32 CPlayer__GetPickupCountLow(CPlayer *player, s32 item) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return player->m_Data.m_SpecialPickupCount[item - 0xFA0] & 0xF;
}

/* De-faked in round 21 (j4): the matched draft had a dead if/else with two identical
 * arms (a permuter find); the early return for the cheat (flag 0x10) matches, so the N64 and the host share it. */
/* The high nibble of the special pickup's count (1 with cheat flag 0x10). */
u32 CPlayer__GetPickupCountHigh(CPlayer *player, s32 item) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return player->m_Data.m_SpecialPickupCount[item - 0xFA0] >> 4;
}

void CPlayer__OnParticleHit(CPlayer *arg0, sptr arg1, sptr arg2, CParticle *arg3) {
    f32 temp_f0;
    f32 temp_f1;
    f32 var_f0;
    u16 temp_v1;

    if ((arg0->m_Data.m_Health != 0) && (CPlayer__IsInMatchEndState((sptr) arg0) == 0)) {
        temp_v1 = arg3->header.m_nObjType;
        switch (temp_v1) {                          /* irregular */
        case 0x128:
            if (arg0->m_Mode != 0x23) {
                temp_f1 = arg0->m_ShockedTimer;
                if (temp_f1 > 0.0f) {
                    temp_f0 = temp_f1 + 30.0f;
                    if (!(temp_f0 <= 150.0f)) {
                        var_f0 = 150.0f;
                        goto block_12;
                    }
                    arg0->m_ShockedTimer = temp_f0;
                } else {
                    var_f0 = 60.0f;
block_12:
                    arg0->m_ShockedTimer = var_f0;
                }
                arg0->m_pHitMeWithChargeDart = (sptr) arg3->m_pOwner;
                return;
            }
            break;
        case 0x129:
            if (arg0->m_pHitMeWithCBore == 0) {
                arg0->m_pHitMeWithCBore = (sptr) arg3->m_pOwner;
            }
            arg0->m_ShockedTimer = 0.0f;
            CPlayer__SetState(arg0, arg0, 0x23);
            break;
        }
    }
}

extern void CGameObjectInstance__UpdateAI(CGameObjectInstance *);

/* Rebuilds the weapon actor (keeping gFrameIncrement across it) and puts it where
 * the character is. */
void CPlayer__RebuildWeaponActor(CPlayer *player) {
    CGameObjectInstance *weapon = &player->m_WeaponActor;
    s32 oldAnim = player->m_CharAnim;
    f32 saved = gFrameIncrement;
    s32 changed;

    weapon->m_dwHotPointFlags = 0x10;
    weapon->m_dwFlags &= ~0x20000;
    CGameObjectInstance__UpdateAI(weapon);
    CGameObjectInstance__UpdateAnimation(weapon);
    changed = oldAnim != player->m_CharAnim;
    gFrameIncrement = saved;
    if (changed) {
        player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted = 0;
    }
    player->m_WeaponActor.header.m_vPos = player->m_CharacterActor.header.m_vPos;
    player->m_WeaponActor.m_RotY = player->m_CharacterActor.m_RotY;
    player->m_WeaponActor.header.m_pCurrentRegion = player->m_CharacterActor.header.m_pCurrentRegion;
    player->m_WeaponActor.m_qGround = player->m_CharacterActor.m_qGround;
}

/* FAKEMATCH (round 21 audit): 2 register pins (`register ... asm("$n")`) and 1 empty `__asm__` barrier;
 * without them: func_00253934 14 of 22 words (size 0x54 vs 0x58). */
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022AA34.c, func_0022AA34.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Actor Actor;
struct Actor { /* Turok 2 layout */
    u8 pad0[0xBE4];
    f32 unkBE4;         /* Rage Wars: 0x840 */
    u8 padBE8[4];
    s32 unkBEC;         /* Rage Wars: 0x848 */
};

f32 func_00253934(Actor *a) {
    f32 t = a->unkBE4 - 585.0f;
    register f32 tmp asm("$f0");
    register f32 v asm("$f1");
    if (a->unkBEC != 0) goto c0;
    tmp = t * 0.06666667f;
    __asm__("" : "+f"(tmp));
    v = tmp;
    if (1.0f < v) {
c0:
        v = 1.0f;
    }
    return 1.0f - v;
}

/* The ammo entry a weapon fires now: the lower ammo when selected and stocked,
 * else the first stocked one, else the last one listed. */
AmmoInfo *CPlayer__GetFiringAmmo(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo;
    AmmoInfo *found;
    s32 i;
    WeaponInfo *def = gWeaponInfoTable[weapon];

    if (player->m_UseLowerAmmo && (ammo = def->ammo[1]) != NULL
        && player->m_Data.m_AmmoList[ammo->type] > 0) {
        return ammo;
    }
    found = NULL;
    for (i = 0; i < 3; i++) {
        ammo = def->ammo[i];
        if (ammo == NULL) {
            break;
        }
        found = ammo;
        if (player->m_Data.m_AmmoList[ammo->type] > 0) {
            return found;
        }
    }
    return found;
}

/* Ammo type a weapon fires now, -1 if it has none. */
s32 CPlayer__GetFiringAmmoType(CPlayer *player, s32 weapon) {
    AmmoInfo *ammo = GetWeaponAmmo(player, weapon);

    if (ammo != NULL) {
        return ammo->type;
    }
    return -1;
}

/* Most rounds of an ammo type any weapon carries (only held weapons if heldOnly). */
s32 CPlayer__GetMaxAmmo(CPlayer *player, s32 type, s32 heldOnly) {
    s32 max;
    s32 w;
    s32 i;
    WeaponInfo *def;
    AmmoInfo *ammo;
    s32 n;

    if (type == -1) {
        return 0;
    }
    max = 0;
    for (w = 0; w < 35; w++) {
        if (heldOnly && (s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        def = gWeaponInfoTable[w];
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            if (ammo->type == type) {
                n = ammo->max;
                if (n < max) {
                    n = max;
                }
                max = n;
            }
        }
    }
    return max;
}

/* CPlayer__GetMaxAmmo, inlined. */
static __inline__ s32 GetMaxAmmo(CPlayer *player, s32 type, s32 heldOnly) {
    s32 max;
    s32 w;
    s32 i;
    WeaponInfo *def;
    AmmoInfo *ammo;
    s32 n;

    if (type == -1) {
        return 0;
    }
    max = 0;
    for (w = 0; w < 35; w++) {
        if (heldOnly && (s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        def = gWeaponInfoTable[w];
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            if (ammo->type == type) {
                n = ammo->max;
                if (n < max) {
                    n = max;
                }
                max = n;
            }
        }
    }
    return max;
}

/* Adds rounds of an ammo type up to the most any weapon carries; returns
 * whether there was room. */
/* Adds rounds of an ammo type, up to the most any weapon carries; returns
 * whether there was room. */
s32 CPlayer__AddAmmo(CPlayer *player, s32 type, s32 count) {
    s16 *ammo = &player->m_Data.m_AmmoList[type];
    s32 max = GetMaxAmmo(player, type, 0);
    s32 room = max > *ammo;

    if (room) {
        s32 n;

        *ammo += count;
        n = *ammo;
        if (max < n) {
            n = max;
        }
        *ammo = n;
    }
    return room;
}

/* the ammo pickup row (CPickupRow14): its ammo type follows the message pointer, at
 * m_Params.m_Ammo.m_Type on the host (N64 0xC) */
#include "structs/pickup.h"

typedef CPickupRow14 PickupInfo;
#define ammoType m_Params.m_Ammo.m_Type

extern PickupInfo *Pickup_FindAmmoEntry(s32);

/* Whether the player has room for more of a pickup ammo, counting only the
 * weapons held. */
s32 CPlayer__HasRoomForAmmo(CPlayer *player, s32 pickup) {
    PickupInfo *info = Pickup_FindAmmoEntry(pickup);
    s32 type;
    s16 *ammo;

    if (info == NULL) {
        return 0;
    }
    type = info->ammoType;
    ammo = &player->m_Data.m_AmmoList[type];
    return *ammo < GetMaxAmmo(player, type, 1);
}

s32 CPlayer__IsBelowFullHealth(CPlayer *arg0) {
    return arg0->m_Data.m_Health < 0x6400;
}

f32 Actor_GetEyeHeight(CGameObjectInstance *actor) {
    CIntelligence *def = actor->header.m_pIntelligence;

    if (def != 0) {
        return def->m_Enemy.m_CollisionHeight * 0.9f;
    }
    return 82.94399f;
}

struct LevelActor;
struct Box;
typedef struct Box Box;
struct Box {
    CVector3 min;
    CVector3 max;
};
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
typedef struct LevelActor LevelActor;
struct LevelActor {
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    s16 modelIndex;
    u16 pad6;
    CVector3 position0;
    CIntelligence *definition; /* 0x14 in Turok 2 */
    s32 model;
    CVector3 position1;
    CVector3 motionAngles;
    void *attached;
    s32 motionState;
    s32 motionIndex;
    f32 floorHeight;
    CVector3 floorDirection;
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    Box bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    Shared_ParticleLighting lighting[2];
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS GameObject_GetCollisionHeight receives Actor_GetHeightOrDefault's own argument
 * registers (pass-through); the host passes them. */
f32 Actor_GetHeightOrDefault(LevelActor *arg0) {
    if (arg0->definition != 0) {
        return GameObject_GetCollisionHeight(arg0);
    } else {
        return 92.159996f;
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80225D10_10.CPP, func_8022AE28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

/* Turok 2: position 0x4 and region 0x10 (Rage Wars 0x8 / 0x14). */
/* Turok 2: LevelActor is 0x288 bytes, so the weapon actor follows at 0x288. */
typedef struct Shared_ActorView_253D68 {
    CGameObjectInstance actor;
    CGameObjectInstance weapon;
} Shared_ActorView_253D68;
typedef struct Shared_World Shared_World;

int CPlayer__SetPosition(void *arg0, void *arg1) {
    sptr region;
    CVector3 *src = arg1;
    Shared_ActorView_253D68 *view = arg0;
    /* the region pointer-wide, and the position passed (round 9 x2): the ROM calls
     * CScene__FindRegionAt through an int-returning one-argument cast (the position stays in a1
     * from its own caller); on the host that cut the region to 32 bits: with --high-memory the
     * Mother's phase-1 cinema end (MotherBoss_OnPhase1CinemaDone) put the player in a truncated
     * region and the camera crashed */
    region = (sptr) CScene__FindRegionAt((struct CScene *) &gScene, (f32 *) src);
    view->actor.header.m_vPos = *src;
    view->actor.header.m_pCurrentRegion = (struct CGameRegion *) region;
    view->weapon.header.m_vPos = *src;
    view->weapon.header.m_pCurrentRegion = (struct CGameRegion *) region;
    return region != 0;
}

extern struct CLoopingSoundData *LoopSounds_Start(void *pool, s32 id, CVector3 *pos,
                                               CVector3 *pos2, sptr arg4);

/* Restarts the player's weapon background looping sound with sound id. */
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id) {
    if (gScene.m_WarpState == 0 && gEngineApp_m_Settings.paused == 0) {
        LoopSounds_Stop(LoopSounds_Get(), player->m_pLoopingSound_WeaponBackground);
        player->m_pLoopingSound_WeaponBackground =
            LoopSounds_Start(LoopSounds_Get(), id, &player->m_pCamera->m_vPos,
                          &player->m_pCamera->m_vPos, -1);
    }
}

/* LoopSounds' node (SoundSourceNode, rw/abu_types.h): the sound id at N64 0xC follows a
 * pointer, so on the host it is at 0x14 */
typedef struct {
    s32 unk0;
    void *next;
    s32 value;
    s32 id;
} LoopSound;
void Sound__StopSoundId(s32);

void CPlayer__StopWeaponBackgroundSound(CPlayer *arg0) {
    LoopSound *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponBackground;
    if (temp_v0 != NULL) {
        Sound__StopSoundId(temp_v0->id);
    }
    LoopSounds_Stop(LoopSounds_Get(), arg0->m_pLoopingSound_WeaponBackground);
    arg0->m_pLoopingSound_WeaponBackground = NULL;
}

/* Restarts the player's weapon firing looping sound with sound id. */
void CPlayer__StartWeaponFiringSound(CPlayer *player, s32 id) {
    if (gScene.m_WarpState == 0 && gEngineApp_m_Settings.paused == 0) {
        LoopSounds_Stop(LoopSounds_Get(), player->m_pLoopingSound_WeaponFiring);
        player->m_pLoopingSound_WeaponFiring =
            LoopSounds_Start(LoopSounds_Get(), id, &player->m_pCamera->m_vPos,
                          &player->m_pCamera->m_vPos, -1);
    }
}

/* A looping sound: only its sound handle here. */
/* LoopSounds' node (SoundSourceNode, rw/abu_types.h): the sound id at N64 0xC follows a
 * pointer, so on the host it is at 0x14 */
struct CLoopingSoundData {
    s32 unk0;
    void *next;
    s32 value;
    s32 m_nHandle;
};

/* Stops the player's weapon firing looping sound. */
void CPlayer__StopWeaponFiringSound(CPlayer *arg0) {
    struct CLoopingSoundData *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponFiring;
    if (temp_v0 != NULL) {
        Sound__StopSoundId(temp_v0->m_nHandle);
    }
    LoopSounds_Stop(LoopSounds_Get(), arg0->m_pLoopingSound_WeaponFiring);
    arg0->m_pLoopingSound_WeaponFiring = NULL;
}

/* Transforms a point by the player camera's view orientation, in place. */
void CPlayer__TransformByView(CPlayer *player, CVector3 *pos) {
    CVector3 in = *pos;

    CMtxF__TransformPoint(&player->m_pCamera->m_mfViewOrient, &in, pos);
}

s32 PlayerPool_GetNumPlayers(void) {
    return gEngineApp_m_Game.m_PlayerList.m_nPlayers;
}

s32 CPlayer__HasMenuOpen(CPlayer *arg0) {
    s32 var_v0;
    CCamera *temp_v1;

    temp_v1 = arg0->m_pCamera;
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = temp_v1->m_Menus.count != 0;
    }
    return var_v0;
}

/* Whether the game is paused (gEngineApp_m_Settings_paused) or a menu other than the death
 * screen is open on the player's camera. */
s32 CPlayer__IsPausedOrInMenu(CPlayer *p) {
    Menu *menu;

    if (gEngineApp_m_Settings.paused) {
        return 1;
    }
    if (p->m_pCamera != NULL && (menu = p->m_pCamera->m_Menus.head) != NULL) {
        if (menu->desc != (MenuDesc *) &gPauseMenuDesc) {
            return 1;
        }
    }
    return 0;
}

/* A spawn point record (from CScene__FindIntelligence). */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ f32 pos[3];
} Spawn;

/* Picks a spawn point for the player and copies its position. */
void CPlayer__PickSpawnPoint(CPlayer *arg0) {
    s32 *var_s1;
    s32 var_a2;
    Spawn *var_v0;
    CPlayer *var_s0;

    var_s0 = arg0;
    var_a2 = var_s0->m_Data.m_nDeathmatchCharacter;
    if (var_s0->m_pOptions->m_TagMode == 1) {
        var_a2 = 0xB;
    }
    var_s1 = &gScene;
    var_s1 = &gScene;
    var_v0 = CScene__FindIntelligence(&gScene, 0xB, var_a2);
    if (var_v0 == 0) {
        var_v0 = CScene__FindIntelligence(&gScene, 0xB, -1);
        if (var_v0 == 0) {
            var_v0 = CScene__FindIntelligence(&gScene, -1, -1);
        }
    }
    var_s0->m_CharacterActor.header.m_pIntelligence = var_v0;
    var_s0->m_CharacterActor.m_vScale.x = (f32) var_v0->pos[0];
    var_s0->m_CharacterActor.m_vScale.y = (f32) var_v0->pos[1];
    var_s0->m_CharacterActor.m_vScale.z = (f32) var_v0->pos[2];
}

/* Respawn a player at a spawn point with full health. */
void CPlayer__Respawn(CPlayer *p) {
    place_at_spawn(p);
    p->m_Data.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_Health = p->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
    p->m_CharacterActor.m_AI.m_nTexture = (p->m_Data.m_nDeathmatchCharacter != 7) ? -1 : Random_Next() % 18;
}

void CPlayer__LoadCharacterModel(CPlayer *player) {
    GameSettings *state = &gEngineApp_m_Settings;
    s32 id;

    if (state->multiplayer == 0) {
        id = 0x66;
    } else if (state->match.tag != 0
               && ((CPlayerOptions *)player->m_pOptions)->m_TagMode != 0) {
        id = gTagModeModel;
    } else {
        id = gCharacterModels[player->m_CharacterActor.header.m_pIntelligence->m_Id];
    }
    CScene__SetInstanceObjectType(&gScene, player, id, player->m_CharAnim);
}

void CPlayer__LoadWeaponModel(CPlayer *arg0) {
    CScene__SetInstanceObjectType(&gScene, &arg0->m_WeaponActor, gWeaponInfoTable[arg0->m_Data.m_CurrentWeapon]->model, arg0->m_WeaponActor.m_AI.m_pModeTable->unk_10);
}

extern void CGameObjectInstance__UpdateBounds(CGameObjectInstance *, void *);
extern void Shadow_DrawBlob(CGameObjectInstance *, CVector3 *, s32);

void Actor_ReloadAnimInfo(CGameObjectInstance *actor, s32 arg1) {
    sptr *res;
    CPlayer *owner;
    CVector3 dir;

    if (actor->m_dwFlags & 0x40000) {
        res = CCache__Request(0, actor->m_rpObject, actor->m_rpObjectHeader, 4, 0, 0,
                            "player anim object info", 1);
        if (res != NULL) {
            owner = (CPlayer *)actor->m_AI.m_pParent;
            CGameObjectInstance__UpdateBounds(actor, CISet__GetBlock(*res, 0));
            if (arg1) {
                dir.x = owner->m_pCamera->m_vPos.x - actor->header.m_vPos.x;
                dir.y = 0.0f;
                dir.z = owner->m_pCamera->m_vPos.z - actor->header.m_vPos.z;
                Shadow_DrawBlob(actor, &dir, arg1);
            }
            CCache__Unlock(0, res);
        }
    }
}
void CPlayer__UpdateOxygen(CPlayer *arg0, sptr arg1);
void CPlayer__UpdateSubmerged(CPlayer *arg0, CGameObjectInstance *arg1);

void CPlayer__UpdateWaterRegion(CPlayer *arg0, CGameObjectInstance *arg1, CRegionInfo *arg2)
{
  int new_var;
  s32 temp_s0;
  s32 temp_s2;
  s32 temp_v1;
  if (arg2 != 0)
  {
    temp_v1 = arg1->m_AI.m_dwFlags;
    temp_s2 = arg2->m_wFlags52 & 2;
    new_var = temp_v1 & 0x100;
    temp_s0 = temp_v1 & 0x80;
    if (temp_s0 == 0)
    {
      arg0->m_Oxygen = 0;
    }
    if ((!new_var) && (temp_s2 != 0))
    {
      CPlayer__UpdateSubmerged(arg0, arg1);
    }
    if ((temp_s0 != 0) && (temp_s2 == 0))
    {
      CPlayer__UpdateOxygen(arg0, arg1);
    }
  }
}

void CPlayer__UpdateOxygen(CPlayer *arg0, sptr arg1) {
    f32 temp_f0;
    f32 temp_f1;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s1;

    temp_f1 = arg0->m_Oxygen;
    temp_f0 = temp_f1 + gFrameIncrement;
    arg0->m_Oxygen = temp_f0;
    if ((temp_f1 < 300.0f) && (temp_f0 > 300.0f) && (gEngineApp_m_Settings.multiplayer == 0)) {
        Sound__PlaySpeech(0x163);
    }
    if (arg0->m_Oxygen > 600.0f) {
        temp_v0 = (s32) arg0->m_Data.m_Health >> 8;
        var_s1 = 1;
        if ((0x96 - temp_v0) > 0) {
            temp_v1 = (0x96 - temp_v0) / 25;
            if (temp_v1 > 0) {
                var_s1 = temp_v1;
            }
        }
        if (arg0->m_bOxygenOut == 0) {
            if (gEngineApp_m_Settings.multiplayer == 0) {
                Sound__PlaySpeech(0x164);
            }
        }
        arg0->m_bOxygenOut = 1;
        CPlayer__Hurt((sptr) arg0, arg1, var_s1 << 8, 0, 0x1C);
        arg0->m_Oxygen = (f32) (arg0->m_Oxygen - 37.5f);
    }
}

s32 CPlayer__HasScope(CPlayer *arg0) {
    return (s16) gWeaponInfoTable[arg0->m_Data.m_CurrentWeapon]->zoomModel > 0;
}

void CPlayer__UpdateSubmerged(CPlayer *arg0, CGameObjectInstance *arg1) {
    s32 var_v0;

    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        var_v0 = 1;
    } else {
        var_v0 = arg0->m_Data.m_SpecialPickupCount[380] & 0xF;
    }
    if (var_v0 != 0) {
        if (arg1->m_AI.m_dwFlags & 0x80) {
            CPlayer__UpdateOxygen((sptr) arg0, (sptr) arg1);
        }
    } else {
        CPlayer__Hurt((sptr) arg0, (sptr) arg1, 0xA00, 0x02000000, 0x1C);
    }
}

void CPlayer__EaseEyeOffset(CPlayer *arg0, sptr arg1) {
    f32 temp_f1;
    CRegionInfo *temp_v0;

    temp_v0 = GameObject_GetRegionInfo(arg1);
    if (temp_v0 != NULL) {
        arg0->m_ActualCameraVertEyeOffset += (temp_v0->unk_2C - arg0->m_ActualCameraVertEyeOffset) * 0.2f;
    }
}

#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/intelligence.h"
#include "structs/region.h"
/* The host body on the members (STABILITY.md section 7): the N64 offsets (CPlayer 0xA08 m_Mode,
 * 0xB28 m_CameraMovementScaler; the actor's CAI flags at 0x140) wrote a float into the host
 * CPlayer every frame. The intelligence (0x1C: m_SpeedScale) and the region info
 * (0x30, 0x52) are ROM records at the same offsets on both sides (offsetof checks them). */
void CPlayer__UpdateSpeedScale(void *arg0, void *arg1, void *arg2) {
    CPlayer *player = arg0;
    CGameObjectInstance *actor = arg1;
    CRegionInfo *info = arg2;
    f32 s;

    if ((u16) player->m_Mode < 2U) {
        player->m_CameraMovementScaler = 1.0f;
        return;
    }
    /* m_SpeedScale: typed in structs/intelligence.h, so the archive's swap plan swaps it (round 12
     * a5; round 11 read the big-endian bytes of CPlayerIntelligence.pad08 here). */
    s = actor->header.m_pIntelligence->m_Player.m_SpeedScale;
    player->m_CameraMovementScaler = s;
    if (gEngineApp_m_Settings.multiplayer != 0) {
        player->m_CameraMovementScaler = s * gEngineApp_m_Settings.speed;
    }
    if (!(actor->m_AI.m_dwFlags & 0xC0000) && info != NULL
        && (GameObject_IsOnGround((sptr) actor) != 0 || (info->m_wFlags52 & 0x800))) {
        player->m_CameraMovementScaler *= info->unk_30;
    }
}
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.m_SpeedScale) == 0x1C, "CIntelligence 0x1C");
_Static_assert(__builtin_offsetof(CRegionInfo, unk_30) == 0x30, "CRegionInfo 0x30");
_Static_assert(__builtin_offsetof(CRegionInfo, m_wFlags52) == 0x52, "CRegionInfo 0x52");
#else
void CPlayer__UpdateSpeedScale(void *arg0, void *arg1, void *arg2)
{
  s32 *new_var;
  f32 temp_f1;
  if (((u16) (*((u16 *) (((s8 *) arg0) + 0xA08)))) < 2U)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = 1.0f;
    return;
  }
  temp_f1 = *((f32 *) (((s8 *) (*((void **) (((s8 *) arg1) + 0x14)))) + 0x1C));
  *((f32 *) (((s8 *) arg0) + 0xB28)) = temp_f1;
  new_var = &gEngineApp_m_Settings;
  if ((*((u8 *) (((s8 *) new_var) + 0x19))) != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) (temp_f1 * (*((f32 *) (((s8 *) new_var) + 0x1C))));
  }
  if (((!((*((s32 *) (((s8 *) arg1) + 0x140))) & 0xC0000)) && (arg2 != 0)) && ((GameObject_IsOnGround((sptr) arg1) != 0) || ((*((u16 *) (((s8 *) arg2) + 0x52))) & 0x800)))
  {
    *((f32 *) (((s8 *) arg0) + 0xB28)) = (f32) ((*((f32 *) (((s8 *) arg0) + 0xB28))) * (*((f32 *) (((s8 *) arg2) + 0x30))));
  }
}
#endif

void CPlayer__TickFlashlight(CPlayer *arg0) {
    f32 temp_f0;
    f32 temp_f1;

    temp_f1 = arg0->m_FlashlightTime;
    if (temp_f1 > 0.0f) {
        temp_f0 = temp_f1 - gFrameIncrement;
        arg0->m_FlashlightTime = temp_f0;
        if (temp_f0 < 0.0f) {
            arg0->m_FlashlightTime = 0.0f;
        }
    }
}

void CPlayer__ResetLives(CPlayer *arg0) {
    if (gEngineApp_m_Settings.multiplayer != 0) {
        arg0->m_Data.m_Lives = 1;
        return;
    }
    arg0->m_Data.m_Lives = 3;
}

#ifdef TARGET_PORT
/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): the caller
 * (CPlayer__Update, tag mode) passes the player and its camera; 0x288-0x294 are N64 offsets of
 * the camera's viewport and 0xB60 of CPlayer.m_SightIconFrame. */
#include "structs/player.h"
#include "structs/camera.h"
void CPlayer__DrawSightIcon(void *arg0, void *arg1)
{
  CPlayer *player = arg0;
  CCamera *view = arg1;
  f32 w;
  f32 h;
  f32 sx;
  f32 sy;
  Hud_SetupTextureMode((s32) gEngineApp_m_Settings.hudAlpha);
  w = view->m_Viewport.m_Width;
  sx = w / ((f32) gScreenWidth);
  h = view->m_Viewport.m_Height;
  sy = h / ((f32) gScreenHeight);
  Hud_DrawSprite(0x1FC, (s32) player->m_SightIconFrame,
                 (s16) ((s32) ((view->m_Viewport.m_ScreenPosX + w) + (sx * -5.0e1f))),
                 (s16) ((s32) ((view->m_Viewport.m_ScreenPosY + h) + (sy * -5.0e1f))), sx, sy, 1);
}
#else
void CPlayer__DrawSightIcon(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f2;
  float new_var2;
  f32 temp_f3;
  f32 temp_f5;
  float new_var;
  Hud_SetupTextureMode((s32) gEngineApp_m_Settings.hudAlpha);
  temp_f0 = *((f32 *) (((s8 *) arg1) + 0x288));
  temp_f3 = temp_f0 / ((f32) gScreenWidth);
  temp_f5 = *((f32 *) (((s8 *) arg1) + 0x28C));
  temp_f2 = temp_f5 / ((f32) gScreenHeight);
  new_var = (*((f32 *) (((s8 *) arg1) + 0x290))) + temp_f0;
  new_var2 = (*((f32 *) (((s8 *) arg1) + 0x294))) + temp_f5;
  Hud_DrawSprite(0x1FC, (s32) (*((f32 *) (((s8 *) arg0) + 0xB60))), (s16) ((s32) (new_var + (temp_f3 * -5.0e1f))), (s16) ((s32) (new_var2 + (temp_f2 * -5.0e1f))), temp_f3, temp_f2, 1);
}
#endif

struct ScreenRectangle;
#ifdef TARGET_PORT
/* The camera's viewport at its host offsets (round 12 a5, tools/port/n64viewcheck.py): the N64 view
 * read 0x288.. of the host CCamera, which has pointers before m_Viewport. */
#include "structs/camera.h"
#define T2_HOFF_ScreenRectangle(m) __builtin_offsetof(CCamera, m)
typedef struct ScreenRectangle {
    union {
        struct { u8 _hp_x[T2_HOFF_ScreenRectangle(m_Viewport.m_Width)]; f32 x; }; /* N64 0x288 */
        struct { u8 _hp_y[T2_HOFF_ScreenRectangle(m_Viewport.m_Height)]; f32 y; }; /* N64 0x28C */
        struct { u8 _hp_width[T2_HOFF_ScreenRectangle(m_Viewport.m_ScreenPosX)]; f32 width; }; /* N64 0x290 */
        struct { u8 _hp_height[T2_HOFF_ScreenRectangle(m_Viewport.m_ScreenPosY)]; f32 height; }; /* N64 0x294 */
    };
} ScreenRectangle;
#else
typedef struct ScreenRectangle { char pad0[0x288]; f32 x,y,width,height; } ScreenRectangle;
#endif
void CPlayer__DrawCenterIcon(void *arg0, struct ScreenRectangle *arg1);

void CPlayer__DrawCenterIcon(void *arg0, struct ScreenRectangle *arg1)
{
  f32 x;
  f32 y;
  f32 ax;
  f32 ay;
  f32 sx;
  f32 sy;
  f32 bx;
  f32 by;
  f32 screenX;
  f32 screenY;
  Hud_SetupTextureMode(gEngineApp_m_Settings.hudAlpha);
  x = arg1->x;
  ax = x * 0.5f;
  y = arg1->y;
  ay = y * 0.5f;
  sx = x / ((f32) gScreenWidth);
  bx = sx * 16.0f;
  sy = y / ((f32) gScreenHeight);
  by = sy * 16.0f;
  screenX = ((arg1->width) + ax) - bx;
  screenY = ((arg1->height) + ay) - by;
  Hud_DrawSprite(0x1FB, 0, (s16) ((s32) (((arg1->width) + ax) - bx)), (s16) ((s32) screenY), sx, sy, 1);
}

s32 CPlayer__IsInMatchEndState(CPlayer *arg0) {
    s16 temp_v1;

    temp_v1 = arg0->m_Mode;
    if ((temp_v1 == 0x12) || (temp_v1 == 0x10) || (temp_v1 == 0x11)) {
        return 1;
    }
    return 0;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
    /* 0x20 */ u8 pad20[0x34 - 0x20];
    /* 0x34 */ s32 tagMode;
} MatchState_25492C;
/* The globals from gEngineApp_m_Game_m_MenuPool: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* gEngineApp_m_Settings_multiplayer */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* gEngineApp_m_Settings_paused */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState_25492C match;       /* gEngineApp_m_Settings_match */
} Globals_25492C;
typedef struct PlayerList_25492C PlayerList_25492C;
extern Globals_25492C gMenuPoolGlobalsView_25492C __asm__("gEngineApp_m_Game_m_MenuPool"); /* gEngineApp_m_Game_m_MenuPool (declared by globals/menu.h) seen as this file's type: what keeps the ROM's code */
s32 CPlayerPool__CountIdleMenus(CPlayerPool *arg0);
/* Run the match clock and end the match when the time is up or, outside tag
 * mode, when the frag limit is reached. Not while a menu is open. */
#ifdef TARGET_PORT
#include "structs/game.h"
#include "globals/game.h"

/* The port: the settings by member of gEngineApp_m_Settings (the N64 code reaches multiplayer,
 * paused and the match through a view of the menu pool at 0xCC5/0xE78/0xE84, N64 offsets past
 * the host menu pool); the menu pool itself is the first member of that view. */
void CPlayerPool__UpdateMatchClock(void *list_) {
    PlayerList_25492C *list = list_;
    GameMatch *match = &gEngineApp_m_Settings.match;

    if (match->over || !gEngineApp_m_Settings.multiplayer) {
        return;
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && match->timeLeft > 0.0f && !gEngineApp_m_Settings.paused) {
        if (CPlayerPool__CountIdleMenus(list)) {
            match->timeLeft -= gFrameIncrement;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            CPlayerPool__EndMatch(list);
        }
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && CPlayerPool__IsFragLimitReached(list)) {
        if (!match->over && !gEngineApp_m_Settings.paused && match->scoreLimit > 0 && !match->tag) {
            CPlayerPool__EndMatch(list);
        }
    }
}
#else
void CPlayerPool__UpdateMatchClock(void *list_) {
    PlayerList_25492C *list = list_;
    MatchState_25492C *match = &gMenuPoolGlobalsView_25492C.match;
    Globals_25492C *g;

    if (match->over || !gMenuPoolGlobalsView_25492C.timed) {
        return;
    }
    if (!MenuPool_CountOpen(&gMenuPoolGlobalsView_25492C) && match->timeLeft > 0.0f && !gMenuPoolGlobalsView_25492C.paused) {
        if (CPlayerPool__CountIdleMenus(list)) {
            match->timeLeft -= gFrameIncrement;
        }
        if (match->timeLeft <= 0.0f && !match->over) {
            match->timeLeft = 0.0f;
            CPlayerPool__EndMatch(list);
        }
    }
    g = &gMenuPoolGlobalsView_25492C;
    if (!MenuPool_CountOpen(g) && CPlayerPool__IsFragLimitReached(list)) {
        MatchState_25492C *m = &g->match;

        if (!m->over && !g->paused && m->fragLimit > 0 && !m->tagMode) {
            CPlayerPool__EndMatch(list);
        }
    }
}
#endif

/* Whether some player sums, over the slots other than CPlayerPool__GetIndex's, at
 * least gEngineApp_m_Settings_match.scoreLimit in options field 0x10. */
s32 CPlayerPool__IsScoreLimitReached(CPlayerPool *list) {
    GameMatch *state;
    CPlayer *p;
    sptr skip;
    s32 sum;
    s32 i;

    for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
        state = &gEngineApp_m_Settings.match;
        skip = CPlayerPool__GetIndex(list, p);
        sum = 0;
        for (i = 0; i < 4; i++) {
            if (i != skip) {
                sum += ((CPlayerOptions *)p->m_pOptions)->m_Tagged[i];
            }
        }
        if (sum >= state->scoreLimit) {
            return 1;
        }
    }
    return 0;
}

/* The index-th (0-3) player with a camera whose camera word 0x54C and
 * options byte 0x3F are both clear. */
CPlayer *CPlayerPool__GetViewPlayer(CPlayerPool *list, u32 index) {
    CPlayer *p = NULL;
    s32 n = 0;

    if (index < 4) {
        for (p = list->m_ActiveList.m_pHead; p != NULL; p = p->m_pNext) {
            /* N64 0x54C: the camera's menu pool count, by member */
            if (p->m_pCamera != NULL && ((CCamera *) p->m_pCamera)->m_Menus.count == 0
                && ((CPlayerOptions *)p->m_pOptions)->m_LastTagMode == 0) {
                if (n == index) {
                    return p;
                }
                n++;
            }
        }
    }
    return p;
}

void CPlayerPool__ClearLastTagMode(CPlayerPool *arg0) {
    struct CPlayerOptions *temp_a0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->m_pOptions;
            if (temp_a0->m_LastTagMode == 1) {
                temp_a0->m_LastTagMode = 0U;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
}

/* Counts the players with no menu open on their camera and m_LastTagMode clear. */
s32 CPlayerPool__CountPlaying(CPlayerPool *arg0) {
    s32 var_a0;
    CCamera *temp_v0;
    CPlayer *var_v1;

    var_v1 = arg0->m_ActiveList.m_pHead;
    var_a0 = 0;
    if (var_v1 != NULL) {
        do {
            temp_v0 = var_v1->m_pCamera;
            if ((temp_v0 != NULL) && (temp_v0->m_Menus.count == 0) && (var_v1->m_pOptions->m_LastTagMode == 0)) {
                var_a0 += 1;
            }
            var_v1 = var_v1->m_pNext;
        } while (var_v1 != NULL);
    }
    return var_a0;
}

/* Counts the active players whose camera's menu pool is idle. */
s32 CPlayerPool__CountIdleMenus(CPlayerPool *arg0) {
    s32 var_s1;
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            if ((var_s0->m_bActive != 0) && (MenuPool_CountOpen((sptr) var_s0->m_pCamera->m_MenuPool) == 0)) {
                var_s1 += 1;
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    return var_s1;
}

struct SlotRecAt14;
typedef struct SlotRecAt14 SlotRecAt14;
struct SlotRecAt14 {
    s32 weapon;
    void *info;
    s32 owned;
    f32 angle;
    s32 y;
    f32 scale;
};
struct Menu_254C44;
struct Menu_254C44 {
    s32 active;
    s32 pad4;
    f32 open;
    s32 reset;
    u8 reserved10[4];
    f32 spin;
    s32 selected;
    SlotRecAt14 slots[36];
    s32 cursor;
    s32 count;
    u8 reserved384[4];
    s32 hover;
    s32 scroll;
    s32 last;
};
typedef struct Menu_254C44 Menu_254C44;

void CBarrel__Reset(void *menu_) {
    Menu_254C44 *menu = menu_;

    s32 i;
    menu->active = 0;
    menu->pad4 = 0;
    menu->open = 0;
    menu->reset = 0;
    menu->spin = 0;
    menu->cursor = -1;
    menu->count = 1;
    menu->hover = 0; /* -1 in Rage Wars */
    menu->selected = 0;
    menu->scroll = 0;
    menu->last = -1;
    for (i = 0; i < 36; i++) {
        menu->slots[i].angle = -(i * 0.17453295f);
        menu->slots[i].y = 0;
        menu->slots[i].scale = gBarrelSlotScale;
    }
}

typedef struct {
    /* 0x000 */ u8 pad0[0x380];
    /* 0x380 */ s32 field380;
} Unk254CC0;

void CBarrel__Draw(Unk254CC0 *arg0, s32 arg1, s32 arg2) {
    Gfx *g;

    Hud_SetupTextureMode(0xFF);
    g = gDList++;
    g->w0 = 0xE3001201; /* gDPSetTextureFilter(G_TF_BILERP) */
    g->w1 = 0x2000;
    if (arg0->field380) {
        CBarrel__DrawEight(arg0, arg1, arg2);
    } else {
        CBarrel__DrawRing(arg0, arg1, arg2);
    }
}

void CMtxF__RotateZ(f32 *m, f32 angle);

/* Lays out the eight slots of the weapon wheel, dropping in as it opens,
 * and draws them. */
void CBarrel__DrawEight(CBarrel *menu, sptr arg1, sptr arg2) {
    f32 m[16];
    CVector3 position;
    CBarrelIcon items[36];
    CVector3 offset;
    CBarrelIcon *item;
    CBarrelItem *slot;
    s32 count;
    s32 i;
    f32 target;

    if (menu->m_State == 0) {
        return;
    }
    item = items;
    count = 0;
    offset.x = 0.0f;
    offset.y = -0.6f - (0.4f - menu->m_Open * 0.4f);
    offset.z = 0.0f;
    for (i = 0; i < 8; i++) {
        slot = &menu->m_Items[i];
        if (slot->m_Weapon == -1) {
            continue;
        }
        if (i == menu->m_nCursor) {
            target = gBarrelSlotScale * 1.3f;
        } else {
            target = gBarrelSlotScale;
        }
        count++;
        slot->m_Scale = Math_Approach(slot->m_Scale, target, gBarrelSlotScale * 0.1f);
        CMtxF__Identity(m);
        CMtxF__RotateZ(m, slot->m_Angle);
        CMtxF__TransformPoint(m, &offset, &position);
        item->m_Item = i;
        item->m_vPos = position;
        item->m_Icon = gWeaponInfoTable[slot->m_Weapon]->icon;
        item->m_Alpha = 1.0f;
        item->m_Size = 1.0f;
        item++;
    }
    CBarrel__DrawIcons(menu, items, count, arg2);
}

#ifdef TARGET_PORT
#include "structs/player.h"
#include "structs/weapon.h"
#include "structs/controller.h"
#endif
#ifdef TARGET_PORT
/* The host body by member (round 9 x4): the N64 view (0x380 m_bStickWheel, 0x37C m_nCursor,
 * 4 m_Turn of the CBarrel; 0xA50 m_ContState.m_pContState of the CPlayer) read other host
 * fields, so the open wheel closed on its first frame while A was still held: the head
 * movement came back on and the demo's stick turned the player instead of choosing a weapon
 * (attract demo 1 walked into the wall beside its door). The same fix came from the soak (x5
 * S6): with the stick wheel the N64 offsets read m_Items and crashed on a garbage controller.
 * One body, typed like its caller (func_0024BD18.c). */
s32 CBarrel__ShouldClose(void *self_, void *other_) {
    CBarrel *self = self_;
    CPlayer *other = other_;

    if (self->m_bStickWheel != 0) {
        if (other->m_ContState.m_pContState->buttons & 0xC000) {
            return 0;
        }
        self->m_nCursor = -1;
        return 1;
    }
    self->m_Turn -= gFrameIncrement;
    return self->m_Turn <= 0.0f;
}
#else
s32 CBarrel__ShouldClose(void *self_, void *other_) {
    u8 *self = self_;
    u8 *other = other_;

    if (*(s32 *)(self + 0x380) != 0) {
        if (*(s32 *)(*(u8 **)(other + 0xA50) + 0xB0) & 0xC000) {
            return 0;
        }
        *(s32 *)(self + 0x37C) = -1;
        return 1;
    }
    *(f32 *)(self + 4) -= gFrameIncrement;
    return *(f32 *)(self + 4) <= 0.0f;
}
#endif

/* Whether the player holds a usable weapon that is not in the list. */
s32 CBarrel__HasUnlistedWeapon(CBarrel *list, CPlayer *player) {
    WeaponInfo *def;
    s32 usable;
    s32 found;
    s32 w;
    s32 i;
    s32 *aiFlags = &player->m_WeaponActor.m_AI.m_dwFlags;

    for (w = 0; w < 35; w++) {
        def = gWeaponInfoTable[w];
        usable = 0;
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        if (*aiFlags & 0x80) {
            if (def->flags & 2) {
                usable = 1;
            }
        } else if (def->flags & 1) {
            usable = 1;
        }
        if (usable) {
            found = 0;
            for (i = 0; i < list->m_nCount; i++) {
                if (list->m_Items[i].m_Weapon == w) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                return 1;
            }
        }
    }
    return 0;
}

/* Fills arg1[0..34] with whether each weapon can be selected and returns how many can.
 * The m_WeaponList[i] reads are written as (player + i)->m_WeaponList[0], read signed: the code adds the
 * index to the player first. */
s32 CPlayer__GetSelectableWeapons(CPlayer *arg0, s32 *arg1) {
    s16 temp_a2;
    s32 *var_a1;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    WeaponInfo *temp_v1;

    var_a1 = arg1;
    var_t1 = 0;
    var_a3 = 0;
    var_t0 = 0;
    do {
        temp_v1 = *(WeaponInfo **) ((s8 *) gWeaponInfoTable + var_t0);
        if (*(s8 *) ((CPlayer *) ((s8 *) arg0 + var_a3))->m_Data.m_WeaponList == 0) {
            var_v1 = 0;
        } else {
            if (arg0->m_WeaponActor.m_AI.m_dwFlags & 0x80) {
                var_v0 = temp_v1->flags & 2;
            } else {
                var_v0 = temp_v1->flags & 1;
            }
            if (var_v0 == 0) {
                var_v1 = 0;
            } else {
                temp_a2 = temp_v1->alternate;
                var_v1 = 1;
                if (*(s8 *) ((CPlayer *) ((s8 *) arg0 + temp_a2))->m_Data.m_WeaponList != 0) {
                    var_v1 = (var_a3 < temp_a2) ^ 1;
                }
            }
        }
        *var_a1 = var_v1;
        if (var_v1 != 0) {
            var_t1 += 1;
        }
        var_a1 = (__typeof__(var_a1))((s8 *)((__typeof__(var_a1))((s8 *)var_a1 + 4)));
        var_a3 += 1;
        var_t0 += 4;
    } while (var_a3 < 0x23);
    return var_t1;
}

/* Fills entry index with a weapon, selecting it if none is and it is the
 * player's requested weapon. */
void CBarrel__SetEntry(CBarrel *list, s32 weapon, s32 index, CPlayer *player) {
    WeaponInfo *def = gWeaponInfoTable[weapon];
    s32 hasAmmo = CPlayer__GetWeaponAmmo(player, weapon) != 0;

    list->m_Items[index].m_Weapon = weapon;
    list->m_Items[index].m_bAvailable = hasAmmo;
    list->m_Items[index].m_pInfo = def;
    if (list->m_nCursor == -1 && weapon == player->m_RequestedWeapon) {
        list->m_nCursor = index;
        list->m_Spin = list->m_Items[index].m_Angle;
    }
    list->m_nCount++;
}

void CBarrel__Close(CBarrel *arg0) {
    arg0->m_State = 0;
    arg0->m_Turn = 0;
    arg0->m_nCursor = -1;
    arg0->m_Open = 0;
}
float func_002551B8(void *arg0);

float func_002551B8(void *arg0) {
    ObjRecAtB8 *obj = arg0;
    float f1 = 437.5f;
    float f2 = -675.0f;
    float f0 = 0.25f;
    float f3 = 0.1875f;
    float f4 = 1.5707965f;
    obj->unkB4 = 0;
    obj->unk8 = 0;
    obj->unkB8 = 0;
    obj->unk4 = -1;
    obj->unk0 = -1;
    obj->unkA8 = 0;
    obj->unk9C = 0;
    obj->unkA4 = 0;
    obj->unkAC = f1;
    obj->unkB0 = f2;
    obj->unk90 = f0;
    obj->unk94 = f3;
    obj->unk98 = f0;
    obj->unkA0 = f4;
}

/* s16 player mode at 0xA08 (LibTEngine's CPlayer has m_Mode at 0xA0C). */
#define PLAYER_MODE(p) (*(s16 *)((u8 *)(p) + 0xA08))

static __inline__ s32 InMode10To12(CPlayer *player) {
    s32 mode = PLAYER_MODE(player);

    if (mode == 0x12 || mode == 0x10 || mode == 0x11) {
        return 1;
    }
    return 0;
}

/* Puts the player in mode 0xC when hit by an actor with AI flag 0x800,
 * unless already in modes 0xC, 0xD or 0x10-0x12. */
s32 CPlayer__CheckState12Hit(CPlayer *player, CGameObjectInstance *actor) {
    if (InMode10To12(player)) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x800) {
        if (PLAYER_MODE(player) == 0xC) {
            return 0;
        }
        if (PLAYER_MODE(player) == 0xD) {
            return 0;
        }
        CPlayer__SetState(player, actor, 0xC);
        return 1;
    }
    return 0;
}

#define PLAYER_FLAGS_A1C(p) (*(s32 *)((u8 *)(p) + 0xA1C))

/* Puts the player in mode 0xE when touching an actor whose intelligence has
 * flag 1 and whose AI has flag 0x1000 (and not 0x80). */
s32 CPlayer__CheckClimb(CPlayer *player, CGameObjectInstance *actor) {
    if (InMode10To12(player)) {
        return 0;
    }
    if (actor->header.m_pIntelligence->m_dwTypeFlags & 1) {
        if (PLAYER_MODE(player) == 0x23) {
            return 0;
        }
        if (PLAYER_FLAGS_A1C(player) & 0x8000) {
            return 0;
        }
        if (actor->m_AI.m_dwFlags & 0x80) {
            return 0;
        }
        if (PLAYER_MODE(player) == 0xE) {
            return 0;
        }
        if (!(actor->m_AI.m_dwFlags & 0x1000)) {
            return 0;
        }
        CPlayer__SetState(player, actor, 0xE);
        return 1;
    }
    return 0;
}

#define PLAYER_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define PLAYER_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))

/* Puts the player in mode 4 when touching an actor whose intelligence has
 * flag 2 (and AI flags 0x6000 clear), if player flag 0x10 at 0xA68 is set. */
/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): 0xB8C, 0xA9C and 0xA68
 * are N64 offsets of CPlayer (m_SniperMode, m_TimeOffGround, m_ContState.m_dwButtonsSingle). */
s32 CPlayer__CheckJumpOn(CPlayer *player, CGameObjectInstance *actor) {
    if (player->m_SniperMode != 0) {
        return 0;
    }
    if (player->m_TimeOffGround > 11.25f) {
        return 0;
    }
    if (actor->m_AI.m_dwFlags & 0x6000) {
        return 0;
    }
    if (!(actor->header.m_pIntelligence->m_dwTypeFlags & 2)) {
        return 0;
    }
    if (!(player->m_ContState.m_dwButtonsSingle & 0x10)) {
        return 0;
    }
    CPlayer__SetState(player, actor, 4);
    return 1;
}

/* Releases the object the player is carrying (0xBB0) and throws it ahead of
 * the player: it is moved 102.4 units along the facing (angle 0x50 + pi),
 * raised 163.84, and given a velocity of 358.4 forward and 358.4 up. */


extern void EnemyAI_Dismount(void *obj, void *ai, CPlayer *player);
extern f32 func_002982F0(f32 x);   /* sinf */
extern f32 func_002974C0(f32 x);   /* cosf */

void CPlayer__Dismount(CPlayer *player, CGameObjectInstance *obj) {
    if (player->m_ModeFlags & 0x8000) {
        EnemyAI_Dismount(((u8 *)player->m_pOnEnemy), (u8 *)&((CGameObjectInstance *)player->m_pOnEnemy)->m_AI, player); /* N64 +0x140 */
        player->m_pOnEnemy = NULL;
        player->m_Speed = 0;
        player->m_SideSpeed = 0;
        CPlayer__SetState(player, obj, 5);
        obj->header.m_vPos.x -= func_002982F0(obj->m_RotY + 3.1415927f) * 102.399994f;
        obj->header.m_vPos.y += 163.84f;
        obj->header.m_vPos.z -= func_002974C0(obj->m_RotY + 3.1415927f) * 102.399994f;
        obj->m_vVelocity.x = func_002982F0(obj->m_RotY + 3.1415927f) * -358.4f;
        obj->m_vVelocity.y = 358.4f;
        obj->m_vVelocity.z = func_002974C0(obj->m_RotY + 3.1415927f) * -358.4f;
        CCache__SetFrameDelay(8);
    }
}

/* gCollisionResults is the collision results (CCollisionResults, structs/collision.h);
 * this reads its first word, m_pInstance (what the last test hit), as a
 * pointer variable: gCollisionResults.m_pInstance does not give this code. */
extern CGameObjectInstance * gCollisionHitInstance __asm__("gCollisionResults"); /* gCollisionResults (declared by globals/collision.h) seen as this file's type: what keeps the ROM's code */

static __inline__ u8 ActorKind(CGameObjectInstance *actor) {
    return actor->header.m_Type;
}

/* In multiplayer, when actor 0x64E is a kind-1 actor whose 0x174/0x176 bytes
 * differ, ends the player's tag mode 1 and records it. */
s32 CPlayer__CheckTagHit(CPlayer *p) {
    CGameObjectInstance **slot;
    CGameObjectInstance *a;
    CPlayerOptions *opts;
    s32 kind;
    s32 mode;

    if (gCollisionHitInstance != NULL) {
        slot = &gCollisionHitInstance; /* read through the slot: the ROM copies the pointer */
        a = *slot;
        kind = a->header.m_Type;
        if (kind == 1 && gEngineApp_m_Game.m_Settings.multiplayer && gCollisionHitInstance->m_nTypeFlag == 0x64E
            && gCollisionHitInstance->m_AI.m_ModeBefore != gCollisionHitInstance->m_AI.m_Mode) {
            opts = p->m_pOptions;
            mode = opts->m_TagMode;
            if (mode == kind) {
                opts->m_TagMode = 0;
                p->m_pOptions->m_LastTagMode = mode;
                CPlayer__Respawn(p);
                gEngineApp_m_Game.m_Settings.match.tagPhase = mode;
            }
        }
    }
    return 0;
}

void Player_ZeroVelocity(s32 arg0, CPlayer *arg1) {
    arg1->m_CharacterActor.m_vVelocity.x = 0;
    arg1->m_CharacterActor.m_vVelocity.y = 0;
    arg1->m_CharacterActor.m_vVelocity.z = 0;
}

void PlayerStand_Enter(CPlayer *arg0, sptr arg1) {
    if ((arg0->m_Speed != 0.0f) || (arg0->m_SideSpeed != 0.0f)) {
        CPlayer__SetState(arg0, arg1, 3);
    }
}

void PlayerWalk_Enter(void) {

}

#ifdef TARGET_PORT
/* The port: typed on CGameObjectInstance (the m2c fields were N64 offsets: the velocity at 0x1C,
 * the definition at 0x14); the platform pointer-wide; the jump sound takes the position as the
 * CVector3 Sound__Play is defined with (three s32 words went in integer registers). */
#include "common.h"
#include "structs/game_object.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "structs/intelligence.h"
#include <string.h>

sptr GameObject_GetPlatformBelow(CGameObjectInstance *);
void GameObject_GetFrameDelta(sptr, CVector3 *);
s32 Sound__Play(s16, CVector3, sptr, sptr);
void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.m_JumpScale) == 0x20, "CIntelligence 0x20");

void PlayerJump_Enter(void *arg0, CGameObjectInstance *arg1) {
    CVector3 delta;
    f32 speed;
    sptr platform;

    speed = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        speed = 481.28f;
    }
    /* The jump scale, CPlayerIntelligence.m_JumpScale (0x20 of the intelligence). Round 11 (z5) read
     * its big-endian bytes (the field was pad08, left unswapped: every jump left with no upward
     * speed); round 12 (a5) typed it, so the archive swaps it. */
    arg1->m_vVelocity.y = speed * arg1->header.m_pIntelligence->m_Player.m_JumpScale;
    platform = GameObject_GetPlatformBelow(arg1);
    if (platform != 0) {
        GameObject_GetFrameDelta(platform, &delta);
        CVector3__Add(&arg1->m_vVelocity, &arg1->m_vVelocity, &delta);
    }
    if (gEngineApp_m_Settings.multiplayer == 0) {
        Sound__Play(0x3AC1, arg1->header.m_vPos, 0, -1);
    }
}
#else
#include "common.h"
#include "m2c_macros.h"
#include "globals/game.h"
#include "globals/scene.h"

sptr GameObject_GetPlatformBelow(s32);
s32 GameObject_GetFrameDelta(sptr, sptr);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play Sound__Play_i3
#endif
/* Sound__Play with its CVector3 as the three words it travels in (a1-a3) */
s32 Sound__Play_Words(s32, s32, sptr, sptr, s32, s32) __asm__("Sound__Play");

void CVector3__Add(CVector3 *, CVector3 *, CVector3 *);

void PlayerJump_Enter(sptr arg0, void *arg1) {
    u8 sp18[0x10];
    f32 var_f1;
    sptr temp_v0;
    void *temp_a0;

    var_f1 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f1 = 481.28f;
    }
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) (var_f1 * M2C_FIELD(M2C_FIELD(arg1, void **, 0x14), f32 *, 0x20));
    temp_v0 = GameObject_GetPlatformBelow((sptr) arg1);
    if (temp_v0 != 0) {
        GameObject_GetFrameDelta(temp_v0, (sptr) sp18);
        temp_a0 = arg1 + 0x18;
        CVector3__Add(temp_a0, temp_a0, sp18);
    }
    if (gEngineApp_m_Settings.multiplayer == 0) {
        Sound__Play_Words(0x3AC1, M2C_FIELD(arg1, s32 *, 4), M2C_FIELD(arg1, s32 *, 8), M2C_FIELD(arg1, s32 *, 0xC), 0, -1);
    }
}
#endif

void PlayerFall_Update(CPlayer *player, CGameObjectInstance *body);
typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 speed;
} MoveDef;
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MoveDef *def;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 speed;
} MoveState;
#ifdef TARGET_PORT
/* The port (round 11 z5): MoveState is the N64 layout of the body (the definition at 0x14, the
 * vertical velocity at 0x1C), which missed both host members, and the jump scale (0x20 of the
 * player's intelligence) is CPlayerIntelligence.m_JumpScale (round 12 a5: typed, so the archive
 * swaps it; round 11 read its big-endian bytes). Without it a held jump fell at once (demo 3, gf 72). */
#include "structs/game_object.h"
#include "structs/intelligence.h"
#include <string.h>
_Static_assert(__builtin_offsetof(CIntelligence, m_Player.m_JumpScale) == 0x20, "CIntelligence 0x20");

void PlayerJump_Update(CPlayer *arg0, MoveState *arg1) {
    CGameObjectInstance *body = (CGameObjectInstance *) arg1;
    f32 var_f2;

    var_f2 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->m_ContState.m_dwButtons & 0x10) || (arg0->m_ModeTime >= 3.0f)) {
        CPlayer__SetState(arg0, arg1, 5);
    } else {
        body->m_vVelocity.y = var_f2 * body->header.m_pIntelligence->m_Player.m_JumpScale;
    }
    PlayerFall_Update((sptr) arg0, (sptr) arg1);
}
#else
void PlayerJump_Update(CPlayer *arg0, MoveState *arg1) {
    f32 var_f2;

    var_f2 = 389.12f;
    if (gScene_m_LevelID == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->m_ContState.m_dwButtons & 0x10) || (arg0->m_ModeTime >= 3.0f)) {
        CPlayer__SetState(arg0, arg1, 5);
    } else {
        arg1->speed = (f32) (var_f2 * arg1->def->speed);
    }
    PlayerFall_Update((sptr) arg0, (sptr) arg1);
}
#endif

extern void Math_EaseToward(f32 *value, f32 target, f32 rate);
extern f32 GameObject_GetGroundHeight(void *body, s32 arg);

/* Eases the lean back to zero, runs the movement tables, and on the ground
 * (falling) picks animation 0x7F8 when the slope speed is under 61.44. */
void PlayerFall_Update(CPlayer *player, CGameObjectInstance *body) {
    Math_EaseToward(&player->m_vHeadRotOffset.z, 0.0f, 0.25f);
    CPlayer__TurnAndLook(player, body, &gStandLookTuning);
    if (!(player->m_ModeBeforeFlags & 0x8000)) {
        CPlayer__MoveOnFoot(player, body, &gStandMoveTuning);
    }
    if (body->m_vVelocity.y <= 0.0f) {
        if (GameObject_IsOnGround(body) != 0) {
            CPlayer__SetState(player, body, 2);
        }
    }
    if (body->m_vVelocity.y <= 0.0f) {
        if (GameObject_GetGroundHeight(body, 0) < 0.0f) {
            if (-GameObject_GetGroundHeight(body, 0) < 61.44f) {
                goto set_value;
            }
        } else if (GameObject_GetGroundHeight(body, 0) < 61.44f) {
set_value:
            player->m_CharAnim = 0x7F8;
        }
    }
}

#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif

typedef struct {
#ifdef TARGET_PORT /* the body: its vertical velocity at the host offset (round 11 z5; at the N64 0x1C the
                    * dodge's hop wrote into another member on the host) */
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_vVelocity.y)];
#else
    /* 0x00 */ u8 pad0[0x1C];
#endif
    /* 0x1C */ f32 unk1C;
} Arg1;

void PlayerDodge_Enter(CPlayer *arg0, Arg1 *arg1) {
    if (arg0->m_ContState.m_FinalMoveX < 0.0f) {
        arg0->m_SideSpeed = (f32) -30.72f;
    } else {
        arg0->m_SideSpeed = (f32) 30.72f;
    }
    arg1->unk1C = (f32) 307.19998f;
}
void PlayerDodgeFall_Update(sptr arg0, sptr arg1);

void PlayerDodge_Update(CPlayer *arg0, CGameObjectInstance *arg1) {
    f32 temp_f2;

    temp_f2 = arg0->m_SideSpeed;
    if (((temp_f2 < 0.0f) && (arg0->m_ContState.m_FinalMoveX >= 0.0f)) || ((temp_f2 > 0.0f) && (arg0->m_ContState.m_FinalMoveX <= 0.0f)) || (arg0->m_ModeTime >= 1.5f)) {
        CPlayer__SetState(arg0, arg1, 7);
    } else {
        arg1->m_vVelocity.y = 307.19998f;
    }
    PlayerDodgeFall_Update((sptr) arg0, (sptr) arg1);
}

struct LevelActor_255940;
typedef struct LevelActor_255940 LevelActor_255940;
struct LevelActor_255940 {
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    s16 modelIndex;
    u16 pad6;
    CVector3 position0;
    CIntelligence *definition; /* 0x14 in Turok 2 (Rage Wars has an extra s32 before it) */
    CVector3 position1;
    CVector3 motionAngles;
    void *attached;
    s32 motionState;
    s32 motionIndex;
    f32 floorHeight;
    CVector3 floorDirection;
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    Box bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    Shared_ParticleLighting lighting[2];
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};

void PlayerDodgeFall_Update(sptr arg0, sptr arg1) {
    Math_EaseToward(arg0 + 0xAD0, 0.0f, 0.25f);
    CPlayer__TurnAndLook((void *)arg0, (void *)arg1, &gStandLookTuning);
    CPlayer__MoveOnFoot((void *)arg0, (void *)arg1, &gDodgeMoveTuning);
    if (((LevelActor_255940 *)arg1)->position1.y <= 0.0f) {
        if (GameObject_IsOnGround((void *) arg1) != 0) {
            CPlayer__SetState((void *) arg0, (void *) arg1, 2);
        }
    }
}

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#endif

extern f32 GameObject_GetWaterHeight(CGameObjectInstance *actor);

void PlayerWade_Enter(CPlayer *player, CGameObjectInstance *actor) {
    if (!(actor->m_AI.m_dwFlags & 0xC0000)) {
        actor->m_vVelocity.y = 0.0f;
        Math_EaseToward(&P_F32(player, 0xAA4), GameObject_GetWaterHeight(actor) - P_F32(player, 0xAE4) * 0.6f, 0.5f);
    }
}

extern void CCamera__ResetWaterWobble(void *camera);

/* Tracks the lowest height reached, damps the player's motion and the actor's
 * vertical speed (clamped at -51.2), resets state and plays sound 2000 there. */
void PlayerSwim_Enter(CPlayer *player, CGameObjectInstance *actor) {
    f32 h;
    CBarrel *s;

    h = GameObject_GetWaterHeight(actor) - player->m_vEyeOffset.y;
    if (h < player->m_vDesiredPos.y) {
        player->m_vDesiredPos.y = h;
    }
    player->m_Speed *= 0.5f;
    player->m_SideSpeed *= 0.5f;
    actor->m_vVelocity.y *= 0.75f;
    if (actor->m_vVelocity.y < -51.199997f) {
        actor->m_vVelocity.y = -51.199997f;
    }
    player->m_bOxygenOut = 0;
    CCamera__ResetWaterWobble(player->m_pCamera);
    s = &player->m_Barrel;
    s->m_State = 0;
    s->m_Turn = 0;
    s->m_nCursor = -1;
    s->m_Open = 0;
    Sound__Play(2000, actor->header.m_vPos, 0, -1);
}

/* Byte-offset field access keeps the original load order. */
#define FIELD_AT(ptr, type, T, field) (*(type *)((s8 *)(ptr) + (s32) &((T *)0)->field))

void PlayerState12_Enter(CPlayer *arg0, CGameObjectInstance *arg1) {
    FIELD_AT(arg0, f32, CPlayer, m_Speed) = (FIELD_AT(arg0, f32, CPlayer, m_Speed) * 0.5f);
    FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) = (FIELD_AT(arg0, f32, CPlayer, m_SideSpeed) * 0.5f);
    FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) = (FIELD_AT(arg1, f32, CGameObjectInstance, m_vVelocity.y) * 0.5f);
}

/* Landing state: pose 0xC or 0xD, movement tables, animation 0x8A2, 0x8A7
 * or 0x14 by the stick (unless animation 0x1144 is still running), then
 * state 3 once the pose timer passes 2 without body bit 0x800. */
void PlayerState12_Update(CPlayer *player, CGameObjectInstance *body) {
    s32 busy;
    f32 x;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 0xC;
    } else {
        player->m_Mode = 0xD;
    }
    CPlayer__TurnAndLook(player, body, &gPlayerState12LookTuning);
    CPlayer__MoveInWater(player, body, &gPlayerState12MoveTuning);
    busy = 0;
    if (player->m_CharAnim == 0x1144) {
        busy = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!busy) {
        x = player->m_Speed;
        if (1.024f <= x) {
            player->m_CharAnim = 0x8A2;
        } else if (x <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    if (!(body->m_AI.m_dwFlags & 0x800) && player->m_ModeTime > 2.0f) {
        CPlayer__SetState(player, body, 3);
    }
}

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
} Arg1_255C5C;

/* The port (round 9 x1): arg1 is the player's body; the N64 words 0x18/0x1C/0x20 are its
 * m_vVelocity (zeroed as 0 words = 0.0f). Through the N64 view the host cleared the header's
 * m_pIntelligence and m_vVelocity.x instead: PlayerClimb_Update's jump test then read
 * m_pIntelligence->m_dwTypeFlags through NULL (the first ladder/climbable wall crashed). */
void PlayerClimb_Enter(CPlayer *arg0, Arg1_255C5C *arg1_) {
    CGameObjectInstance *arg1 = (CGameObjectInstance *) arg1_;

    arg0->m_Speed = 0;
    arg0->m_SideSpeed = 0;
    arg0->m_HandSide = 0.024348905f;
    arg1->m_vVelocity.x = 0.0f;
    arg1->m_vVelocity.y = 0.0f;
    arg1->m_vVelocity.z = 0.0f;
}

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_S16(p, off) (*(s16 *)((u8 *)(p) + (off)))
#endif

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__StopLoopingSounds receives PlayerState15_Enter's own argument
 * registers (pass-through); the host passes them. */
void PlayerState15_Enter(CPlayer *player, CGameObjectInstance *actor) {
    char *text;

    actor->m_vVelocity.x = 0.0f;
    actor->m_vVelocity.y = 0.0f;
    actor->m_vVelocity.z = 0.0f;
    CPlayer__StopLoopingSounds(player);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0) {
        switch ((u32) CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player)) {
        default:
        case 0:
            text = (char *)&gStatsContinueMenuDesc1;
            break;
        case 1:
            text = (char *)&gStatsContinueMenuDesc2;
            break;
        case 2:
            text = (char *)&gStatsContinueMenuDesc3;
            break;
        case 3:
            text = (char *)&gStatsContinueMenuDesc4;
            break;
        }
        MenuPool_OpenMenu((u8 *)player->m_pCamera->m_MenuPool, text, player, (sptr) player->m_ContState.m_pContState, NULL);
    } else {
        text = (char *)&gGameOverMenuDesc;
        if (P_S16(player, 0x52A) != 0) {
            text = (char *)&gContinueMenuDesc;
        }
        MenuPool_OpenMenu((u8 *)player->m_pCamera->m_MenuPool, text, player, (sptr) player->m_ContState.m_pContState, NULL);
    }
}

void PlayerState15_Update(void) {

}

/* CPlayer__StopLoopingSounds takes the player (as in CPlayer__Kill): the call keeps it in $a0,
 * which is what lets sched2 keep the mask's lui/ori together. */
/* Clears the player's flag 0x800000 and, in multiplayer, opens the screen of
 * the player's slot (one of four) on the player's camera. */
void PlayerState16_Enter(CPlayer *player) {
    struct MenuDesc *screen;

    player->m_ShockedTimer = 0;
    player->m_CharacterActor.m_dwFlags &= ~0x800000;
    CPlayer__StopLoopingSounds(player);
    if (gEngineApp_m_Game.m_Settings.multiplayer) {
        switch ((u32)CPlayerPool__GetIndex(&gEngineApp_m_Game.m_PlayerList, player)) {
        case 0:
        default:
            screen = &gStatsSaveDoneMenuDesc1;
            break;
        case 1:
            screen = &gStatsSaveDoneMenuDesc2;
            break;
        case 2:
            screen = &gStatsSaveDoneMenuDesc3;
            break;
        case 3:
            screen = &gStatsSaveDoneMenuDesc4;
            break;
        }
        MenuPool_OpenMenu(player->m_pCamera->m_MenuPool, screen, player, player->m_ContState.m_pContState, 0);
    }
}

void func_00255E44(void) {

}

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#endif

void PlayerState17_Enter(CPlayer *player) {
    CPlayer *current = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;

    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    if (player == current) {
        MenuPool_OpenMenu(gEngineApp_m_Game.m_MenuPool, &gFinalScoreMenuDesc, player, (sptr) player->m_ContState.m_pContState, NULL);
    }
}

void PlayerState17_Update(void) {

}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__StopLoopingSounds receives PlayerDeathWait_Enter's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeathWait_Enter(CPlayer *arg0) {
    arg0->m_WaitDeathmatchTimer = 0;
    CPlayer__StopLoopingSounds(arg0);
    arg0->m_CharacterActor.m_vVelocity.x = 0;
    arg0->m_CharacterActor.m_vVelocity.y = 0;
    arg0->m_CharacterActor.m_vVelocity.z = 0;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags = arg0->m_CharacterActor.m_dwFlags & 0xFF7FFFFF;
}

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#endif

void PlayerDeathWait_Update(CPlayer *player) {
    GameMatch *state;
    f32 timer;

    timer = P_F32(player, 0xBFC) + gFrameIncrement;
    P_S32(player, 0xC04) = 1;
    P_S32(player, 0x18) = 0;
    P_S32(player, 0x1C) = 0;
    P_S32(player, 0x20) = 0;
    P_S32(player, 0x1298) = 0;
    P_S32(player, 0xD4) &= ~0x800000;
    P_F32(player, 0xBFC) = timer;
    if (timer > 75.0f || ((state = &gEngineApp_m_Settings.match)->teamOver != 0 && state->over != 0)) {
        state = &gEngineApp_m_Settings.match;
        if (state->teamOver != 0 && state->over == 0) {
            CPlayer__SetState(player, player, 0x11);
        } else {
            CPlayer__SetState(player, player, 0x10);
        }
    }
}
void PlayerCinema50_Done(void);

void PlayerCinema50_Enter(CPlayer *player) {
    sCinema50Player = player;
    Cinema_SetNextCallbacks(0x50, 0, &PlayerCinema50_Done);
}

void CPlayer__UpdateDeath(CPlayer *player) {
    if (player->m_Mode == 29) {
        player->m_CharacterActor.m_vVelocity.y = -307.19998f;
    }
    if (player->m_DoDeathCin == 0) {
        if (!(player->m_ModeFlags & 0x8000)) {
            Math_EaseToward(&player->m_vHeadRotOffset.z, 1.308997f, 0.25f);
        }
        if (player->m_ModeTime > 37.5f) {
            CPlayer__LoseLife(player);
        }
    }
}

void PlayerDeath0A_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xA);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0A_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0A_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath32_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x32);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath32_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath32_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath1E_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x1E);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath1E_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath1E_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath28_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x28);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath28_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath28_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath0B_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xB);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0B_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0B_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath0C_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xC);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0C_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0C_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath0D_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xD);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0D_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0D_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath0E_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xE);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0E_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0E_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath0F_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0xF);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath0F_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath0F_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath14_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x14);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath14_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath14_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath46_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x46);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath46_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath46_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

void PlayerDeath3C_Enter(sptr arg0, sptr arg1) {
    CPlayer__StartDeathCinema(arg0, arg1, 0x3C);
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateDeath receives PlayerDeath3C_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerDeath3C_Update(void * pt0) {
    CPlayer__UpdateDeath(pt0);
}

#ifdef TARGET_PORT
/* the N64 CPlayer offsets mapped to the host members (port/include/t2_player_offsets.h) */
#include "t2_player_offsets.h"
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + T2PlayerOff(off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + T2PlayerOff(off)))
#else
#define P_S32(p, off) (*(s32 *)((u8 *)(p) + (off)))
#define P_F32(p, off) (*(f32 *)((u8 *)(p) + (off)))
#define P_VEC(p, off) (*(CVector3 *)((u8 *)(p) + (off)))
#endif

extern void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b); /* out = a - b */
extern f32 CVector3__GetYaw(CVector3 *v);

void PlayerJumpPad_Enter(CPlayer *player, CGameObjectInstance *actor) {
    CVector3 pos;
    CVector3 delta;
    CRegionInfo *region = GameObject_GetRegionInfo(actor);

    P_S32(player, 0xB98) = 0;
#ifdef TARGET_PORT
    *(CRegionInfo **)((u8 *)player + T2PlayerOff(0xBA0)) = region;
#else
    *(CRegionInfo **)((u8 *)player + 0xBA0) = region;
#endif
    P_F32(player, 0xBA4) = actor->header.m_vPos.x;
    P_F32(player, 0xBA8) = actor->header.m_vPos.y;
    P_F32(player, 0xBAC) = actor->header.m_vPos.z;
    pos.x = region->m_vPos.x;
    pos.y = region->m_vPos.y;
    pos.z = region->m_vPos.z;
    CVector3__Subtract(&delta, &pos, &P_VEC(player, 0xBA4));
    delta.y = 0.0f;
    P_F32(player, 0xB9C) = CVector3__GetYaw(&delta);
    if (region == NULL || !(region->m_wFlags54 & 0x10)) {
        Sound__Play2D(0x1450);
    }
}

void PlayerRideIdle_Enter(CPlayer *arg0) {
    CBarrel *temp_v0;

    temp_v0 = &arg0->m_Barrel;
    arg0->m_Barrel.m_State = 0;
    temp_v0->m_Turn = 0;
    temp_v0->m_nCursor = -1;
    temp_v0->m_Open = 0;
    arg0->m_bHeadMovementDisabled = 0;
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateRiding receives PlayerRideIdle_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerRideIdle_Update(CPlayer *arg0, sptr arg1) {
    CPlayer__UpdateRiding(arg0, arg1);
    if (arg0->m_Speed != 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x22);
    }
}

void PlayerRideMove_Enter(CPlayer *arg0) {
    CBarrel *temp_v0;

    temp_v0 = &arg0->m_Barrel;
    arg0->m_Barrel.m_State = 0;
    temp_v0->m_Turn = 0;
    temp_v0->m_nCursor = -1;
    temp_v0->m_Open = 0;
    arg0->m_bHeadMovementDisabled = 0;
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CPlayer__UpdateRiding receives PlayerRideMove_Update's own argument
 * registers (pass-through); the host passes them. */
void PlayerRideMove_Update(void *arg0, sptr arg1) {
    CPlayer__UpdateRiding(arg0, arg1);
    if (((CPlayer *)arg0)->m_Speed == 0.0f) {
        CPlayer__SetState(arg0, arg1, 0x21);
    }
}

extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct Object126;
typedef struct Object126 Object126;
struct Actor126;
typedef struct Actor126 Actor126;
struct Actor126 {         /* Turok 2 offsets (Rage Wars in comments) */
    u8 pad0[0xD4];
    s32 flags;            /* 0x0D4 (0x100) */
    u8 padD8[0xCC];
    f32 value1A4;         /* 0x1A4 (0x1D4) */
    u8 pad1A8[0x862];
    s16 stateA0A;         /* 0xA0A (0x652) */
    u8 padA0C[0xC0];
    f32 offsetACC;        /* 0xACC (0x728) */
};
struct Object126 {
    u8 pad0[0x18];
    f32 x;                /* 0x18 (0x1C) */
    u8 pad1C[4];
    f32 y;                /* 0x20 (0x24) */
    u8 pad24[0x2C];
    f32 angle;            /* 0x50 (0x6C): yaw */
};
#ifndef TARGET_PORT
extern void PlayerShock_Enter(Actor126 *actor, Object126 *arg1);
#endif
#ifdef TARGET_PORT
/* The port (round 11 C26): the player and the body by member. The N64 views (Actor126: 0xD4 the
 * flags, 0x1A4 m_AI.m_ConditionTimer, 0xA0A m_ModeBefore, 0xACC m_vHeadRotOffset.y; Object126:
 * 0x18/0x20 the velocity's x/z, 0x50 m_RotY) all missed on the host: the shock never set its
 * timer (PlayerShock_Update reads it by member) nor the 0x800000 flag. */
void PlayerShock_Enter(CPlayer *actor, CGameObjectInstance *object) {
    f32 object_angle;
    f32 angle;
    f32 sine;
    f32 cosine;

    actor->m_CharacterActor.m_dwFlags |= 0x800000;
    actor->m_CharacterActor.m_AI.m_ConditionTimer = 52.5f;
    if (actor->m_ModeBefore == 14) {
        object_angle = object->m_RotY;
        angle = 6.2831855f;
        sine = func_002982F0(object_angle + angle);
        cosine = func_002974C0(object->m_RotY + angle);
        sine *= 384.0f;
        cosine *= 384.0f;
        object->m_vVelocity.x += sine;
        object->m_vVelocity.z += cosine;
        object->m_RotY += actor->m_vHeadRotOffset.y;
        actor->m_vHeadRotOffset.y = 0.0f;
    }
}
#else
void PlayerShock_Enter(Actor126 *actor, Object126 *arg1) {
    Object126 *object;
    f32 angle;
    f32 object_angle;
    f32 sine;
    f32 cosine;
    actor->flags |= 0x800000;
    actor->value1A4 = 52.5f;
    if (actor->stateA0A == 14) {
        object = arg1;
        object_angle = object->angle;
        angle = 6.2831855f;
        sine = func_002982F0(object_angle + angle);
        cosine = func_002974C0(object->angle + angle);
        sine *= 384.0f;
        cosine *= 384.0f;
        object->x += sine;
        object->y += cosine;
        object->angle += actor->offsetACC;
        actor->offsetACC = 0.0f;
    }
}
#endif

f32 Ease_CubicOut(f32 arg0) {
    f32 temp_f12;

    temp_f12 = 1.0f - arg0;
    return 1.0f - (temp_f12 * temp_f12 * temp_f12);
}

void CPlayer__ResetHeadSway(CPlayer *player) {
    P_S32(player, 0xABC) = 0;
    P_F32(player, 0xAB4) = P_F32(player, 0xAB0);
    P_F32(player, 0xAB8) = 0.4014257788658142f; /* 23 degrees */
}

void CPlayer__ResetJumpAngle(CPlayer *arg0) {
    arg0->m_DesiredJumpAngle = 0;
    arg0->m_JumpU = 0;
    arg0->m_StartJumpAngle = (f32) arg0->m_JumpAngle;
}

void func_002565D4(void) {

}

s32 CPlayer__IsCrouched(CPlayer *arg0) {
    s32 var_v0;

    var_v0 = 1;
    if (!(arg0->m_DuckOffset > 5.12f)) {
        var_v0 = 0;
    }
    return var_v0;
}

/* Crouch: toggled by button 0x400 while standing or walking (poses 2 and 3),
 * forced by flags 0x6000; eases the crouch height toward 40.96 or 0 and sets
 * bit 0x80 of *out while it is within 10.24 of fully crouched. */
void CPlayer__UpdateCrouch(CPlayer *player, s32 unused, s32 *flags, s32 *out) {
    s32 crouching;
    f32 delta;
    u8 *input;

    crouching = 0;
    if (*flags & 0x6000) {
        crouching = 1;
    } else if ((u16)(player->m_Mode - 2) < 2) {
        if (player->m_ContState.m_dwButtonsSingle & 0x400) {
            input = (u8 *)&gEngineApp_m_Settings;
            if (input[0x19] == 0) {
                if (*(s32 *)(input - 0x38) == 0 || !(player->m_ContState.m_dwButtons & 0x20)) {
                    player->m_bDuckMode ^= 1;
                }
            }
        }
    } else {
        player->m_bDuckMode = 0;
    }
    if (player->m_bDuckMode != 0) {
        crouching = 1;
    }
    if (crouching) {
        delta = 0.25f * (40.96f - player->m_DuckOffset);
    } else {
        delta = -player->m_DuckOffset * 0.25f;
    }
    player->m_DuckOffset += delta * gFrameIncrement;
    delta = 40.96f - player->m_DuckOffset;
    if (-10.24f <= delta && delta <= 10.24f) {
        *out |= 0x80;
    } else {
        *out &= ~0x80;
    }
}

typedef struct SoundRange {
    char pad0[0x18];
    s8 min;                    /* 0x18 */
    s8 max;                    /* 0x19 */
    s8 divisor;                /* 0x1A */
} SoundRange;
typedef struct SoundEvent {
    CGameObjectInstance *object;        /* 0x00 (Rage Wars 0x08) */
    char pad4[0x14 - 0x4];
    SoundRange *range;         /* 0x14 (Rage Wars 0x1C) */
    char pad18[0x90 - 0x18];
    s32 value;                 /* 0x90 (Rage Wars 0x04) */
} SoundEvent;

void Actor_PlayRangedSound(void *arg0, void *arg1) {
    SoundRange *range;
    CPlayer *actor;
    s32 value;
    f32 amount;
    SoundEvent *event = arg1;
    CGameObjectInstance *object;
    range = event->range;
    value = event->value;
    if (value < range->min) {
        return;
    }
    if (range->max < value) {
        return;
    }
    object = event->object;
    actor = ((CPlayer *)object->m_AI.m_pParent);
    amount = -actor->m_vHeadRotOffset.x;
    if (actor->m_Mode == 0xE) {
        amount += 1.5707965f;
    }
    CMtxF__RotateX(arg0, amount / (f32)range->divisor);
}

s32 Actor_IsInRegionFlag40(CGameObjectInstance *actor)
{
    sptr id = ((sptr)actor->header.m_pCurrentRegion);
    if (id != 0) {
        void *p = CScene__GetRegionInfo(&gScene, id);
        if (p != ((void *)0)) {
            if (*(u16 *)((u8 *)p + 0x52) & 0x40) {
                return 1;
            }
        }
    }
    return 0;
}

#define TEAM(p) (*(s32 *) ((char *) (p) + 0xBF4))

void CPlayerPool__AddTeamScore(char *player, char *other, s32 event) {
    GameMatch *score = &gEngineApp_m_Settings.match;
    s32 *count;

    if (score->teams == 0) {
        return;
    }
    switch (TEAM(player)) {
        case 0:
            count = &score->teamScore[0];
            break;
        case 1:
            count = &score->teamScore[1];
            break;
        default:
            return;
    }
    switch (event) {
        case 0:
            (*count)--;
            break;
        case 1:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
        case 2:
            if (TEAM(other) != TEAM(player)) {
                (*count)++;
            }
            break;
    }
}

void CPlayer__OnParticleHitCallback(CGameObjectInstance *arg0, sptr arg1, sptr arg2) {
    CPlayer__OnParticleHit((sptr) arg0->m_AI.m_pParent, arg0, arg1, arg2);
}

void CPlayer__ResetControlState(CPlayer *player) {
    CVector3 *v;
    f32 zero;

    CPlayerContState__Init(&player->m_ContState, &gControllers[player->m_nController]);
    P_S32(player, 0xA78) = 0;
    P_S32(player, 0xA7C) = 0;
    P_S32(player, 0xA80) = 0;
    P_S32(player, 0xA84) = 0;
    P_S32(player, 0xA88) = 1;
    P_S32(player, 0xA8C) = 0;
    P_S32(player, 0xA90) = 0;
    P_S32(player, 0xA94) = 0;
    P_S32(player, 0xA9C) = 0;
    P_S32(player, 0xAFC) = 0;
    P_S32(player, 0xB00) = 0;
    P_S32(player, 0x1284) = 0;
    P_S32(player, 0xAB0) = 0;
    P_S32(player, 0xAB4) = 0;
    P_S32(player, 0xAB8) = 0;
    P_S32(player, 0xABC) = 0;
    P_S32(player, 0xAC8) = 0;
    P_S32(player, 0xACC) = 0;
    P_S32(player, 0xAD0) = 0;
    P_S32(player, 0xAD4) = 0;
    P_S32(player, 0xAD8) = 0;
    P_S32(player, 0xADC) = 0;
    P_S32(player, 0xAE0) = 0;
    P_F32(player, 0xAE4) = Actor_GetEyeHeight(player);
    P_F32(player, 0xAE8) = 0.0f;
    v = &P_VEC(player, 0xAEC);
    zero = P_F32(player, 0xAE8);
    v->x = v->y = v->z = zero;
    P_F32(player, 0xAF8) = 1.0f;
    P_F32(player, 0xB24) = zero;
    P_F32(player, 0xB28) = 1.0f;
    P_VEC(player, 0xAA0) = player->m_CharacterActor.header.m_vPos;
}

/* Fills the ammo of every held weapon to its maximum; returns whether any
 * ammo was added. */
s32 CPlayer__FillAllAmmo(CPlayer *player) {
    s32 added = 0;
    s32 w;
    s32 i;
    WeaponInfo *def;
    AmmoInfo *ammo;
    s16 *count;
    s16 n;

    for (w = 0; w < 35; w++) {
        def = gWeaponInfoTable[w];
        if ((s8)player->m_Data.m_WeaponList[w] == 0) {
            continue;
        }
        for (i = 0; i < 3; i++) {
            ammo = def->ammo[i];
            if (ammo == NULL) {
                break;
            }
            count = &player->m_Data.m_AmmoList[ammo->type];
            if (*count < ammo->max) {
                added = 1;
            }
            n = ammo->max;
            if (ammo->max < *count) {
                n = *count;
            }
            *count = n;
        }
    }
    return added;
}

s32 CPlayer__TopUpHealth(char *obj) {
    if (*(s32 *) (obj + 0x524) < 0x6400) {
        *(s32 *) (obj + 0x524) = 0x6400;
        return 1;
    }
    return 0;
}

u8 PickupCounter_Get(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->m_Data.m_SpecialPickupCount[index] >> 4;
    }
    return owner->m_Data.m_SpecialPickupCount[index] & 0xF;
}

/* PickupCounter_Get, inlined. */
static __inline__ u8 getCounter(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        return owner->m_Data.m_SpecialPickupCount[index] >> 4;
    }
    return owner->m_Data.m_SpecialPickupCount[index] & 0xF;
}

/* Bumps one nibble of a packed counter byte and returns that nibble. */
u8 PickupCounter_Inc(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->m_Data.m_SpecialPickupCount[index] += 0x10;
    } else {
        owner->m_Data.m_SpecialPickupCount[index] += 1;
    }
    return getCounter(owner, index, high);
}

/* Drops one nibble of a packed counter byte and returns that nibble. */
u8 PickupCounter_Dec(CPlayer *owner, s32 index, s32 high) {
    if (high != 0) {
        owner->m_Data.m_SpecialPickupCount[index] -= 0x10;
    } else {
        owner->m_Data.m_SpecialPickupCount[index] -= 1;
    }
    return getCounter(owner, index, high);
}

/* Whether weapon arg1 fires projectile arg2 (its ammo list stops at the first NULL). */
s32 Weapon_FiresProjectile(s32 arg0, s32 arg1, s32 arg2) {
    s32 i;
    WeaponInfo *p = gWeaponInfoTable[arg1];
    for (i = 0; i < 3; i++) {
        AmmoInfo *e = p->ammo[i];
        if (e != NULL) {
            if (e->projectile == arg2) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 0;
}

/* Probes a move of obj, raised by half its height plus its step height, to
 * *to; writes where it ends up back to *to and returns 1 if that differs
 * from the target. The object itself is left as it was. */
s32 Actor_ProbeMove(CGameObjectInstance *obj, CVector3 *to) {
    CObjectHead saved;
    s32 blocked;

    saved = *(CObjectHead *)obj;
    obj->header.m_vPos.y += GameObject_GetCollisionHeight(obj) * 0.5f;
    obj->header.m_vPos.y += obj->m_CollisionYOffset;
    Collision_MoveInstance(obj, *to, &gCollisionInfoLineOfSight);
    blocked = 0;
    if (to->x != obj->header.m_vPos.x || to->y != obj->header.m_vPos.y || to->z != obj->header.m_vPos.z) {
        blocked = 1;
    }
    *to = obj->header.m_vPos;
    *(CObjectHead *)obj = saved;
    return blocked;
}

f32 CPlayerPool__SumViewWeights(CPlayerPool *arg0, s32 arg1) {
    f32 var_f20;
    CPlayer *var_s0;

    var_s0 = arg0->m_ActiveList.m_pHead;
    var_f20 = 0.0f;
    if (var_s0 != NULL) {
        do {
            if (var_s0->m_pCamera != 0) {
                var_f20 += CVector3__Distance(arg1, &var_s0->m_CharacterActor.header.m_vPos);
            }
            var_s0 = var_s0->m_pNext;
        } while (var_s0 != NULL);
    }
    return var_f20;
}

/* Whether the player is in state 3 with 0xC04 in 0x7DF..0x7E3. */
s32 CPlayer__IsWalkAnim7DF(CPlayer *arg0) {
    s32 temp_a0;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_Mode == 3) {
        temp_a0 = arg0->m_CharAnim;
        if (temp_a0 >= 0x7DF) {
            var_a1 = temp_a0 < 0x7E4;
        }
    }
    return var_a1;
}

s32 CPlayer__IsClimbing(CPlayer *arg0) {
    return arg0->m_Mode == 0xE;
}

extern f32 func_00298470(f32 x); /* square root */

/* Keeps an actor inside a sphere of radius 20000 in regions with flag 0x4000. */
void CPlayer__KeepInSphere(CPlayer *player, CGameObjectInstance *actor) {
    CRegionInfo *region = GameObject_GetRegionInfo(actor);
    f32 xx;
    f32 zz;
    f32 y;

    if (region != NULL && (region->m_dwFlags & 0x4000)) {
        xx = actor->header.m_vPos.x * actor->header.m_vPos.x;
        zz = actor->header.m_vPos.z * actor->header.m_vPos.z;
        if (xx + actor->header.m_vPos.y * actor->header.m_vPos.y + zz > 400000000.0f) {
            y = 400000000.0f - xx - zz;
            if (y < 0.0f) {
                y = 0.0f;
            }
            y = func_00298470(y);
            if (actor->header.m_vPos.y < 0.0f) {
                y = -y;
            }
            actor->header.m_vPos.y = y;
            if (region->m_dwFlags & 0x4000) {
                actor->header.m_vPos.y = GameObject_GetRegionFloorHeight(actor, 0) - 1.0f;
            }
        }
    }
}

typedef struct {
    char pad0[0x24];
    s32 unk_24;                 /* 0x24 */
    char pad28[0x34 - 0x28];
    s32 unk_34;                 /* 0x34 */
} SubSettings;
typedef struct {
    u8 enabled;                 /* 0x000 */
    char pad1[0xB - 0x1];
    u8 value_B;                 /* 0x00B */
    char padC[0x10 - 0xC];
    u8 value_10;                /* 0x010 */
    char pad11[0x15 - 0x11];
    u8 value_15;                /* 0x015 */
} Settings;
#define SETTINGS_VIEW (*(Settings *) &gEngineApp_m_Settings_multiplayer) /* the file's view of gEngineApp_m_Settings_multiplayer, which globals/game.h declares: this type is what keeps the ROM's code */

u8 Hud_GetSpriteValue(s32 unused, s32 id) {
    Settings *settings;
    SubSettings *sub;

    if (id == 0x2DA) {
        settings = &SETTINGS_VIEW;
        sub = (SubSettings *)((char *)settings + 0x1BF);
        if (settings->enabled != 0) {
            if (sub->unk_34 == 0) {
                if (sub->unk_24 != 0) {
                    return settings->value_15;
                }
                return settings->value_B;
            }
            return settings->value_10;
        }
    }
    return 1;
}

#undef SETTINGS_VIEW

void Particle_AttachToActor(CGameObjectInstance *actor, void **slot, void *id, f32 z) {
    CDLight *p;
    CVector3 dir;
    CVector3 out;

    p = *slot;
    if (p == NULL) {
        p = CDLightSet__Alloc(&gScene.m_DLightSet, id);
        if (p == NULL) {
            return;
        }
    }
    *slot = p;
    dir.x = 0;
    dir.y = 0;
    dir.z = z;
    CGameObjectInstance__LocalToWorld(&out, actor, dir);
    CVector3__Add(&out, &out, &actor->header.m_vPos);
    p->m_bEnabled = 1;
    p->x = out.x;
    p->y = out.y + GameObject_GetCollisionHeight(actor) * 0.5f;
    p->z = out.z;
    p->m_ColorFade = 1.0f;
}

void Particle_FreeSlot(s32 arg0, s32 *arg1) {
    s32 temp_a1;

    temp_a1 = *arg1;
    if (temp_a1 != 0) {
        CDLightSet__Free((s32 *) &gScene.m_DLightSet, temp_a1);
        *arg1 = 0;
    }
}

f32 Angle_StepToward(f32 arg0, f32 arg1, s32 arg2, f32 arg3) {
    f32 temp_f3;
    f32 var_f0;
    f32 var_f1;
    f32 var_f2;

    Angle_Wrap((sptr) &arg0);
    Angle_Wrap((sptr) &arg1);
    if (arg2 > 0) {
        if (arg1 < arg0) {
            var_f0 = arg1 + 6.283186f;
            goto block_5;
        }
    } else if (arg1 > arg0) {
        var_f0 = arg1 - 6.283186f;
block_5:
        arg1 = var_f0;
    }
    temp_f3 = arg1 - arg0;
    var_f1 = temp_f3 * arg3 * gFrameIncrement;
    var_f2 = var_f1;
    if (var_f1 < 0.0f) {
        var_f2 = -var_f1;
    }
    if (temp_f3 < 0.0f) {
        if (!(-temp_f3 < var_f2)) {

        } else {
            goto block_12;
        }
    } else if (temp_f3 < var_f2) {
block_12:
        var_f1 = temp_f3;
    }
    return var_f1;
}

/* Swaps two 0x1C-byte records. */
void CBarrel__SwapIcons(Record1C *a, Record1C *b) {
    Record1C tmp = *a;

    *a = *b;
    *b = tmp;
}

/* qsort-style comparator on the float at 0xC. */
s32 CBarrel__CompareIcons(Sortable *arg0, Sortable *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}

void CBarrel__MarkUsable(CBarrel *list, void *player) {
    s32 i;

    for (i = 0; i < list->m_nCount; i++) {
        if (CPlayer__GetWeaponAmmo(player, list->m_Items[i].m_Weapon)) {
            list->m_Items[i].m_bAvailable = 1;
        } else {
            list->m_Items[i].m_bAvailable = 0;
        }
    }
}

/* Host body: SlotOwner is the CPlayer (N64 0x3C8 m_WeaponActor.m_AI.m_dwFlags, 0x972
 * m_Data.m_WeaponList). */
s32 CPlayer__CanSelectWeapon(CPlayer *self, s32 slot) {
    WeaponInfo *def = gWeaponInfoTable[slot];

    if (self->m_Data.m_WeaponList[slot] == 0) {
        return 0;
    }
    if (self->m_WeaponActor.m_AI.m_dwFlags & 0x80) {
        if (!(def->flags & 2)) {
            return 0;
        }
    } else if (!(def->flags & 1)) {
        return 0;
    }
    if (self->m_Data.m_WeaponList[def->alternate] != 0 && slot < def->alternate) {
        return 0;
    }
    return 1;
}

s32 Weapon_IsAllowedInMode(s16 id, s32 mode) {
    switch (mode) {
    case 0:
        if (gWeaponInfoTable[id]->flags & 1) {
            return 1;
        }
        break;
    case 1:
        if (gWeaponInfoTable[id]->flags & 2) {
            return 1;
        }
        break;
    case 2:
        if (id == 0x13) {
            return 1;
        }
        break;
    }
    return 0;
}

void CPlayer__ToggleLowerAmmo(CPlayer *arg0) {
#ifdef TARGET_PORT
    /* the N64 reads the automap's first word (gEngineApp_m_Game_m_DebugLines, 0x51 bytes before the
     * multiplayer flag); on the host that distance lands elsewhere (round 18 g2, n64viewcheck gptr) */
    if ((gEngineApp_m_Settings.multiplayer == 0) && ((gEngineApp_m_Game_m_DebugLines.unk0 == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x800)) {
#else
    if ((*(u8 *)&gEngineApp_m_Settings.multiplayer == 0) && ((*(s32 *)((u8 *)&gEngineApp_m_Settings.multiplayer - 0x51) == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x800)) {
#endif
        if (arg0->m_UseLowerAmmo != 0) {
            arg0->m_UseLowerAmmo = 0;
            return;
        }
        arg0->m_UseLowerAmmo = 1;
    }
}

#define gEngineApp_m_Game_m_DebugLines (*(s32 *) ((u8 *) &gEngineApp_m_Settings.multiplayer - 0x51))

void CPlayer__PreferAlternateWeapon(CPlayer *arg0) {
    s16 temp_a1;
    WeaponInfo *temp_v0;

    if ((*(u8 *) &gEngineApp_m_Settings.multiplayer == 0) && ((arg0->m_Barrel.m_bStickWheel == 0) || (arg0->m_Barrel.m_State == 0)) && ((gEngineApp_m_Game_m_DebugLines == 0) || !(arg0->m_ContState.m_dwButtons & 0x20)) && (arg0->m_ContState.m_dwButtonsSingle & 0x200)) {
        temp_v0 = gWeaponInfoTable[arg0->m_RequestedWeapon];
        temp_a1 = temp_v0->alternate;
        if ((temp_a1 != -1) && (arg0->m_Data.m_WeaponList[temp_a1] != 0)) {
            arg0->m_RequestedWeapon = (s16) (u16) temp_v0->alternate;
        }
    }
}

void CPlayer__RegenerateHealth(CPlayer *player) {
    f32 rate;
    f32 cur;
    f32 next = 0.0f; /* doubles as the zero for the tests below */
    f32 max;

    if (gEngineApp_m_Settings.multiplayer != 0) {
        rate = gCharacterHealthRegen[P_S32(player, 0x520)];
        if (rate != next) {
            cur = P_S32(player, 0x524);
            if (cur != next && ((CPlayerOptions *)player->m_pOptions)->m_TagMode == 0) {
                next = cur + (s32)(rate * 256.0f) * (gFrameIncrement * (1.0f / 15));
                max = (u32)(player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8);
                if (!(max <= next)) {
                    max = next;
                }
                P_S32(player, 0x524) = max;
            }
        }
    }
}

extern void Sound__SetReverb(f32 arg0);

void Sound_SetUnderwaterFilter(void *arg0, CVector3 *arg1) {
    f32 first;
    f32 amount;
    if (arg0 != 0 && arg1 != 0 &&
        (((Shared_Func80243A80Collision *)arg0)->flags & 0x40)) {
        first = CGameRegion__GetCeilingHeight(arg0,
            arg1->x, arg1->z);
        amount = (f32)(s32)(first - CGameRegion__GetHeight(arg0,
            arg1->x, arg1->z));
        if (amount < 1024.0f) {
            Sound__SetReverb(1.0f - (amount * 0.0009765625f));
            return;
        }
    }
    Sound__SetReverb(0.0f);
}

s32 func_00257634(void) {
    s32 buf[4];

    return 0;
}

s32 CPlayer__CheckLanded(s32 arg0, CGameObjectInstance *arg1) {
    if ((arg1->m_vVelocity.y <= 0.0f) && (GameObject_IsOnGround(arg1) != 0)) {
        CPlayer__SetState(arg0, arg1, 2);
        return 1;
    }
    return 0;
}

void CPlayer__PickLandingAnim(CPlayer *arg0) {
    f32 temp_f1;
    s32 var_a1;

    var_a1 = 0;
    if (arg0->m_CharAnim == 0x1144) {
        var_a1 = (s8)arg0->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (var_a1 == 0) {
        temp_f1 = arg0->m_Speed;
        if (1.024f <= temp_f1) {
            arg0->m_CharAnim = 0x8A2;
            return;
        }
        if (temp_f1 <= -1.024f) {
            arg0->m_CharAnim = 0x8A7;
            return;
        }
        arg0->m_CharAnim = 0x14;
    }
}

/* Same body as PlayerWade_Enter. */
void PlayerWade_EnterAlt(CPlayer *player, CGameObjectInstance *actor) {
    if (!(actor->m_AI.m_dwFlags & 0xC0000)) {
        actor->m_vVelocity.y = 0.0f;
        Math_EaseToward(&P_F32(player, 0xAA4), GameObject_GetWaterHeight(actor) - P_F32(player, 0xAE4) * 0.6f, 0.5f);
    }
}

void PlayerDeath_CinemaDone(void) {
    if (sDeathCinemaPlayer->m_Data.m_Lives == 1) {
        CPlayer__LoseLife((sptr) sDeathCinemaPlayer);
    } else {
        CPlayer__SetState(sDeathCinemaPlayer, sDeathCinemaPlayer, 0x1F);
    }
    gCinemaHidesActors = 1;
    gDeathTint = 0;
}

void PlayerCinema50_Done(void) {
    CPlayer__LoseLife(sCinema50Player);
}

void CPlayer__ClearShock(CPlayer *arg0) {
    arg0->m_CharacterActor.m_vVelocity.x = 0.0f;
    arg0->m_CharacterActor.m_vVelocity.y = 0.0f;
    arg0->m_CharacterActor.m_vVelocity.z = 0.0f;
    arg0->m_ShockedTimer = 0;
    arg0->m_CharAnim = 1;
    arg0->m_CharacterActor.m_dwFlags &= 0xFF7FFFFF;
}

typedef struct {
    /* 0x00 */ s32 *block;
    /* 0x04 */ s32 players;
    /* 0x08 */ s32 count;
} PlayerPool;

/* Allocates and clears count player records (0x1328 bytes each). */
void CPlayerPool__AllocPlayers(PlayerPool *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_a0;
    s32 temp_s0;

    temp_s0 = arg1 * 0x1328;
    arg0->count = arg1;
    temp_v0 = CCache__Allocate(0, temp_s0, 0x23, &sCPlayerPoolCacheDesc_Players);
    arg0->block = temp_v0;
    temp_a0 = *temp_v0;
    arg0->players = temp_a0;
    memset(temp_a0, 0, temp_s0);
}

void CPlayerPool__FreePlayers(void *arg0) {
    s32 temp_a1;

    Controller_SetRumbleOptionAll(0);
    CCache__Flush(0);
    temp_a1 = M2C_FIELD(arg0, s32 *, 0);
    if (temp_a1 != 0) {
        CCache__Deallocate(0, temp_a1);
        M2C_FIELD(arg0, s32 *, 0) = 0;
        M2C_FIELD(arg0, s32 *, 4) = 0;
        M2C_FIELD(arg0, s32 *, 8) = 0;
    }
}

s32 Level_GetHub(s32);

void CPlayer__ShowLevelNotCompleteScreen(s32 arg0, CPlayer *arg1)
{
  int new_var;
  u32 temp_a0;
  if (arg1->m_bLevelNotCompleteExit != 0)
  {
    temp_a0 = Level_GetHub(gScene_m_LevelID) - (new_var = 1);
    if ((temp_a0 < 6U) && (arg1->m_Data.m_dwLevelFirstTimeFlags & (new_var << temp_a0)))
    {
      MenuPool_OpenMenu((&gEngineApp_m_Game_m_MenuPool), (&gMissionFailedMenuDesc), (sptr) arg1, arg1->m_ContState.m_pContState, 0);
    }
    arg1->m_bLevelNotCompleteExit = 0;
  }
}

/* Only what this reads and writes. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 started;
    /* 0x38 */ s32 unk38;
} CinemaState;
s32 Cinema_PathPlaying();                           /* extern */
s32 Cinema_GetScriptId();                                /* extern */

void Cinema_CheckPlayerStart(CinemaState *arg0, CPlayer *arg1) {
    CCamera *temp_a1;

    if ((MenuPool_CountOpen(&gEngineApp_m_Game_m_MenuPool) == 0) && (Cinema_PathPlaying() != 0) && (Cinema_GetScriptId() == 0x190) && (arg0->started == 0)) {
        temp_a1 = arg1->m_pCamera;
        if ((temp_a1 != NULL) && (temp_a1->m_Menus.count == 0)) {
            arg0->started = 1;
            arg0->unk38 = 0;
        }
    }
}
s32 String_CountTrailingSpaces(s32 arg0, s32 arg1);

s32 String_CountTrailingSpaces(s32 arg0, s32 arg1) {
    s32 var_a1;
    s32 var_v1;
    s32 space;
    var_v1 = 0;
    space = 0x20;
    var_a1 = arg1;
loop_1:
    var_a1 -= 1;
    if (*((u8 *)arg0 + var_a1) == space) {
        var_v1 += 1;
        if (var_a1 > 0) {
            goto loop_1;
        }
    }
    return var_v1;
}

/* Stops the player's two looping sounds. */
VM_TEXT void CPlayer__StopLoopingSounds(CPlayer *p) {
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponBackground);
    p->m_pLoopingSound_WeaponBackground = 0;
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponFiring);
    p->m_pLoopingSound_WeaponFiring = 0;
}
