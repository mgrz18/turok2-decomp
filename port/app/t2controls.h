// t2controls.h - the port's controls for a human player (round 14 c4, docs/port/QUICKSTART.md):
// Turok 2's default mappings for LUS's ControlDeck, gamepad hot-plug, mouse look, and the
// headless controller test (--pad-test, docs/port/MACOS.md section 16).
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace Ship {
class ControllerDefaultMappings;
class Config;
class ConsoleVariable;
class Window;
} // namespace Ship

// The default mappings for a fresh config: a modern twin-stick FPS layout (left stick / WASD
// walk and strafe = the C buttons, right stick / arrow keys / mouse turn and look = the N64
// stick). LUS keeps the user's own mappings in turok2.cfg.json once they exist.
std::shared_ptr<Ship::ControllerDefaultMappings> T2Controls_DefaultMappings();

// A Ship::Window with no window system behind it (no SDL window, no GL): LUS's ControlDeck needs
// one to read gamepads and keys. Used by --pad-test; its mouse state comes from the test script.
// Its geometry (size, position, fullscreen) starts from the config's Window.* keys and is saved
// back by LUS at exit when it is a Context child (the config round-trip check, MACOS.md 17).
std::shared_ptr<Ship::Window> T2Controls_MakeHeadlessWindow(std::shared_ptr<Ship::ConsoleVariable> cvars,
                                                            std::shared_ptr<Ship::Config> config);

// The window the controls read (mouse look, mouse buttons). Null: none.
void T2Controls_SetWindow(std::shared_ptr<Ship::Window> window);

// Once after osContInit: the gamepads present at start (LUS only connects them through its
// add/remove GUI handler, which this port does not install).
void T2Controls_AfterContInit();

// Every controller read, before osContGetReadData: gamepads plugged in or out.
void T2Controls_PollHotplug();

// The player's settings (round 15 d4), CVars in turok2.cfg.json: gT2MouseSensitivity (2.0 stick
// units per pixel), gT2MouseInvertY (0), gT2MouseDeadZone (3), gT2StickInvertY (0: the pad's
// look in a level), gT2ShowControlsAtStart (1: the controls table in the log at start; and, with
// firstLaunchBox (play mode, a real window), the F1 box once at the first launch, remembered as
// gT2ControlsShown 1). Registers them (save: and writes the config) and prints them.
void T2Controls_RegisterSettings(bool save, bool firstLaunchBox = false);

// Every controller read, port 1, before the mouse: gT2StickInvertY (inLevel: engine mode 11).
void T2Controls_Look(int8_t* sy, bool inLevel);
// The window has the keyboard focus (always true without a window; --pad-test: "focus 0|1").
// On a loss the mouse is released; the caller gives the game no input while it returns false.
bool T2Controls_InputFocus();

// Every controller read: F1 prints the controls (and shows them in a message box with a window).
void T2Controls_Hotkeys();

// After osContGetReadData, port 1: the mouse (captured: look = stick, left = Z, right = R).
void T2Controls_Mouse(uint16_t* buttons, int8_t* sx, int8_t* sy);

// --pad-test FILE (debug): scripted SDL virtual gamepad / keyboard / mouse events, applied at
// their frame through the real LUS path. Returns false on a bad file.
bool T2PadTest_Load(const std::string& path);
bool T2PadTest_Active();
void T2PadTest_Apply(int frame); // after osContInit, before each read
