// offscreen_gl.cpp - `--offscreen-gl` (macOS): Fast3D's OpenGL renderer without any window.
//
// SDL's offscreen video driver has no GL and no Metal on macOS (no EGL), and the user's Mac must
// not get a window, visible or hidden (MACOS.md section 8). Core OpenGL (CGL) can create a
// context with no drawable at all: no window, no NSApplication, no window-server surface. Every
// draw then has to go to framebuffer objects, which is what Fast3D does on macOS anyway
// (Interpreter::ViewportMatchesRendererResolution is false there, so the game frame is rendered
// into the interpreter's game framebuffer and only copied to framebuffer 0 by the GUI). This file
// builds that interpreter by hand, as Fast3dWindow::OnInit does, but with:
//   - a GfxWindowBackend that has no window (fixed size, no events, a flush for the swap),
//   - LUS's GfxRenderingAPIOGL on the CGL context,
// and runs StartFrame / Run / EndFrame per frame without the GUI. T2_SHOT reads the game
// framebuffer back. Framebuffer 0 has no storage here: the few clears Fast3D sends to it are
// GL errors that change nothing.
#include "offscreen_gl.h"

#include <cstdio>
#include <unordered_map>

#ifdef __APPLE__
#include "fast/backends/gfx_opengl.h" // GL/glew.h first
#include "fast/backends/gfx_window_manager_api.h"
#include "fast/debug/GfxDebugger.h"
#include "fast/interpreter.h"
#include <OpenGL/OpenGL.h>
#include <chrono>

namespace Fast {
void GfxSetInstance(std::shared_ptr<Interpreter> gfx); // interpreter.cpp (no header declares it)
}

namespace {

class NoWindowBackend final : public Fast::GfxWindowBackend {
  public:
    NoWindowBackend(uint32_t w, uint32_t h) : mWidth(w), mHeight(h) {
    }
    void Init(const char*, const char*, bool, uint32_t, uint32_t, int32_t, int32_t) override {
    }
    void Close() override {
        mIsRunning = false;
    }
    void SetKeyboardCallbacks(bool (*)(int), bool (*)(int), void (*)()) override {
    }
    void SetMouseCallbacks(bool (*)(int), bool (*)(int)) override {
    }
    void SetFullscreenChangedCallback(void (*)(bool)) override {
    }
    void SetFullscreen(bool) override {
    }
    void GetActiveWindowRefreshRate(uint32_t* refreshRate) override {
        *refreshRate = 60;
    }
    void SetCursorVisibility(bool) override {
    }
    void SetMousePos(int32_t, int32_t) override {
    }
    void GetMousePos(int32_t* x, int32_t* y) override {
        *x = *y = 0;
    }
    void GetMouseDelta(int32_t* x, int32_t* y) override {
        *x = *y = 0;
    }
    void GetMouseWheel(float* x, float* y) override {
        *x = *y = 0.0f;
    }
    bool GetMouseState(uint32_t) override {
        return false;
    }
    void SetMouseCapture(bool) override {
    }
    bool IsMouseCaptured() override {
        return false;
    }
    void GetDimensions(uint32_t* width, uint32_t* height, int32_t* posX, int32_t* posY) override {
        *width = mWidth;
        *height = mHeight;
        *posX = *posY = 0;
    }
    void SetDimensions(uint32_t width, uint32_t height, int32_t, int32_t) override {
        mWidth = width;
        mHeight = height;
    }
    Ship::WindowRect GetPrimaryMonitorRect() override {
        return { 0, 0, (int32_t)mWidth, (int32_t)mHeight };
    }
    void HandleEvents() override {
    }
    bool IsFrameReady() override {
        return true;
    }
    void SwapBuffersBegin() override {
        glFlush();
    }
    void SwapBuffersEnd() override {
    }
    double GetTime() override {
        return std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }
    int GetTargetFps() override {
        return (int)mTargetFps;
    }
    void SetTargetFps(int fps) override {
        mTargetFps = fps;
    }
    void SetMaxFrameLatency(int) override {
    }
    const char* GetKeyName(int) override {
        return "";
    }
    bool CanDisableVsync() override {
        return true;
    }
    bool IsRunning() override {
        return mIsRunning;
    }
    void Destroy() override {
    }
    bool IsFullscreen() override {
        return false;
    }

  private:
    uint32_t mWidth, mHeight;
};

CGLContextObj sContext = nullptr;
NoWindowBackend* sWapi = nullptr;
Fast::GfxRenderingAPIOGL* sRapi = nullptr;
std::shared_ptr<Fast::Interpreter> sInterp;

} // namespace

