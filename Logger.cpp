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

#include "Logger.h"
#include <fstream>
#include <iostream>
#include <string>

#include <SFML/System/Clock.hpp>

using namespace std;
using namespace sf;


void Logger::log(string msg)
{
    logFile << "[" << clock.getElapsedTime().asSeconds() << "s] " << msg << "\n";

}

void Logger::close()
{
    logFile << "--- END OF LOG FILE ---\n";
    logFile.close();
}

Logger::Logger()
{
    logFile.open("debug.log", fstream::out);
    logFile << "--- START OF LOG FILE ---\n";
}



