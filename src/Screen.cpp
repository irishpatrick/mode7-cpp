#include "Screen.hpp"
#include "Shader.hpp"
#include "Util.hpp"
#include "FrameBuffer.hpp"

#include "gl.hpp"
#include <SDL.h>
#include <SDL_ttf.h>

#define WMAX 1.f

static SDL_Window* window;
static SDL_GLContext ctx;
static int width;
static int height;

static mode7::FrameBuffer fb;

static mode7::Shader screenShader;
static int RESX;
static int RESY;

static float screen_quad_data[] = {
    -1.0f, -1.0f,  0.f,  0.f,
     WMAX, -1.0f,  1.f,  0.f,
     WMAX,  WMAX,  1.f,  1.f,

     WMAX,  WMAX,  1.f,  1.f,
    -1.0f,  WMAX,  0.f,  1.f,
    -1.0f, -1.0f,  0.f,  0.f
};

void mode7::Screen::create(int w, int h, bool fullscreen)
{
    auto pair = Util::getMonitorRes();
    RESX = w;
    RESY = h;
    if (fullscreen)
    { 
        width = pair.first;
        height = pair.second;
    }
    else
    {
        int sc = Util::getMonitorScale();
        std::cout << sc << std::endl;
        width = w * sc;
        height = h * sc;
    }

    TTF_Init();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetSwapInterval(0);

    unsigned int flags = 0;
    if (fullscreen)
    {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    window = SDL_CreateWindow(
        "title",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        width, height,
        SDL_WINDOW_OPENGL | flags
    );
    ctx = SDL_GL_CreateContext(window);

    glewExperimental = GL_TRUE;
    const GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        printf("glew init error: %s\n", glewGetErrorString(err));
    }

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.7f, 0.7f, 0.8f, 1.0f);

    // load screen shader
    screenShader.open(
        "assets/shaders/screen_v.glsl",
        "assets/shaders/screen_f.glsl"
    );

    fb.init(RESX, RESY);
    fb.setShader(&screenShader);
}

void mode7::Screen::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void mode7::Screen::beginRender()
{
    fb.begin();
}

void mode7::Screen::flip()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    /*glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );*/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    fb.draw();
    SDL_GL_SwapWindow(window);
}

void mode7::Screen::destroy()
{
    fb.destroy();
    SDL_DestroyWindow(window);
}