bool T2Offscreen_Init(const std::shared_ptr<Ship::ConsoleVariable>& cvars,
                      const std::shared_ptr<Ship::ResourceManager>& resources, uint32_t width, uint32_t height) {
    // A 3.2 core profile request gets the newest core context (4.1 on Apple Silicon, the GLSL
    // 410 that LUS's macOS shaders ask for). No kCGLPFADrawable-type attribute: no window.
    const CGLPixelFormatAttribute attrs[] = {
        kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
        kCGLPFAColorSize,     (CGLPixelFormatAttribute)24,
        kCGLPFAAlphaSize,     (CGLPixelFormatAttribute)8,
        kCGLPFADepthSize,     (CGLPixelFormatAttribute)24,
        kCGLPFAAccelerated,   kCGLPFAAllowOfflineRenderers,
        (CGLPixelFormatAttribute)0,
    };
    CGLPixelFormatObj pix = nullptr;
    GLint npix = 0;
    CGLError err = CGLChoosePixelFormat(attrs, &pix, &npix);
    if (err != kCGLNoError || pix == nullptr) {
        fprintf(stderr, "offscreen-gl: CGLChoosePixelFormat: %s\n", CGLErrorString(err));
        return false;
    }
    err = CGLCreateContext(pix, nullptr, &sContext);
    CGLReleasePixelFormat(pix);
    if (err != kCGLNoError || sContext == nullptr) {
        fprintf(stderr, "offscreen-gl: CGLCreateContext: %s\n", CGLErrorString(err));
        return false;
    }
    CGLSetCurrentContext(sContext);
    printf("offscreen-gl: CGL context without a drawable: %s, %s\n", (const char*)glGetString(GL_RENDERER),
           (const char*)glGetString(GL_VERSION));

    sWapi = new NoWindowBackend(width, height);
    sRapi = new Fast::GfxRenderingAPIOGL(cvars, resources);
    sInterp = std::make_shared<Fast::Interpreter>();
    Fast::GfxSetInstance(sInterp);
    // The debugger is a Ship::Component whose destructor terminates outside a component tree,
    // and the interpreter has no clean teardown without its window: both live to the exit.
    auto debugger = std::make_shared<Fast::GfxDebugger>();
    new std::shared_ptr<Fast::GfxDebugger>(debugger);
    new std::shared_ptr<Fast::Interpreter>(sInterp);
    sInterp->SetGfxDebugger(debugger);
    sInterp->Init(sWapi, sRapi, "Turok2", false, width, height, 0, 0, cvars, resources);
    sRapi->SetTextureFilter(Fast::FILTER_THREE_POINT);
    return true;
}

std::shared_ptr<Fast::Interpreter> T2Offscreen_Interpreter() {
    return sInterp;
}

bool T2Offscreen_Draw(void* gfx) {
    if (!sInterp) {
        return false;
    }
    sInterp->StartFrame();
    sInterp->Run(static_cast<Gfx*>(gfx), {});
    sInterp->EndFrame();
    return true;
}

int T2Offscreen_ShotFramebuffer() {
    return sInterp ? sInterp->mGameFb : 0;
}

void T2Offscreen_Finish() {
    if (sInterp) {
        glFinish();
    }
}

#else // !__APPLE__: Linux has SDL's offscreen driver with EGL; nothing to do here

bool T2Offscreen_Init(const std::shared_ptr<Ship::ConsoleVariable>&, const std::shared_ptr<Ship::ResourceManager>&,
                      uint32_t, uint32_t) {
    fprintf(stderr, "--offscreen-gl: macOS only (Linux: SDL_VIDEODRIVER=offscreen without --no-window)\n");
    return false;
}
std::shared_ptr<Fast::Interpreter> T2Offscreen_Interpreter() {
    return nullptr;
}
bool T2Offscreen_Draw(void*) {
    return false;
}
int T2Offscreen_ShotFramebuffer() {
    return 0;
}
void T2Offscreen_Finish() {
}

#endif

// ---- T2_SHOT at 8 bits per channel (both platforms; offscreen_gl.h) ----
#ifdef ENABLE_OPENGL
#include "fast/backends/gfx_opengl.h"
#include "fast/interpreter.h"
#include <cstring>
#include <vector>

bool T2Gl_ReadRGB8(const std::shared_ptr<Fast::Interpreter>& interp, int fbId, uint32_t w, uint32_t h, uint8_t* rgb) {
    // the OpenGL renderer only (a Mac window may run LUS's Metal one: no GL context then)
    if (!interp || !interp->mRapi || strcmp(interp->mRapi->GetName(), "OpenGL") != 0) {
        static bool told = false;
        if (!told) {
            told = true;
            fprintf(stderr, "T2_SHOT: renderer %s, the RGBA5551 readback\n",
                    interp && interp->mRapi ? interp->mRapi->GetName() : "(none)");
        }
        return false;
    }
    GLint prevRead = 0;
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &prevRead);
    static GLuint sReadFbo = 0; // a framebuffer of our own around the game framebuffer's colour texture
    if (fbId == 0) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
    } else {
        const GLuint tex = (GLuint)(uintptr_t)interp->mRapi->GetFramebufferTextureId(fbId);
        if (sReadFbo == 0) {
            glGenFramebuffers(1, &sReadFbo);
        }
        glBindFramebuffer(GL_READ_FRAMEBUFFER, sReadFbo);
        glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
        if (glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)prevRead);
            return false;
        }
    }
    static bool toldBits = false;
    if (!toldBits) { // once: the colour depth the frame was drawn at (Linux's offscreen EGL: 5-6-5)
        toldBits = true;
        GLint r = 0, g = 0, b = 0;
        const GLenum att = fbId == 0 ? GL_BACK_LEFT : GL_COLOR_ATTACHMENT0;
        glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, att, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &r);
        glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, att, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &g);
        glGetFramebufferAttachmentParameteriv(GL_READ_FRAMEBUFFER, att, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &b);
        printf("game: T2_SHOT reads framebuffer %d: colour bits %d-%d-%d\n", fbId, r, g, b);
    }
    std::vector<uint8_t> rgba((size_t)w * h * 4);
    glReadPixels(0, 0, (GLsizei)w, (GLsizei)h, GL_RGBA, GL_UNSIGNED_BYTE, rgba.data());
    glBindFramebuffer(GL_READ_FRAMEBUFFER, (GLuint)prevRead);
    for (size_t i = 0; i < (size_t)w * h; i++) {
        rgb[i * 3 + 0] = rgba[i * 4 + 0];
        rgb[i * 3 + 1] = rgba[i * 4 + 1];
        rgb[i * 3 + 2] = rgba[i * 4 + 2];
    }
    return true;
}
#else
bool T2Gl_ReadRGB8(const std::shared_ptr<Fast::Interpreter>&, int, uint32_t, uint32_t, uint8_t*) {
    return false;
}
#endif
