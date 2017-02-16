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

#include "LocalPlayer.h"

#include <SFML/System.hpp>
#include <string>

#include "Constants.h"

using namespace std;
using namespace sf;

void LocalPlayer::update()
{
    if (!isXMoving) {
       if (vel.x > 0) {
            vel.x -= PLAYER_ACCEL;
            if (vel.x < 0) vel.x = 0.0f;
       } else if(vel.x < 0) {
            vel.x += PLAYER_ACCEL;
            if (vel.x > 0) vel.x = 0.0f;
       }
    }


    if (charge != CHARGE_MAX && chargeClock.getElapsedTime().asMilliseconds() > CHARGE_TIME/CHARGE_MAX) { //250 is alpha which will be applied to sprite
        charge++;
        spr.setColor(Color(255,255,255,charge));
        chargeClock.restart();
    }

    spr.move((float) vel.x, 0);
    if (spr.getPosition().x < 0) {
            vel.x = 0.0f;
            spr.setPosition(0, spr.getPosition().y);
    }
    if (spr.getPosition().x > WINDOW_WIDTH-spr.getGlobalBounds().width) {
            vel.x = 0.0f;
            spr.setPosition(WINDOW_WIDTH-spr.getGlobalBounds().width, spr.getPosition().y);
    }

    spr.setRotation(vel.x);
}
bool LocalPlayer::isCharged()
{
    return (charge >= CHARGE_MAX);
}

bool LocalPlayer::damage(int value)
{
    hp -= value;
    return (hp <= 0);
}



//OK! So how this works, because when I was developing it, I really FUCKED UP
//The dx represents the velocity. (dx alwatys >= 0)
//The dir represents the direction: 1 - right, -1 - left
//It increases dx every move only if it is smaller than const
void LocalPlayer::moveByX(int mdir)
{
    isXMoving = true;
    vel.x += PLAYER_ACCEL*mdir;
    if (mdir == 1) {
        if (vel.x > PLAYER_MAX_ACCELL) vel.x = PLAYER_MAX_ACCELL;
    } else {
        if (vel.x < -PLAYER_MAX_ACCELL) vel.x = -PLAYER_MAX_ACCELL;
    }
}
void LocalPlayer::shoot()
{
    charge = 128;
}




LocalPlayer::LocalPlayer(Texture& tex, string _name, float x, float y)
{
    spr.setTexture(tex);
    spr.setPosition(x, y);
    name = String(_name);
    hp = PLAYER_MAX_HEALTH;
    vel.x = 0.0f;
    vel.y = 0.0f;
    charge = CHARGE_MAX;
}

LocalPlayer::LocalPlayer()
{

}
