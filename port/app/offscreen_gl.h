// offscreen_gl.h - `--offscreen-gl` (macOS): Fast3D's OpenGL renderer without any window
// (docs/port/MACOS.md section 8).
#pragma once

#include <cstdint>
#include <memory>

namespace Ship {
class ConsoleVariable;
class ResourceManager;
} // namespace Ship
namespace Fast {
class Interpreter;
}

// Creates a CGL context with no drawable (no window, no NSApplication) and a Fast3D interpreter
// on LUS's OpenGL renderer over it, with a window backend that has no window. Returns false (with
// a message) when this host cannot do it (not macOS, or no OpenGL renderer).
bool T2Offscreen_Init(const std::shared_ptr<Ship::ConsoleVariable>& cvars,
                      const std::shared_ptr<Ship::ResourceManager>& resources, uint32_t width, uint32_t height);
// The interpreter T2Offscreen_Init made (null without one).
std::shared_ptr<Fast::Interpreter> T2Offscreen_Interpreter();
// Runs one game display list: StartFrame, Run, EndFrame (the parts of
// Fast3dWindow::DrawAndRunGraphicsCommands that need no GUI).
bool T2Offscreen_Draw(void* gfx);
// The framebuffer that holds the game frame (the interpreter's game framebuffer: there is no
// default framebuffer without a drawable).
int T2Offscreen_ShotFramebuffer();
// --bench: wait until the GL commands of the frame have finished on the GPU (glFinish), so the
// draw time includes the GPU work (no-op without a context).
void T2Offscreen_Finish();
