/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00263760.c, func_00263760.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "globals/pak.h"
#include "globals/controller.h"
#include "protos/ultra.h"

sptr osContReset(OSMesgQueue *mq, OSContStatus *data);
void osContGetQuery(OSContStatus *data);
void Controller_UpdateConnection(ControllerProfile *arg0);
void Controller_ProcessInput(ControllerProfile *c, s8 x, s8 y, s32 btn);

void Controller_ReadAll(void) {
    s32 i;
    switch ((u8)gPakQueueState) {
    case 0:
        osRecvMesg(&gSiMessageQueue, &gSiMesgBuf, 1);
        osContGetReadData(gContPadData);
        gMergedController.port = -1;
        gMergedController.connected = 0;
        gMergedController.inputStickX = 0;
        gMergedController.inputStickY = 0;
        gMergedController.stickX = 0;
        gMergedController.stickY = 0;
        gMergedController.buttons = 0;
        gMergedController.previousButtons = 0;
        gMergedController.buttonsSingle = 0;
        gMergedController.buttonsSingleNoHold = 0;
        gMergedController.buttonsDouble = 0;
        gMergedController.buttonsRepeat = 0;
        gMergedController.inputButtons = 0;
        gMergedController.inputPreviousButtons = 0;
        gMergedController.inputButtonsSingle = 0;
        gMergedController.inputButtonsSingleNoHold = 0;
        gMergedController.inputButtonsDouble = 0;
        gMergedController.inputButtonsRepeat = 0;
        for (i = 0; i < 4; i++) {
            Controller_ProcessInput(&gControllers[i], gContPadData[i].stick_x, gContPadData[i].stick_y, gContPadData[i].button);
            gMergedController.connected |= gControllers[i].connected;
            gMergedController.inputStickX += gControllers[i].inputStickX;
            gMergedController.inputStickY += gControllers[i].inputStickY;
            gMergedController.stickX += gControllers[i].stickX;
            gMergedController.stickY += gControllers[i].stickY;
            gMergedController.buttons |= gControllers[i].buttons;
            gMergedController.buttonsSingle |= gControllers[i].buttonsSingle;
            gMergedController.buttonsSingleNoHold |= gControllers[i].buttonsSingleNoHold;
            gMergedController.buttonsDouble |= gControllers[i].buttonsDouble;
            gMergedController.buttonsRepeat |= gControllers[i].buttonsRepeat;
            gMergedController.inputButtons |= gControllers[i].inputButtons;
            gMergedController.inputPreviousButtons |= gControllers[i].inputPreviousButtons;
            gMergedController.inputButtonsSingle |= gControllers[i].inputButtonsSingle;
            gMergedController.inputButtonsSingleNoHold |= gControllers[i].inputButtonsSingleNoHold;
            gMergedController.inputButtonsDouble |= gControllers[i].inputButtonsDouble;
            gMergedController.inputButtonsRepeat |= gControllers[i].inputButtonsRepeat;
        }
        gMergedController.inputStickX = (s8)((f32)(s8)gMergedController.inputStickX * 0.25f);
        gMergedController.inputStickY = (s8)((f32)(s8)gMergedController.inputStickY * 0.25f);
        gMergedController.stickX = (s8)((f32)(s8)gMergedController.stickX * 0.25f);
        gMergedController.stickY = (s8)((f32)(s8)gMergedController.stickY * 0.25f);
        break;
    case 1:
        osRecvMesg(&gSiMessageQueue, &gSiMesgBuf, 1);
        osContGetQuery(gContStatus);
        for (i = 0; i < 4; i++) {
            Controller_UpdateConnection(&gControllers[i]);
        }
        break;
    case 3:
        osContReset(&gSiMessageQueue, gContStatus);
    case 2:
        break;
    }
}

#include "globals/game.h"
#include "globals/rumble.h"

/* Controller pak / rumble service, run once per frame while enabled. */


