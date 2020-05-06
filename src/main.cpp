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

SDL_Event e;
int running;
Shader spriteShader;
Shader shadowShader;

Scene skybox;
Tree tree;
Car car;
Scene track;

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

    tree.update();
    skybox.update();
    track.update();
    car.update();
    Camera::updateView();

    rltest.update(car.position);
    //std::cout << glm::to_string(rltest.getTarget()) << "\t" << glm::to_string(car.position) << std::endl;
    //std::cout << glm::to_string(car.position) << std::endl;
}

void draw()
{
    Screen::clear();

    skybox.draw(spriteShader);
    track.draw(spriteShader);
    tree.draw(spriteShader);
    car.draw(spriteShader);

    Screen::flip();
}

int main(int argc, char** argv)
{
    printf("hello world!\n");

    Screen::create(1024, 768);
    Camera::create(1024.0f, 768.0f, 80.0f, 0.1f, 1000.0f);
    Keyboard::attach();

    spriteShader.open(
        "assets/shaders/skybox_v.glsl", 
        "assets/shaders/skybox_f.glsl");

    shadowShader.open(
        "assets/shaders/shadow_v.glsl",
        "assets/shaders/shadow_f.glsl"
    );

    skybox = ModelLoader::open("assets/models/skybox.dae");
    skybox.scale = glm::vec3(500.0f);

    track = ModelLoader::open("assets/models/oval.dae");
    //track.scale = glm::vec3(2.0f);
    track.position.y = 1 * -1.1f;

    tree.init();
    tree.position.x = 3;
    tree.position.y = 2;
    tree.position.z = -5;
    tree.setAltShader(shadowShader);

    car.open("assets/cars/testCar.json");
    car.position.y = 1;
    car.position.x = 0;
    Camera::getObject().position = glm::vec3(0.0f, 2.0f, 5.0f);
    Camera::getObject().rotate(-atanf(2.0f / 12.0f), 0, 0);
    car.addChild(&Camera::getObject());
    car.setTracked(true);
    car.setAltShader(shadowShader);

    rltest.open("assets/track_data/test.json");

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

    return 0;
}
