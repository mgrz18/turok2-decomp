// t2controls.h - the port's controls for a human player (round 14 c4, docs/port/QUICKSTART.md):
// Turok 2's default mappings for LUS's ControlDeck, gamepad hot-plug, mouse look, and the
// headless controller test (--pad-test, docs/port/MACOS.md section 16).
#pragma once

#include <cstdint>
#include <memory>
#include <string>

namespace Ship {
class ControllerDefaultMappings;
class ConsoleVariable;
class Window;
} // namespace Ship

// The default mappings for a fresh config: a modern twin-stick FPS layout (left stick / WASD
// walk and strafe = the C buttons, right stick / arrow keys / mouse turn and look = the N64
// stick). LUS keeps the user's own mappings in turok2.cfg.json once they exist.
std::shared_ptr<Ship::ControllerDefaultMappings> T2Controls_DefaultMappings();

// A Ship::Window with no window system behind it (no SDL window, no GL): LUS's ControlDeck needs
// one to read gamepads and keys. Used by --pad-test; its mouse state comes from the test script.
std::shared_ptr<Ship::Window> T2Controls_MakeHeadlessWindow(std::shared_ptr<Ship::ConsoleVariable> cvars);

// The window the controls read (mouse look, mouse buttons). Null: none.
void T2Controls_SetWindow(std::shared_ptr<Ship::Window> window);

// Once after osContInit: the gamepads present at start (LUS only connects them through its
// add/remove GUI handler, which this port does not install).
void T2Controls_AfterContInit();

// Every controller read, before osContGetReadData: gamepads plugged in or out.
void T2Controls_PollHotplug();

// After osContGetReadData, port 1: the mouse (captured: look = stick, left = Z, right = R).
void T2Controls_Mouse(uint16_t* buttons, int8_t* sx, int8_t* sy);

// --pad-test FILE (debug): scripted SDL virtual gamepad / keyboard / mouse events, applied at
// their frame through the real LUS path. Returns false on a bad file.
bool T2PadTest_Load(const std::string& path);
bool T2PadTest_Active();
void T2PadTest_Apply(int frame); // after osContInit, before each read
