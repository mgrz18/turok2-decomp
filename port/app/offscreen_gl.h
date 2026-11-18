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
// --offscreen-metal (macOS, round 18 g4): the same with LUS's Metal renderer (a CAMetalLayer in no
// window); T2Offscreen_Draw/Interpreter/Finish then use it. T2Offscreen_MetalRGB8: the last
// frame at 8 bits per channel, top row first (false when not running Metal).
bool T2Offscreen_InitMetal(const std::shared_ptr<Ship::ConsoleVariable>& cvars,
                           const std::shared_ptr<Ship::ResourceManager>& resources, uint32_t width, uint32_t height);
bool T2Offscreen_MetalRGB8(uint32_t w, uint32_t h, uint8_t* rgb);
// T2_SHOT at 8 bits per channel (round 17, f4): reads framebuffer `fbId` of the interpreter's
// OpenGL renderer (0 = the default framebuffer: the window's) as RGB8 into `rgb` (w * h * 3
// bytes, GL row order: bottom row first). LUS's ReadFramebufferToCPU returns RGBA5551, which
// drops the 3 low bits of every channel (shots ~3 levels darker than the N64 reference's 8-bit
// PNGs). Leaves the GL framebuffer bindings as it found them. False without a GL renderer.
bool T2Gl_ReadRGB8(const std::shared_ptr<Fast::Interpreter>& interp, int fbId, uint32_t w, uint32_t h, uint8_t* rgb);
