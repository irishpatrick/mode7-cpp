#include <cstdio>
#include <SDL.h>
#include "Camera.hpp"
#include "Screen.hpp"
#include "Shader.hpp"
#include "Keyboard.hpp"
#include "Mesh.hpp"
#include "ModelLoader.hpp"
#include "Clock.hpp"
#include <iostream>
#include "Tree.hpp"
#include "Track.hpp"
#include "Texture.hpp"
#include "Scene.hpp"
#include "Car.hpp"
#include "RacingLine.hpp"
#include "AI.hpp"
#include "Collisions.hpp"
#include "HUD.hpp"

#define WIDTH 1280
#define HEIGHT 720

using namespace mode7;

SDL_Event e;
int running;
Shader spriteShader;
Shader shadowShader;

Scene skybox;
Tree tree;
Car car;
Scene track;
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

    tree.update();
    skybox.update();
    track.update();
    car.update();
    aitest.update();
    Camera::updateView();
    //rltest.update(car.position);

    cl.update();

    hud.update();
}

void draw()
{
    Screen::beginRender();

    skybox.draw(spriteShader);
    track.draw(spriteShader);
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
        "assets/shaders/skybox_v.glsl",
        "assets/shaders/skybox_f.glsl");

    shadowShader.open(
        "assets/shaders/shadow_v.glsl",
        "assets/shaders/shadow_f.glsl"
    );

    skybox = ModelLoader::open("assets/models/skybox.dae");
    skybox.scale = glm::vec3(500.f);
    track = ModelLoader::open("assets/models/oval2.dae");
    //track.scale = glm::vec3(2.0f);
    track.position.y = 1 * -1.1f;

    tree.init();
    tree.position.x = 3;
    tree.position.y = 2;
    tree.position.z = -5;
    tree.setAltShader(shadowShader);

    rltest.load("assets/track_data/oval2.line");

    car.open("assets/cars/testcar");
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
