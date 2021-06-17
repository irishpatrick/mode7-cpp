#include "Screen.hpp"
#include "Shader.hpp"
#include "Util.hpp"
#include "Camera.hpp"

#include "gl.hpp"
#include <SDL.h>
#include <SDL_ttf.h>
#include <map>
#include <vector>
#include <iostream>

#define WMAX 1.f

static SDL_Window* window;
static SDL_GLContext ctx;
static int width;
static int height;

static mode7::FrameBuffer fb;
static std::map<std::string, GLint> fb_lookup;
static std::vector<mode7::FrameBuffer*> fb_pipeline;
static std::vector<GLint> fb_target;

static mode7::Shader screenShader;
static int RESX;
static int RESY;

/*static float screen_quad_data[] = {
    -1.0f, -1.0f,  0.f,  0.f,
     WMAX, -1.0f,  1.f,  0.f,
     WMAX,  WMAX,  1.f,  1.f,

     WMAX,  WMAX,  1.f,  1.f,
    -1.0f,  WMAX,  0.f,  1.f,
    -1.0f, -1.0f,  0.f,  0.f
};*/

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

    int err;

    err = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    if (err < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(1);
    }

    err = TTF_Init();
    if (err < 0)
    {
        std::cout << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
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
    const GLenum glerr = glewInit();
    if (glerr != GLEW_OK)
    {
        printf("glew init error: %s\n", glewGetErrorString(glerr));
    }

    //glEnable(GL_MULTISAMPLE);
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

void mode7::Screen::addFrameBuffer(const std::string& name, mode7::FrameBuffer* ofb)
{
    fb_target.push_back(
        glGetUniformLocation(fb.getScreenShader()->pid(), name.c_str()));
    //assert(fb_target.back() >= 0);
    fb_pipeline.push_back(ofb);
    fb_lookup[name] = fb_pipeline.size() - 1;
    assert(fb_pipeline.size() == fb_target.size());
    assert(fb_pipeline.size() == fb_lookup.size());
}

const std::vector<mode7::FrameBuffer*>& mode7::Screen::getPipeline()
{
    return fb_pipeline;
}

void mode7::Screen::runPipeline(void (*draw)(int32_t))
{
    int32_t i = 0;
    for (auto& e : fb_pipeline)
    {
        e->begin();
        draw(i);
        ++i;
    }

    fb.begin();
    draw(-1);
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    /*glBlitFramebuffer(
        0, 0, width, height,
        0, 0, width, height,
        GL_COLOR_BUFFER_BIT, GL_NEAREST
    );*/
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    fb.preDraw();
    for (uint32_t i = 0; i < fb_pipeline.size(); ++i)
    {
        fb.feedTex(fb_target[i], fb_pipeline[i]->getTexGL());
    }

    fb.draw();
    //fb_pipeline[0]->preDraw(); fb_pipeline[0]->draw();
    SDL_GL_SwapWindow(window);

    //flip();
}

void mode7::Screen::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

mode7::Shader& mode7::Screen::getShader()
{
    return screenShader;
}

void mode7::Screen::beginRender()
{
    //fb.begin();
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
    //fb_pipeline[0]->preDraw(); fb_pipeline[0]->draw();
    SDL_GL_SwapWindow(window);
}

void mode7::Screen::destroy()
{
    fb.destroy();
    SDL_DestroyWindow(window);
}
