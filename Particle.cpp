/*
    QuadStrike Project

    Copyright (c) 2017 MrOnlineCoder (github.com/MrOnlineCoder or vk.com/mronlinecoder)

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "Particle.h"
#include "Constants.h"

#include <SFML/Graphics.hpp>

using namespace sf;

Particle::Particle(Color color, float x, float y, int mdir)
{
    spr.setSize(Vector2f(PARTICLE_SIZE, PARTICLE_SIZE));
    spr.setFillColor(color);
    spr.setPosition(x,y);
    dir = mdir;
    vel.y = PARTICLE_Y_MAX_ACCEL;
    finishedJump = false;
    finishedX = false;
}

Particle::Particle()
{

}


void Particle::update()
{
    if (finishedJump) {
        if (vel.y < PARTICLE_GRAVITY_MAX) vel.y += PARTICLE_GRAVITY;
        spr.move(0, vel.y);
    } else {
        if (vel.y > 0) {
            vel.y -= PARTICLE_ACCEL;
            spr.move(0, -vel.y);
        } else {
            finishedJump = true;
        }
    }

    if (!finishedX) {
        if (dir == 1) {
            vel.x -= PARTICLE_ACCEL;
            if (vel.x < -PARTICLE_X_MAX_ACCEL) finishedX = true;
        } else {
            vel.x += PARTICLE_ACCEL;
            if (vel.x > PARTICLE_X_MAX_ACCEL) finishedX = true;
        }
        spr.move(vel.x, 0);
    }

}