extern void RumblePool_FreeFinished(RumblePool *pool);
void Controller_UpdateRumble(ControllerProfile *arg0);
s32 Controller_AnyPakWatched(void);
extern void osContStartQuery(void *);

void Controller_Service(void) {
    ControllerProfile *pad;
    s32 i;

    if (gControllersInitialised == 0) {
        return;
    }
    RumblePool_FreeFinished(&gRumblePool);
    if (osRecvMesg(&gPakAccessQueue, NULL, 0) == 0) {
        Controller_ReadAll();
        for (i = 0, pad = gControllers; i < 4; i++, pad++) {
            Controller_UpdateRumble(pad);
        }
        switch ((u8)gPakQueueState) {
        case 0:
            if (Controller_AnyPakWatched() != 0) {
                gPakQueueState = 1;
                osContStartQuery(&gSiMessageQueue);
                break;
            }
            osContStartReadData(&gSiMessageQueue);
            break;
        case 1:
        case 2:
        case 3:
            gPakQueueState = 0;
            osContStartReadData(&gSiMessageQueue);
            break;
        }
        osSendMesg(&gPakAccessQueue, NULL, 0);
    }
    if (gEngineApp_m_Settings.mode == 8) {
        if (--sControllerRescanTimer <= 0) {
            sControllerRescanTimer = 60;
            gPakQueueState = 1;
        }
    }
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802636B0_de_7.c, func_80263C24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Controller_UpdatePakInsertion(void);
s32 Controller_PakInserted(s32 arg0);

void Controller_UpdatePakInsertion(void)
{
    s32 i;
    for (i = 0; i < 4; i++) {
        if (Controller_IsPakWatched(i) != 0) {
            if (Controller_PakInserted(i) != 0) {
                gPakInserted[i] = 1;
                if (gPakRemoved[i] != 0) {
                    gPakChanged[i] = 1;
                    gPakRemoved[i] = 0;
                }
            } else if (gPakInserted[i] != 0) {
                gPakRemoved[i] = 1;
            }
        } else {
            gPakChanged[i] = 0;
            gPakRemoved[i] = 0;
            gPakInserted[i] = 0;
        }
    }
}

s32 RumblePool_Init(void *dst, void *src, s32 n);

void Controller_UpdateConnection(ControllerProfile *arg0) {
    ControllerProfile *c = arg0;
    s32 type;
    s32 bit;
    s32 bit2;
    s32 i;
    type = c->port;
    bit = ((gContStatus[type].errno >> 3) ^ 1) & 1;
    if (bit != 0 && c->connected == 0) {
        i = 0;
        c->port = type;
        c->connected = 0;
        c->rumblePak = 0;
        c->rumbleOn = 0;
        c->reset = 0;
        c->unk8 = 0;
        c->unkC = 0;
        c->unk10 = 0;
        c->inputStickX = 0;
        c->inputStickY = 0;
        c->stickX = 0;
        c->stickY = 0;
        c->inputPreviousButtons = 0;
        c->inputButtons = 0;
        c->inputButtonsSingle = 0;
        c->inputButtonsSingleNoHold = 0;
        c->inputButtonsDouble = 0;
        c->inputButtonsRepeat = 0;
        do {
            c->buttonHistory[i].taps = 0;
            c->buttonHistory[i].age = 0;
            c->buttonHistory[i].repeat = 0x30;
            c->buttonHistory[i].held = 0;
            i += 1;
        } while (i < 0x20);
        bit2 = ((gContStatus[type].errno >> 3) ^ 1) & 1;
        c->rumbleLevel = 0;
        c->rumbleTimer = 0;
        c->connected = bit2;
        RumblePool_Init(c->rumblePool, c->rumbleNodes, 3);
    }
    c->connected = bit;
}

void Controller_ProcessInput(ControllerProfile *c, s8 x, s8 y, s32 btn) {
    u32 b;
    f32 fx;
    register f32 fy asm("$f1");
    s32 prev;
    s32 pressed;
    s32 m;
    s32 i;
    u8 old;
    ControllerButtonHistory *e;
    if (c->connected != 0) {
        b = btn & 0xFFFF;
        if (x < -30) {
            b |= 0x20000;
        } else if (x >= 31) {
            b |= 0x40000;
        }
        if (y < -30) {
            b |= 0x80000;
        } else if (y >= 31) {
            b |= 0x10000;
        }
        if (x > 0) {
            x = x - 3;
            if (x < 0) {
                x = 0;
            }
        } else {
            x = x + 3;
            if (x > 0) {
                x = 0;
            }
        }
        if (y > 0) {
            y = y - 3;
            if (y < 0) {
                y = 0;
            }
        } else {
            y = y + 3;
            if (y > 0) {
                y = 0;
            }
        }
        fx = x;
        fy = y;
        if (fx == -128.0f) {
            fx = -127.0f;
        }
        if (fy == -128.0f) {
            fy = -127.0f;
        }
        if (fx < -80.0f) {
            fx = -80.0f;
        }
        if (80.0f < fx) {
            fx = 80.0f;
        }
        if (fy < -80.0f) {
            fy = -80.0f;
        }
        if (80.0f < fy) {
            fy = 80.0f;
        }
        prev = c->inputButtons;
        c->inputStickX = (s32)fx;
        c->inputStickY = (s32)fy;
        c->inputButtons = b;
        c->inputPreviousButtons = prev;
        pressed = (prev ^ b) & b;
        c->inputButtonsSingle |= pressed;
        c->inputButtonsRepeat |= pressed;
        m = 1;
        e = c->buttonHistory;
        i = 0x13;
        do {
            if (pressed & m) {
                if (e->age < 0x1E) {
                    e->age = 0;
                    e->taps++;
                    if (e->taps >= 2) {
                        c->inputButtonsDouble |= m;
                        e->taps = 0;
                    }
                } else {
                    e->taps = 1;
                    e->age = 0;
                }
            }
            e->age++;
            if (e->age >= 0x1F) {
                e->age = 0x1E;
            }
            if (b & m) {
                if ((u8)e->held < 0xFF) {
                    e->held += 1;
                }
                old = e->repeat;
                e->repeat = old - 1;
                if (e->repeat < 0) {
                    e->repeat = old + 4;
                    c->inputButtonsRepeat |= m;
                }
            } else {
                if ((u8)e->held < 0x11 && (c->inputPreviousButtons & m)) {
                    c->inputButtonsSingleNoHold |= m;
                }
                e->held = 0;
                c->inputButtonsRepeat &= ~m;
                e->repeat = 0x30;
            }
            e++;
            m <<= 1;
        } while (i-- != 0);
    }
}

#include "globals/scene.h"

u32 osMotorStop(void *pfs);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Rumble_GetLevelAt Rumble_GetLevelAt_i3
#endif

extern f32 Rumble_GetLevelAt(void *arg0, s32 arg1, s32 arg2, s32 arg3);
extern void RumblePool_FreeAll(RumblePool *pool);
extern unsigned int osMotorStart(void *arg0);

void Controller_UpdateRumble(ControllerProfile *arg0) {
    Triple zero;
    void *object;
    s32 *global;
    f32 first;
    if (arg0->rumblePak != 0) {
        zero.x = 0;
        zero.y = 0;
        zero.z = 0;
        if (arg0->rumbleOn == 0) {
            goto reset;
        }
        global = &gEngineApp_m_Settings.paused;
        if ((*global != 0) || (gScene.m_WarpState != 0) || (global[-6] != 13)) {
reset:
            arg0->rumbleLevel = 0.0f;
            RumblePool_FreeAll(arg0->rumblePool);
        } else {
            object = arg0->rumblePool;
            RumblePool_FreeFinished(object);
            first = Rumble_GetLevelAt(&gRumblePool, arg0->unk8, arg0->unkC, arg0->unk10);
            arg0->rumbleLevel = first + Rumble_GetLevelAt(object, zero.x, zero.y, zero.z);
        }
        if (arg0->rumblePak != 0) {
            if (arg0->rumbleLevel != 0.0f) {
                arg0->rumbleTimer += arg0->rumbleLevel;
                if (arg0->rumbleTimer >= 1.0f) {
                    arg0->rumbleTimer -= 1.0f;
osMotorStart
((void *)arg0->rumblePfs);
                    return;
                }
            }
((unsigned int (*)(void *))osMotorStop)
((void *)arg0->rumblePfs);
        }
    }
}

/* Per-controller rumble pak state. */

extern s32 osMotorInit(void *queue, void *pfs, s32 channel);

static __inline__ void ResetPadMotor(ControllerProfile *pad) {
    osRecvMesg(&gPakAccessQueue, NULL, 1);
    Controller_ReadAll();
    gPakQueueState = 2;
    pad->rumbleOn = 0;
    pad->rumbleLevel = 0;
    pad->rumbleTimer = 0;
    RumblePool_Init(pad->rumblePool, pad->rumbleNodes, 3);
    osMotorInit(&gSiMessageQueue, pad->rumblePfs, pad->port);
    osMotorStop(pad->rumblePfs);
    osMotorStop(pad->rumblePfs);
    if (osMotorInit(&gSiMessageQueue, pad->rumblePfs, pad->port) == 0) {
        pad->rumblePak = 1;
    }
    gPakQueueState = 2;
    osSendMesg(&gPakAccessQueue, NULL, 0);
}

void Controller_ResetAllRumble(void) {
    ControllerProfile *pad;
    s32 i;

    for (i = 0; i < 4; i++) {
        pad = &gControllers[i];
        if (gControllersInitialised != 0 && pad->connected != 0) {
            ResetPadMotor(pad);
        }
    }
}

void Controller_SetRumbleOptionAll(s32 arg0) {
    ControllerProfile *pad;
    s32 i;
    s32 slot;

    for (i = 0, slot = 0; i < 4; i++, slot++) {
        pad = &gControllers[slot];
        pad->rumbleOn = arg0;
        if (arg0 == 0 && gControllersInitialised != 0 && pad->connected != 0) {
            ResetPadMotor(pad);
        }
    }
    if (arg0 == 0) {
        RumblePool_FreeAll(&gRumblePool);
    }
}

/* What this reads of the controller state. */


s32 Cinema_IsActive(void);
void Sound__Play2D(s32);

/* Returns whether the buttons (START_BUTTON) were pressed; when they were and
 * gMenuSoundsEnabled is set and Cinema_IsActive returns 0, calls
 * Sound__Play2D(300). */
s32 Controller_StartPressedNoCinema(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0x1000)) {
        return 0;
    }
    if (gMenuSoundsEnabled == 0) {
        return 1;
    }
    if (!Cinema_IsActive()) {
        Sound__Play2D(300);
    }
    return 1;
}

