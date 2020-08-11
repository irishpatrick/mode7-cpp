#include <stdio.h>
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

#define WIDTH 1920
#define HEIGHT 1080

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

    int left = Keyboard::isDown("left");
    int right = Keyboard::isDown("right");

    if (Keyboard::isDown("c"))
    {
        car.gas();
    }
    if (Keyboard::isDown("x"))
    {
        car.brake();
    }
    if (left)
    {
        car.turnLeft();
    }
    if (right)
    {
        car.turnRight();
    }

    skybox.position = car.position;

    tree.update();
    skybox.update();
    track.update();
    car.update();
    aitest.update();
    Camera::updateView();
    //rltest.update(car.position);

    cl.update();
    //std::cout << glm::to_string(rltest.getTarget()) << "\t" << glm::to_string(car.position) << std::endl;
    //std::cout << glm::to_string(car.position) << std::endl;
}

void draw()
{
    Screen::clear();

    skybox.draw(spriteShader);
    track.draw(spriteShader);
    tree.draw(spriteShader);
    aitest.draw(spriteShader);
    car.draw(spriteShader);

    Screen::flip();
}

int main(int argc, char** argv)
{
    printf("hello world!\n");

    //Screen::create(3840, 2160);
    //Camera::create(3840.f, 2160.f, 80.0f, 0.1f, 1000.0f);
    //Screen::create(1920, 1080);
    //Camera::create(1920.f, 1080.f, 80.f, 0.1f, 1000.f);
    //Screen::create(1600, 900);
    //Camera::create(1600.f, 900.f, 80.f, 1.0f, 2000.f);
    Screen::create(WIDTH, HEIGHT);
    Camera::create(WIDTH, HEIGHT, 70.f, 1.0f, 1500.f);
    //Screen::create(1720 * 2, 968 * 2);
    //Camera::create(1720.f * 2, 968.f * 2, 70.f, 1.0f, 2000.f);

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

    rltest.load("assets/track_data/oval.line");

    car.open("assets/cars/testCar.json");
    car.position.y = 1;
    car.position.x = 0;
    Camera::getObject().position = glm::vec3(0.0f, 2.0f, 5.0f);
    Camera::getObject().rotate(-atanf(2.0f / 12.0f), 0, 0);
    car.addChild(&Camera::getObject());
    car.setTracked(true);
    car.setAltShader(shadowShader);

    aitest.open("assets/cars/testCar.json");
    aitest.position.y = 1;
    aitest.position.x = 1;
    //aitest.addChild(&Camera::getObject());
    aitest.setAltShader(shadowShader);
    aitest.setRacingLine(&rltest);
    //aitest.setTracked(true);
    aitest.setTracked(false);
    aitest.update();

    cl.addObject(&car, 2.f);
    cl.addObject(&tree, 0.f);

    running = 1;
    Clock::start();
    while (running)
    {
        Clock::tick();
        while (Clock::lagging())
        {
            update();
            Clock::lagTick();
        }
        draw();
    }

    Screen::destroy();
    SDL_Quit();

    return 0;
}
