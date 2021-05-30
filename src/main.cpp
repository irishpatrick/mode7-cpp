#include "Camera.hpp"
#include "Screen.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include "Mesh.hpp"
#include "ModelLoader.hpp"
#include "Clock.hpp"
#include "Tree.hpp"
#include "Track.hpp"
#include "Texture.hpp"
#include "Scene.hpp"
#include "Car.hpp"
#include "RacingLine.hpp"
#include "AI.hpp"
#include "Collisions.hpp"
#include "HUD.hpp"

#include <cstdio>
#include <SDL.h>
#include <iostream>

#define WIDTH 1280
#define HEIGHT 720

using namespace mode7;

SDL_Event e;
int running;
Shader spriteShader;
Shader shadowShader;
Shader skyboxShader;

Scene skybox;
Tree tree;
Car car;
//Scene track;
Track track;
AI aitest;
Collisions cl;
RacingLine rltest;
HUD hud;

void update()
{
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            running = 0;
        }
    }

    Keyboard::poll();

    int esc = Keyboard::isDown("escape");

    if (esc)
    {
        running = 0;
    }

    car.input();
    skybox.position = car.position;

    car.update();
    skybox.update();
    //aitest.update();
    Camera::updateView();
    //rltest.update(car.position);

    cl.update();

    hud.update();
}

void draw()
{
    Screen::beginRender();

    skybox.draw(skyboxShader);
    track.getScene()->draw(spriteShader);
    tree.draw(spriteShader);
    aitest.draw(spriteShader);
    car.draw(spriteShader);
    rltest.getDebugPath()->draw();
    hud.draw();

    Screen::flip();
}

int main(int argc, char** argv)
{
    Screen::create(WIDTH, HEIGHT, false);
    Camera::create(WIDTH, HEIGHT, 70.f, 1.0f, 1500.f);

    Keyboard::attach();

    spriteShader.open(
        "assets/shaders/sprite_v.glsl",
        "assets/shaders/sprite_f.glsl");

    shadowShader.open(
        "assets/shaders/shadow_v.glsl",
        "assets/shaders/shadow_f.glsl"
    );
    skyboxShader.open(
        "assets/shaders/skybox_v.glsl",
        "assets/shaders/skybox_f.glsl"
    );

    skybox = ModelLoader::open("assets/models/skybox.dae");
    skybox.scale = glm::vec3(1200.f);
    //skybox.getMesh(0)->visible = false;
    //track = ModelLoader::open("assets/models/oval2.dae");
    //track = ModelLoader::open("assets/models/oval_small.dae");
    //track = ModelLoader::open("assets/blender/newtrack.dae");
    //track = ModelLoader::open("assets/track_data/track1.dae");
    track.open("assets/track_data/track1.dae");
    track.attachData("assets/track_data/track1.obj.tdat");
    track.getScene()->scale = glm::vec3(20.f);
    track.getScene()->position.y = track.getScene()->scale.y - track.getScene()->scale.y * 0.125;
    //track.position.y = -track.scale.y;
    //track.position.y = -track.scale.y - 4.f;
    //track.scale = glm::vec3(2.0f);
    //track.position.y = 1 * -1.1f;

    tree.init();
    tree.position.x = 3;
    tree.position.y = 2;
    tree.position.z = -5;
    tree.setAltShader(shadowShader);

    rltest.load("assets/track_data/oval2.line");

    car.open("assets/cars/testcar");
    car.openMaps(
        "assets/cars/newcar/accelmap.csv",
        "assets/cars/newcar/brakemap.csv",
        "assets/cars/newcar/driftmap.csv",
        "assets/cars/newcar/turnmap.csv"
    );
    car.parseConstants("assets/cars/newcar/constants.json");

    car.position.y = 1;
    car.position.x = 0;
    Camera::getObject().position = glm::vec3(0.0f, 2.0f, 5.0f);
    Camera::getObject().rotate(-atanf(2.0f / 12.0f), 0, 0);
    car.addChild(&Camera::getObject());
    car.setTracked(true);
    car.setAltShader(shadowShader);
    car.rotate(0.f, -M_PI/2.f, 0.f);
    car.setRacingLine(&rltest);

    hud.setCar(&car);
    hud.init();

    aitest.open("assets/cars/testcar");
    aitest.position.y = 1;
    aitest.position.x = 1;
    //aitest.addChild(&Camera::getObject());
    aitest.setAltShader(shadowShader);
    aitest.setRacingLine(&rltest);
    //aitest.setTracked(true);
    aitest.rotate(0.f, -M_PI/2.f + 0.03f, 0.f);
    aitest.update();

    cl.addObject(&car, 2.f);
    cl.addObject(&tree, 0.f);

    // only need to update once
    tree.update();
    skybox.update();
    track.getScene()->update();

    running = 1;
    Clock::start();
    while (running)
    {
        draw();
        Clock::tick();
        while (Clock::lagging())
        {
            update();
            Clock::lagTick();
        }
    }

    Screen::destroy();
    SDL_Quit();

    return 0;
}
