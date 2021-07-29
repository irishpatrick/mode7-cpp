/**
 * This file is part of mode7-cpp.
 * Copyright (C) 2021  Patrick Roche
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#include "Tree.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "TexCache.hpp"

namespace mode7
{

Tree::Tree() :
    Mesh()
{

}

Tree::~Tree()
{

}

void Tree::init()
{
    shadow.create();
    shadow.apply(*this);

    // set texture
    material.addMap(TexCache::open("assets/textures/tree.png", TexType::DIFFUSE));
    Mesh::createFromShape(Mesh::PLANE);
    scale *= 2;
}

void Tree::update()
{
    // set rotation to camera
    rx = Camera::getObject().getWorldRx();
    ry = Camera::getObject().getWorldRy();

    Object::update();
}

void Tree::draw(Shader& s)
{
    shadow.draw(*alt);

    Mesh::draw(s);
}

}
