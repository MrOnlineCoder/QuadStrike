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
#include <SFML/Audio.hpp>
#include "Constants.h"
#include <vector>

using namespace sf;
using namespace std;

const int ALIGN = 30;
const int FADE_ANIM_DELAY = 2000; // in milliseconds

class MainScreen: public Screen {
    public:
        int run(RenderWindow& window);
        void init();
        MainScreen(Logger* glogger);
        Logger* logger;

        Font f;

        Text logo;
        Text author;
        Text credits;
        Text help;

        UIButton playBtn;
        UIButton exitBtn;
        RectangleShape divider;
        RectangleShape divider2;

        Music music;

        Texture sfmlLogo;
        Sprite sfmlSpr;
        Texture blackTex;
        Sprite black;

        int alpha;
        Clock blackClock;
};


int MainScreen::run(RenderWindow& window)
{
    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::Closed) {
            window.close();
            return -1;
        }

        if (e.type == Event::MouseMoved) {
             Vector2i mousePos = Mouse::getPosition(window);
             playBtn.sendMouseMove(mousePos);
             exitBtn.sendMouseMove(mousePos);
        }

        if (e.type == Event::MouseButtonPressed) {
            Vector2i mousePos = Mouse::getPosition(window);;
            if (exitBtn.isClicked(mousePos)) {
                logger->log("exitBtn was clicked, calling engine cleanup...");
                window.close();
                return -1;
            }

            if (playBtn.isClicked(mousePos)) {
                logger->log("playBtn was clicked, cleaning up and setting screen to #1");
                music.stop();
                return 1;
            }
        }
    }

    //Logic
    if (alpha > 0 && blackClock.getElapsedTime().asMilliseconds() > FADE_ANIM_DELAY/128) {
        alpha--;
        black.setColor(Color(255,255,255,alpha));
        blackClock.restart();
    }

    divider.move(-1, 0);
    divider2.move(-1, 0);
    if (divider.getPosition().x < 0-divider.getGlobalBounds().width) {
        divider.setPosition(WINDOW_WIDTH, 130);
    }

    if (divider2.getPosition().x < 0-divider2.getGlobalBounds().width) {
        divider2.setPosition(WINDOW_WIDTH, 130);
    }

    //Render
    window.clear(COLOR_BACKGROUND);
    window.draw(logo);
    window.draw(author);
    window.draw(credits);
    window.draw(playBtn.text);
    window.draw(divider);
    window.draw(divider2);
    window.draw(exitBtn.text);
    window.draw(sfmlSpr);
    window.draw(help);
    if (alpha>0) window.draw(black);
    window.display();

    return 0;
}

void MainScreen::init()
{
    alpha = 255;
    music.play();
}


MainScreen::MainScreen(Logger* glogger)
{
    logger = glogger;
    logger->log("Preparing menu screen...");
    logger->log("Loading font...");
    f.loadFromFile("res/fonts/Gasalt-Black.ttf");
    logger->log("Creating UI...");
    logo.setFont(f);
    logo.setCharacterSize(64);
    logo.setString("QuadStrike");
    logo.setColor(Color::Black);
    //logo.setPosition(WINDOW_WIDTH/2-logo.getGlobalBounds().width/2, 50);
    logo.setPosition(ALIGN, 25);

    author.setFont(f);
    author.setCharacterSize(24);
    author.setString("by MrOnlineCoder");
    author.setColor(Color::White);
    //author.setPosition(WINDOW_WIDTH/2-author.getGlobalBounds().width/2, 55+logo.getCharacterSize());
    author.setPosition(ALIGN, 30+logo.getCharacterSize());


    credits.setFont(f);
    credits.setCharacterSize(24);
    credits.setString("Gasalt Font by Remi Lagast (rlagast.1001fonts.com) \nMusic by Kevin MacLeod (incompetech.com)\nProgrammed by MrOnlineCoder \nPowered by SFML library");
    credits.setColor(Color::Black);
    credits.setPosition(ALIGN, WINDOW_HEIGHT-10-credits.getCharacterSize()-credits.getGlobalBounds().height);

    int btnNum = 1;
    playBtn = UIButton(f, "Start new game", ALIGN, 150);
    exitBtn = UIButton(f, "Exit from QuadStrike", ALIGN, 200);

    divider.setSize(Vector2f(WINDOW_WIDTH-ALIGN*2, 2));
    divider.setPosition(ALIGN, 130);

    divider2.setSize(Vector2f(WINDOW_WIDTH-ALIGN*2, 2));
    divider2.setPosition(WINDOW_WIDTH, 130);


    sfmlLogo.loadFromFile("res/images/sfml.png");
    sfmlSpr.setTexture(sfmlLogo);
    sfmlSpr.setPosition(WINDOW_WIDTH-10-sfmlSpr.getGlobalBounds().width, WINDOW_HEIGHT-10-sfmlSpr.getGlobalBounds().height);

    blackTex.loadFromFile("res/images/black.png");
    black.setTexture(blackTex);
    blackClock.restart();

    help.setFont(f);
    help.setString("How to play: \n\nGoal: Destroy enemy who has 3 HP using laser\n\nControls:\nRed: WASD to move and Q to shoot\nBlue: Arrows to move and Right Shift to shoot");
    help.setPosition(WINDOW_WIDTH-ALIGN-help.getGlobalBounds().width, 150);
    help.setColor(Color::Black);

    logger->log("Loading music...");
    music.openFromFile("res/music/menu.ogg");
    music.setLoop(true);
    music.play();
    logger->log("Menu screen is ready.");
}

