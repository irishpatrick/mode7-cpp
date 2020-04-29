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
#include "Car.hpp"

SDL_Event e;
int running;
Shader spriteShader;

Mesh skybox;
Tree tree;
Car car;
Mesh track;

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
    Track::update();
    track.update();
    car.update();
    Camera::update();
}

void draw()
{
    Screen::clear();

    //skybox.draw(spriteShader);
    track.draw(spriteShader);
    //Track::draw(spriteShader);
    tree.draw(spriteShader);
    car.draw(spriteShader);

    Screen::flip();
}

int main(int argc, char** argv)
{
    printf("hello world!\n");

    Screen::create(1024, 768);
    Camera::create(1024.0f, 768.0f, 80.0f, 0.01f, 5000.0f);
    Keyboard::attach();

    spriteShader.open(
        "assets/shaders/skybox_v.glsl", 
        "assets/shaders/skybox_f.glsl");

    skybox = ModelLoader::open("assets/models/skybox.obj");
    skybox.scale = glm::vec3(2000.0f);

    track = ModelLoader::open("assets/models/track.obj");
    track.scale *= 2;
    track.position.y = -1.0f;
    //track.material.setDiffuseTexture(Texture::open("assets/textures/road.png"));

    tree.init();
    tree.position.x = 3;
    tree.position.y = 2;
    tree.position.z = -5;

    Track::open("nothing");

    car.open("assets/cars/testCar.json");
    car.position.y = 1;
    car.position.x = 0;
    Camera::getObject().position = glm::vec3(0.0f, 2.0f, 5.0f);
    Camera::getObject().rotation.x = -atanf(2.0f / 12.0f);
    car.addChild(Camera::getObject());
    car.setTracked(true);

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
