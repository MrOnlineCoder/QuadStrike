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


#ifndef QS_UIBUTTON_H
#define QS_UIBUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace std;
using namespace sf;

class UIButton {
    public:
        Text text;
        bool isClicked(Vector2i clickPos);
        void sendMouseMove(Vector2i movePos);
        UIButton(Font &f, string str, float x, float y);
        UIButton();
};



#endif // QS_UIBUTTON_H