/* Returns whether the buttons (START_BUTTON) were pressed; when they were and
 * gMenuSoundsEnabled is set, calls Sound__Play2D(300). */
s32 Controller_StartPressed(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0x1000)) {
        return 0;
    }
    if (gMenuSoundsEnabled == 0) {
        return 1;
    }
    Sound__Play2D(300);
    return 1;
}

/* Returns whether the buttons (A_BUTTON | B_BUTTON | START_BUTTON) were pressed; when they were and
 * gMenuSoundsEnabled is set, calls Sound__Play2D(300). */
s32 Controller_ConfirmPressed(ControllerProfile *pad) {
    if (!(pad->buttonsSingle & 0xD000)) {
        return 0;
    }
    if (gMenuSoundsEnabled == 0) {
        return 1;
    }
    Sound__Play2D(300);
    return 1;
}

/* ControllerProfile profile, stride 0x224. */

void Sound__Play2D(s32 sound);

s32 Controller_MenuLeft(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x20202) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}

s32 Controller_MenuRight(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x40101) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}

s32 Controller_MenuUp(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x10808) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}

s32 Controller_MenuDown(ControllerProfile *ctrl) {
    if (ctrl->buttonsRepeat & 0x80404) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12D);
        }
        return 1;
    }
    return 0;
}

