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

#include "Screen.h"
#include "Logger.h"
#include "UIButton.h"
#include "Colors.h"


#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "LocalPlayer.h"
#include "Particle.h"
#include <vector>
#include <random>

using namespace sf;
using namespace std;



class PlayScreen: public Screen {
    public:
        int run(RenderWindow& window);
        void init();
        void shootLaser(int player);
        void boomParticles(float x, float y, Color c);
        void endGame(int player);
        PlayScreen(Logger* glogger);
        Logger* logger;

        Texture redTex;
        Texture blueTex;
        Texture rlaserTex;
        Texture blaserTex;
        Texture particleTex;

        Sprite redLaser;
        Sprite blueLaser;

        LocalPlayer red;
        LocalPlayer blue;

        SoundBuffer shotBuff;
        Sound shotSnd;

        SoundBuffer winBuff;
        Sound winSnd;

        RectangleShape line;

        Text infoText;

        Music bg;

        Font f;
        Text winText;
        bool isGameEnded;
        vector<Particle> particles;
};




void PlayScreen::boomParticles(float x, float y, Color c)
{
    srand(time(0));
    for (int i=0;i<PARTICLES;i++) {
        int rx = rand() % 30 + PARTICLE_SIZE;
        int ry = rand() % 30 + PARTICLE_SIZE;
        Particle particle(c, x+rx, y+ry, -1);
        particles.push_back(particle);
    }

    for (int i=0;i<PARTICLES;i++) {
        int rx = rand() % 30 + PARTICLE_SIZE;
        int ry = rand() % 30 + PARTICLE_SIZE;
        Particle particle(c, x+rx, y+ry, 1);
        particles.push_back(particle);
    }
}

void PlayScreen::endGame(int player)
{
    Color winColor;
    string winStr;

    if (player == 0) {
        winColor = COLOR_RED_PLAYER;
        winStr = "Red wins";
        blue.spr.setColor(Color(255,255,255,0));
    }

    if (player == 1) {
        winColor = COLOR_BLUE_PLAYER;
        winStr = "Blue wins";
        red.spr.setColor(Color(255,255,255,0));
    }
    isGameEnded = true;
    bg.setVolume(25);
    winSnd.play();
    winText.setString(winStr);
    winText.setPosition(WINDOW_WIDTH/2-winText.getGlobalBounds().width/2, WINDOW_HEIGHT/2-winText.getGlobalBounds().height/2);
    winText.setColor(winColor);
    logger->log("GG WP! "+winStr);
}


void PlayScreen::shootLaser(int player)
{
    if (player == 1) {
        if (red.isCharged()) {
            shotSnd.play();
            red.shoot();
            redLaser.setPosition(red.spr.getPosition().x+red.spr.getGlobalBounds().width/2, red.spr.getPosition().y-rlaserTex.getSize().y+26);
            redLaser.setColor(Color(255,255,255,255));
            redLaser.setRotation(red.spr.getRotation());
            if (redLaser.getGlobalBounds().intersects(blue.spr.getGlobalBounds())) {
                boomParticles(blue.spr.getPosition().x+blue.spr.getGlobalBounds().width/2, blue.spr.getPosition().y+blue.spr.getGlobalBounds().height/2, COLOR_BLUE_PLAYER);
                if (blue.damage(1)) {
                    endGame(0);
                }
                logger->log("Blue player was damaged.");
            }
        }
    }

    if (player == 2) {
        if (blue.isCharged()) {
            shotSnd.play();
            blue.shoot();
            blueLaser.setPosition(blue.spr.getPosition().x+blue.spr.getGlobalBounds().width/2, blue.spr.getPosition().y+blue.spr.getGlobalBounds().height-15);
            blueLaser.setColor(Color(255,255,255,255));
            blueLaser.setRotation(blue.spr.getRotation());
            if (blueLaser.getGlobalBounds().intersects(red.spr.getGlobalBounds())) {
                boomParticles(red.spr.getPosition().x+red.spr.getGlobalBounds().width/2, red.spr.getPosition().y+red.spr.getGlobalBounds().height/2, COLOR_RED_PLAYER);
                if (red.damage(1)) {
                    endGame(1);
                }
                logger->log("Red player was damaged.");
            }
        }
    }
}



