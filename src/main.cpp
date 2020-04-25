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

SDL_Event e;
int running;
Shader spriteShader;

Mesh skybox;
Tree tree;

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

    int up = Keyboard::isDown("up");
    int down = Keyboard::isDown("down");
    int left = Keyboard::isDown("left");
    int right = Keyboard::isDown("right");
    int fly = Keyboard::isDown("f");
    if (fly)
    {
        Camera::getObject().position -= 0.5f * glm::normalize(Camera::getObject().getFront());
        if (up)
        {
            Camera::getObject().rotation.x += 0.01f;
        }
        if (down)
        {
            Camera::getObject().rotation.x -= 0.01f;
        }
        if (left)
        {
            Camera::getObject().rotation.y += 0.01f;
        }
        if (right)
        {
            Camera::getObject().rotation.y -= 0.01f;
        }
    }

    tree.update();
    skybox.update();
    Track::update();
    Camera::update();
}

void draw()
{
    Screen::clear();

    Track::draw(spriteShader);
    tree.draw(spriteShader);
    skybox.draw(spriteShader);

    Screen::flip();
}

int main(int argc, char** argv)
{
    printf("hello world!\n");

    Screen::create(1024, 768);
    Camera::create(1024.0f, 768.0f, 60.0f, 0.01f, 2000.0f);
    Keyboard::attach();

    spriteShader.open(
        "assets/shaders/skybox_v.glsl", 
        "assets/shaders/skybox_f.glsl");

    skybox = ModelLoader::open("assets/models/skybox.obj");
    skybox.material.setDiffuseTexture(Texture::open("assets/textures/skybox.png"));
    skybox.scale = glm::vec3(1000.0f);
    skybox.position.z = -10;

    tree.init();
    tree.position.x = 3;
    tree.position.y = 1.0f;
    tree.position.z = -5;

    Track::open("nothing");

    Camera::getObject().position = glm::vec3(1.0f);

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