s32 Controller_MenuBack(ControllerProfile *ctrl) {
    if (ctrl->buttonsSingle & 0x4000) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12C);
        }
        return 1;
    }
    return 0;
}

s32 Controller_MenuSelect(ControllerProfile *ctrl) {
    if (ctrl->buttonsSingle & 0x8000) {
        if (gMenuSoundsEnabled != 0) {
            Sound__Play2D(0x12C);
        }
        return 1;
    }
    return 0;
}
void Controller_Latch(ControllerProfile *arg0);

void Controller_LatchAll(void) {
    ControllerProfile *var_s0;
    s32 var_s1;

    if (gControllersInitialised != 0) {
        var_s1 = 0;
        var_s0 = gControllers;
        do {
            Controller_Latch(var_s0);
            var_s1 += 1;
            var_s0++;
        } while (var_s1 < 4);
        Controller_Latch(&gMergedController);
    }
}

/* Once a frame: copies what the reads gathered (the input* fields) into
 * the fields the game reads, and clears the gathered presses. */
void Controller_Latch(ControllerProfile *arg0) {
    arg0->stickX = arg0->inputStickX;
    arg0->stickY = arg0->inputStickY;
    arg0->buttons = arg0->inputButtons;
    arg0->buttonsSingle = arg0->inputButtonsSingle;
    arg0->buttonsSingleNoHold = arg0->inputButtonsSingleNoHold;
    arg0->buttonsDouble = arg0->inputButtonsDouble;
    arg0->buttonsRepeat = arg0->inputButtonsRepeat;
    arg0->inputButtonsSingle = 0;
    arg0->inputButtonsSingleNoHold = 0;
    arg0->inputButtonsDouble = 0;
    arg0->inputButtonsRepeat = 0;
}