int PlayScreen::run(RenderWindow& window)
{
    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
            return -1;
        }

        if (e.type == Event::KeyReleased) {
            if (e.key.code == Keyboard::A || e.key.code == Keyboard::D ) {
                red.isXMoving = false;
                continue;
            }

            if (e.key.code == Keyboard::Left || e.key.code == Keyboard::Right ) {
                blue.isXMoving = false;
                continue;
            }

            if (e.key.code == Keyboard::Q) {
                shootLaser(1);
            }

            if (e.key.code == Keyboard::Return) {
                shootLaser(2);
            }

            if (e.key.code == Keyboard::R && isGameEnded) {
                logger->log("Reinitalizing PlayScreen...");
                init();
                return 1;
            }

            if (e.key.code == Keyboard::Escape && isGameEnded) {
                logger->log("Returning to main menu...");
                bg.stop();
                return 0;
            }
        }

    }

    //Input
    if (!isGameEnded) {
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            red.moveByX(-1);
        }

        if (Keyboard::isKeyPressed(Keyboard::D)) {
            red.moveByX(1);
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            blue.moveByX(-1);
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            blue.moveByX(1);
        }
    }


    //Logic
    if (!isGameEnded) red.update();
    if (!isGameEnded) blue.update();
    if (redLaser.getColor().a > 0) redLaser.setColor(Color(255,255,255,redLaser.getColor().a-5));
    if (blueLaser.getColor().a > 0) blueLaser.setColor(Color(255,255,255,blueLaser.getColor().a-5));

    //Render
    window.clear(COLOR_GAME_BACKGROUND);
    if (!isGameEnded) window.draw(line);
    for (int i=0;i<particles.size();i++) {
        particles[i].update();
        window.draw(particles[i].spr);
        if (particles[i].spr.getPosition().y>WINDOW_HEIGHT) {
            particles.erase(particles.begin()+i);
        }
    }
    window.draw(red.spr);
    window.draw(blue.spr);
    window.draw(redLaser);
    window.draw(blueLaser);
    if (isGameEnded) {
            window.draw(infoText);
            window.draw(winText);
    }
    window.display();


    return 1;
}

void PlayScreen::init()
{
    logger->log("Creating players...");
    isGameEnded = false;
    red = LocalPlayer(redTex, "The Red", WINDOW_WIDTH*0.25-redTex.getSize().x/2, (WINDOW_HEIGHT*0.75)-redTex.getSize().x/2);
    blue = LocalPlayer(blueTex, "The Blue", WINDOW_WIDTH*0.75-redTex.getSize().x/2, (WINDOW_HEIGHT*0.25)-redTex.getSize().x/2);
    logger->log("Play!");
    bg.play();
}


PlayScreen::PlayScreen(Logger* glogger)
{
    logger = glogger;
    logger->log("Preparing play screen...");
    logger->log("Loading resources...");
    redTex.loadFromFile("res/images/redplayer.png");
    blueTex.loadFromFile("res/images/blueplayer.png");
    rlaserTex.loadFromFile("res/images/redlaser.png");
    blaserTex.loadFromFile("res/images/bluelaser.png");
    particleTex.loadFromFile("res/images/particle.png");
    redLaser.setTexture(rlaserTex);
    redLaser.setColor(Color(255,255,255,0));

    f.loadFromFile("res/fonts/Gasalt-Black.ttf");
    winText.setFont(f);

    shotBuff.loadFromFile("res/sounds/shot.wav");
    shotSnd.setBuffer(shotBuff);

    winBuff.loadFromFile("res/sounds/winner.wav");
    winSnd.setBuffer(winBuff);

    blueLaser.setTexture(blaserTex);
    blueLaser.setColor(Color(255,255,255,0));

    line.setSize(Vector2f(WINDOW_WIDTH-20, 2));
    line.setPosition(10, WINDOW_HEIGHT/2-2);
    line.setFillColor(Color::Black);

    bg.openFromFile("res/music/game.ogg");
    bg.stop();
    bg.setLoop(true);


    infoText.setFont(f);
    infoText.setColor(Color::Black);
    infoText.setString("    Press R to play again. \nPress Esc to go to main menu");
    infoText.setPosition(WINDOW_WIDTH/2-infoText.getGlobalBounds().width/2, WINDOW_HEIGHT*0.75);
    logger->log("Play screen is ready. Have fun XD");
}