s32 Controller_Lock(s32 arg0) {
    return osRecvMesg(&gPakAccessQueue, 0, arg0) == 0;
}

void Controller_Unlock(void) {
    osSendMesg(&gPakAccessQueue, 0, 0);
}

s32 Controller_TakePakChanged(s32 arg0) {
    s32 old;
    if (Controller_IsPakWatched(arg0) == 0) {
        return 0;
    }
    old = gPakChanged[arg0];
    gPakChanged[arg0] = 0;
    return old;
}

s32 Controller_PakChanged(s32 arg0) {
    if (Controller_IsPakWatched(arg0) == 0) {
        return 0;
    }
    return gPakChanged[arg0];
}

s32 Controller_IsConnected(s32 arg0) {
    return ((gContStatus[arg0].errno >> 3) ^ 1) & 1;
}

s32 Controller_PakInserted(s32 arg0) {
    return gContStatus[arg0].status & 1;
}

s32 Controller_AnyConnected(void) {
    s32 i = 0;
    while (1) {
        if (!(((gContStatus[i].errno >> 3) ^ 1) & 1)) {
            i += 1;
            if (i >= 4) {
                return 0;
            }
        } else {
            return 1;
        }
    }
}

#include "m2c_macros.h"

extern u8 gContStatus_errno[]; /* local: the field symbol; gContStatus[n].errno makes clang load gContStatus as an aligned vector on the host (see the names table) */

s32 Controller_CountConnected(void)
{
  s32 var_a0;
  s32 var_v0;
  s32 var_v1;
  var_a0 = 0;
  var_v1 = 0;
  var_v0 = 0 * 4;
  do
  {
    var_v0 = var_v1 * 4;
    if (((((u8) (*((u8 *) (gContStatus_errno + var_v0)))) >> 3) ^ 1) & 1)
    {
      var_a0 += 1;
    }
    var_v1 += 1;
  }
  while (var_v1 < 4);
  return var_a0;
}

s32 Controller_AnyPakWatched(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (gPakWatched[i] != 0) {
            return 1;
        }
    }
    return 0;
}

u8 Controller_IsPakWatched(s32 arg0) {
    return *(((u8 *)(gPakWatched + arg0)));
}

void Controller_WatchPak(s32 arg0) {
    gPakWatched[arg0] = 1;
}

void Controller_UnwatchPak(sptr arg0) {
    *(((u8 *)(gPakWatched + arg0))) = 0;
}

void Controller_WatchAllPaks(void)
{
  s32 var_v1;
  s8 *var_v0;
  int new_var;
  new_var = 1;
  var_v1 = 3;
  var_v0 = (s8 *) &gPakWatched[3];
  do
  {
    *var_v0 = new_var;
    var_v1 -= 1;
    var_v0 -= 1;
  }
  while (var_v1 >= 0);
}

void Controller_UnwatchAllPaks(void) {
    s32 var_v1;
    s8 *var_v0;

    var_v1 = 3;
    var_v0 = (s8 *) &gPakWatched[3];
    do {
        *var_v0 = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
}

/* ControllerProfile profile, stride 0x224. */

s32 osMotorInit(void *mq, void *pfs, s32 channel);

void Controller_ResetRumble(ControllerProfile *ctrl) {
    if (gControllersInitialised != 0 && ctrl->connected != 0) {
        osRecvMesg(&gPakAccessQueue, 0, 1);
        Controller_ReadAll();
        gPakQueueState = 2;
        ctrl->rumbleOn = 0;
        ctrl->rumbleLevel = 0;
        ctrl->rumbleTimer = 0;
        RumblePool_Init(ctrl->rumblePool, ctrl->rumbleNodes, 3);
        osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port);
        osMotorStop(ctrl->rumblePfs);
        osMotorStop(ctrl->rumblePfs);
        if (osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port) == 0) {
            ctrl->rumblePak = 1;
        }
        gPakQueueState = 2;
        osSendMesg((u8 *) &gPakQueueState + 8, 0, 0); /* same queue, addressed from the state byte */
    }
}

#include "protos/game.h"

/* Controller_ResetRumble, inlined here. */
static __inline__ void rumbleReset(ControllerProfile *ctrl) {
    if (gControllersInitialised != 0 && ctrl->connected != 0) {
        osRecvMesg(&gPakAccessQueue, 0, 1);
        Controller_ReadAll();
        gPakQueueState = 2;
        ctrl->rumbleOn = 0;
        ctrl->rumbleLevel = 0;
        ctrl->rumbleTimer = 0;
        RumblePool_Init(ctrl->rumblePool, ctrl->rumbleNodes, 3);
        osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port);
        osMotorStop(ctrl->rumblePfs);
        osMotorStop(ctrl->rumblePfs);
        if (osMotorInit(&gSiMessageQueue, ctrl->rumblePfs, ctrl->port) == 0) {
            ctrl->rumblePak = 1;
        }
        gPakQueueState = 2;
        osSendMesg((u8 *) &gPakQueueState + 8, 0, 0); /* same queue, addressed from the state byte */
    }
}

void Controller_SetRumbleOption(ControllerProfile *ctrl, s32 state) {
    ctrl->rumbleOn = state;
    if (state == 0) {
        rumbleReset(ctrl);
    }
}

void Controller_DisableMenuSounds(void) {
    gMenuSoundsEnabled = 0;
}

void Controller_EnableMenuSounds(void) {
    gMenuSoundsEnabled = 1;
}
